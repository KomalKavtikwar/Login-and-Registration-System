#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>  // For getch() function
#include <direct.h> // For mkdir() function
#include <sys/stat.h> // For checking if directory exists

using namespace std;

// Function prototypes
void registerUser();
bool loginUser();
bool checkUserExists(const string& username);
bool createUserFile(const string& username, const string& password);
void createDatabaseDirectory();
void displayMenu();
void displayHeader();

// Constants
const string DATABASE_DIR = "database";

int main() {
    // Create database directory if it doesn't exist
    createDatabaseDirectory();
    
    int choice;
    bool loggedIn = false;
    
    do {
        system("cls"); // Clear the console screen
        displayHeader();
        displayMenu();
        
        cout << "\nEnter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                loggedIn = loginUser();
                if (loggedIn) {
                    system("cls");
                    displayHeader();
                    cout << "\n\n\tWelcome! You are now logged in." << endl;
                    cout << "\n\tPress any key to logout...";
                    _getch(); // Wait for a key press
                    loggedIn = false;
                }
                break;
            case 3:
                cout << "\n\tThank you for using our system. Goodbye!" << endl;
                break;
            default:
                cout << "\n\tInvalid choice. Please try again." << endl;
                cout << "\n\tPress any key to continue...";
                _getch(); // Wait for a key press
        }
    } while (choice != 3);
    
    return 0;
}

void displayHeader() {
    cout << "\n\t=======================================" << endl;
    cout << "\t    LOGIN AND REGISTRATION SYSTEM     " << endl;
    cout << "\t=======================================" << endl;
}

void displayMenu() {
    cout << "\n\t1. Register" << endl;
    cout << "\t2. Login" << endl;
    cout << "\t3. Exit" << endl;
}

void createDatabaseDirectory() {
    // Check if directory exists
    struct stat info;
    if (stat(DATABASE_DIR.c_str(), &info) != 0) {
        // Directory doesn't exist, create it
        if (_mkdir(DATABASE_DIR.c_str()) != 0) {
            cerr << "Error creating database directory!" << endl;
            exit(1);
        }
    }
}

bool checkUserExists(const string& username) {
    ifstream file(DATABASE_DIR + "/" + username + ".txt");
    return file.good();
}

bool createUserFile(const string& username, const string& password) {
    ofstream file(DATABASE_DIR + "/" + username + ".txt");
    
    if (!file) {
        return false;
    }
    
    // Write username and password to file
    file << username << endl;
    file << password << endl;
    file.close();
    
    return true;
}

void registerUser() {
    system("cls");
    displayHeader();
    cout << "\n\t---- REGISTRATION ----" << endl;
    
    string username, password, confirmPassword;
    
    cin.ignore(); // Clear input buffer
    
    cout << "\n\tEnter username: ";
    getline(cin, username);
    
    // Check if username already exists
    if (checkUserExists(username)) {
        cout << "\n\tUsername already exists. Please try a different one." << endl;
        cout << "\n\tPress any key to continue...";
        _getch();
        return;
    }
    
    cout << "\tEnter password: ";
    char ch;
    password = "";
    
    while ((ch = _getch()) != 13) { // 13 is the ASCII value for Enter key
        if (ch == 8) { // 8 is the ASCII value for Backspace key
            if (!password.empty()) {
                cout << "\b \b"; // Move cursor back, print space, move cursor back again
                password.pop_back();
            }
        }
        else {
            password.push_back(ch);
            cout << '*';
        }
    }
    
    cout << "\n\tConfirm password: ";
    confirmPassword = "";
    
    while ((ch = _getch()) != 13) {
        if (ch == 8) {
            if (!confirmPassword.empty()) {
                cout << "\b \b";
                confirmPassword.pop_back();
            }
        }
        else {
            confirmPassword.push_back(ch);
            cout << '*';
        }
    }
    
    // Check if passwords match
    if (password != confirmPassword) {
        cout << "\n\n\tPasswords do not match. Registration failed." << endl;
        cout << "\n\tPress any key to continue...";
        _getch();
        return;
    }
    
    // Create user file
    if (createUserFile(username, password)) {
        cout << "\n\n\tRegistration successful!" << endl;
    }
    else {
        cout << "\n\n\tError creating user file. Registration failed." << endl;
    }
    
    cout << "\n\tPress any key to continue...";
    _getch();
}

bool loginUser() {
    system("cls");
    displayHeader();
    cout << "\n\t---- LOGIN ----" << endl;
    
    string username, password, storedPassword;
    
    cin.ignore(); // Clear input buffer
    
    cout << "\n\tEnter username: ";
    getline(cin, username);
    
    // Check if user exists
    if (!checkUserExists(username)) {
        cout << "\n\tUsername does not exist. Please register first." << endl;
        cout << "\n\tPress any key to continue...";
        _getch();
        return false;
    }
    
    cout << "\tEnter password: ";
    char ch;
    password = "";
    
    while ((ch = _getch()) != 13) {
        if (ch == 8) {
            if (!password.empty()) {
                cout << "\b \b";
                password.pop_back();
            }
        }
        else {
            password.push_back(ch);
            cout << '*';
        }
    }
    
    // Read stored password from file
    ifstream file(DATABASE_DIR + "/" + username + ".txt");
    string line;
    
    getline(file, line); // Skip username line
    getline(file, storedPassword);
    
    file.close();
    
    // Check if password matches
    if (password == storedPassword) {
        cout << "\n\n\tLogin successful!" << endl;
        cout << "\n\tPress any key to continue...";
        _getch();
        return true;
    }
    else {
        cout << "\n\n\tIncorrect password. Login failed." << endl;
        cout << "\n\tPress any key to continue...";
        _getch();
        return false;
    }
}
