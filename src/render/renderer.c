#include "renderer.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "widgets/widgets.h"

enum sui_arrattribs {
    SUI_RECT_POS
};

extern const char sui_shader_quad_vert[];
extern const char sui_shader_rect_frag[];

bool sui_renderer_init(sui_renderer *r, char **error)
{
    memset(r, 0, sizeof(sui_renderer));
    tgl_vao_init(&r->vao);
    tgl_vao_bind(&r->vao);
    tgl_quad_init(&r->rect_vbo, SUI_RECT_POS);
    if (!tgl_make_shader(&r->rect_shader.vertex, GL_VERTEX_SHADER, sui_shader_quad_vert, strlen(sui_shader_quad_vert), error)) {
        return false;
    }
    if (!tgl_make_shader(&r->rect_shader.fragment, GL_FRAGMENT_SHADER, sui_shader_rect_frag, strlen(sui_shader_rect_frag), error)) {
        return false;
    }
    r->rect_shader.program = glCreateProgram();
    glAttachShader(r->rect_shader.program, r->rect_shader.vertex);
    glAttachShader(r->rect_shader.program, r->rect_shader.fragment);
    if (!tgl_link_program(r->rect_shader.program, error)) {
        return false;
    }
    r->rect_upos = glGetUniformLocation(r->rect_shader.program, "upos");
    r->rect_ucolor = glGetUniformLocation(r->rect_shader.program, "ucolor");
    return true;
}

void sui_renderer_draw(sui_renderer *r, unsigned w, unsigned h, sui_cmd *cmds, size_t len)
{
    tgl_vao_bind(&r->vao);
    for (unsigned i = 0; i < len; i++) {
        switch (cmds[i].type) {
        case SUI_RECT:
            glUseProgram(r->rect_shader.program);
            float ux = cmds[i].aabb.lx / (float)w;
            float uy = 1.0 - cmds[i].aabb.ly / (float)h;
            float uw = (cmds[i].aabb.hx - cmds[i].aabb.lx) / (float)w;
            float uh = (cmds[i].aabb.hy - cmds[i].aabb.ly) / -(float)h;
            glUniform4f(r->rect_upos, ux, uy, uw, uh);
            unsigned char *col = cmds[i].col;
            glUniform4f(r->rect_ucolor, col[0] / 255.0, col[1] / 255.0, col[2] / 255.0, col[3] / 255.0);
            tgl_quad_draw_once(&r->rect_vbo);
            break;
        }
    }
}
