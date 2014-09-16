#version 140

in vec2 texcoord;
uniform vec4 ucolor;
uniform sampler2D usampler;
out vec4 ocolor;

void main()
{
    ocolor = vec4(ucolor.xyz, ucolor.w * texture(usampler, texcoord).x);
}
