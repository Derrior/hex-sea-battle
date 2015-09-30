attribute vec2 v_coord;

void main(void) {
    gl_Position = vec4(v_coord.x / (13.66 * 2) - 0.5, v_coord.y / (7.68 * 2) - 0.5, 1, 1); 
}
