#include "Book.h"
#include <sstream>

Book::Book(int i, string t, string a, bool b) : id(i), title(t), author(a), isBorrowed(b) {}

string Book::toString() const {
    return to_string(id) + "," + title + "," + author + "," + (isBorrowed ? "1" : "0");
}
