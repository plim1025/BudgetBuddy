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

/*
 ** Function: check_valid_input
 ** Description: check for 3 command line arguments and for valid file names
 ** Parameters: argc, argv, string pointers to user file and budget file names
 ** Pre-conditions: command line arguments and user file and budget file names must tbe passed
 ** Post-conditions: stores valid file names into string pointers
*/
void check_valid_input(int argc, char **argv, string *user_file_name, string *bud_file_name) {
    if(argc != 3) {
        cout << "Wrong number of command line arguments" << endl;
        *user_file_name = get_file_name("user");
        *bud_file_name = get_file_name("budget");
    } else {
        *user_file_name = get_file_name("user", argv[1]);
        *bud_file_name = get_file_name("budget", argv[2]);
    }
}

/*
 ** Function: get_file_name
 ** Description: if given file name exists, return it, otherwise prompt 
 ** user for new file name until enter one that exists
 ** Parameters: string for file type (user or budget), and string for file name
 ** Pre-condition: string for file type and file names must be passed
 ** Post-conditions: returns string for existing file name
 */
string get_file_name(string file_type, string file_name) {
    string new_file_name = "";
    // If given file name is valid, return it
    if(test_file(file_name))
        return file_name;
    // Else, prompt for file name until valid
    do {
        cout << "Bad " << file_type << " file name, please input again: ";
        getline(cin, new_file_name);
    } while(!test_file(new_file_name));

    return new_file_name;
}

/*
 ** Function: frequency_given
 ** Description: tests given file name and returns true if it exists, false otherwise
 ** Parameters: string for file name
 ** Pre-conditions: take in string for file name
 ** Post-conditions: return true if file name exists, false otherwise
 */
bool test_file(string file_name) {
    ifstream test(file_name);
    if(test) {
        test.close();
        return true;
    }
    test.close();
    return false;
}

/*
 ** Function: create_users
 ** Description: dynamically creates 1D array of users and returns it
 ** Parameters: int for size of 1D array
 ** Pre-conditions: take in an int for size of 1D array
 ** Post-conditions: return dynamically created 1D array
 */
user* create_users(int num_users) {
    user *arr = new user[num_users];
    return arr;
}

/*
 ** Function: create_budgets
 ** Description: dynamically creates 1D array of budgets and returns it
 ** Parameters: int for size of 1D array
 ** Pre-conditions: take in an int for size of 1D array
 ** Post-conditions: return dynamically created 1D array
 */
budget* create_budgets(int num_buds) {
    budget *arr = new budget[num_buds];
    return arr;
}

/*
 ** Function: create_transactions
 ** Description: dynamically creates 1D array of transactions and returns it
 ** Parameters: int for size of 1D array
 ** Pre-conditions: take in an int for size of 1D array
 ** Post-conditions: return dynamically created 1D array
 */
transaction* create_transactions(int num_trans) {
    transaction *arr = new transaction[num_trans];
    return arr;
}

/*
 ** Function: get_user_data
 ** Description: extracts user data from files and stores it in user array
 ** Parameters: user array, integer for number of users, and reference to file object
 ** Pre-conditions: take in user array, integer for number of users, and reference to file object
 ** Post-conditions: extracts user data from files and stores it in user array
*/
void get_user_data(user* user_arr, int num_users, ifstream &file) {
    string word;
    int num;
    // For each user
    for(int i = 0; i < num_users; i++) {
        // Add member variables to each user
        file >> word;
        user_arr[i].name = word;
        file >> num;
        user_arr[i].id = num;
        file >> word;
        user_arr[i].password = word;
    }
}

