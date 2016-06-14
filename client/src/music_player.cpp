#include <music_player.h>

using namespace std;

SDL_Surface* Surface;

unsigned char* wav_buffer;
unsigned int wav_length;
unsigned int audio_len;
unsigned char* audio_pos;

int min_32t(int a, int b) {
    return (a < b ? a : b);
}

int sdl_init() {
    return SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
}

void my_audio_callback(void *, Uint8 *stream, int len) {
    
    if (audio_len == 0) {
        audio_pos = wav_buffer;
        audio_len = wav_length;
    }
    len = min_32t(len, audio_len); 
    SDL_memcpy(stream, audio_pos, len);  
    
    audio_pos += len;
    audio_len -= len;
}

