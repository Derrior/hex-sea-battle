#ifndef NET_NET_H
#define NET_NET_H

#include <stdio.h>
#include <net/socket.h>
struct client_t {
    int tcp;
    char alive;
};
int init_net();
int update_net();
#endif //NET_NET_H

