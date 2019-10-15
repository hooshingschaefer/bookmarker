#ifndef __DB__H__
#define __DB__H__
#include <string>
#include <libpq-fe.h>
using namespace std;
        
#define USERS_TABLE test.users
#define DATABASE_NAME website3

enum class DBStatus{
    CONNECTION_ERROR = 2, 
    OK = 3, 
    INVALID_USER = 4,
    USER_EXISTS = 5
};

namespace DB{
    //DBStatus check_password(const char* username, const uint32_t password);
    DBStatus check_password(const char* username, const char*  password);
    //DBStatus change_password(const char* username, const uint32_t password);
    DBStatus change_password(const char* username, const char*  password);
    DBStatus delete_user(const char* username);
    //DBStatus add_user(const char* username, const uint32_t password);
    DBStatus add_user(const char* username, const char*  password);
    
    PGconn* connect_db();
}

#endif
