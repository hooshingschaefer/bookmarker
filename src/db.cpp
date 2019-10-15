//Hooshing Schaefer 2019
#include "db.h"
#include "debug.h"
#include <iostream> //remove after debugging
#include <arpa/inet.h>

DBStatus DB::check_password(const char* username, const char* password){
    PGconn* conn = DB::connect_db();
    if(conn == nullptr){
        return DBStatus::CONNECTION_ERROR;
    }
    const char* params[2];
    params[0] = username;
    params[1] = password;
    PGresult* PGres = PQexecParams(conn, //pgconn
                                   "SELECT COUNT(*) FROM USERS_TABLE u WHERE u.username = $1 AND u.password =  $2", //query
                                   2, //number of params 
                                   NULL, //paramtypes
                                   params, //actual param array
                                   NULL,//lengths, //lengths, //array for length of binary params
                                   NULL, //formats, //param formats. 0 for text 1 for binary
                                   1 //result format 0 for text 1 for binary
                                   );

    //ExecStatusType result_status = PQresultStatus(PGres);
    //cout << "result status for check_password " << (PQresStatus(result_status)) << endl;
    if (PQresultStatus(PGres) == PGRES_TUPLES_OK){//implies that a tuple existed as opposed to PGRES_COMMAND_OK? WRONG! 
        if(PQgetvalue(PGres, 0,0) == 1){
            d("valid user and pw combo");
            PQclear(PGres);
            PQfinish(conn);
            return DBStatus::OK;
        }else{
            d("invalid user and pw combo");
            PQclear(PGres);
            PQfinish(conn);
            return DBStatus::INVALID_USER;
        }      
    }else{
        d("pg failed");
        PQclear(PGres);
        PQfinish(conn);
        return DBStatus::CONNECTION_ERROR;
    }
}

DBStatus DB::check_password(const char* username, const uint32_t password){
    PGconn* conn = DB::connect_db();
    if(conn == nullptr){
        return DBStatus::CONNECTION_ERROR;
    }
    const char* params[2];
    params[0] = username;
    params[1] = ( char*)(&password);
    int lengths[2];
    lengths[0] = 0;
    lengths[1]= sizeof(password); //0 cause first param isnt binary 4 (bytes) for a 32 bit integer
    int formats[2];
    formats[0] = 0;
    formats[1] = 1; //first is text second is binary
    PGresult* PGres = PQexecParams(conn, //pgconn
                                   "SELECT * FROM test.users u WHERE  u.username = $1 AND u.password =  $2", //query
                                   2, //number of params 
                                   NULL, //paramtypes
                                   params, //actual param array
                                   lengths, //lengths, //array for length of binary params
                                   formats, //formats, //param formats. 0 for text 1 for binary
                                   1 //result format 0 for text 1 for binary
                                   );

    //ExecStatusType result_status = PQresultStatus(PGres);
    //cout << "result status for check_password " << (PQresStatus(result_status)) << endl;
    if (PQresultStatus(PGres) == PGRES_TUPLES_OK){//implies that a tuple existed as opposed to PGRES_COMMAND_OK? WRONG! 
        if(PQntuples(PGres) == 1){
            d("valid user and pw combo");
            PQclear(PGres);
            PQfinish(conn);
            return DBStatus::OK;
        }else{
            d("invalid user and pw combo");
            PQclear(PGres);
            PQfinish(conn);
            return DBStatus::INVALID_USER;
        }      
    }else{
        d("pg failed");
        PQclear(PGres);
        PQfinish(conn);
        return DBStatus::CONNECTION_ERROR;
    }
}

    
DBStatus DB::change_password(const char* username, const uint32_t password){
    PGconn* conn = DB::connect_db();
    if(conn == nullptr){
        return DBStatus::CONNECTION_ERROR;
    }
    const char* params[2];
    params[0] = username;
    params[1] = (const char*)(&password);
    const int lengths[2] = {0, 4}; //0 cause first param isnt binary 4 (bytes) for a 32 bit integer
    const int formats[2] = {0, 1}; //first is text second is binary
    PGresult* PGres = PQexecParams(conn, //pgconn
                                   "UPDATE test.users u SET password = $2 WHERE u.username = $1", //query
                                   2, //number of params 
                                   NULL, //paramtypes
                                   (const char * const *)params, //actual param array
                                   lengths, //array for length of binary params
                                   formats, //param formats. 0 for text 1 for binary
                                   0 //result format 0 for text 1 for binary
                                   );
    //ExecStatusType result_status = PQresultStatus(PGres);
    //cout << "result status for change_password " << (PQresStatus(result_status)) << endl;
    if (PQresultStatus(PGres) == PGRES_COMMAND_OK){ 
            d("user %s updated password", username);
            PQclear(PGres);
            PQfinish(conn);
            return DBStatus::OK;
    }else{
        d("pg failed on change password for user, ", username);
        PQclear(PGres);
        PQfinish(conn);
        return DBStatus::CONNECTION_ERROR;
    }
}

