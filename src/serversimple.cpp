//Hooshing Schaefer 2019
#include <fcntl.h>
//#include <iostream>
#include <errno.h>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <time.h>
//#include <string>
#include <thread>
#include <mutex>
#include <fstream>
#include <vector>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <poll.h>
#include <signal.h>
#include <netinet/in.h>
#include <netdb.h>
#include <libpq-fe.h>
#include "respond.h"
#include "debug.h"
#include "cookies.h"
#include "db.h"
#include "users.h"
#include <curl/curl.h>
using namespace std;
/*  use uint8_t uint16_t data types
 *
 * */
constexpr int MAX_CLIENTS = 50;
constexpr int BUF_SIZE = 16000;
mutex mtx;
/* 
+-----------------------------+-----------------------------+-----------------------------+-----------------------------+
|           message           |           flags/            |   least significant byte    |    most significant byte    |
|            type             |     extention of message    |       of payload size       |       of payload size       |     
+-----------------------------+-----------------------------+-----------------------------+-----------------------------+
|          payload            |          payload            |          payload            |          payload            |
|                             |                             |                             |                             |     
+-----------------------------+-----------------------------+-----------------------------+-----------------------------+
*/



/* cookie is required to be null terminated
 * returns 1 if db wasnt updated properly
 * returns 0 if db is updated successfully
 * interacts with database, doesnt send message to client--this is caller's job
 * */
/*int bind_username_to_cookie(char* username, char* cookie, int sock) {
        
    char* params[2];
    params[0] = cookie;
    params[1] = username;
    PGresult* PGres = PQexecParams(conn, "SELECT register_user($1, $2)",
                                   2, NULL, (const char * const *)params, NULL, NULL, 0);
    ExecStatusType PGstatus = PQresultStatus(PGres);
    PQclear(PGres);
    if (PGstatus == PGRES_TUPLES_OK){
        d("PG success");// , PQgetvalue(PGres, 0, 1));
        //send_cookie(sock, cookie);
        return 0;
    }else{
        d("pg failed");
        //send_nack(sock);
        return 1;
    }
}
*/
//complete
int handle_login(char* start, char* cookie_find, int sock){
    int user_len = *(uint16_t*)(start + 2) ;
    if (user_len > MAX_USERNAME_LEN or user_len == 0){
        send_error(sock, ERROR::BAD_USERNAME);
        return 0;
    }
    char user[user_len+1];
    user[user_len] = '\0';
    uint32_t pw_hash = *(uint32_t*)(start + 4 + user_len); 
    //start_of_data += HEADER_LEN;
    memcpy(user, start + 4, user_len);
    d("handling user %s. len=%d", user, user_len);
    char cookie [COOKIE_LEN + 1];
    cookie[COOKIE_LEN] = '\0';
    //char* cookie_find = memmem(buf, bytes_read, COOKIE_SEARCH, strlen(COOKIE_SEARCH));
    //if (cookie_find == NULL){
    if(DBStatus::OK != DB::check_password(user, pw_hash)){
       //remove this once registration is complete
        DB::add_user(user, pw_hash);
        d("adding new user");
        //send_error(sock, ERROR::BAD_PASSWORD);
        //return 0;
    }
    //d("good password...generating cookie");
    Cookies::generate_cookie(cookie);
    UsersManager* um = UsersManager::get_instance();
    if (!um->add_user( cookie, user)){
        send_nack(sock);
    }else{
        send_cookie(sock, cookie);
    }
    
    return 0;
}
/*
int make_lobby_in_database(char* owner, char* name, PGconn* conn){
    char* params[2];
    params[0] = ;
    PGresult res = PQexecParams(conn, "INSERT INTO lobbies values($1, $2, 1)", 2,
                                NULL, (const char * const *)params, NULL, NULL, 0);
    ExecStatusType status = PQresultStatus(res);
    if (status == PGCOMMAND_OK){
        return 0;
    }
    return 1;
}*/
/*

class StringWithLen {
    public:
        StringWithLen(const char* s, const int l): str(s), len(l){} 
        const char* str;
        const int len;
};*/

