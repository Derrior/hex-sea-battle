#include <init_world.h>
#include <net/net.h>
#include <vector>
#include <ctime>
#include <gl.h>
using namespace std;


int amount_of_polygons, amount_of_ships, curr_ship;
field field1, field2;
long double curr_time;
long long time_last_check;
vector<polygon> Field;
vector<ship> ships;
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

            battles.push_back(battle(i, best_o));
            battles.back().set_can_go(true);
            battles.back().set_is_ready(false);
            clients[i].battle_idx = clients[best_o].battle_idx = battles.back().idx;
        } else if (clients[best_o].best_opponent != i) {
            clients[i].is_ready = clients[i].can_go = false;
        }
    }
    for (int i = 0; i < (int)battles.size(); i++) {
        if (battles[i].clients_ready()) {
            battles[i].set_can_go(true);
            battles[i].set_is_ready(false);
        }
        if (!clients[battles[i].client[0]].has_ship() or !clients[battles[i].client[1]].has_ship()) {
            clients[battles[i].client[0]].F.use_all_cells();
            clients[battles[i].client[1]].F.use_all_cells();
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
    cout << "beforeft" << endl;
    create_field_vbo();
    cout << "created" << endl;
    init_net();
    long double last_fps_time = 0;
    int fps_counter = 0;
    while (1) {
        curr_time = (long double)clock() / CLOCKS_PER_SEC;
        fps_counter++;
        if (curr_time - last_fps_time > 1) {
            cout << "fps: " << fps_counter << endl;
            last_fps_time = curr_time;
        }
        update_net();
        update_all();
    }
    return 0;
}

