attribute vec2 v_coord;

void main(void) {
    gl_Position = vec4(v_coord, 1, 1); 
}