/*
 ** Function: get_budget_data
 ** Description: extracts budget data from files and stores it in budget array
 ** Parameters: budget array, integer for number of budgets, and reference to file object
 ** Pre-conditions: take in budget array, integer for number of budgets, and reference to file object
 ** Post-conditions: extracts budget data from files and stores it in budget array
*/
void get_budget_data(budget* budget_arr, int num_buds, ifstream &file) {
    int id;
    float balance;
    int num_transactions;
    struct t;

    transaction **transaction_arr = new transaction *[num_buds];
    // For each transaction
    for(int i = 0; i < num_buds; i++) {
        // Add member variables to each budget
        file >> id >> balance >> num_transactions;
        budget_arr[i].id = id;
        float balance_rounded = (int)(balance * 100 + .5);
        budget_arr[i].balance = (float)balance_rounded / 100;
        budget_arr[i].num_transactions = num_transactions;
        transaction_arr[i] = new transaction[budget_arr[i].num_transactions];
        budget_arr[i].t = transaction_arr[i];
        // Create transaction array
        get_transaction_data(budget_arr[i].t, budget_arr[i].num_transactions, file);
    }
}

/*
 ** Function: get_transaction_data
 ** Description: extracts transaction data from files and stores it in transaction array
 ** Parameters: transaction array, integer for number of transactions, and reference to file object
 ** Pre-conditions: take in transaction array, integer for number of transactions, and reference to file object
 ** Post-conditions: extracts transaction data from files and stores it in transaction array
*/
void get_transaction_data(transaction* transactions_arr, int num_trans, ifstream &file) {
    string date;
    float amount;
    string description;
    string category;
    for(int i = 0; i < num_trans; i++) {
        // Add member variables to each transaction
        file >> date >> amount >> description >> category;
        transactions_arr[i].date = date;
        float amount_rounded = (int)(amount * 100 + .5);
        transactions_arr[i].amount = (float)amount_rounded / 100;
        transactions_arr[i].amount = amount;
        transactions_arr[i].description = description;
        transactions_arr[i].category = category;
    }
}

/*
 ** Function: login
 ** Description: main function for prompting user to login
 ** Parameters: user array and int for number of users
 ** Pre-conditions: take in user array and int for number of users
 ** Post-conditions: prompt user to login and error handle bad logins
*/
user login(user *user_arr, int num_users) {
    user current_user;
    int login_attempts = 0;
    int username_line = -1;
    string temp_id;

    while(login_attempts < 3) {
        current_user.id = get_id();
        current_user.password = get_password();
        username_line = check_login(current_user, user_arr, num_users);
        if(username_line == -1)
            cout << "Incorrect, login info, try again." << endl;
        else
            break;
        login_attempts++;
    }
    // Exit program if login attempts exceeded 1
    if(login_attempts > 2) {
        cout << "Too many login attempts. Exiting program..." << endl;
        exit(EXIT_FAILURE);
    }
    // Set username into current_user based on line returns from check_login function
    current_user.name = user_arr[username_line].name;
    return current_user;
}

/*
 ** Function: get_id
 ** Description: fetches ID from user and returns it
 ** Parameters: none
 ** Pre-conditions: none
 ** Post-conditions: prompts user for integer ID and returns it
*/
int get_id() {
    string temp_id;
    int id;
    while(true) {
            cout << "Enter ID#: ";
            getline(cin, temp_id);
            if(is_int(temp_id))
                break;
            else
                cout << "ID only contains digits, try again." << endl;
        }
    id = atoi(temp_id.c_str());
    return id;
}

/*
 ** Function: get_password
 ** Description: fetches password from user and returns it
 ** Parameters: none
 ** Pre-conditions: none
 ** Post-conditions: prompts user for integer/character password and returns it
*/
string get_password() {
    string password;
    cout << "Enter password: ";
    getline(cin, password);
    return password;
}

/*
 ** Function: is_int
 ** Description: checks if given word contains all digits, returns true if yes, false otherwise
 ** Parameters: string
 ** Pre-conditions: take in a string
 ** Post-conditions: checks if given word contains all digits, returns true if yes, false otherwise
*/
bool is_int(string word) {
    for(int i = 0; i < word.length(); i++) {
        if(word[i] < '0' || word[i] > '9')
            return false;
    }
    return true;
}

