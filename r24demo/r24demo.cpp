// Sample code showing how to create a modern OpenGL window and rendering context on Win32.

#include <windows.h>
#include <gl/gl.h>
#include <stdbool.h>
#include <math.h>

#include "4klang.h"
#include "audio.h"
#include "fbo.h"
#include "glext-stubs.h"
#include "particles.h"
#include "transformations.h"

typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext, const int* attribList);

wglCreateContextAttribsARB_type* wglCreateContextAttribsARB;

// See https://www.opengl.org/registry/specs/ARB/wgl_create_context.txt for all values
#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001

typedef BOOL WINAPI wglChoosePixelFormatARB_type(HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);

wglChoosePixelFormatARB_type* wglChoosePixelFormatARB;

// See https://www.opengl.org/registry/specs/ARB/wgl_pixel_format.txt for all values
#define WGL_DRAW_TO_WINDOW_ARB                    0x2001
#define WGL_ACCELERATION_ARB                      0x2003
#define WGL_SUPPORT_OPENGL_ARB                    0x2010
#define WGL_DOUBLE_BUFFER_ARB                     0x2011
#define WGL_PIXEL_TYPE_ARB                        0x2013
#define WGL_COLOR_BITS_ARB                        0x2014
#define WGL_DEPTH_BITS_ARB                        0x2022
#define WGL_STENCIL_BITS_ARB                      0x2023

#define WGL_FULL_ACCELERATION_ARB                 0x2027
#define WGL_TYPE_RGBA_ARB                         0x202B


static void
init_opengl_extensions(void)
{
    // Before we can load extensions, we need a dummy OpenGL context, created using a dummy window.
    // We use a dummy window because you can only set the pixel format for a window once. For the
    // real window, we want to use wglChoosePixelFormatARB (so we can potentially specify options
    // that aren't available in PIXELFORMATDESCRIPTOR), but we can't load and use that before we
// have a context.
    WNDCLASSA window_class = {
        .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
        .lpfnWndProc = DefWindowProcA,
        .hInstance = GetModuleHandle(0),
        .lpszClassName = "Dummy_WGL_djuasiodwa",
    };

    if (!RegisterClassA(&window_class)) {
        return;
    }

    HWND dummy_window = CreateWindowExA(
        0,
        window_class.lpszClassName,
        "Dummy OpenGL Window",
        0,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        window_class.hInstance,
        0);

    if (!dummy_window) {
        return;
    }

    HDC dummy_dc = GetDC(dummy_window);

    PIXELFORMATDESCRIPTOR pfd = {
        .nSize = sizeof(pfd),
        .nVersion = 1,
        .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        .iPixelType = PFD_TYPE_RGBA,
        .cColorBits = 32,
        .cAlphaBits = 8,
        .cDepthBits = 24,
        .cStencilBits = 8,
        .iLayerType = PFD_MAIN_PLANE,
    };

    int pixel_format = ChoosePixelFormat(dummy_dc, &pfd);
    if (!pixel_format) {
        return;
    }
    if (!SetPixelFormat(dummy_dc, pixel_format, &pfd)) {
        return;
    }

    HGLRC dummy_context = wglCreateContext(dummy_dc);
    if (!dummy_context) {
        return;
    }

    if (!wglMakeCurrent(dummy_dc, dummy_context)) {
        return;
    }

    wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type*)wglGetProcAddress("wglCreateContextAttribsARB");
    wglChoosePixelFormatARB = (wglChoosePixelFormatARB_type*)wglGetProcAddress("wglChoosePixelFormatARB");

    wglMakeCurrent(dummy_dc, 0);
    wglDeleteContext(dummy_context);
    ReleaseDC(dummy_window, dummy_dc);
    DestroyWindow(dummy_window);
}

