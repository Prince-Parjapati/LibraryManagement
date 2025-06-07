#include "Auth.h"
#include <fstream>
#include <iostream>

bool Auth::login(const string& username, const string& password) {
    ifstream file("../data/users.txt");
    
    if (!file.is_open()) {
        cerr << "Error: Could not open users.txt file!" << endl;
        cerr << "Make sure the data directory exists in: " << endl;
        system("pwd"); // Print working directory
        return false;
    }
    
    string u, p;
    while (file >> u >> p) {
        if (u == username && p == password) {
            return true;
        }
    }
    
    cout << "Invalid username or password. Please try again." << endl;
    return false;
}
