/*
** Program: BudgetBuddy.cpp
** Author: Paul Lim
** Date: 1/12/2020
** Description: a program that will let your users log in to see their account,
** as well as import transactions and allow the users to filter them
** Input: user/budget files, login info, filtering method
** Output: Transaction data, either printed or written to a file
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include "BudgetBuddy.h"

using namespace std;

int main(int argc, char **argv) {
    int num_users;
    int num_buds;
    user *user_arr;
    budget *budget_arr;
    string user_file_name;
    string bud_file_name;
    user current_user;
    int error_flag;

    // Error handling for command line arguments and file existence
    check_valid_input(argc, argv, &user_file_name, &bud_file_name);
    // Create file objects
    ifstream user_file(user_file_name.c_str());
    ifstream budget_file(bud_file_name.c_str());
    // Get number of users/budgets from first word of files
    user_file >> num_users;
    budget_file >> num_buds;
    // Create array of users/budgets
    user_arr = create_users(num_users);
    budget_arr = create_budgets(num_buds);
    // Parse user/budget data and store in user/budget array
    get_user_data(user_arr, num_users, user_file);
    get_budget_data(budget_arr, num_buds, budget_file);
    // Login user and return user info
    error_flag = login(user_arr, num_users, budget_arr, num_buds, current_user);
    if(error_flag != -1) {
        // Display user info
        display_info(current_user, budget_arr, num_buds);
        // Sort user info and print to screen or write to file
        sort(budget_arr, num_buds, user_arr, num_users, current_user);
    }
    // Delete heap arrays
    delete_info(&user_arr, num_users, &budget_arr, num_buds);
    // Close file objects
    user_file.close();
    budget_file.close();
    return 0;
}