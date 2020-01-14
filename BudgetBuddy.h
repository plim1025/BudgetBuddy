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

#ifndef BUDGET_BUDDY_H
#define BUDGET_BUDDY_H

using namespace std;

struct user {
    string name;
    int id;
    string password;
};

struct budget {
    int id;
    float balance;
    int num_transactions;
    struct transaction *t;
};

struct transaction {
    float amount;
    string date;
    string category;
    string description;
};

void check_valid_input(int argc, char **argv, string *user_file_name, string *bud_file_name);
string get_file_name(string file_type, string file_name = "");
bool test_file(string file);

user* create_users(int num_users);
budget* create_budgets(int num_buds);
transaction* create_transactions(int num_trans);

void get_user_data(user* user_arr, int num_users, ifstream &file);
void get_budget_data(budget* budget_arr, int num_buds, ifstream &file);
void get_transaction_data(transaction* transaction_arr, int trans, ifstream &file);

user login(user *user_arr, int num_users);
int get_id();
string get_password();
bool is_int(string word);
int check_login(user current_user, user *user_arr, int num_users);
void display_info(user current_user, budget *budget_arr, int num_buds);

void sort(budget *budget_arr, int num_buds, user current_user);
void sort_transactions(string sort_type, budget budget_arr, int num_buds, user current_user);
int get_user_budget(budget *budget_arr, budget &user_budget, int num_buds, user current_user);
void swap_trans(transaction *xp, transaction *yp);

void print_budget(budget budget);
void write_budget(ofstream &file, budget *budget_arr, int num_buds, budget user_budget, int budget_index);
budget* copy_budget_arr(budget *budget_arr, int num_buds);

void delete_info(user **user_arr, budget **budget_arr);

#endif
