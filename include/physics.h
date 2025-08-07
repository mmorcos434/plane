#pragma once

#include "sensvarsandeq.h"

/**
 * @brief Calculates the required aerodynamic power for a given flight condition.
 * @param V Flight speed in m/s.
 * @param CL The lift coefficient for the flight condition.
 * @param aircraft The aircraft configuration.
 * @return The required power in Watts.
 */
double calc_power_at_CL(double V, double CL, const Aircraft& aircraft);

/**
 * @brief Calculates the takeoff performance of the aircraft.
 * @param W_total_N The total takeoff weight in Newtons.
 * @param aircraft The aircraft configuration.
 * @return A TakeoffPerformance struct with success flag and results.
 */
TakeoffPerformance calculate_takeoff_performance(double W_total_N, const Aircraft& aircraft);

/**
 * @brief Calculates the performance and score for a given mission strategy.
 * @param aircraft The aircraft configuration.
 * @param strategy The flight strategy (fuel, speed, turn rate).
 * @return A MissionPerformance struct with scores and performance data.
 */
MissionPerformance calculate_mission_performance(const Aircraft& aircraft, const Strategy& strategy);
