#ifndef AUTH_H
#define AUTH_H

#include <string>
using namespace std;

class Auth {
public:
    bool login(const string& username, const string& password);
};

#endif