DBStatus DB::delete_user(const char* username){
    PGconn* conn = DB::connect_db();
    if(conn == nullptr){
        return DBStatus::CONNECTION_ERROR;
    }
    const char* params[1];
    params[0] = username;//can i just use &username ???? TODO try this
    PGresult* PGres = PQexecParams(conn, //pgconn
                                   "DELETE FROM USERS_TABLE u WHERE u.username = $1", //query
                                   1, //number of params 
                                   NULL, //paramtypes
                                   (const char * const *)params, //actual param array
                                   NULL,//lengths, //array for length of binary params
                                   NULL,//formats, //param formats. 0 for text 1 for binary
                                   1 //result format 0 for text 1 for binary
                                   );
    ExecStatusType result_status = PQresultStatus(PGres);
    cout << "result status for remove_user " << (PQresStatus(result_status)) << endl;
    if (result_status == PGRES_COMMAND_OK){ 
            printf("removed user %s \n", username);
            PQclear(PGres);
            PQfinish(conn);
            return DBStatus::OK;
    }else{
        d("pg failed on deleting user, ", username);
        PQclear(PGres);
        PQfinish(conn);
        return DBStatus::CONNECTION_ERROR;
    }
}

DBStatus DB::add_user(const char* username, const char* password){
    PGconn* conn = DB::connect_db();
    if(conn == nullptr){
        return DBStatus::CONNECTION_ERROR;
    }
    const char* params[2];
    params[0] = username;
    params[1] = password;
    PGresult* PGres = PQexecParams(conn, //pgconn
                                   "SELECT * from add_user($1, $2)", 
                                   2, //number of params 
                                   NULL, //paramtypes
                                   (const char * const *)params, //actual param array
                                   NULL,NULL,// no lengths or formats of the parameters since they are strings
                                   1 //result format 0 for text 1 for binary
                                   );
    ExecStatusType result_status = PQresultStatus(PGres);
    cout << "result status for add_user " << (PQresStatus(result_status)) << endl;
    if (result_status == PGRES_TUPLES_OK){ 
        int add_user_status = ntohl(*(int*)(PQgetvalue(PGres,0,0)));
        if(add_user_status == DBStatus::OK ){
            d("successfully added user %s \n", username);
        }else{
            d("user %s exists\n", username);
        }
            PQclear(PGres);
            PQfinish(conn);
            return add_user_status;
    }else{
        d("pg failed on adding user, ", username);
        PQclear(PGres);
        PQfinish(conn);
        return DBStatus::CONNECTION_ERROR;
    }
}


DBStatus DB::add_user(const char* username, const uint32_t password){
    PGconn* conn = DB::connect_db();
    if(conn == nullptr){
        return DBStatus::CONNECTION_ERROR;
    }
    const char* params[2];
    params[0] = username;
    params[1] = (const char*)(&password);
    const int lengths[2] = {0, 4}; //0 cause first param isnt binary 4 (bytes) for a 32 bit integer
    const int formats[2] = {0, 1}; //first is text second is binary
    PGresult* PGres = PQexecParams(conn, //pgconn
                                   "SELECT * from add_user($1, $2)", //query
                                   2, //number of params 
                                   NULL, //paramtypes
                                   (const char * const *)params, //actual param array
                                   lengths, //array for length of binary params
                                   formats, //param formats. 0 for text 1 for binary
                                   1 //result format 0 for text 1 for binary
                                   );
    ExecStatusType result_status = PQresultStatus(PGres);
    cout << "result status for add_user " << (PQresStatus(result_status)) << endl;
    if (result_status == PGRES_TUPLES_OK){ 
        int x = ntohl(*(int*)(PQgetvalue(PGres,0,0)));
        printf("result %i \n", x);
        if(x ){
            printf("added user %s \n", username);
            PQclear(PGres);
            PQfinish(conn);
            return DBStatus::OK;
        }else{
        
            printf("user %s exists\n", username);
            PQclear(PGres);
            PQfinish(conn);
            return DBStatus::USER_EXISTS;
        }
    }else{
        d("pg failed on adding user, ", username);
        PQclear(PGres);
        PQfinish(conn);
        return DBStatus::CONNECTION_ERROR;
    }
}

// caller must free return value if not nullptr
PGconn* DB::connect_db(){
    const char* connection_params = "dbname = DATABASE_NAME";// website3";
    PGconn* pgc = PQconnectdb(connection_params);
    if (PQstatus(pgc) != CONNECTION_OK){
        cout << "conn failed because " << PQerrorMessage(pgc) << endl;
        PQfinish(pgc);
        d("connection to database failed");
        return nullptr;
    }
    return pgc;
}
