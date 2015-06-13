#version 140

in vec2 texcoord;
uniform int uchar;
uniform vec4 ucolor;
uniform sampler2DArray usampler;
uniform vec2 usize;
out vec4 ocolor;

void main()
{
    vec2 co = texcoord;
    ocolor = vec4(ucolor.xyz, ucolor.w * texture(usampler, vec3(co, float(uchar))).x);
}
