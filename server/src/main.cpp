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
    }
    return 0;
}

