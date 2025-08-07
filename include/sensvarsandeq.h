// This file basically has all the parameters and constants that need to be generated before any analysis is done. Basically section 1 and 2 in the matlab program 

#pragma once

#include <string>
#include <vector>

// ========================================================================
// SECTION 1: CONSTANTS
// ========================================================================

//--- Physical and Competition Constants (Generally Fixed)---
constexpr double RHO = 1.18;                      //air density (kg/m^3)
constexpr double G = 9.81;                        //gravity (m/s^2)
constexpr double PI = 3.1415926535;
constexpr double LAP_STRAIGHT = 2000 * 0.3048;    //length of straights (m)
constexpr double FLIGHT_WINDOW = 300;             //mission max flight time (s)
constexpr double USABLE_BATTERY = 100 * 0.8 * 3600; //usable battery energy (Joules)
constexpr double MAX_LIFT_CL = 1.6;               //max lift coefficient

//--- User-Defined Simulation Parameters & Constraints ---
inline double VEL_TAKEOFF_MS = 18.0;              //assumed takeoff speed (m/s)
inline double MIN_CHORD_M = 8 * 0.0254;           //minimum manufacturable chord (m)
inline double MAX_GROUND_RUN_M = 100 * 0.3048;    //max takeoff distance constraint (m)
inline double MU_FRICTION = 0.040;                //coefficient of rolling friction
inline int NUM_ITERATIONS = 100000;

//--- Unit Conversion Factors ---
constexpr double LBS_TO_NEWTONS = 4.44822;
constexpr double NEWTONS_TO_LBS = 1.0 / LBS_TO_NEWTONS;
constexpr double FT_TO_METERS = 0.3048;
constexpr double METERS_TO_FT = 1.0 / FT_TO_METERS;
constexpr double M2_TO_FT2 = 10.7639;
constexpr double METERS_TO_IN = 39.3701;
// I defined these because while copying the program I noticed they kept popping up 

// ========================================================================
// SECTION 2: DATA STRUCTURES
// ========================================================================

// Defined structures here..didn't even know I could do this 
struct Aircraft {
    double AR;
    double b_m;             // Wingspan (m)
    double S;               // Wing Area (m^2)
    double W_empty_N;       // Empty Weight (N)
    double W_payload1_N;    // Glider Weight (N)
    double W_payload2_N;    // Empty Tank Weight (N)
    double T_static_N;      // Static Thrust (N)
    double CD0;             // Zero-lift drag coefficient
    double e;               // Oswald efficiency factor
    double eta_total;       // Total propulsion efficiency
};
// Other important variable defs below
// Output of the takeoff calculation
struct TakeoffPerformance {
    bool success = false;
    double s_G_m = 0.0;    // Ground run (m)
    double V_TO_ms = 0.0;  // Takeoff speed (m/s)
};

// Output of the mission performance calculation
struct MissionPerformance {
    double total_score = 0.0;
    double M2_score = 0.0;
    double M3_score = 0.0;
    double lap_time = 0.0;
    double elec_P_M2 = 0.0;
    double elec_P_M3 = 0.0;
};

// Holds the parameters for a single flight strategy
struct Strategy {
    double W_fuel_N;
    double n_turn;
    double V_s_ms;
    double M3_bonus_points;
};

// Holds the best result found for a single aircraft
struct BestStrategy {
    double score = 0.0;
    double M2_score = 0.0;
    double M3_score = 0.0;
    double W_fuel_lbs = 0.0;
    double V_s_fts = 0.0;
    double n_turn = 0.0;
    TakeoffPerformance takeoff_perf;
    double M2_lap_time = 0.0;
    double M2_power = 0.0;
    double M3_power = 0.0;
    double M2_MTOW_N = 0.0;
};

// Single row in the final results table
struct ResultsRow {
    double Total_Score;
    double Empty_Weight_lbf;
    double M2_MTOW_lbf;
    double Wing_Area_ft2;
    double AR;
    double Wingspan_ft;
    double Chord_in;
    double Max_Lift_at_Takeoff_lbf;
    double Opt_Fuel_lbf;
    double Opt_V_straight_fts;
    double Opt_Load_Factor_n;
    double Takeoff_Speed_fts;
    double Ground_Run_ft;
    double M2_Score;
    double M3_Score;
    double Static_Thrust_lbf;
    double M2_Lap_Time_s;
    double M2_Avg_Power_W;
    double M3_Avg_Power_W;
};
