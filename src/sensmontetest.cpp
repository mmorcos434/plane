#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm> // For std::min
#include <string>
#include <span>
#include "sim.h" // Includes everything else needed

// ROOT Headers for visualization
#include "TApplication.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TColor.h"

/**
 * @brief Writes the final results table to a CSV file.
 */
void write_csv(const std::string& filename, const std::vector<ResultsRow>& table) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return;
    }

    // Write CSV header
    file << "Total_Score,Empty_Weight_lbf,M2_MTOW_lbf,Wing_Area_ft2,AR,Wingspan_ft,Chord_in,"
        << "Max_Lift_at_Takeoff_lbf,Opt_Fuel_lbf,Opt_V_straight_fts,Opt_Load_Factor_n,"
        << "Takeoff_Speed_fts,Ground_Run_ft,M2_Score,M3_Score,Static_Thrust_lbf,"
        << "M2_Lap_Time_s,M2_Avg_Power_W,M3_Avg_Power_W\n";

    // Write data rows
    for (const auto& row : table) {
        file << row.Total_Score << "," << row.Empty_Weight_lbf << "," << row.M2_MTOW_lbf << ","
            << row.Wing_Area_ft2 << "," << row.AR << "," << row.Wingspan_ft << "," << row.Chord_in << ","
            << row.Max_Lift_at_Takeoff_lbf << "," << row.Opt_Fuel_lbf << ","
            << row.Opt_V_straight_fts << "," << row.Opt_Load_Factor_n << ","
            << row.Takeoff_Speed_fts << "," << row.Ground_Run_ft << "," << row.M2_Score << ","
            << row.M3_Score << "," << row.Static_Thrust_lbf << "," << row.M2_Lap_Time_s << ","
            << row.M2_Avg_Power_W << "," << row.M3_Avg_Power_W << "\n";
    }
    file.close();
    std::cout << "Results saved to " << filename << std::endl;
}


int main(int argc, char** argv) {
    // This is needed to launch a ROOT GUI application context.
    TApplication theApp("App", &argc, argv);

    // Run the simulation to get the results
    std::vector<ResultsRow> results_table = run_mdao_simulation();

    // Check if any valid results were found
    if (results_table.empty()) {
        std::cerr << "Error: No valid configurations were found. Consider increasing iterations or relaxing constraints." << std::endl;
        return 1;
    }

    // --- Output Results to Console and CSV ---
    write_csv("DBF_MDAO_Results_V15.csv", results_table);

    std::cout << "\n--- Top 5 Aircraft Configurations ---\n";
    std::cout << std::left << std::fixed << std::setprecision(2)
        << std::setw(14) << "Total_Score" << std::setw(18) << "Empty_Weight_lbf" << std::setw(14) << "M2_MTOW_lbf"
        << std::setw(14) << "Wingspan_ft" << std::setw(8) << "AR" << std::setw(15) << "Opt_Fuel_lbf"
        << std::setw(16) << "Ground_Run_ft" << "Max_Lift_Takeoff_lbf\n";
    std::cout << "------------------------------------------------------------------------------------------------------------------\n";
    for (int i = 0; i < std::min(5, (int)results_table.size()); ++i) {
        auto& row = results_table[i];
        std::cout << std::left << std::setw(14) << row.Total_Score << std::setw(18) << row.Empty_Weight_lbf
            << std::setw(14) << row.M2_MTOW_lbf << std::setw(14) << row.Wingspan_ft << std::setw(8) << row.AR
            << std::setw(15) << row.Opt_Fuel_lbf << std::setw(16) << row.Ground_Run_ft << row.Max_Lift_at_Takeoff_lbf << "\n";
    }

    // ========================================================================
    // SECTION 5: ROOT VISUALIZATION
    // ========================================================================
    gStyle->SetPalette(kRainBow);
    TCanvas* c1 = new TCanvas("c1", "MDAO Results", 1200, 800);
    c1->Divide(2, 1);

    // Plot 1: Score vs. Empty Weight
    c1->cd(1);
    TGraph* g_score_vs_weight = new TGraph();
    for (const auto& row : results_table) {
        g_score_vs_weight->SetPoint(g_score_vs_weight->GetN(), row.Empty_Weight_lbf, row.Total_Score);
    }
    g_score_vs_weight->SetTitle("Score vs. Empty Weight;Empty Weight (lbf);Total Score");
    g_score_vs_weight->SetMarkerStyle(20);
    g_score_vs_weight->SetMarkerSize(1.0);
    g_score_vs_weight->Draw("AP");

    // Plot 2: Design Space (Wingspan vs. Weight, Color for Score)
    c1->cd(2);
    TH2F* h_design_space = new TH2F("h_design_space", "Design Space: Avg Score;Empty Weight (lbf);Wingspan (ft)",
        50, 4.0, 25.0, 50, 3.5, 6.5);
    for (const auto& row : results_table) {
        h_design_space->Fill(row.Empty_Weight_lbf, row.Wingspan_ft, row.Total_Score);
    }
    h_design_space->Divide(h_design_space, h_design_space, 1, 1, "B"); // Average score per bin
    h_design_space->Draw("COLZ");

    c1->Print("DBF_MDAO_Plots.pdf");
    std::cout << "\nVisualization saved to DBF_MDAO_Plots.pdf" << std::endl;

    // Uncomment the line below to keep the plot window open after the script finishes.
    // theApp.Run(); 

    return 0;
}

