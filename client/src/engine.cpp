#include <gl.h>
#include <ship.h>
#include <math.h>
#include <player.h>
#include <iostream>

int mode;
long double begin_switch_mode;
char shoot_cell;
bool check_pressed, go_pressed, go_allowed, shoot_pressed;

using namespace std;

int get_cell_idx(point a) {

    for (int i = 0; i < amount_of_polygons; i++) {
        if (in_polygon(a, Field[i])) {
            return i;
        }
    }
    return -1;
}


void get_triangle(int idx, point* &res) {
    res = new point[3];
    res[0] = point(ship_vbo_data[ship_ibo_data[idx]][0], ship_vbo_data[ship_ibo_data[idx]][1]);
    res[1] = point(ship_vbo_data[ship_ibo_data[idx + 1]][0], ship_vbo_data[ship_ibo_data[idx + 1]][1]);
    res[2] = point(ship_vbo_data[ship_ibo_data[idx + 2]][0], ship_vbo_data[ship_ibo_data[idx + 2]][1]);
    return;
}

bool check() {
    cout << "hey" << endl;
    time_last_check = curr_time;
    bombs_removed = false;
    check_pressed = true;    
}

bool turn(int x, int y, field& r, ship* b) {
    
    for (int i = 0; i < amount_of_ships; i++) {
        for (int j = 0; j < b[i].strength; j++) {
            //if (b[i].is_damaged[j]) {
            //    continue;
            //}
            point point_to_check(x - b[i].pos.m[2] + r.move.m[2], y - b[i].pos.m[5] + r.move.m[5]);
            //cout << point_to_check.x << ' ' << point_to_check.y << ' ' << b[i].rot <<  endl;
            point_to_check = rotate(point(0, 0), point_to_check, atan2(matrixes[b[i].rot][1], matrixes[b[i].rot][0]));
            //cout << point_to_check.x << ' ' << point_to_check.y << endl;
            for (int k = 0; k < SHIP_SIZE; k += 3) {
                point* tr;
                get_triangle(j * SHIP_SIZE + k, tr);
                //cout << tr[0].x << ' ' << tr[0].y << "; " << tr[1].x << ' ' << tr[1].y << "; " << tr[2].x << ' ' << tr[2].y << endl;
                if (in_triangle(point_to_check, tr)) {
                    point which_cell = b[i].get_point(j);
                    which_cell.x -= r.move.m[2];
                    which_cell.y -= r.move.m[5];
                    int cell_idx = get_cell_idx(which_cell);
                    if (cell_idx == -1) {
                        continue;
                    }
                    r.bombs.push_back(cell_idx);
                    //b[i].is_damaged[j] = true;
                    if (!b[i].is_alive()) {
                        bool used[amount_of_polygons];
                        memset(used, 0, amount_of_polygons);
                        for (int ship_cell = 0; ship_cell < b[i].strength; ship_cell++) {
                            used[get_cell_idx(vec(point(r.move.m[2], r.move.m[5]), b[i].get_point(ship_cell)).get_point())] = true;
                        }
                        vec neighbour(0, 45.1);
                        for (int ship_cell = 0; ship_cell < b[i].strength; ship_cell++) {
                            point curr_centre = vec(point(r.move.m[2], r.move.m[5]), b[i].get_point(ship_cell)).get_point();
                            //cout << curr_centre << endl;
                            for (int m = 0; m < 6; m++) {
                                point curr_neighbour = curr_centre + neighbour.rotate(matrixes[m]);
                                cell_idx = get_cell_idx(curr_neighbour);
                                //cout << curr_neighbour << cell_idx << endl;
                                if (cell_idx != -1 and !used[cell_idx]) {
                                    r.aqua.push_back(cell_idx);
                                }
                            }
                        }
                                    
                        
                    }
                    delete[] tr;
                    return false;
                }
                delete[] tr;
            }
        }
    }
    int cell_idx = get_cell_idx(point(x, y));
    if (cell_idx != -1) {
        r.aqua.push_back(cell_idx);
    }
    return true;
}