//TODO this must be rewritten
//caller must free the block if it isnt null
//returns the char* username associated with  the cookie or NULL if the cookie is invalid
/*StringWithLen match_cookie_to_user(char* cookie){
    char* params[1];
    params[0] = cookie;
    PGresult* res = PQexecParams(conn, "SELECT username FROM users WHERE userid=$1",
                                   1, NULL, (const char * const *)params, NULL, NULL, 0);
    ExecStatusType stat = PQresultStatus(res);
    char* ret = NULL;
    if (stat == PGRES_TUPLES_OK && PQntuples(res) == 1){
        ret = PQgetvalue(res, 0, 0);
        int len = strlen(ret);
        char* real_ret = (char*)malloc(len + 1);
        strcpy(real_ret, ret);
        PQclear(res);
        return {real_ret, len};
    }
    //int len = strlen(ret);
    //char* real_ret = malloc(strlen(ret) + 1);
      //  strcpy(real_ret, ret);
        
    PQclear(res);
    return {ret,0 };
}
*/
/*
//TODO FIND OUT IF ITS BETTER TO WRITE TO MEMORY AND COALESCE WRITES OR NOT TO COPY AND JUST SEND A LOT OF WRITE OPERATIONS 
int verify_file(char* start){
    char buff[2048];
    char* cur = start;
    //int count 
    cur = strchr(cur,"\"");
    switch(){}
    strtok(start, ":,");






}*/


    
//extract and make sure both list_len and full_tree_len are positive nonzero values
//return value points to the char following the full_tree_len string representation of the number
/*char* extract_and_verify_file_size(int* list_len, int* full_tree_len, char* start){
    char* next ;
    //                          start,   char after, base    
    *list_len = (int)strtoul(start + 2, &next, 10);
    if(*list_len == 0){
        send_nack(sock);
        return 0;
    } 
    *full_tree_len = (int)strtoul(next + 1, &next, 10);
    if (full_tree_len == 0){
        send_nack(sock);
        return 0;
    }
    return next;
}*/
/*
bool write_to_file(){
    
}*/

