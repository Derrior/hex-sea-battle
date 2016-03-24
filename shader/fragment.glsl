uniform vec4 f_color;
varying vec3 new_c;

void main(void) {
    gl_FragColor.rgba = f_color * 200 / (new_c.x + new_c.y);
}
