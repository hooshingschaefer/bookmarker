#ifndef __USERS__H__
#define __USERS__H__
#include <string>
//#include <iostream>
#include <unordered_map>
using namespace std;

class User{
    public:    
        User(const string&);
        inline const string& get_username() const{
            return username;
        };
    private:
        //int x;
        string username;
};

class UsersManager{
    public:
        static UsersManager* get_instance();
        bool add_user(const string& cookie, const string& username);
        const User* lookup_user(const string& cookie) const;
        void dump();
    private:    
        static UsersManager* um;
        unordered_map<string, User> users{};
};
#endif


