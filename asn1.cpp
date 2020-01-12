#include <iostream>
#include <fstream>
#include <cstdlib>

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
string get_file_name(string file_name = "");
bool test_file(string file);

user* create_users(int num_users);
budget* create_budgets(int num_buds);
transaction* create_transactions(int num_trans);

void get_user_data(user* user_arr, int num_users, ifstream &file);
void get_budget_data(budget* budget_arr, int num_buds, ifstream &file);
void get_transaction_data(transaction* transaction_arr, int trans, ifstream &file);

int main(int argc, char **argv) {
    string username;
    string password;
    int id;

    int num_users;
    int num_buds;

    user *user_arr;
    budget *budget_arr;

    int login_attempt = 0;
    string user_file_name;
    string bud_file_name;
    int username_line;

    // Error handling for command line arguments and file existence
    check_valid_input(argc, argv, &user_file_name, &bud_file_name);
    // Create file objects
    ifstream user_file(user_file_name);
    ifstream budget_file(bud_file_name);
    // Get number of users/budgets from first word of files
    user_file >> num_users;
    budget_file >> num_buds;
    // Create array of users/budgets
    user_arr = create_users(num_users);
    budget_arr = create_budgets(num_buds);
    // Parse user/budget data and store in user/budget array
    get_user_data(user_arr, num_users, user_file);
    get_budget_data(budget_arr, num_buds, budget_file);

    return 0;
}

void print_budget_data(budget* budget_arr, int num_buds) {
    for(int i = 0; i < num_buds; i++) {
        cout << budget_arr[i].id << " ";
        cout << budget_arr[i].balance << " ";
        cout << budget_arr[i].num_transactions << endl;
        for(int i = 0; i < budget_arr[i].num_transactions; i++) {
            cout << budget_arr[i].t->amount << " ";
            cout << budget_arr[i].t->date << " ";
            cout << budget_arr[i].t->category << " ";
            cout << budget_arr[i].t->description << endl;
        }
    }
}

// Check for 3 command line arguments and for valid file names
void check_valid_input(int argc, char **argv, string *user_file_name, string *bud_file_name) {
    if(argc != 3) {
        cout << "Wrong number of command line arguments, please enter user file name: ";
        *user_file_name = get_file_name();
        cout << "Now enter transaction file name: ";
        *bud_file_name = get_file_name();
    } else {
        *user_file_name = get_file_name(argv[1]);
        *bud_file_name = get_file_name(argv[2]);
    }
}

// If given file name exists, return it, otherwise prompt user for new file name until enter one that exists
string get_file_name(string file_name) {
    string new_file_name = "";
    // If given file name is valid, return it
    if(test_file(file_name))
        return file_name;
    // Else, prompt for file name until valid
    while(true) {
        cin >> new_file_name;
        if(test_file(new_file_name))
            break;
        else
            cout << "Bad file name, please input again: ";
    }
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
        file >> id;
        budget_arr[i].id = id;
        file >> balance;
        budget_arr[i].balance = balance;
        file >> num_transactions;
        budget_arr[i].num_transactions = num_transactions;
        transaction_arr[i] = new transaction[budget_arr[i].num_transactions];
        budget_arr[i].t = transaction_arr[i];
        // Create transaction array
        get_transaction_data(budget_arr[i].t, budget_arr[i].num_transactions, file);
    }
}

void get_transaction_data(transaction* transactions_arr, int num_trans, ifstream &file) {
    string date;
    double amount;
    string description;
    string category;
    for(int i = 0; i < num_trans; i++) {
        // Add member variables to each transaction
        file >> date >> amount >> description >> category;
        transactions_arr[i].date = date;
        transactions_arr[i].amount = amount;
        transactions_arr[i].description = description;
        transactions_arr[i].category = category;
    }
}
