#ifndef __PROTOCOL__H__
#define __PROTOCOL__H__
#include <stdint.h>

constexpr char port[] =  "6970";
constexpr char COOKIE_SEARCH[] = "Cookie: hauth=";
constexpr int HEADER_LEN = 1;
constexpr int COOKIE_LEN = 16;
constexpr int MAX_USERNAME_LEN = 30;
//constexpr int MAX_LOBBY_LEN = 25;
//constexpr int BOUNDARY_LEN = 10;

enum class REQUEST : uint8_t{
    UPLOAD_FILE       = 1,
    RETRIEVE_FILE     = 2,
    LOGIN             = 3,
    SEARCH            = 4

};

enum class RESPONSE : uint8_t{
    ACK = 1,
    NACK = 2,
    BAD_USERNAME = 4
};

enum class ERROR{
    MAX_CLIENTS,
    IMPROPER_FORMAT,
    BAD_USERNAME,
    BAD_PASSWORD,
    NO_COOKIE,
    GENERIC
};


#endif
