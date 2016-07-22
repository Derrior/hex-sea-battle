#ifndef BATTLE_H
#define BATTLE_H
#include <net/net.h>
extern int number_battles;

struct battle {
    int client[2];
    int idx;
    int turn;
    battle();
    battle(int a, int b);
    void set_is_ready(bool val);
    void set_can_go(bool val);
    bool clients_ready();
    bool my_turn(int client);
    int other(int client);
};

#endif //BATTLE_H
