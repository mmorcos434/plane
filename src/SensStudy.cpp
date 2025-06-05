#include <iostream>
#include <vector>
#include "exprtk.hpp"
int main()
{
    double m1_max;
    double m2_max;
    double m3_max;
    double ground;
    std::vector<double*> mission_scores = { &m1_max, &m2_max, &m3_max, &ground};
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
    return 0;
}
