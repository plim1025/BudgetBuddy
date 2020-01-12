#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>

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
void sort_transactions(string sort_type, budget *budget_arr, int num_buds, user current_user);
int get_user_budget(budget *budget_arr, budget &user_budget, int num_buds, user current_user);
void alphebatize(budget budget, string sort_type);
void swap_trans(transaction *xp, transaction *yp);

void print_budget(budget budget) {
    for(int i = 0; i < budget.num_transactions; i++)
        cout << budget.t[i].category << endl;
}

int main(int argc, char **argv) {
    int num_users;
    int num_buds;
    user *user_arr;
    budget *budget_arr;
    string user_file_name;
    string bud_file_name;

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

    user current_user = login(user_arr, num_users);
    display_info(current_user, budget_arr, num_buds);
    sort(budget_arr, num_buds, current_user);


    return 0;
}


void sort(budget *budget_arr, int num_buds, user current_user) {
    string sort_type = "0";
    while(sort_type != "1" && sort_type != "2" && sort_type != "3" && sort_type != "4") {
        getline(cin, sort_type);
        if(sort_type == "1" || sort_type == "2" || sort_type == "3")
            sort_transactions(sort_type, budget_arr, num_buds, current_user);
        else if (sort_type == "4")
            exit(EXIT_SUCCESS);
        else
            cout << "Invalid option. Enter a sorting option: By category (1), by date (2), by dollar amount (3), or exit the program (4): ";
    }
}

void sort_transactions(string sort_type, budget *budget_arr, int num_buds, user current_user) {
    budget user_budget;
    int user_budget_index = get_user_budget(budget_arr, user_budget, num_buds, current_user);

    // Sort by category
    if(sort_type == "1")
        alphebatize(user_budget, sort_type);
    // Sort by date
    else if(sort_type == "2")
        alphebatize(user_budget, sort_type);
    // Sort by dollar amount
    else if(sort_type == "3")
        alphebatize(user_budget, sort_type);

    /* Only do this once copy budget array to separate budget array
    // Replace budget in budget array with modified user budget
    budget_arr[user_budget_index] = user_budget;
    */
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

// Sorts transactions within budget by sort_type
void alphebatize(budget budget, string sort_type) {
    for(int i = 0; i < budget.num_transactions - 1; i++) {
        // Last i elements are already in place  
        for (int j = 0; j < budget.num_transactions - i - 1; j++) {
            if(sort_type == "1") {
                // If first char of category in transaction is greater than first char of next transaction, swap
                if(budget.t[j].category[0] > budget.t[j+1].category[0])
                    swap_trans(&budget.t[j], &budget.t[j+1]);
            }
            else if(sort_type == "2") {
                // If first char of date in transaction is greater than first char of next transaction, swap
                if(budget.t[j].date[0] > budget.t[j+1].date[0])
                    swap_trans(&budget.t[j], &budget.t[j+1]);
            }
            else if(sort_type == "3") {
                // If amount in transaction is less than next, swap
                if(budget.t[j].amount < budget.t[j+1].amount)
                    swap_trans(&budget.t[j], &budget.t[j+1]);
            }
        }
    }
    print_budget(budget);
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
        budget_arr[i].balance = balance;
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
    cout << "Username: " << current_user.name << endl;
    cout << "ID#: " << current_user.id << endl;
    cout << "Current Account Balance: " << current_balance << endl;
    cout << "Enter a sorting option: By category (1), by date (2), by dollar amount (3), or exit the program (4): ";
}