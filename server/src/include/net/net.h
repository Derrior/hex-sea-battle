#ifndef NET_NET_H
#define NET_NET_H
#define CHECK 1
#define HELLO 0
#define OK 1
#include <stdio.h>
#include <net/socket.h>
#include <field.h>
#include <ship.h>
struct client_t {
    field F;
    ship* ships;
    char num;
    char alive;
};
int init_net();
int update_net();
#endif //NET_NET_H

