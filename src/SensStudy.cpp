#include <iostream>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <random>
#include <tuple>
#include <string>
#include "exprtk.hpp"
//#include "SensStudy.h"


std::unordered_map<std::string, std::tuple<int, double, double, double>> equation_variables_from_user;

//declaring it so we can use later
void evaluate(const std::string& reordered_mission_equation);
void montecarlo(const std::string& reordered_mission_equation);

void max_comp_score() {
    double m1_max;
    double m2_max;
    double m3_max;
    double ground;

    std::vector<double*> mission_scores = {&m1_max, &m2_max, &m3_max, &ground};
    //mission_scores = {&m1_max, &m2_max, &m3_max, &ground};
    std::vector<std::string> mission_names = { "m1_max", "m2_max", "m3_max", "ground" };

    for (size_t i = 0; i < mission_scores.size(); i++) {
        std::cout << "Put in a value for " << mission_names[i] << ": ";
        std::cin >> *mission_scores[i];
        std::cout << mission_names[i] << " = " << *mission_scores[i] << std::endl;
    }

    std::cout << "Is there Anything Else That Affects Total Score Reply 1 for yes and 2 for no";
    std::string user_response_to_mission_q;
    std::cin >> user_response_to_mission_q;

    while (user_response_to_mission_q == "1") {
        std::string new_parap_name;
        double new_para_val;

        std::cout << "Provide the name of this new parameter: ";
        std::cin >> new_parap_name;

        std::cout << "Provide the value of this new parameter: ";
        std::cin >> new_para_val;

        // make dyanic doub to store var
        double* new_var_ptr = new double(new_para_val);
        mission_scores.push_back(new_var_ptr);
        mission_names.push_back(new_parap_name);

        std::cout << new_parap_name << " = " << new_para_val << std::endl;
        std::cout << "Add another extra parameter? (1 = yes, 2 = no): ";
        std::cin >> user_response_to_mission_q;
        while (user_response_to_mission_q != "1" && user_response_to_mission_q != "2") {
            std::cout << "Invalid input. Please enter 1 (yes) or 2 (no): ";
            std::cin >> user_response_to_mission_q;
        }
    }

    std::cin.ignore();
    std::string total_score_equation;
    std::cout << "Provide the formula to calculate the total mission score: ";
    std::getline(std::cin, total_score_equation);

    // really gonna put exptr to work donw here chat gpt helped out a lot below :)
    exprtk::symbol_table<double> symbol_table;
    exprtk::expression<double> expression;
    exprtk::parser<double> parser;

    for (size_t i = 0; i < mission_scores.size(); ++i) {
        symbol_table.add_variable(mission_names[i], *mission_scores[i]);
    }
    expression.register_symbol_table(symbol_table);

    if (parser.compile(total_score_equation, expression)) {
        double total_score = expression.value();
        std::cout << "Total Mission Score: " << total_score << std::endl;
    }
    else {
        std::cout << "Invalid formula. Please check your syntax." << std::endl;
    }

    for (size_t i = 4; i < mission_scores.size(); ++i) {
        delete mission_scores[i];
    }
}

void find_eq_vars() {
    std::cout << "How many variables does this equation have: ";
    double number_of_variables_in_reorder;
    std::cin >> number_of_variables_in_reorder;

    for (size_t k = 1; k <= number_of_variables_in_reorder; ++k) {
        std::cout << "Input variable " << k << ":" << std::endl;
        std::string temp_input;
        double temp_Val; 
        double temp_Val1;
        double temp_Val2; 
        std::cin >> temp_input;
        std::cout << "Input variable " << k << "'s type of distribution (ex/ 0: skip Monte Carlo, 1: Uniform, 2: For: Normal):" << std::endl;
        int temp_dist;
        std::cin >> temp_dist;

        std::cout << "Input variable " << k << "'s value:" << std::endl;
        std::cin >> temp_Val;

        if (temp_dist == 1) { //Uniform Distrubtion
            std::cout << "Input Variable's miminium value: " << std::endl;
            std::cin >> temp_Val1;
            std::cout << "Input Variable's maximum value: " << std::endl;
            std::cin >> temp_Val2;
        }
        else if (temp_dist == 2) { //Normal
            std::cout << "Input Variable's mean: " << std::endl;
            std::cin >> temp_Val1;
            std::cout << "Input Variable's standard deviation: " << std::endl;
            std::cin >> temp_Val2;
        }
        else { //didnt wanna use monte carlo
            temp_Val1 = 0;
            temp_Val2 = 0;
        }

        equation_variables_from_user[temp_input] = std::make_tuple(temp_dist, temp_Val, temp_Val1, temp_Val2);
        //var name = distrubtion, val of var, mean/min, std/max
    }

    for (const auto& pair : equation_variables_from_user) {     //this was a snippet frm a chat gpt example code on maps lol...
        int dist;
        double val, p1, p2;
        std::tie(dist, val, p1, p2) = pair.second;
        std::cout << pair.first << ": (" << dist << ", " << val << ", " << p1 << ", " << p2 << ")\n";
    } 

    std::cin.ignore();
    std::string reordered_mission_equation;
    std::cout << "Provide the formula to calculate the max mission score: ";
    std::getline(std::cin, reordered_mission_equation);

    // decide if Monte Carlo is needed
    bool run_mc = false;
    for (const auto& pair : equation_variables_from_user) {
        if (std::get<0>(pair.second) != 0) {
            run_mc = true;
            break;
        }
    }

    if (run_mc) {
        montecarlo(reordered_mission_equation);
    } else {
        evaluate(reordered_mission_equation);
    }
}

