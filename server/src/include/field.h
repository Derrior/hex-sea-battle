#ifndef FIELD_H_
#define FIELD_H_
#include <vector>
#include <memory>
#include <polygon.h>
#include <math_3d.h>

std::vector<polygon> gen_field(int w, int h, int rad=CELL_RAD);
std::vector<polygon> gen_ship(int w);
void to_float(float arr[][4], int& idx, point c);
void draw_polygon(polygon& p, float vbo_data[][4], int idx, std::vector<unsigned int>& ibo);
struct field {
    Matrix3f move;
    std::vector<int> bombs;
    std::vector<int> aqua;
    bool* used;
    int* containing;
    int size;
    field() {
        used = NULL;
    }

    field(int am) {
        used = new bool[am];
        containing = new int[am];
        memset(used, 0, am);
        size = am;
    }

    ~field() {
        delete[] used;
        delete[] containing;
    }
    void use_cell(int cell_idx);
    void use_all_cells();
    int contain_ship(int cell_idx);
    char* print_field(char* msg); 
    char* write_field(char* msg); 
};
#endif //FIELD_H_
