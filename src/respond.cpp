//Hooshing Schaefer 2019

#include <unistd.h>
#include <string.h>
#include "respond.h"
#include "debug.h"
using namespace std;

constexpr char ACK_STR[] =  "HTTP/1.1 200 OK\r\n"
                            "Access-Control-Allow-Origin: http://35.197.22.120\r\n"
                            "Access-Control-Allow-Credentials: true\r\n\r\n\1";

constexpr char LOGIN_ACK_STR[] =  "HTTP/1.1 200 OK\r\n"
                            "Access-Control-Allow-Origin: http://35.197.22.120\r\n"
                            "Access-Control-Allow-Credentials: true\r\n\r\n\1";

constexpr char NACK_STR[] = "HTTP/1.1 200 OK\r\n"
                            "Access-Control-Allow-Origin: http://35.197.22.120\r\n"
                            "Access-Control-Allow-Credentials: true\r\n\r\n\2\0\0\0";

constexpr char IMPROPER_FORMAT_STR[] = "HTTP/1.1 200 OK\r\n"
                                       "Access-Control-Allow-Origin: http://35.197.22.120\r\n"
                                       "Access-Control-Allow-Credentials: true\r\n\r\n\3\0\0\0";

constexpr char NO_COOKIE_STR[] = "HTTP/1.1 200 OK\r\n"
                                 "Access-Control-Allow-Origin: http://35.197.22.120\r\n"
                                 "Access-Control-Allow-Credentials: true\r\n\r\n\5\0\0\0";
constexpr char BAD_USERNAME_STR[] = "HTTP/1.1 200 OK\r\n"
                                    "Access-Control-Allow-Origin: http://35.197.22.120\r\n"
                                    "Access-Control-Allow-Credentials: true\r\n\r\n\4\0\0\0";

//constexpr char USER_TAKEN[] = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\n\r\5\0\0\0";
constexpr char COOKIE_HEADER[] = "HTTP/1.1 200 OK\r\n"
                                 "Access-Control-Allow-Origin: http://35.197.22.120\r\n"
                                 "Access-Control-Allow-Credentials: true\r\n"
                                 "Set-Cookie: hauth=";
constexpr char COOKIE_CLOSE[] = "\r\n\r\n\1\0\0\0";

void send_error(int sock, ERROR e){
    switch(e){
        case ERROR::IMPROPER_FORMAT : 
            write(sock, IMPROPER_FORMAT_STR, strlen(IMPROPER_FORMAT_STR));
            return;
        case ERROR::BAD_USERNAME :
        case ERROR::BAD_PASSWORD:
            write(sock, BAD_USERNAME_STR, strlen(BAD_USERNAME_STR));
            return;
        case ERROR::NO_COOKIE :
            write(sock, NO_COOKIE_STR, strlen(NO_COOKIE_STR));
            return;
        default:
            write(sock, NACK_STR, strlen(NACK_STR));
            return;
    }
}

void send_cookie(int sock, char* cookie){
    char response[COOKIE_LEN + strlen(COOKIE_HEADER) + strlen(COOKIE_CLOSE)];
    memcpy(response, COOKIE_HEADER, strlen(COOKIE_HEADER));
    memcpy(response + strlen(COOKIE_HEADER), cookie, COOKIE_LEN);
    memcpy(response + strlen(COOKIE_HEADER) + COOKIE_LEN , COOKIE_CLOSE, strlen(COOKIE_CLOSE));
    d("wrote cookie");
    write(sock, response, sizeof(response));
}

void send_nack(int sock){
    write(sock, NACK_STR, strlen(NACK_STR));
}

void send_login_ack(int sock){
    write(sock, LOGIN_ACK_STR, strlen(LOGIN_ACK_STR));
}

void send_ack(int sock){
    write(sock, ACK_STR, strlen(ACK_STR));
}
