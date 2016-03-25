uniform vec4 f_color;
varying vec3 new_c;

void main(void) {
    gl_FragColor.rgba = f_color * 700 / (new_c.x + new_c.y + 1000);
}