// request format as follows
//  REQUEST.UPLOAD_FILE  variable number as string SP variable number as string   inp inp2  
//  <name> NL 
//  owner is deduced from cookie
//  precondition: cookie_find is not null
int handle_upload_file(char* start, char* cookie, int sock, int bytes_left){
    int bytes_read;
    d("handle upload_file");
        /*
    char cookie[COOKIE_LEN + 1];
    cookie[COOKIE_LEN] = '\0';
    memcpy(cookie, cookie_find + strlen(COOKIE_SEARCH), COOKIE_LEN);
*/
    //StringWithLen swl =match_cookie_to_user(cookie);
    UsersManager* um = UsersManager::get_instance(); 
    const User* user = um->lookup_user(cookie);
    //char* user = swl.str; 

    if(user == NULL){
        d("cookie did not match. received cookie: %s\n current usersmanager state:", cookie);
        um->dump();

        send_nack(sock); //send generic to not give info away
        return 0;
    }
    const char* username = user->get_username().c_str();
    d("cookie matched with user %s", username);
    //---------------------------------------------------------------------------------------------------------------------------------------------------------------HERE
    int list_len, full_tree_len;
    char* next;
    list_len = (int)strtoul(start + 2, &next, 10);
    if(list_len == 0){
        send_nack(sock);
        return 0;
    } 
    full_tree_len = (int)strtoul(next + 1, &next, 10);
    if (full_tree_len == 0){
        send_nack(sock);
        return 0;
    }

    //char* next = extract_and_verify_file_size(&list_len, &full_tree_len, start);
    d("list_len=%d", list_len);
    d("full_tree_len=%d", full_tree_len);
    d("username is %s", username);  
    /*make sure file isnt too large
    if (full_tree_len > 10000000 or list_len > 10000000){
        send_nack(sock);
        return 0;
    }*/
    int user_len = strlen(username);
    char filename_buf[MAX_USERNAME_LEN + 10 + 1];
    memcpy(filename_buf, username, user_len);
    //free(user);
    memcpy(filename_buf + user_len, "-links", 7);

    //write_to_file(filename_buf, list_len);
    int fp1 = open(filename_buf, O_CREAT | O_WRONLY);
    int expected_io;

    char buf[BUF_SIZE];
    next++;//now points to the first byte in the list object
    //write anything that was read in the initial read but not yet processed
    expected_io = bytes_left - (next-start);
    //if the entire list is read in before the end of the buffer then 
    //we only read read list_len and then move on to full_tree 
    if (expected_io >= list_len){
        d("entire list (+ maybe more) was read into buf");
        if(list_len != write(fp1, next, list_len)){
            send_nack(sock);
            close(fp1);
            return 1;
        }
        next +=list_len ;//now next points at the starting byte of full_tree
        bytes_left -= list_len;
    }else{
        d("all remianing in buf is part of the list");
        if(expected_io != write(fp1, next, expected_io)){
            send_nack(sock);
            close(fp1);
            return 1;
        }
        list_len -= expected_io;
        while (list_len > 0){
            bytes_read  = read(sock, buf, BUF_SIZE);//io =
            expected_io = bytes_read < list_len ? bytes_read : list_len;
            if (expected_io != write(fp1, buf, expected_io)){
                send_nack(sock);
                close(fp1);
                return 1;
            }
            list_len -= expected_io; 
        } 
        next = buf + expected_io;
        bytes_left = bytes_read - expected_io;
    }
    ftruncate(fp1, lseek(fp1, 0, SEEK_CUR));//truncate file 

    //at this point next is the pointer to the first byte to be written to the full_tree file
    // bytes_left is the number of bytes left in the buffer
    close(fp1);
    memcpy(filename_buf + user_len, "-full_tree", 11);
    fp1 = open (filename_buf, O_CREAT| O_WRONLY); 
    if (bytes_left >0){
        if (full_tree_len < bytes_left){
            bytes_left = full_tree_len;
        }
        if(bytes_left != write(fp1, next, bytes_left)){
            send_nack(sock);
            close(fp1);
            return 1;
        }
        full_tree_len -= bytes_left;
    }
    while (full_tree_len > 0){
        bytes_read  = read(sock, buf, BUF_SIZE);//io =
        expected_io = bytes_read < full_tree_len ? bytes_read : full_tree_len;
        if (expected_io != write(fp1, buf, expected_io)){
            send_nack(sock);
            close(fp1);
            return 1;
        }
        full_tree_len -= expected_io; 
    }
    ftruncate(fp1, lseek(fp1, 0, SEEK_CUR));
    close(fp1);

    d("finished writing file");
    
    
    //send file back 
    struct stat stat_ret;
    stat(filename_buf, &stat_ret);
    int bytes_remaining = stat_ret.st_size;
    d("bytes to be sent: %d" , bytes_remaining);

    fp1 = open(filename_buf, O_RDONLY);
    //char buf[BUF_SIZE];
    int io_bytes;
    
    //send ack header followed by the file
    send_ack(sock);
    
    while (bytes_remaining > 0){
        io_bytes = read(fp1, buf, BUF_SIZE);
        if (write(sock, buf, io_bytes) != io_bytes){
            send_nack(sock);
            close(fp1);
            return 1;
        }
        bytes_remaining -= io_bytes; 
    }
    //make sure file is exactly the size of the write
    close(fp1);
    return 0;
}


//only checks length of username, expect user to use a username
//that allows others to recognize them which means no undisplayable chars
/*bool reject_username(char* data){
    int length = (uint16_t)*(data + 2);
    return (length > 40 || length == 0);
}*/

