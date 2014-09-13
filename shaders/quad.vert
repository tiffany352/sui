#version 140

in vec2 iposition;
uniform vec4 upos;

void main()
{
    vec2 p = iposition * upos.zw + upos.xy;
    gl_Position = vec4(p * 2 - vec2(1), 0.0, 1.0);
}
