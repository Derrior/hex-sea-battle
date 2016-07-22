#include <battle.h>
#include <cstdlib>

int number_battles;

battle::battle() {
    idx = number_battles++; 
}

battle::battle(int a, int b) {
    client[0] = a;
    client[1] = b;
    turn = rand() % 2;
    idx = number_battles++; 
}

void battle::set_can_go(bool val) {
    clients[client[0]].can_go = clients[client[1]].can_go = val;
}

void battle::set_is_ready(bool val) {
    clients[client[0]].is_ready = clients[client[1]].is_ready = val;
}

bool battle::clients_ready() {
    return clients[client[0]].is_ready and clients[client[1]].is_ready;
}

int battle::other(int one) {
    if (one == client[0]) {
        return client[1];
    } else if (one == client[1]) {
        return client[0];
    } else {
        return -1;
    }
}

bool battle::my_turn(int me) {
    return (me == client[0]) ^ turn;
}
