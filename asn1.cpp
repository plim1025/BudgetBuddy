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

    while(argc != 3) {
        cout << "Please execute using the following format: [executable] [user_file] [budget_file]" << endl;
        prompt_user(argv[1], argv[2]);
    }
    while(/*file names not valid*/) {
        cout << "One of your files does not exist." << endl;
        prompt_user(argv[1], argv[2]);
    }
    
    return 0;
}

void prompt_user(char *file1, char *file2) {
    cout << "Enter user file name" << endl;
    cin >> 
}