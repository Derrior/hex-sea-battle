#include <init_world.h>
#include <ctime>
#include <gl.h>
using namespace std;

void init_matrixes() {
    for (int i = 0; i < 6; i++) {
        matrixes[i][0] = matrixes[i][3] = cosf(i * (M_PI / 3));
        matrixes[i][1] = -sinf(i * (M_PI / 3));
        matrixes[i][2] = -matrixes[i][1];
    }
}

void set_ship(int& x, int& y, field& F) {
    for (int i = 0; i < amount_of_polygons; i++) {
        if (in_polygon(point(x - F.move.m[2], y - F.move.m[5]), Field[i])) {
            x = Field[i].centre.x + F.move.m[2];
            y = Field[i].centre.y + F.move.m[5];
        }
    }
}


void create_field_vbo() {

    float points[7 * amount_of_polygons][4];
    vector<int> ibo;
    for (int i = 0; i < amount_of_polygons; i++)
    {
        draw_polygon(Field[i], points, i * 7, ibo);
    }
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    unsigned int ibo_data[ibo.size()];
    for (int i = 0; i < (int)ibo.size(); i++) {
        ibo_data[i] = ibo[i];
    }
    glGenBuffers(1, &ibo_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibo_data), ibo_data, GL_STATIC_DRAW);
    ibo_size = ibo.size();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    create_ships();
    //glutSetCursor(0, 0);
}


void init_fields() {
    Field = gen_field(10, 10, 1);
    amount_of_polygons = 100;
    World.m[2] = -100;
    World.m[5] = -100;
    field1 = field(amount_of_polygons);
    field2 = field(amount_of_polygons);
    field1.move.m[2] = field1.move.m[5] = field2.move.m[5] = 40;
    field2.move.m[2] = 860;
    field1.bombs.push_back(19);
}

void init_audio() {
    if (!SDL_LoadWAV("sum.wav", &wav_spec, &wav_buffer,&wav_length)) {
        play_audio = false;;
    };
    if (play_audio) {
        wav_spec.callback = my_audio_callback;
        wav_spec.userdata = NULL;
        audio_pos = wav_buffer;
        audio_len = wav_length;

        SDL_OpenAudio(&wav_spec, NULL);
        SDL_PauseAudio(0);
    }

}

void init_buttons() {
    string name = "check!", n2 = "switch";
    button b1(200, 50, name);
    button b2(100, 670, n2);
    b1.register_callback([&time_last_check, &bombs_removed](){
        time_last_check = time(NULL);
        bombs_removed = false;
        check(field1, ships);
    });
    b2.register_callback([](){
        colorscheme = (colorscheme + 1) % AMOUNT_COLORSCHEMES;
    });
    buttons.push_back(b1);
    buttons.push_back(b2);
}
