#ifndef ENGINE_H
#define ENGINE_H

#include <gl.h>
#include <iostream>

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
                    used[k] = i;
                    may_be_near[i] = false;
                }
            }
            vec neighbour(0, 29);
            for (int m = 0; m < 6; m++) {
                point curr_neighbour = curr_centre + neighbour.rotate(matrixes[m]);
                for (int k = 0; k < amount_of_polygons; k++) {
                    if (in_polygon(curr_neighbour, Field[k])) {
                        if (!may_be_near[k] and used[k] != j) {
                            r.bombs.push_back(k);
                        }
                        //std::cout << i << ' ' << j << ' ' << k << " (" << curr_centre.x << "; " << curr_centre.y << ")\n";
                        used[k] = i;
                    }
            }

            }
        }
    }
    return (r.bombs.empty());
}

#endif //ENGINE_H