int handle_retrieve_file(char* cookie,int sock){
    struct stat stat_ret;
    //StringWithLen swl = match_cookie_to_user(cookie);
    UsersManager* um = UsersManager::get_instance();//erwl.str;
    if (um == NULL) 
        d("UsersManager::get_instance returned null ptr. cookie for the request: %s", cookie);

    const User* user = um->lookup_user(cookie); 
    if(user == NULL){
        d("cookie %s did not match any user", cookie);
        send_nack(sock); //send generic to not give info away
        return 0;
    }
    const char* username = user->get_username().c_str();
    d("cookie matched with user %s", username);

    //make the filename string from the username string
    int user_len =  strlen(username);//swl.len;//strlen(user);
    char filename_buf[user_len + 10 + 1];
    memcpy(filename_buf, user, user_len);
    //free(user);
    memcpy(filename_buf + user_len, "-full_tree", 10);
    filename_buf[user_len + 10 ] = '\0';
    
    //get filesize if file exists
    if ( 0 != stat(filename_buf, &stat_ret)){
        send_ack(sock);
        d("could not find file with name %s", filename_buf);
        write(sock, "{}", 2);
        return 0;
    }
    int bytes_remaining = stat_ret.st_size;
    d("bytes to be sent: %d" , bytes_remaining);

    int fp = open(filename_buf, O_RDONLY);
    char buf[BUF_SIZE];
    int io_bytes;
    
    //send ack header followed by the file
    send_ack(sock);
    while (bytes_remaining > 0){
        io_bytes = read(fp, buf, BUF_SIZE);
        if (write(sock, buf, io_bytes) != io_bytes){
            send_nack(sock);
            close(fp);
            return 1;
        }
        bytes_remaining -= io_bytes; 
    }
    return 0;
}

class id_link{
    public: 
        int id;
        string link;
};

//simple callback for curl that writes to the given fd
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = write(*(int*)stream, ptr, size* nmemb);
  return written; 
}

//simple callback for curl that interfaces with debug module
static int curl_log_write(CURL *handle, curl_infotype type,
    char *data, size_t size, void *userp)
{
    char* data_buf = (char*)malloc(size+1);
    data_buf[size] = '\0';
    memcpy(data_buf, data, size);
    d("curl debugging: %s", data_buf);
    free(data_buf);
    return 0;
}

int do_curl(string filename, string url){
  CURL *curl_handle;
  /* init the curl session */
  curl_handle = curl_easy_init();

  /* set URL to get here */
  curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());

  /* Switch on full protocol/debug output while testing */
  curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);

  /* disable progress meter, set to 0L to enable and disable debug output */
  curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);

  //output of the verbose output
  curl_easy_setopt(curl_handle, CURLOPT_DEBUGFUNCTION, curl_log_write);  

  /* send all data to this function  */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
  //follow redirects
  curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);

  int f = open(filename.c_str(), O_CREAT | O_WRONLY);
  if(f != 0) {
    /* write the page body to this file handle */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA,&f);
    /* get it! */
    curl_easy_perform(curl_handle);
  }else{    
        return 1;
  }
  close(f);
  
  curl_easy_cleanup(curl_handle);
  return 0;  
}

void search_link(vector<id_link>& matched, id_link idl, const char* const username, const char* const query){
    string filename {username};
    filename += to_string(idl.id);//eg "hoosh4"
    //mtx.lock();
    //d(")
    if (access(filename.c_str(), F_OK) == -1){//0 if file exists, -1 if not file does not exist
        //cache the page to the file

        if ( 0 == do_curl(filename, idl.link)){
            d("curl ok");                 
        }else{
            d("curl fail");
        }
    }
    
    string comm = "grep -q '";
    comm += query; 
    comm += "' " ;
    comm += filename;//"grep -q 'query here' hoosh4";
    //0 exit status if line was found
    if (0 == system(comm.c_str())){
        lock_guard<mutex> lck (mtx);//.lock();
        matched.push_back(idl);
      
    }
}

