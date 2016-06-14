uniform vec4 f_color;
varying vec3 new_c;


void main(void) {
    vec3 c = vec3(new_c.x / 100 + 2, new_c.y / 100 + 2, new_c.z / 100);
    gl_FragColor.rgba = 100 * f_color / (c.x * c.x * c.y * c.x - c.x * c.y * c.y * 10 + c.y * c.x * 4 - 1) * min(10, (c.x - c.y));
}
