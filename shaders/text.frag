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
    //ocolor = vec4(texcoord, 0, 1);
    ocolor = vec4(ucolor.xyz, ucolor.w * texture(usampler, vec3(co, float(uchar))).x);
    float n = 0.01;
    if (texcoord.x <= n || texcoord.x >= usize.x-n || texcoord.y <= n || texcoord.y >= usize.y-n) {
        ocolor = vec4(1);
    }
}
