
using namespace std;
int main(){
    DBStatus DB::check_password(const char* username, const char* password){
    PGconn* conn = DB::connect_db();
    if(conn == nullptr){
        return DBStatus::CONNECTION_ERROR;
    }
    const char* params[2];
    params[0] = username;
    params[1] = password;
    /*int lengths[2];
    lengths[0] = 0;
    lengths[1]= si; //0 cause first param isnt binary 4 (bytes) for a 32 bit integer
    int formats[2];
    formats[0] = 0;
    formats[1] = 1; //first is text second is binary
    */
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
}
