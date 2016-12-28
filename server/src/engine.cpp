#include <gl.h>
#include <ship.h>
#include <math.h>
#include <iostream>


using namespace std;

int get_cell_idx(point a) {

    for (int i = 0; i < amount_of_polygons; i++) {
        if (in_polygon(a, Field[i])) {
            return i;
        }
    }
    return -1;
}

vector<int> get_neighbours(int i) {
    cout << i << " : ";
    vector<int> res;
    if (i %  2 == 0) {
        if ((i + 1) % 10 != 0) {
            res.push_back(i + 1);
            if (i - 9 >= 0)
                res.push_back(i - 9);
        }
        if (i % 10 != 0) {
            res.push_back(i - 1);
            if (i - 11 >= 0)
                res.push_back(i - 11);
        }
        if (i + 10 < amount_of_polygons) {
            res.push_back(i + 10);
        }
        if (i - 10 >= 0) {
            res.push_back(i - 10);
        }
    } else {
        if ((i + 1) % 10 != 0) {
            res.push_back(i + 1);
            if (i + 9 < amount_of_polygons)
                res.push_back(i + 9);
        }
        if (i % 10 != 0) {
            res.push_back(i - 1);
            if (i + 11 < amount_of_polygons)
                res.push_back(i + 11);
        }
        if (i + 10 < amount_of_polygons) {
            res.push_back(i + 10);
        }
        if (i - 10 >= 0) {
            res.push_back(i - 10);
        }

    }

    for (int c : res) {
        cout << c << ' ';
    }
    cout << endl;
    return res;
}

void get_triangle(int idx, vector<point> &res) {
    res.resize(3);
    res[0] = point(ship_vbo_data[ship_ibo_data[idx]][0], ship_vbo_data[ship_ibo_data[idx]][1]);
    res[1] = point(ship_vbo_data[ship_ibo_data[idx + 1]][0], ship_vbo_data[ship_ibo_data[idx + 1]][1]);
    res[2] = point(ship_vbo_data[ship_ibo_data[idx + 2]][0], ship_vbo_data[ship_ibo_data[idx + 2]][1]);
}

int check(field& r, const vector<ship>& ships_to_check) {
    r.bombs.clear();
    int used[amount_of_polygons];

    int may_be_near[amount_of_polygons];
    for (int i = 0; i < amount_of_polygons; i++) {
        used[i] = -1;
        may_be_near[i] = -1;

    }
    int used_cell_am = 0;
    std::cout << "checking begin!\n";
    for (int i = 0; i < amount_of_ships; i++) {
        for (int j = 0; j < ships_to_check[i].strength; j++) {
            point curr_centre = ships_to_check[i].get_point(j);
            curr_centre.x -= r.move.m[2];
            curr_centre.y -= r.move.m[5];
            int k = get_cell_idx(curr_centre);
            if (k == -1) {
                continue;
            }
            if (used[k] != -1 and used[k] != i) {
                r.bombs.push_back(k);
            }
            used[k] = i;
            may_be_near[k] = i;
            used_cell_am++;
            vec neighbour(0, 45);
            for (int m = 0; m < 6; m++) {
                point curr_neighbour = curr_centre + neighbour.rotate(matrixes[m]);
                k = get_cell_idx(curr_neighbour);
                if (k == -1) {
                    continue;
                }
                if (may_be_near[k] != -1 and used[k] != i) {
                    r.bombs.push_back(k);
                }
                if (used[k] == -1) {
                    used[k] = i;
                
                }
            }
        }
    }
    if (r.bombs.size() == 0) {
        memcpy(r.containing, may_be_near, amount_of_polygons * sizeof(int));
    }
    return (r.bombs.size() == 0);// && used_cell_am == CELLS_AT_ALL;
}


void next_mode(int &our_mode) {
    if (our_mode == INIT_MODE) {
        our_mode = SHIP_MODE;
    } else if (our_mode == SHIP_MODE) {
        our_mode = BATTLE_MODE;
    } else {
        our_mode = INIT_MODE;   
    }

}
