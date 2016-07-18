#ifndef NET_NET_H
#define NET_NET_H

#include <stdio.h>
#include <net/socket.h>
#include <battle.h>
#include <field.h>
#include <ship.h>
#include <gl.h>
#include <set>


#define BUFF_LEN 1024 * 1024

#define MSG_HELLO 0
#define MSG_CHECK 1
#define MSG_READY 2
#define MSG_SHOT  3
#define MSG_SURRENDER 4
#define MSG_UPDATE 5
#define MSG_BOC   6

#define MAX_NOT_UPDATE 5

#define OK 1

struct client_t {
    field F;
    ship* ships;
    char name[128];
    long double last_update;
    int num, mode, battle_idx, name_len, best_opponent;
    bool alive, field_correct, is_ready, can_go;

    client_t();
    client_t(int n);
    client_t(int n, int battle);
    void fill_in(char *src);
};


extern int client_count;
extern client_t clients[128];
extern std::set<int> unused_numbers;
extern bool is_unused_number[128];
int init_net();

int update_net();
#endif //NET_NET_H

