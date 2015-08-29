#include <stdio.h>
#include <SDL.h>
#include <sys/time.h>

#include "tgl/tgl.h"
#include "sui/widgets.h"
#include "sui/renderer.h"

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

void sui_timersub(struct timeval *a, struct timeval *b, struct timeval *r)
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
    sui_font sanspro[1], meirio[1], droidsans[1];
    struct timeval start, tv;
    sui_layout_format english = {
        SUI_ALIGN_TOPLEFT,
        SUI_DIR_LTR,
        36,
        "en",
        "Latin"
    };
    sui_layout_format japanese = {
        SUI_ALIGN_TOPLEFT,
        SUI_DIR_LTR,
        36,
        "jp",
        "Hiragana"
    };
    sui_layout_format arabic = {
        SUI_ALIGN_TOPLEFT,
        SUI_DIR_RTL,
        36,
        "ar",
        "Arabic"
    };

    glDebugMessageCallback((GLDEBUGPROC)&error_cb, NULL);

    if (!sui_renderer_init(r, &error)) {
        printf("%s\n", error);
        free(error);
        return 1;
    }

    if (!sui_font_fromfc(sanspro, r, &error, FcPatternBuild(
                             0,
                             FC_FAMILY, FcTypeString, "Source Sans Pro Light",
                             FC_FAMILY, FcTypeString, "Sans",
                             NULL))) {
        printf("%s\n", error);
        free(error);
        return 1;
    }

    if (!sui_font_fromfc(meirio, r, &error, FcPatternBuild(
                             0,
                             FC_FAMILY, FcTypeString, "Meiryo",
                             FC_FAMILY, FcTypeString, "Droid Sans Japanese",
                             FC_LANG, FcTypeString, "jp",
                             NULL))) {
        printf("%s\n", error);
        free(error);
        return 1;
    }

    if (!sui_font_fromfc(droidsans, r, &error, FcPatternBuild(
                             0,
                             FC_FAMILY, FcTypeString, "Droid Sans Arabic",
                             FC_LANG, FcTypeString, "ar",
                             NULL))) {
        printf("%s\n", error);
        free(error);
        return 1;
    }

    sui_layout hello_english, hello_japanese, hello_arabic;
    const char *english_text = "hello world";
    if (!sui_layout_init(&hello_english, sanspro, &english, english_text, strlen(english_text), &error)) {
        printf("%s\n", error);
        free(error);
        return 1;
    }
    const char *japanese_text = "こんにちは、世界中のみなさん";
    if (!sui_layout_init(&hello_japanese, meirio, &japanese, japanese_text, strlen(japanese_text), &error)) {
        printf("%s\n", error);
        free(error);
        return 1;
    }
    const char *arabic_text = "مرحبا بالعالم";
    if (!sui_layout_init(&hello_arabic, droidsans, &arabic, arabic_text, strlen(arabic_text), &error)) {
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
        sui_timersub(&tv, &start, &tv);
        char curtime[30];
        sprintf(curtime, "%ld.%05ld", tv.tv_sec, tv.tv_usec);

        sui_layout curtime_layout;
        if (!sui_layout_init(&curtime_layout, sanspro, &english, curtime, strlen(curtime), &error)) {
            printf("%s\n", error);
            free(error);
            return 1;
        }

        uint8_t white[] = {255,255,255, 255};
        uint8_t green[] = { 25,190, 50, 255};
        uint8_t red[]   = {128, 50, 48, 255};

        sui_cmd buf[] = {
            sui_rect(red,   sui_mkpoint(  0,  0), sui_mkpoint(200, 40)),
            sui_rect(white, sui_mkpoint(780,550), sui_mkpoint( 20, 50)),
            sui_rect(green, sui_mkpoint(100,300), sui_mkpoint(200, 40)),
            sui_text(white, sui_mkpoint(  0,  0), &curtime_layout),
            sui_text(white, sui_mkpoint(100,300), &hello_english),
            sui_text(red,   sui_mkpoint(100,340), &hello_japanese),
            sui_text(white, sui_mkpoint(100,380), &hello_arabic)
        };

        sui_renderer_draw(r, 800, 600, buf, sizeof(buf) / sizeof(sui_cmd));

        sui_layout_free(&curtime_layout);

        SDL_GL_SwapWindow(window);
    }
 quit:
    sui_font_free(sanspro);
    sui_font_free(meirio);
    sui_font_free(droidsans);
    sui_layout_free(&hello_english);
    sui_layout_free(&hello_japanese);
    sui_renderer_free(r);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
