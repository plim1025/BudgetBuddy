#include <iostream>
#include <fstream>

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
user* create_users(int users);
budget* create_budgets(int budgets);
transaction* create_transactions(int transactions);


int main(int argc, char **argv) {
    string username;
    string password;
    int id;

    int num_users;
    int num_buds;

    budget *bud_arr;
    int login_attempt = 0;
    string user_file_name;
    string bud_file_name;
    int username_line;
    
    check_valid_input(argc, argv, &user_file_name, &bud_file_name);
    fstream user_file(user_file_name);
    fstream bud_file(bud_file_name);
    user_file >> num_users;
    bud_file >> num_buds;

    return 0;
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

user* create_users(int users) {

}

budget* create_budgets(int budgets) {

}

transaction* create_transactions(int transactions) {
    
}