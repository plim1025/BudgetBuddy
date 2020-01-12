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

int main(int argc, char **argv) {
    string username;
    string password;
    int id;
    int num_buds;
    int num_trans;
    budget *bud_arr;
    int login_attempt = 0;
    string user_file_name;
    string bud_file_name;
    ifstream user_file;
    ifstream bud_file;
    int username_line;
    
    check_valid_input(argc, argv, &user_file_name, &bud_file_name);
    cout << user_file_name << " " << bud_file_name << endl;
    
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

bool test_file(string file_name) {
    ifstream test(file_name);
    if(test) {
        return true;
    }
    return false;
}