static HGLRC
init_opengl(HDC real_dc)
{
    init_opengl_extensions();

    // Now we can choose a pixel format the modern way, using wglChoosePixelFormatARB.
    int pixel_format_attribs[] = {
        WGL_DRAW_TO_WINDOW_ARB,     GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB,     GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB,      GL_TRUE,
        WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
        WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB,         32,
        WGL_DEPTH_BITS_ARB,         24,
        WGL_STENCIL_BITS_ARB,       8,
        0
    };

    int pixel_format;
    UINT num_formats;
    wglChoosePixelFormatARB(real_dc, pixel_format_attribs, 0, 1, &pixel_format, &num_formats);
    if (!num_formats) {
        return 0;
    }

    PIXELFORMATDESCRIPTOR pfd;
    DescribePixelFormat(real_dc, pixel_format, sizeof(pfd), &pfd);
    if (!SetPixelFormat(real_dc, pixel_format, &pfd)) {
        return 0;
    }

    // Specify that we want to create an OpenGL 3.3 core profile context
    int gl33_attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB, /*WGL_CONTEXT_CORE_PROFILE_BIT_ARB*/0,
        0,
    };

    HGLRC gl33_context = wglCreateContextAttribsARB(real_dc, 0, gl33_attribs);
    if (!gl33_context) {
        return 0;
    }

    if (!wglMakeCurrent(real_dc, gl33_context)) {
        return 0;
    }

    return gl33_context;
}

static LRESULT CALLBACK
window_callback(HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
{
    LRESULT result = 0;

    switch (msg) {
    case WM_CLOSE:
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
        if (wparam == VK_ESCAPE) {
            PostQuitMessage(0);
        } else {
            result = DefWindowProcA(window, msg, wparam, lparam);
        }
        break;
    default:
        result = DefWindowProcA(window, msg, wparam, lparam);
        break;
    }

    return result;
}

static HWND
create_window(HINSTANCE inst)
{
    WNDCLASSA window_class = {
        .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
        .lpfnWndProc = window_callback,
        .hInstance = inst,
        .hCursor = LoadCursor(0, IDC_ARROW),
        .hbrBackground = 0,
        .lpszClassName = "WND",
    };

    if (!RegisterClassA(&window_class)) {
        return 0;
    }

    // Specify a desired width and height, then adjust the rect so the window's client area will be
    // that size.
    RECT rect = {
        .right = 1024,
        .bottom = 576,
    };
    DWORD window_style = WS_OVERLAPPEDWINDOW;
    AdjustWindowRect(&rect, window_style, false);

    HWND window = CreateWindowExA(
        0,
        window_class.lpszClassName,
        "",
        window_style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        0,
        0,
        inst,
        0);

    if (!window) {
        return 0;
    }

    return window;
}


float AUDIO_BUFFER[MAX_SAMPLES * 2];

int main()
{
    HWND window = create_window(0);
    HDC gldc = GetDC(window);
    HGLRC glrc = init_opengl(gldc);
    init_glext_stubs();

    _4klang_render(AUDIO_BUFFER);

    const unsigned X = 1920;
    const unsigned Y = 1080;
    const int tsize = 4;

    FBO mainfbo(X, Y);
    FBO depthfbo(X, Y);
    FBO hbloomfbo(512, 512);
    FBO positions(tsize, tsize);

    Particles particles(tsize, positions.getTexture(), 0);

    {
        auto fbo = positions.select();
        for (int x = 0; x < tsize; x++) {
            for (int y = 0; y < tsize; y++) {
                float p = ((float)(x * tsize + y)) / (tsize * tsize);

                glWindowPos2i(x, y);
                float pixel[4] = { cos(p * 2 * 3.14) * 5, sin(p * 2 * 3.14) * 5, 0, 0 };
                glDrawPixels(1, 1, GL_RGBA, GL_FLOAT, pixel);
            }
        }
    }

    ShowWindow(window, 1);
    UpdateWindow(window);

    // play_audio(AUDIO_BUFFER, MAX_SAMPLES * 2 * sizeof(float));
    float t = 0.0;

    bool running = true;
    while (running) {
        MSG msg;
        while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                running = false;
            }
            else {
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
        }

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        {
            matrix4f i, j;
            identity((float(*)[4]) & i);
            rotate((float(*)[4]) & i, (float(*)[4]) & j, t / 0.2, 0, 1, 0);
            scale((float(*)[4]) & j, (float(*)[4]) & i, 0.5, 0.5, 0.5);
            translate((float(*)[4]) & i, (float(*)[4]) & particles.m_modelview, 0, 0, -10.0);
            frustum((float(*)[4]) & particles.m_projection, -1, 1, -1, 1, 1.0, 10000.0);

            particles.render();
        }

        SwapBuffers(gldc);
        t += 1.0 / 60.0;
    }

    return 0;
}

#ifdef NDEBUG
int WinMainCRTStartup() {
    return main();
}
#else
int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    return main();
}
#endif
