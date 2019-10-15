#ifndef __RESPOND__H__
#define __RESPOND__H__
#include "protocol.h"
using namespace std;
void send_cookie(int, char*);
void send_ack(int);
void send_login_ack(int);
void send_nack(int);
void send_error(int, ERROR);
#endif
