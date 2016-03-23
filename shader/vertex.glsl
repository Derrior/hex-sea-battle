attribute vec2 v_coord;
uniform mat3 World;
void main(void) {
    vec3 new_c = World * vec3(v_coord, 1.0);
    gl_Position = vec4(new_c.x / 683 - 1, new_c.y / 384 - 1, 1, 1); 
}
