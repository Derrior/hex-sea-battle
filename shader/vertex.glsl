attribute vec2 v_coord;
uniform mat3 World, camera;
uniform mat2 rotate;
uniform float scale;
varying vec3 new_c;


void main(void) {
    new_c = camera * (World * vec3(rotate * scale * v_coord, 1.0));
    gl_Position = vec4(new_c.x / 683 - 1, new_c.y / 384 - 1, 1, 1); 
}
