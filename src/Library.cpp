#include "Library.h"
#include "Book.h"
#include "Auth.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

vector<Book> loadBooks() {
    vector<Book> books;
    ifstream file("../data/books.txt");
    
    if (!file.is_open()) {
        cerr << "Error: Could not open books database file!" << endl;
        cerr << "Make sure the data directory exists and contains books.txt" << endl;
        return books; // Return empty vector
    }
    
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, title, author, status;
        getline(ss, id, ',');
        getline(ss, title, ',');
        getline(ss, author, ',');
        getline(ss, status, ',');
        
        try {
            books.emplace_back(stoi(id), title, author, status == "1");
        } catch (const exception& e) {
            cerr << "Warning: Invalid book entry found: " << line << endl;
        }
    }
    return books;
}

bool saveBooks(const vector<Book>& books) {
    ofstream file("../data/books.txt");
    if (!file.is_open()) {
        cerr << "Error: Could not open books.txt for writing!" << endl;
        return false;
    }
    
    for (const auto& book : books) {
        file << book.toString() << endl;
    }
    
    return true;
}

bool logTransaction(const string& username, const string& action, const string& bookTitle) {
    ofstream file("../data/logs.txt", ios::app);
    if (!file.is_open()) {
        cerr << "Warning: Could not open transaction log file!" << endl;
        return false;
    }
    
    file << "[LOG] " << username << " " << action << " \"" << bookTitle << "\"" << endl;
    return true;
}

void Library::run() {
    Auth auth;
    string username, password;
    
    // Login process
    cout << "=== Library Management System ===" << endl;
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;
    
    if (!auth.login(username, password)) {
        cout << "Login failed! Please check your credentials and try again." << endl;
        return;
    }
    
    cout << "\nWelcome, " << username << "!" << endl;
    
    // Load books database
    vector<Book> books = loadBooks();
    if (books.empty()) {
        cout << "No books found in the database. The library appears to be empty." << endl;
    }
    
    // Main menu loop
    int choice;
    do {
        cout << "\n=== MENU ===" << endl;
        cout << "1. List All Books" << endl;
        cout << "2. Borrow a Book" << endl;
        cout << "3. Return a Book" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        
        // Handle invalid input
        if (!(cin >> choice)) {
            cin.clear(); // Clear error state
            cin.ignore(10000, '\n'); // Discard invalid input with fixed value
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }
        
        switch (choice) {
            case 1: {
                // List all books
                cout << "\n=== BOOK INVENTORY ===" << endl;
                if (books.empty()) {
                    cout << "No books in the library." << endl;
                } else {
                    cout << setw(5) << "ID" << " | " 
                         << setw(30) << left << "TITLE" << " | "
                         << setw(20) << left << "AUTHOR" << " | "
                         << "STATUS" << endl;
                    cout << string(70, '-') << endl;
                    
                    for (const auto& book : books) {
                        cout << setw(5) << right << book.id << " | " 
                             << setw(30) << left << book.title << " | "
                             << setw(20) << left << book.author << " | "
                             << (book.isBorrowed ? "Borrowed" : "Available") << endl;
                    }
                }
                break;
            }
            
            case 2: {
                // Borrow a book
                int id;
                cout << "\nEnter Book ID to borrow: ";
                cin >> id;
                
                bool found = false;
                bool alreadyBorrowed = false;
                
                for (auto& book : books) {
                    if (book.id == id) {
                        found = true;
                        if (book.isBorrowed) {
                            alreadyBorrowed = true;
                            cout << "Sorry, this book is already borrowed." << endl;
                        } else {
                            cout << "You are about to borrow: \"" << book.title << "\" by " << book.author << endl;
                            cout << "Confirm? (y/n): ";
                            char confirm;
                            cin >> confirm;
                            
                            if (confirm == 'y' || confirm == 'Y') {
                                book.isBorrowed = true;
                                if (logTransaction(username, "borrowed", book.title)) {
                                    if (saveBooks(books)) {
                                        cout << "Book successfully borrowed!" << endl;
                                    } else {
                                        cout << "Error saving transaction. Please try again." << endl;
                                    }
                                }
                            } else {
                                cout << "Borrow operation cancelled." << endl;
                            }
                        }
                        break;
                    }
                }
                
                if (!found) {
                    cout << "Error: Book with ID " << id << " not found." << endl;
                }
                break;
            }
            
            case 3: {
                // Return a book
                int id;
                cout << "\nEnter Book ID to return: ";
                cin >> id;
                
                bool found = false;
                bool notBorrowed = false;
                
                for (auto& book : books) {
                    if (book.id == id) {
                        found = true;
                        if (!book.isBorrowed) {
                            notBorrowed = true;
                            cout << "Error: This book is not currently borrowed." << endl;
                        } else {
                            cout << "You are returning: \"" << book.title << "\" by " << book.author << endl;
                            cout << "Confirm? (y/n): ";
                            char confirm;
                            cin >> confirm;
                            
                            if (confirm == 'y' || confirm == 'Y') {
                                book.isBorrowed = false;
                                if (logTransaction(username, "returned", book.title)) {
                                    if (saveBooks(books)) {
                                        cout << "Book successfully returned!" << endl;
                                    } else {
                                        cout << "Error saving transaction. Please try again." << endl;
                                    }
                                }
                            } else {
                                cout << "Return operation cancelled." << endl;
                            }
                        }
                        break;
                    }
                }
                
                if (!found) {
                    cout << "Error: Book with ID " << id << " not found." << endl;
                }
                break;
            }
            
            case 0:
                cout << "\nThank you for using the Library Management System. Goodbye!" << endl;
                break;
                
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 0);
}
