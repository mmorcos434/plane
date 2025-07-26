#pragma once

#include "sensvarsandeq.h"

/**
 * @brief Runs the entire MDAO simulation.
 * @return A vector of ResultsRow structs containing all valid, sorted results.
 */
std::vector<ResultsRow> run_mdao_simulation();