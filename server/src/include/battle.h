#ifndef BATTLE_H
#define BATTLE_H

extern int number_battles;

struct battle {
    int client_1, client_2;
    int idx;
    battle();
    battle(int a, int b);
};

#endif //BATTLE_H
