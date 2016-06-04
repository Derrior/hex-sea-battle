uniform vec4 f_color;
varying vec3 new_c;

void main(void) {
    gl_FragColor.rgba = f_color * 10 / max(1, (new_c.x - 500) * (new_c.x - 500)) * max(1, (new_c.y - 200) * (new_c.y - 200));
}
