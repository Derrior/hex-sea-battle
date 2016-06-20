uniform vec4 f_color;
uniform sampler2D tex;
uniform int using_textures;
uniform float shadowing;
varying vec3 new_c;
varying vec2 new_tex_coord;

void main(void) {
    vec3 c = vec3(new_c.x / 100 + 2, new_c.y / 100 + 2, new_c.z / 100);
  
    gl_FragColor.rgba += vec4((4 * f_color.rgb / (c.x * c.y * c.y + sqrt(c.y) * (c.x + 1) * 8) * abs(sqrt(c.x) * (c.x - 1) + (c.y - 1)) + f_color.rgb / 3), f_color.a);
    if (using_textures != 0) {
        vec4 col = texture2D(tex, new_tex_coord);
        gl_FragColor = vec4(col.r, col.r, col.r, col.r);
//        gl_FragColor = texture2D(tex, new_tex_coord) + vec4(new_tex_coord.x / 2, new_tex_coord.y / 2, 0.5, 1);
    }
    gl_FragColor.rgba *= (shadowing);

}
