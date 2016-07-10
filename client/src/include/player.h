#ifndef PLAYER_H
#define PLAYER_H

struct player {
    int name_len;
    char name[128];
    char want_to_play;
    bool is_ready;
};

#endif //PLAYER_H