void evaluate(const std::string& reordered_mission_equation) {
    exprtk::symbol_table<double> mission_reo;
    exprtk::expression<double> expression;
    exprtk::parser<double> parser;

    std::unordered_map<std::string, double> temp_vars;
    for (const auto& [var, data] : equation_variables_from_user) {
        temp_vars[var] = std::get<1>(data); // use constant base value
        mission_reo.add_variable(var, temp_vars[var]);
    }

    expression.register_symbol_table(mission_reo);

    if (parser.compile(reordered_mission_equation, expression)) {
        double total_score = expression.value();
        std::cout << "\nTotal Mission Score: " << total_score << std::endl;
    } else {
        std::cout << "\nInvalid formula. Please check your syntax." << std::endl;
    }
}

void montecarlo(const std::string& reordered_mission_equation) {
    exprtk::symbol_table<double> mission_reo;
    std::unordered_map<std::string, double> temp_values;

    // Prepare variable placeholders
    for (auto& [var, data] : equation_variables_from_user) {
        temp_values[var] = std::get<1>(data);
        mission_reo.add_variable(var, temp_values[var]);
    }

    exprtk::expression<double> expression;
    expression.register_symbol_table(mission_reo);
    exprtk::parser<double> parser;

    if (!parser.compile(reordered_mission_equation, expression)) {
        std::cerr << "Invalid expression. Please check the syntax.\n";
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<double> results;

    for (int i = 0; i < 10000; ++i) {
        for (auto& [var, data] : equation_variables_from_user) {
            int dist = std::get<0>(data);
            double base = std::get<1>(data);
            double p1 = std::get<2>(data);
            double p2 = std::get<3>(data);

            if (dist == 1) {
                std::uniform_real_distribution<> d(p1, p2);
                temp_values[var] = d(gen);
            } else if (dist == 2) {
                std::normal_distribution<> d(p1, p2);
                temp_values[var] = d(gen);
            } else {
                temp_values[var] = base;
            }
        }

        results.push_back(expression.value());
    }

    double sum = 0;
    double sum_sq = 0;
    double min_val = results[0];
    double max_val = results[0];

    for (double r : results) {
        sum += r;
        sum_sq += r * r;
        if (r < min_val) min_val = r;
        if (r > max_val) max_val = r;
    }

    double avg = sum / results.size();
    double variance = (sum_sq / results.size()) - (avg * avg);
    double stddev = std::sqrt(variance);

    std::cout << "\nMonte Carlo Simulation Results:\n";
    std::cout << "• Estimated Average Score: " << avg << "\n";
    std::cout << "• Standard Deviation     : " << stddev << "\n";
    std::cout << "• Minimum Score          : " << min_val << "\n";
    std::cout << "• Maximum Score          : " << max_val << "\n";
}

int main() {
    find_eq_vars();
    auto start = std::chrono::high_resolution_clock::now();

    //end t
    auto end = std::chrono::high_resolution_clock::now();

    //duration
    std::chrono::duration<double, std::milli> duration = end - start;

    std::cout << "Execution time: " << duration.count() << " ms\n";
    return 0;
}



