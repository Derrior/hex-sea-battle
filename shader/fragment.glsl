uniform vec4 f_color;
uniform sampler2D tex;
varying vec3 new_c;
varying vec2 new_tex_coord;

void main(void) {
    vec3 c = vec3(new_c.x / 100 + 2, new_c.y / 100 + 2, new_c.z / 100);
  
    gl_FragColor.rgba += vec4((4 * f_color.rgb / (c.x * c.y * c.y + sqrt(c.y) * (c.x + 1) * 8) * abs(sqrt(c.x) * (c.x - 1) + (c.y - 1)) + f_color.rgb / 3), f_color.a);
    gl_FragColor.rgba += texture2D(tex, new_tex_coord);
}
