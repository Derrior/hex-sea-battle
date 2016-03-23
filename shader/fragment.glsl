uniform vec4 f_color;

void main(void) {
    gl_FragColor.rgba = f_color / 2;
}
