#include "physics.h"
#include <cmath> // For sqrt, acos, tan, etc.
#include <algorithm> // For std::min

// Note: This file includes its own header "physics.h", which in turn
// includes "sensvarsandeq.h", so all constants and structs are available.

double calc_power_at_CL(double V, double CL, const Aircraft& aircraft) {
    double CD = aircraft.CD0 + (CL * CL) / (PI * aircraft.AR * aircraft.e);
    double D = 0.5 * RHO * V * V * aircraft.S * CD;
    return D * V;
}

TakeoffPerformance calculate_takeoff_performance(double W_total_N, const Aircraft& aircraft) {
    TakeoffPerformance perf;
    perf.success = false;

    double V_s = sqrt((2 * W_total_N) / (RHO * aircraft.S * MAX_LIFT_CL));
    double V_TO = 1.1 * V_s;
    double V_avg = 0.7 * V_TO;
    double q_avg = 0.5 * RHO * V_avg * V_avg;
    double L_avg = q_avg * aircraft.S * MAX_LIFT_CL;
    double CD_g = aircraft.CD0 + (MAX_LIFT_CL * MAX_LIFT_CL) / (PI * aircraft.AR * aircraft.e);
    double D_avg = q_avg * aircraft.S * CD_g;

    double F_friction = MU_FRICTION * (W_total_N - L_avg);
    // Safety check to ensure friction is never negative (physically impossible)
    if (F_friction < 0) {
        F_friction = 0;
    }

    double F_net = aircraft.T_static_N - D_avg - F_friction;

    perf.V_TO_ms = V_TO;
    if (F_net <= 0) {
        perf.s_G_m = std::numeric_limits<double>::infinity();
        return perf;
    }

    double a_avg = F_net / (W_total_N / G);
    double s_G = (V_TO * V_TO) / (2 * a_avg);
    perf.s_G_m = s_G;

    if (s_G <= MAX_GROUND_RUN_M) {
        perf.success = true;
    }
    return perf;
}

MissionPerformance calculate_mission_performance(const Aircraft& aircraft, const Strategy& strategy) {
    MissionPerformance perf; // Initializes all fields to 0

    double bank_angle = acos(1.0 / strategy.n_turn);
    // Safety check for invalid mathematical operations (e.g. acos of a value > 1)
    if (std::isnan(bank_angle)) {
        return perf; // Return immediately with a score of 0
    }

    double V_turn_ms = strategy.V_s_ms;
    double R_turn = (V_turn_ms * V_turn_ms) / (G * tan(bank_angle));
    double dist_turn = 2 * PI * R_turn;

    double time_straight = LAP_STRAIGHT / strategy.V_s_ms;
    double time_turn = dist_turn / V_turn_ms;
    perf.lap_time = time_straight + time_turn;

    // M2 Performance (with payload/fuel)
    double W_M2 = aircraft.W_empty_N + aircraft.W_payload1_N + strategy.W_fuel_N + aircraft.W_payload2_N;
    double CL_s_M2 = W_M2 / (0.5 * RHO * strategy.V_s_ms * strategy.V_s_ms * aircraft.S);
    double CL_t_M2 = strategy.n_turn * CL_s_M2;

    if (CL_t_M2 <= MAX_LIFT_CL) {
        double P_s_M2 = calc_power_at_CL(strategy.V_s_ms, CL_s_M2, aircraft);
        double P_t_M2 = calc_power_at_CL(V_turn_ms, CL_t_M2, aircraft);
        double avg_P_M2 = (time_straight * P_s_M2 + time_turn * P_t_M2) / perf.lap_time;
        perf.elec_P_M2 = avg_P_M2 / aircraft.eta_total;

        // Calculate endurance for Mission 2
        double t_endur_M2;
        if (perf.elec_P_M2 > 0) {
            t_endur_M2 = USABLE_BATTERY / perf.elec_P_M2;
        }
        else {
            // If power draw is zero, endurance is infinite (prevents division by zero)
            t_endur_M2 = std::numeric_limits<double>::infinity();
        }

        double t_3_laps = 3 * perf.lap_time;

        if (t_endur_M2 >= t_3_laps && t_3_laps <= FLIGHT_WINDOW) {
            perf.M2_score = strategy.W_fuel_N / t_3_laps;
        }
    }

    // M3 Performance (endurance, no fuel)
    double W_M3 = aircraft.W_empty_N + aircraft.W_payload1_N + aircraft.W_payload2_N;
    double CL_s_M3 = W_M3 / (0.5 * RHO * strategy.V_s_ms * strategy.V_s_ms * aircraft.S);
    double CL_t_M3 = strategy.n_turn * CL_s_M3;

    if (CL_t_M3 <= MAX_LIFT_CL) {
        double P_s_M3 = calc_power_at_CL(strategy.V_s_ms, CL_s_M3, aircraft);
        double P_t_M3 = calc_power_at_CL(V_turn_ms, CL_t_M3, aircraft);
        double avg_P_M3 = (time_straight * P_s_M3 + time_turn * P_t_M3) / perf.lap_time;
        perf.elec_P_M3 = avg_P_M3 / aircraft.eta_total;

        // Calculate endurance for Mission 3
        double t_endur_M3;
        if (perf.elec_P_M3 > 0) {
            t_endur_M3 = USABLE_BATTERY / perf.elec_P_M3;
        }
        else {
            t_endur_M3 = std::numeric_limits<double>::infinity();
        }

        double t_flight = std::min(t_endur_M3, FLIGHT_WINDOW);
        double num_laps = floor(t_flight / perf.lap_time);
        perf.M3_score = num_laps + (strategy.M3_bonus_points / aircraft.W_payload1_N);
    }

    if (perf.M2_score > 0 && perf.M3_score > 0) {
        perf.total_score = perf.M2_score + perf.M3_score;
    }
    return perf;
}