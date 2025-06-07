#ifndef BOOK_H
#define BOOK_H

#include <string>
using namespace std;

class Book {
public:
    int id;
    string title;
    string author;
    bool isBorrowed;

    Book(int i, string t, string a, bool b = false);
    string toString() const ;
};

#endif
