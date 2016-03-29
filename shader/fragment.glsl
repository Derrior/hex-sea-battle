uniform vec4 f_color;
varying vec3 new_c;


void main(void) {
    vec3 c = vec3(new_c.x / 100 + 2, new_c.y / 100 + 2, new_c.z / 100);
    //gl_FragColor.rgba += (4 * f_color / (c.x * c.y * c.y + sqrt(c.y) * (c.x + 1) * 8) * abs(sqrt(c.x) * (c.x - 1) + (c.y - 1)) + f_color / 3) * f_color.a;
    gl_FragColor.rgba = f_color;
}
