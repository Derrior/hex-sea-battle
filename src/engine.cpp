#include <gl.h>
#include <ship.h>
#include <math.h>
#include <iostream>

using namespace std;

int get_cell_idx(point a) {
    int x = a.x, y = a.y;

    for (int i = 0; i < amount_of_polygons; i++) {
        if (in_polygon(point(x, y), Field[i])) {
            return i;
        }
    }
    return 0;
}


void get_triangle(int idx, point* &res) {
    res = new point[3];
    res[0] = point(ship_vbo_data[ship_ibo_data[idx] * 2], ship_vbo_data[ship_ibo_data[idx] * 2 + 1]);
    res[1] = point(ship_vbo_data[ship_ibo_data[idx + 1] * 2], ship_vbo_data[ship_ibo_data[idx + 1] * 2 + 1]);
    res[2] = point(ship_vbo_data[ship_ibo_data[idx + 2] * 2], ship_vbo_data[ship_ibo_data[idx + 2] * 2 + 1]);
    return;
}

bool check(field& r, ship* b) {
    r.bombs.clear();
    int used[amount_of_polygons];
    bool may_be_near[amount_of_polygons];
    for (int i = 0; i < amount_of_polygons; i++) {
        used[i] = -1;
        may_be_near[i] = true;
    }
    std::cout << "checking begin!\n";
    for (int i = 0; i < amount_of_ships; i++) {
        for (int j = 0; j < b[i].strength; j++) {
            point curr_centre = b[i].get_point(j);
            curr_centre.x -= r.move.m[2];
            curr_centre.y -= r.move.m[5];
            //std::cout << " (" << curr_centre.x << "; " << curr_centre.y << ")\n";
            for (int k = 0; k < amount_of_polygons; k++) {
                if (in_polygon(curr_centre, Field[k])) {
                    if (used[k] != -1 and used[k] != i) {
                        r.bombs.push_back(k);
                    }
                    //std::cout << i << ' ' << j << ' ' << k << " (" << curr_centre.x << "; " << curr_centre.y << ")\n";
                    //std::cout << k << std::endl;
                    used[k] = i;
                    may_be_near[k] = false;
                }
            }
            vec neighbour(0, 45);
            for (int m = 0; m < 6; m++) {
                point curr_neighbour = curr_centre + neighbour.rotate(matrixes[m]);
                for (int k = 0; k < amount_of_polygons; k++) {
                    if (in_polygon(curr_neighbour, Field[k])) {
                        if (!may_be_near[k] and used[k] != i) {
                            r.bombs.push_back(k);
                        }
                        //std::cout << i << ' ' << j << ' ' << k << " (" << curr_neighbour.x << "; " << curr_neighbour.y << ")" << 
                        //    Field[k].centre.x << ' ' << Field[k].centre.y << "\n";
                        used[k] = i;
                    }
                }
            }
        }
    }
    return (r.bombs.empty());
}

bool turn(int x, int y, field& r, ship* b) {
    
    for (int i = 0; i < amount_of_ships; i++) {
        for (int j = 0; j < b[i].strength; j++) {
            if (b[i].is_damaged[j]) {
                continue;
            }
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
                    r.bombs.push_back(get_cell_idx(which_cell));
                    b[i].is_damaged[j] = true;
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
                                int cell_idx = get_cell_idx(curr_neighbour);
                                //cout << curr_neighbour << cell_idx << endl;
                                if (!used[cell_idx]) {
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
    r.aqua.push_back(get_cell_idx(point(x, y)));
    return true;
}
