#version 140

in vec2 iposition;
uniform mat3 upos;
uniform mat4 umat;
uniform vec2 usize = vec2(1);
out vec2 texcoord;

void main()
{
    vec3 p = upos * vec3(iposition, 1.0);
    gl_Position = umat * vec4(p.xy, 0.0, 1.0);
    texcoord = iposition * usize;
}
