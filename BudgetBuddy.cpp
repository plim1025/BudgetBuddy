#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include "BudgetBuddy.h"

using namespace std;

void print_budget(budget budget) {
    cout << endl;
    for(int i = 0; i < budget.num_transactions; i++)
        cout << budget.t[i].date << " " << budget.t[i].amount << " " << budget.t[i].description << " " << budget.t[i].category << endl;
}

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

// Returns copy of given budget_arr
budget* copy_budget_arr(budget *budget_arr, int num_buds) {
    budget *budget_arr_copy = new budget[num_buds];
    for(int i = 0; i < num_buds; i++) {
        budget temp = budget_arr[i];
        budget_arr_copy[i] = temp;
    }
    return budget_arr_copy;
}

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
}

// Sorts transactions based on sort type - category = 1, date = 2, dollar amount = 3
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

// Finds budget in budget arr with matching id to current user, modifies it, and returns index
int get_user_budget(budget *budget_arr, budget &user_budget, int num_buds, user current_user) {
    for(int i = 0; i < num_buds; i++) {
        if(budget_arr[i].id == current_user.id) {
            user_budget = budget_arr[i];
            return i;
        }
    }
    return -1;
}

// Swaps two transaction objects
void swap_trans(transaction *xp, transaction *yp) {
    transaction temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// Check for 3 command line arguments and for valid file names
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

// If given file name exists, return it, otherwise prompt user for new file name until enter one that exists
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

// Tests given file name and returns true if it exists, false otherwise
bool test_file(string file_name) {
    ifstream test(file_name);
    if(test) {
        return true;
    }
    return false;
}

user* create_users(int num_users) {
    user *arr = new user[num_users];
    return arr;
}

budget* create_budgets(int num_buds) {
    budget *arr = new budget[num_buds];
    return arr;
}

transaction* create_transactions(int num_trans) {
    transaction *arr = new transaction[num_trans];
    return arr;
}

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

// Fetches ID from user and returns it
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

string get_password() {
    string password;
    cout << "Enter password: ";
    getline(cin, password);
    return password;
}

bool is_int(string word) {
    for(int i = 0; i < word.length(); i++) {
        if(word[i] < '0' || word[i] > '9')
            return false;
    }
    return true;
}

// Returns line number that password and username were found on if they match, otherwise returns -1
int check_login(user current_user, user *user_arr, int num_users) {
    for(int i = 0; i < num_users; i++) {
        if(current_user.id == user_arr[i].id && current_user.password == user_arr[i].password)
            return i;
    }
    return -1;
}

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