/*
 ** Function: check_login
 ** Description: returns line number that password and username were found on if they match, otherwise returns -1
 ** Parameters: user, user array, num_users
 ** Pre-conditions: takes in user, user array, num_users
 ** Post-conditions: returns line number that password and username were found on if they match, otherwise returns -1
*/
int check_login(user current_user, user *user_arr, int num_users) {
    for(int i = 0; i < num_users; i++) {
        if(current_user.id == user_arr[i].id && current_user.password == user_arr[i].password)
            return i;
    }
    return -1;
}

/*
 ** Function: display_info
 ** Description: displays current user's username, ID, account balance, and prompts sorting options
 ** Parameters: user, budget array, num_budgets
 ** Pre-conditions: take in user, budget array, num_budgets
 ** Post-conditions:  displays current user's username, ID, account balance, and prompts sorting options
*/
void display_info(user current_user, budget *budget_arr, int num_buds) {
    // fetch current_balance from budget_arr
    float current_balance;
    for(int i = 0; i < num_buds; i++) {
        if(current_user.id == budget_arr[i].id)
            current_balance = budget_arr[i].balance;
    }
    cout << endl << "Username: " << current_user.name << endl;
    cout << "ID#: " << current_user.id << endl;
    cout << "Current Account Balance: $" << current_balance << endl << endl;
    cout << "Enter a sorting option: By category (1), by date (2), by dollar amount (3), or exit the program (4): ";
}

/*
 ** Function: sort
 ** Description: main function for sorting given transactions and printing/writing to file
 ** Parameters: budget array, num_buds, user
 ** Pre-conditions: take in budget array, num_buds, user
 ** Post-conditions: sorts given transactions and prints/writes to file
*/
void sort(budget *budget_arr, int num_buds, user current_user) {
    budget user_budget;
    int budget_index = get_user_budget(budget_arr, user_budget, num_buds, current_user);

    string sort_type = "0";
    while(sort_type != "1" && sort_type != "2" && sort_type != "3" && sort_type != "4") {
        getline(cin, sort_type);
        if(sort_type == "1" || sort_type == "2" || sort_type == "3")
            sort_transactions(sort_type, user_budget, num_buds, current_user);
        else if (sort_type == "4")
            exit(EXIT_SUCCESS);
        else
            cout << "Invalid option. Enter a sorting option: By category (1), by date (2), by dollar amount (3), or exit the program (4): ";
    }

    string write_or_print = "0";
    ofstream file;
    while(write_or_print != "1" && write_or_print != "2") {
        cout << "Do you want to print (1) these changes or write them to a new file (2)? ";
        getline(cin, write_or_print);
        if(write_or_print == "1")
            print_budget(user_budget);
        else if(write_or_print == "2") {
            string file_name = "";
            while(true) {
                cout << "Enter a file name: ";
                getline(cin, file_name);
                file.open(file_name);
                if(!file.is_open())
                    cout << "Bad file name, try again: ";
                else
                    break;
            }
            write_budget(file, budget_arr, num_buds, user_budget, budget_index);
        }
    }
    file.close();
}

/*
 ** Function: sort_transactions
 ** Description: sorts transactions based on sort type - category = 1, date = 2, dollar amount = 3
 ** Parameters: string, budget, num_buds, user
 ** Pre-conditions: take in string, budget, num_buds, user
 ** Post-conditions: sorts transactions based on sort type - category = 1, date = 2, dollar amount = 3
*/
void sort_transactions(string sort_type, budget user_budget, int num_buds, user current_user) {
    // Sort by category
    if(sort_type == "1") {
        for(int i = 0; i < user_budget.num_transactions - 1; i++) {
            // Last i elements are already in place
            for (int j = 0; j < user_budget.num_transactions - i - 1; j++) {
                // If first char of category in transaction is greater than first char of next transaction, swap
                if(user_budget.t[j].category[0] > user_budget.t[j+1].category[0])
                    swap_trans(&user_budget.t[j], &user_budget.t[j+1]);
            }
        }
    }
    // Sort by date
    else if(sort_type == "2") {
        for(int i = 0; i < user_budget.num_transactions - 1; i++) {
            // Last i elements are already in place
            for (int j = 0; j < user_budget.num_transactions - i - 1; j++) {
                // If first char of date in transaction is greater than first char of next transaction, swap
                if(user_budget.t[j].date[0] > user_budget.t[j+1].date[0])
                    swap_trans(&user_budget.t[j], &user_budget.t[j+1]);
            }
        }
    }
    // Sort by dollar amount
    else if(sort_type == "3") {
        for(int i = 0; i < user_budget.num_transactions - 1; i++) {
            // Last i elements are already in place
            for (int j = 0; j < user_budget.num_transactions - i - 1; j++) {
                // If amount in transaction is less than next, swap
                if(user_budget.t[j].amount < user_budget.t[j+1].amount)
                    swap_trans(&user_budget.t[j], &user_budget.t[j+1]);
            }
        }
    }
}

