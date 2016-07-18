#include <gl.h>
#include <net/net.h>
#include <ctime>
using namespace std;


polygon* Field;
int amount_of_polygons, amount_of_ships, curr_ship;
ship* ships;
field field1, field2;
long double curr_time;
long long time_last_check;
vector<battle> battles;

int update_all() {
    for (int i = 0; i < 128; i++) {
        if (is_unused_number[i] or clients[i].mode != INIT_MODE) {
            continue;
        }
        if (clients[i].is_ready) {
//            cout << i << endl;
        }
        int best_o = clients[i].best_opponent;
        if (best_o < 0 or is_unused_number[best_o]) {
            clients[i].best_opponent = -1; 
            continue;
        }
        if (clients[best_o].best_opponent == i and clients[best_o].is_ready and clients[i].is_ready) {
            clients[best_o].can_go = clients[i].can_go = true;
            clients[best_o].is_ready = clients[i].is_ready = false;

            battles.push_back(battle(i, best_o));
        } else if (clients[best_o].best_opponent != i) {
            clients[i].is_ready = clients[i].can_go = false;
        }
    }
    for (int i = 0; i < (int)battles.size(); i++) {
        if (clients[battles[i].client_1].is_ready and clients[battles[i].client_2].is_ready) {
            clients[battles[i].client_1].can_go = clients[battles[i].client_2].can_go = true;
            clients[battles[i].client_1].is_ready = clients[battles[i].client_2].is_ready = false;
        }

    }
    return 0;
}

int main()
{
    curr_time = time(NULL);
    init_matrixes();
    init_ship_object();
    init_fields();
    init_colors();
    cout << "beforeft" << endl;
    create_field_vbo();
    cout << "created" << endl;
    init_net();
    while (1) {
        curr_time = time(NULL);
        update_net();
        update_all();
    }
    return 0;
}

