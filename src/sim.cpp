#include "sim.h"
#include "physics.h"
#include <iostream>
#include <random>
#include <set>
#include <algorithm> // for std::sort

std::vector<ResultsRow> run_mdao_simulation() {
    std::cout << "--- Starting Physics-Constrained MDAO Simulation for "
        << NUM_ITERATIONS << " configurations ---" << std::endl;

    std::vector<ResultsRow> results_table;
    results_table.reserve(NUM_ITERATIONS);

    // Setup for random number generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> b_ft_dis(4.0, 6.0);
    std::uniform_real_distribution<> AR_dis(3.0, 10.0);
    std::uniform_real_distribution<> W_struct_lbs_dis(5.0, 12.0);
    std::uniform_real_distribution<> W_avionics_lbs_dis(5.0, 10.0);
    std::uniform_real_distribution<> W_glider_lbs_dis(0.1, 0.45);
    std::uniform_real_distribution<> thrust_ratio_dis(0.5, 1.5);

    // Main simulation loop
    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        if ((i + 1) % 10000 == 0) {
            std::cout << "Processing base configuration " << i + 1 << " of " << NUM_ITERATIONS << "...\n";
        }

        // --- Step 1: Generate a Random Aircraft ---
        Aircraft aircraft;
        aircraft.b_m = b_ft_dis(gen) * FT_TO_METERS;
        aircraft.AR = AR_dis(gen);
        aircraft.S = (aircraft.b_m * aircraft.b_m) / aircraft.AR;

        // Apply Minimum Chord Length Constraint
        if ((aircraft.b_m / aircraft.AR) < MIN_CHORD_M) continue;

        aircraft.W_empty_N = (W_struct_lbs_dis(gen) + W_avionics_lbs_dis(gen)) * LBS_TO_NEWTONS;
        aircraft.W_payload1_N = W_glider_lbs_dis(gen) * LBS_TO_NEWTONS; // Glider
        aircraft.W_payload2_N = 0.2 * LBS_TO_NEWTONS;                   // Tanks
        aircraft.T_static_N = aircraft.W_empty_N * thrust_ratio_dis(gen);
        aircraft.CD0 = 0.035;
        aircraft.e = 0.82;
        aircraft.eta_total = 0.60;

        // --- Step 2: Find the Optimal Mission Strategy for THIS aircraft ---
        BestStrategy best_strategy;

        // Define strategy ranges to iterate over
        std::vector<double> W_fuel_range_lbs;
        for (int w = 5; w <= 30; w += 2) { // Need a way to generalize these range definitions
            W_fuel_range_lbs.push_back(w);
        }

        std::vector<double> V_s_range_fts;
        for (int v = 70; v <= 150; v += 15) {
            V_s_range_fts.push_back(v);
        }

        const std::vector<double> BP_range = { 0, 1, 2.5 }; // Bonus point scenarios might honestly be useless next year
        // For i in range 
        for (double w_fuel_lbs : W_fuel_range_lbs) {
            for (double v_s_fts : V_s_range_fts) {
                // Mission 2 takeoff weight for configuration values fed into it
                double W_M2_N = aircraft.W_empty_N + aircraft.W_payload1_N + (w_fuel_lbs * LBS_TO_NEWTONS) + aircraft.W_payload2_N;
                TakeoffPerformance takeoff_perf = calculate_takeoff_performance(W_M2_N, aircraft); // Physics Check
                if (!takeoff_perf.success) continue;

                double v_s_ms = v_s_fts * FT_TO_METERS;
                double n_max_at_V = (0.5 * RHO * v_s_ms * v_s_ms * aircraft.S * MAX_LIFT_CL) / W_M2_N; // G force calculation
                std::set<double> n_turn_set = { 1.2, 1.5, 2.0 }; // Need to make these user defined as well
                n_turn_set.insert(floor(n_max_at_V * 10.0) / 10.0);

                for (double n_turn : n_turn_set) {
                    if (n_turn > n_max_at_V || n_turn <= 1.0) continue;
                    for (double bp : BP_range) { // If there's no bonus points for that year then this would be useless
                        Strategy strategy = { w_fuel_lbs * LBS_TO_NEWTONS, n_turn, v_s_ms, bp };
                        MissionPerformance perf_data = calculate_mission_performance(aircraft, strategy);
                        if (perf_data.total_score > best_strategy.score) {
                            best_strategy = { perf_data.total_score, perf_data.M2_score, perf_data.M3_score, w_fuel_lbs, v_s_fts, n_turn, takeoff_perf, perf_data.lap_time, perf_data.elec_P_M2, perf_data.elec_P_M3, W_M2_N };
                        }
                    }
                }
            }
        }

        // --- Step 3: Store the BEST result for this aircraft ---
        if (best_strategy.score > 0) {
            double L_max_at_TO_N = 0.5 * RHO * pow(best_strategy.takeoff_perf.V_TO_ms, 2) * aircraft.S * MAX_LIFT_CL;
            results_table.push_back({ best_strategy.score, aircraft.W_empty_N * NEWTONS_TO_LBS, best_strategy.M2_MTOW_N * NEWTONS_TO_LBS, aircraft.S * M2_TO_FT2, aircraft.AR, aircraft.b_m * METERS_TO_FT, (aircraft.b_m / aircraft.AR) * METERS_TO_IN, L_max_at_TO_N * NEWTONS_TO_LBS, best_strategy.W_fuel_lbs, best_strategy.V_s_fts, best_strategy.n_turn, best_strategy.takeoff_perf.V_TO_ms * METERS_TO_FT, best_strategy.takeoff_perf.s_G_m * METERS_TO_FT, best_strategy.M2_score, best_strategy.M3_score, aircraft.T_static_N * NEWTONS_TO_LBS, best_strategy.M2_lap_time, best_strategy.M2_power, best_strategy.M3_power });
        }
    }

    std::cout << "--- MDAO Simulation Complete ---" << std::endl;

    // Sort the final results before returning
    std::sort(results_table.begin(), results_table.end(), [](const ResultsRow& a, const ResultsRow& b) {
        return a.Total_Score > b.Total_Score;
        });

    return results_table;
}
