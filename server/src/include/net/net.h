#ifndef NET_NET_H
#define NET_NET_H

#include <stdio.h>
#include <net/socket.h>
#include <field.h>
#include <ship.h>
#include <gl.h>


#define BUFF_LEN 1024 * 1024

#define MSG_HELLO 0
#define MSG_CHECK 1

#define OK 1

struct client_t {
    field F;
    ship* ships;
    char num;
    char alive;
    client_t();
    client_t(int n);
    void fill_in(char *src);
};


int init_net();

int update_net();
#endif //NET_NET_H