/*
 ** Function: get_user_budget
 ** Description: finds budget in budget arr with matching id to current user, modifies it, and returns index
 ** Parameters: budget array, user budget, num_buds, current user
 ** Pre-conditions: take in budget array, user budget, num_buds, current user
 ** Post-conditions: finds budget in budget arr with matching id to current user, modifies it, and returns index
*/
int get_user_budget(budget *budget_arr, budget &user_budget, int num_buds, user current_user) {
    for(int i = 0; i < num_buds; i++) {
        if(budget_arr[i].id == current_user.id) {
            user_budget = budget_arr[i];
            return i;
        }
    }
    return -1;
}

/*
 ** Function: swap_trans
 ** Description: swaps two transaction objects
 ** Parameters: two pointers to transaction objects
 ** Pre-conditions: take in two pointers to transaction objects
 ** Post-conditions: swaps two transaction objects
*/
void swap_trans(transaction *xp, transaction *yp) {
    transaction temp = *xp;
    *xp = *yp;
    *yp = temp;
}

/*
 ** Function: print_budget
 ** Description: prints given budget array to screen
 ** Parameters: budget object
 ** Pre-conditions: take in budget object
 ** Post-conditions: prints given budget array to screen
*/
void print_budget(budget budget) {
    cout << endl;
    for(int i = 0; i < budget.num_transactions; i++)
        cout << budget.t[i].date << " " << budget.t[i].amount << " " << budget.t[i].description << " " << budget.t[i].category << endl;
}

/*
 ** Function: write_budget
 ** Description: write given budget to file
 ** Parameters: file object, budget array, num_buds, user budget, budget index
 ** Pre-conditions: take in file object, budget array, num_buds, user budget, budget index
 ** Post-conditions: write given budget to file
*/
void write_budget(ofstream &file, budget *budget_arr, int num_buds, budget user_budget, int budget_index) {
    // Make copy of budget array
    budget *budget_arr_copy = copy_budget_arr(budget_arr, num_buds);
    // Swap modified user_budget into copy of budget array
    budget_arr[budget_index] = user_budget;
    // Write copied budget array into file
    file << budget_arr_copy[budget_index].id << " " << budget_arr_copy[budget_index].balance << " " << budget_arr_copy[budget_index].num_transactions << endl;
    for(int j = 0; j < budget_arr_copy[budget_index].num_transactions; j++)
        file << budget_arr_copy->t[j].date << " " << budget_arr_copy->t[j].amount << " " << budget_arr_copy->t[j].description << " " << budget_arr_copy->t[j].category << endl;
}

/*
 ** Function: copy_budget_arr
 ** Description: returns copy of given budget_arr
 ** Parameters: budget array, num_buds
 ** Pre-conditions: take in budget array, num_buds
 ** Post-conditions: returns copy of given budget_arr
*/
budget* copy_budget_arr(budget *budget_arr, int num_buds) {
    budget *budget_arr_copy = new budget[num_buds];
    for(int i = 0; i < num_buds; i++) {
        budget temp = budget_arr[i];
        budget_arr_copy[i] = temp;
    }
    return budget_arr_copy;
}