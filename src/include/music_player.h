#ifndef PLAYER_H
#define PLAYER_H

#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>
#include <cstdlib>
extern SDL_Surface* Surface;

extern unsigned char* wav_buffer;
extern unsigned int wav_length;
extern unsigned int audio_len;
extern unsigned char* audio_pos;

int min_32t(int a, int b);

int sdl_init();


void my_audio_callback(void *, Uint8 *stream, int len);
#endif //PLAYER_H