int handle_search(char* start, char* cookie, int sock){
    //struct stat stater; //for checking cache time TODO
    d("begin handle_search");
    //check to see that the cookie sent is valid
    //StringWithLen user_swl = match_cookie_to_user(cookie);
    UsersManager* um = UsersManager::get_instance();
    if(um == NULL)
        d("UserManager::get_instance return nullptr for handle_search() for cookie %s", cookie);
    const User* user = um->lookup_user(cookie);
    if (user == NULL){
        d("cookie: \"%s\" did not match any user in usermanager", cookie);
        send_nack(sock); //send generic to not give info away
        return 0;
    }
    const char* username = user->get_username().c_str();
    d("valid user: %s", username);

    int i;
    
    //parse the payload and extract the query terms and the link ids to be searched
    //payload expected to be of the form "queryterm\n8 267 38 29..."
    start++;//pass over the header byte
    char* query_end = strchr(start, '\n'); //process multiple queries later
    //quick check to see that query isnt the empty string TODO make sure injection is impossible
    if (query_end - start == 1){
        send_nack(sock);
        return 0;
    }

    *query_end = '\0';
    char* next = query_end+1;
    
    d("query: %s", start);
    //grab all the link ids and put them into links_to_be_searched
    vector<int> linkids_to_be_searched{};
    while((i = strtol(next, &next, 10))!= 0){
        //cout << i << endl;
        d("linkid added: %d", i);
        linkids_to_be_searched.push_back(i);
    }
    //have to select at least one link
    if (linkids_to_be_searched.size() == 0){
        send_nack(sock);
        return 0;
    }
    d("linkids to be searched: ");
    for(auto x : linkids_to_be_searched){
        d("  %d", x );
    }
    
    //going through the link ids from the request, 
    //iterate through the links file corresponding to the user. when we find the link,
    //spawn a thread that does the following:
    //  if the webpage isnt cached then 
    //      curl the url to the file
    //  use grep to search the file 
    //  if grep exit status indicates a match, add the link to the vector
    //join all the threads and reply with the links

    vector<id_link> ret{};//this will hold the urls that contain the search query
    //links_vector = {};
    vector<thread> to_join_threads{}; //vector of threads so we can join() them
    string links_filename {username};
    links_filename += "-links";//eg hoosh-links 
    ifstream inp {links_filename}; 
    int link_num;
    string link;
    id_link idl;
    for(auto x: linkids_to_be_searched){
        //cout << x << endl;
        //INSECURE TODO
        inp >> link_num;
        while(!inp.eof() && link_num < x){
            d("linknum parsed %d", link_num);
            inp.ignore(numeric_limits<streamsize>::max(), '\n');//might have to do this twice? nope
            inp >> link_num;
        }
        if (link_num == x){
            //spawn a thread that may push a id_link to ret 
            d("link_num matched a querylink: %d", x);
            inp >> link;
            idl = {link_num, link};
            to_join_threads.push_back(thread(search_link, ref(ret), idl, username, start));
            d("thread pushed with args idl.link_num: %d, idl.link: %s, username: %s, query: %s", idl.id, idl.link.c_str(), username, start);
        } else{
            send_nack(sock);
            d("error: improperly formatted links file. sending nack"); 
            return 0;
        }
    }
    d("num threads to be joined: %d", to_join_threads.size());
    for( auto& x : to_join_threads){
        x.join();
    }
    d("all threads joined");
    //construct a string, write to socket after sending ack byte. 
    string ret_str;
    for(auto x : ret){
        ret_str +=  x.link + " ";
    }
    send_ack(sock);
    d("ret_str= %s", ret_str.c_str());
    write(sock, ret_str.c_str(), ret_str.size());
    return 0;
}


