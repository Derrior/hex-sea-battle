attribute vec2 v_coord;

void main(void) {
    gl_Position = vec4(v_coord.x / 683 - 1, v_coord.y / 384 - 1, 1, 1); 
}
