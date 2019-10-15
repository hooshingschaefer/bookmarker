//Hooshing Schaefer 2019
/*  make sure access is controlled no race conditions!!!
 *  add periodic cleaning of users
 *
 *
 */
#include <iostream>
#include <mutex>
#include "users.h"
using namespace std;
mutex mtx2;
User::User(const string& username): username(username){ }

UsersManager* UsersManager::um = nullptr;

UsersManager* UsersManager::get_instance(){
    if (um == nullptr){
        um = new UsersManager();
    }
    return um;
}

bool UsersManager::add_user(const string& cookie, const string& username){
    if (users.count(cookie) > 0) return false;
    lock_guard<mutex> lck (mtx2);//mtx2.lock();
    users.emplace(cookie, username);
 
    return true;
    /*if (users.size() > START_FREEING_USERS){
        
    }*/
}

const User* UsersManager::lookup_user(const string& cookie) const{
//    return users[];
    unordered_map<string, User>::const_iterator it = users.find(cookie);
    if (it == users.end()){
        return nullptr;
    }else{
        return &(it->second);
    }
}


void UsersManager::dump(){
    for (auto x : users){
        cout << "cookie: " << x.first << ", username: " << x.second.get_username() << endl;
    }
}
/*
int main(int argc, char** argv){
    UsersManager* um = UsersManager::get_instance();
    um->add_user("he84TJjnc", "jamesjim");
    um->add_user("48dfj6oDsh", "hellotom");
    um->dump();
    const User* user = um->lookup_user("834jkd");
    if (user != nullptr) cout << user->get_username();
    else cout << "nullptr" ;
    um->add_user("djk", "fortiom");
    um->add_user("834jkd", "bigboye");
    um->add_user("u4jdDTDsh", "jaredthre");
    user = um->lookup_user("834jkd");
    if (user != nullptr) cout << user->get_username();
    else cout << "nullptr" ;
    um->dump();
    return 0;

}
*/
