#include <battle.h>

int number_battles;

battle::battle() {
    idx = number_battles++; 
}

battle::battle(int a, int b) {
    client_1 = a;
    client_2 = b;
    idx = number_battles++; 
}
