#include "db.h"
#include "assert.h"
#include <iostream>
using DB::check_password;
using DB::add_user;
using DB::delete_user;
using DB::change_password;

string dbstatus_as_string(DBStatus d){
    switch(d){
        case DBStatus::OK :
            return "ok";
            break;
        case DBStatus::INVALID_USER :
            return "invalid user";
            break;
        case DBStatus::USER_EXISTS :
            return "user exists";
            break;
        case DBStatus::CONNECTION_ERROR :
            return "CONNECTION_ERROR";
            break;
        default :
            return "unknown DBStatus type";
            break;
    }
}

int main(int argc, char** argv){    
    DBStatus pw_check = check_password("hoosh", 90);
    assert(pw_check == DBStatus::INVALID_USER);    

    pw_check = add_user("bob", 89);
    assert(pw_check == DBStatus::OK);
    
    pw_check = add_user("bob", 83);
    assert(pw_check == DBStatus::USER_EXISTS);
    
    pw_check = add_user("hoosh", 90);
    assert(pw_check == DBStatus::OK);
    pw_check = change_password("bob", 2743);
    assert(pw_check == DBStatus::OK);
    pw_check = check_password("hoosh", 90);
    assert(pw_check == DBStatus::OK);
    pw_check = check_password("bob", 89);
    assert(pw_check == DBStatus::INVALID_USER);
    pw_check = check_password("bob", 2743);
    assert(pw_check == DBStatus::OK);    

    
    pw_check = delete_user("bob");
    assert(pw_check == DBStatus::OK);    

    pw_check = check_password("bob", 2743);
    assert(pw_check == DBStatus::INVALID_USER);    

    pw_check = add_user("bob", 2734343);
    assert(pw_check == DBStatus::OK);    
    pw_check = add_user("bob", 3);
    assert(pw_check == DBStatus::USER_EXISTS);    


}