//0 is success
//  this could include errors in format or invalid data but as long as the request is processed its ok 
//1 is failure, stop the server due to possible corruption 
int process_request(int sock){//, char* req, int req_len){
    /*char buf[BUF_SIZE];
    int red, fd = open("finalog", O_WRONLY | O_CREAT);
    while ((red = read(sock, buf, BUF_SIZE)) != 0){
        write(fd, buf, red);
    } 
    close(fd);
    return 0;*/
    //processing_states.emplace(sock, STATE::INITIAL);
    char buf[BUF_SIZE + 1];
    char cookie_buf[COOKIE_LEN + 1];
    cookie_buf[COOKIE_LEN] = '\0';
    int bytes_read;
    char *start_of_data, *cookie_find;
    
    bytes_read = read(sock, buf, BUF_SIZE);
    if (bytes_read == 0) {
            //processing_states.remove;
        send_nack(sock);
        return 0;
   }
   buf[bytes_read] = 0;
        
    cookie_find = (char*)memmem(buf, bytes_read, COOKIE_SEARCH, strlen(COOKIE_SEARCH));
    if (cookie_find == NULL){
        //cookie_p = nullptr;
        //start looking for the end of the http headers from the start of the buffer
        start_of_data = strstr(buf, "\r\n\r\n");
    }else{
        //start looking for the end of the http header from the cookie end point
        start_of_data = strstr(cookie_find + strlen(COOKIE_SEARCH), "\r\n\r\n");//TODO make more robust by checking for \r\n and reading    
        //copy cookie into the cookie bufferand set the cookie pointer to the buf
        memcpy(cookie_buf, cookie_find + strlen(COOKIE_SEARCH), COOKIE_LEN);
        cookie_find = cookie_buf;
    }
    d("bytes read = %d ", bytes_read);
    //assume theres an error if the entire http header section is not read all at once
    if (start_of_data == NULL){ 
        send_error(sock, ERROR::IMPROPER_FORMAT);
        d("couldnt find start of data");
        return 0;
    }
   
    start_of_data += 4;//now actually points at start of data

    //if the first byte of the payload wasnt read the first time then read in
    if (start_of_data - buf == bytes_read){
        d("must read more");
        bytes_read = read(sock, buf, BUF_SIZE); 
        if ( bytes_read ==0 ){
            send_nack(sock);
            return 0;
        }
        start_of_data = buf;
    }

    int xt = *(uint8_t*)start_of_data;
    printf("switching on header byte: %d\n", xt);
    d("switching on header byte: %d\n", xt);

    switch((uint8_t)*start_of_data ){
        case (int)REQUEST::LOGIN:
            return handle_login(start_of_data, cookie_find, sock);
        case (int)REQUEST::RETRIEVE_FILE:
            if(cookie_find == nullptr){
                send_nack(sock);
                return 0;
            }else{
                return handle_retrieve_file(cookie_find, sock);
            }
        case (int)REQUEST::UPLOAD_FILE:
            if(cookie_find == nullptr){
                d("bad request: uploading file with no cookie");
                send_nack(sock);
                return 0;
            }else{
                return handle_upload_file(start_of_data, cookie_find, sock,  bytes_read - (start_of_data-buf));
            }
        case (int)REQUEST::SEARCH:
            if(cookie_find == nullptr){
                d("no cookie");
                send_nack(sock);
                return 0;
            }else{
                return handle_search(start_of_data, cookie_find, sock);
            }
        default: 
            //TODO send bad request reply
            break;
    }
    return 0;
}



