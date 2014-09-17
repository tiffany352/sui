#include <stdio.h>
#include <SDL.h>
#include <sys/time.h>

#include "tgl/tgl.h"
#include "render/widgets.h"
#include "render/renderer.h"

static GLvoid error_cb(GLenum source, GLenum type, GLuint id, GLenum severity,
                       GLsizei length, const GLchar* message, GLvoid* user)
{
    (void)user;
    const char *ssource;
    switch(source) {
        case GL_DEBUG_SOURCE_API_ARB:               ssource=" API";              break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:     ssource=" Window System";    break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:   ssource=" Shader Compiler";  break;
        case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:       ssource=" Third Party";      break;
        case GL_DEBUG_SOURCE_APPLICATION_ARB:       ssource=" Application";      break;
        case GL_DEBUG_SOURCE_OTHER_ARB:             ssource="";            break;
        default: ssource="???";
    }
    const char *stype;
    switch(type) {
        case GL_DEBUG_TYPE_ERROR_ARB:               stype=" error";                 break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB: stype=" deprecated behaviour";  break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:  stype=" undefined behaviour";   break;
        case GL_DEBUG_TYPE_PORTABILITY_ARB:         stype=" portability issue";     break;
        case GL_DEBUG_TYPE_PERFORMANCE_ARB:         stype=" performance issue";     break;
        case GL_DEBUG_TYPE_OTHER_ARB:               stype="";                       break;
        default: stype="???";
    }
    const char *sseverity;
    switch(severity) {
        case GL_DEBUG_SEVERITY_HIGH_ARB:    sseverity="high";   break;
        case GL_DEBUG_SEVERITY_MEDIUM_ARB:  sseverity="medium"; break;
        case GL_DEBUG_SEVERITY_LOW_ARB:     sseverity="low";    break;
        default: sseverity="???";
    }
    char msg[length+1];
    strncpy(msg, message, length+1);
    if (msg[length-1] == '\n') {
        msg[length-1] = 0; // cut off newline
    }

    printf("(OpenGL%s) %s%s #%u: %s\n", ssource, sseverity, stype, id, msg);
}

void timersub(struct timeval *a, struct timeval *b, struct timeval *r)
{
    bool carry = b->tv_usec > a->tv_usec;
    r->tv_usec = carry? a->tv_usec + 1000000 - b->tv_usec : a->tv_usec - b->tv_usec;
    r->tv_sec = a->tv_sec - b->tv_sec - carry;
}

int main(int argc, char *argv[])
{
    (void)argc, (void)argv;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_Window* window = SDL_CreateWindow("SDL Example", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_Event ev;
    sui_renderer r[1];
    char *error;
    sui_font georgia[1], meirio[1];
    struct timeval start, tv;
    sui_textfmt english = {
        SUI_ALIGN_TOPLEFT,
        SUI_DIR_LTR,
        14,
        georgia,
        "en",
        "Latin"
    };
    sui_textfmt japanese = {
        SUI_ALIGN_TOPLEFT,
        SUI_DIR_LTR,
        14,
        meirio,
        "jp",
        "Hiragana"
    };

    glDebugMessageCallback((GLDEBUGPROC)&error_cb, NULL);

    if (!sui_renderer_init(r, &error)) {
        printf("%s\n", error);
        free(error);
        return 1;
    }

    if (!sui_font_fromfile(georgia, r, &error, "georgia.ttf")) {
        printf("%s\n", error);
        free(error);
        return 1;
    }

    if (!sui_font_fromfile(meirio, r, &error, "meirio.ttf")) {
        printf("%s\n", error);
        free(error);
        return 1;
    }

    gettimeofday(&start, NULL);

    while (1) {
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
            case SDL_QUIT:
                goto quit;
            }
        }
        glClearColor(0.4, 0.5, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        gettimeofday(&tv, NULL);
        timersub(&tv, &start, &tv);
        char curtime[30];
        sprintf(curtime, "%ld.%05ld", tv.tv_sec, tv.tv_usec);

        sui_cmd buf[] = {
            sui_rect(sui_size(0,0,     60, 20), sui_col(128, 50, 48, 255)),
            sui_rect(sui_size(780,550, 20, 50), sui_col(255,255,255, 255)),
            sui_rect(sui_size(100,300, 200,40), sui_col( 25,190, 50, 255)),
            sui_text(sui_size(0,0,     100,40), sui_col(255,255,255, 255), english, curtime),
            sui_text(sui_size(100,300, 200,40), sui_col(255,255,255, 255), english, "hello world"),
            sui_text(sui_size(100,320, 200,40), sui_col(255,255,255, 255), japanese, "こんにちは、世界中のみなさん")
        };

        sui_renderer_draw(r, 800, 600, buf, sizeof(buf)/sizeof(sui_cmd));

        SDL_GL_SwapWindow(window);
    }
 quit:
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
