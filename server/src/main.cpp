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
        int best_o = clients[i].best_opponent;
        if (best_o < 0 or is_unused_number[best_o]) {
            clients[i].best_opponent = -1; 
            continue;
        }
        if (clients[best_o].best_opponent == i) {
            clients[best_o].can_go = clients[i].can_go = true;
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