int main(int argc, char** argv){
    //bool end_server = false;//set to true to exit main loop
    struct pollfd fds[MAX_CLIENTS];
    int listen_sd, new_sd;
    int yes = 1;
    int nfds = 1;
    int num_waiting;
    //char buf[BUF_SIZE];
    //struct sockaddr_in addr;
    //setup curl environment
    curl_global_init(CURL_GLOBAL_ALL);
    
    struct addrinfo hints, *server_info, *p;
    
    //crude way of turning on debugging
    if (argc == 2){
        debug_init();
        printf("debugging mod on");
    }    
    //fill in the array of chars for cookie
    Cookies::initialize();//ookiespopulate_cookie_string();

    srandom(time(NULL));//seed the rng
    for (int i =0; i < MAX_CLIENTS;i++){
        fds[i].events = POLLIN;
    }
    //ipv4 or ipv6. reliable data transfer, use any network address
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port, &hints, &server_info)!= 0){
        perror( "error getting address info");
        exit(1);
    }
    for(p = server_info; p != NULL; p = p->ai_next) {
        listen_sd = socket(p->ai_family, p->ai_socktype,p->ai_protocol);
        if(listen_sd == -1) {
            continue;
        }

        if (setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt() failed");
            exit(1);
        } 
        int flags = fcntl(listen_sd, F_GETFL);

        if (fcntl(listen_sd,F_SETFL, flags | O_NONBLOCK) < 0){
            perror("failure on ioctl (setting non blocking socket");
            exit(1);
        }
        if (bind(listen_sd, p->ai_addr, p->ai_addrlen) == 0) {
            break;
        }
        perror("bind() failed");
        close(listen_sd);
        exit(1);
    }
    freeaddrinfo(server_info);
    //max clients isnt saying the max clients we can hold but the max number 
    //that can be queued to connect
    if(listen(listen_sd, MAX_CLIENTS) == -1){
        perror("error on listen");
        exit(1);
    }
    
    fds[0].fd = listen_sd;
    fds[0].revents = 0;
/*
    PGconn *conn;
    //PGresult *res;
    const char* connection_params = "dbname=website2";
    conn = PQconnectdb(connection_params);
    if (PQstatus(conn) != CONNECTION_OK){
        PQfinish(conn);
        perror("connection to database failed");
        exit(1);
    }
  */ 
    printf("starting...\n");
    while(true) {
        num_waiting = poll(fds, nfds, -1);//3rd arg negative means wait indefinitely 
        if (num_waiting  < 0){
            perror("error on polling");
            exit(1);
        }
        int i=0;
        printf( "%d open fds\n", nfds );
        d( "%d open fds\n", nfds );
        while(num_waiting >0){
            //not a socket that has input
            if(fds[i].revents == 0){
                i++;
                continue;
            } 
            //is this the listening socket
            //equivalent to fds[i] == listen_fd
            if(i==0){
                printf("connection\n");
                d("connection\n");
                while((new_sd= accept(listen_sd, NULL,NULL)) > 0){
                    //if at max clients write a msg and close the socket
                    if(nfds == MAX_CLIENTS){
                        send_error(new_sd, ERROR::MAX_CLIENTS);
                        close(new_sd);
                    }else{
                        fds[nfds].fd = new_sd;
                        fds[nfds++].revents = 0;
                        int poop = fcntl(new_sd, F_GETFL);
                        printf("flags: %d\n" , poop);
                        d("flags: %d\n" , poop);
                        if (fcntl(new_sd, F_SETFL, poop & ~O_NONBLOCK) < 0){
                            perror("failure on fnctl (setting blocking socket");
                            exit(1);
                        }
                    }
                }
                //anything other than a ewouldblock is an error
                if(errno != EWOULDBLOCK){
                    perror("error accepting connection");
                    break;//end_server = true;
                }
                
            }else{
                printf("regular read\n");
                d("regular read\n");
                d("is nonblocking: %d\n" , O_NONBLOCK  & fcntl(fds[i].fd, F_GETFL));
                printf("is nonblocking: %d\n" , O_NONBLOCK  & fcntl(fds[i].fd, F_GETFL));
                if(process_request(fds[i].fd) == 0){
                    d("success on servicing request..closing socket\n");
                    close(fds[i].fd);
                    nfds--;
                    fds[i].fd = fds[nfds].fd;
                    fds[i].revents = 0;
                }else{
                    printf("failure on servicing request..stopping now\n");
                    d("failure on servicing request..stopping now\n");
                    break;//end_server = true;
                }
            }
            i++;
            num_waiting--;
        }
    }
    //PQfinish(conn);
    for(int j = 0; j < nfds;j++){
        close(fds[j].fd);
    }
    //TODO
    //any clean up that needs to be done
    curl_global_cleanup();
}



