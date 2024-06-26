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
#include "stepper.h"
#include "quad.h"
#include "accelerator.h"
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
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
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

int width, height;

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

    HMONITOR hmon = MonitorFromPoint(POINT{0, 0}, MONITOR_DEFAULTTOPRIMARY);
    MONITORINFO mi = { sizeof(mi) };
    GetMonitorInfo(hmon, &mi);

    width = mi.rcMonitor.right - mi.rcMonitor.left;
    height = mi.rcMonitor.bottom - mi.rcMonitor.top;

    HWND window = CreateWindowExA(
        0,
        window_class.lpszClassName,
        "",
        WS_POPUP | WS_VISIBLE,
        mi.rcMonitor.left,
        mi.rcMonitor.top,
        mi.rcMonitor.right - mi.rcMonitor.left,
        mi.rcMonitor.bottom - mi.rcMonitor.top,
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

int nextParticle = 0;
void addParticle(float *positions, float *colors, float *velocities, int tsize, const vec3 &pos, const vec3 &velocity, const vec3 &color, float t) {
    memcpy(positions + nextParticle * 4, &pos, sizeof(float) * 3);
    positions[nextParticle * 4 + 3] = 0;

    memcpy(colors + nextParticle * 4, &color, sizeof(float) * 3);
    colors[nextParticle * 4 + 3] = t;

    memcpy(velocities + nextParticle * 4, &velocity, sizeof(float) * 3);
    velocities[nextParticle * 4 + 3] = 0;

    nextParticle = (nextParticle + 1) % (tsize * tsize);
}

int main()
{
    HWND window = create_window(0);
    HDC gldc = GetDC(window);
    HGLRC glrc = init_opengl(gldc);
    init_glext_stubs();

    _4klang_render(AUDIO_BUFFER);

    const unsigned X = 1920;
    const unsigned Y = 1080;
    const int tsize = 512;

    FBO mainfbo(X, Y);
    FBO depthfbo(X, Y);
    FBO hbloomfbo(512, 512);
    FBO positions(tsize, tsize);
    FBO colors(tsize, tsize);
    FBO velocities(tsize, tsize);
    FBO velocitiesDest(tsize, tsize);

    FBO* currentVelocities = &velocities;
    FBO* otherVelocities = &velocitiesDest;

    Particles particles(tsize, positions.getTexture(), colors.getTexture());
    Stepper stepper;
    Accelerator accelerator(positions.getTexture());

    ShowWindow(window, 1);
    UpdateWindow(window);

    auto playback = play_audio(AUDIO_BUFFER, MAX_SAMPLES * 2 * sizeof(float));
    bool running = true;
    vec3 lastpos = { 0, 0, 0 };
    float last_t = 0;
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

        glViewport(0, 0, width, height);
        glEnable(GL_BLEND);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const auto t = playback.get_progress();
        const auto dt = t - last_t;
        last_t = t;

        if (t > 60)
            break;

        {

            identity(particles.m_modelview);
            rotate(particles.m_modelview, particles.m_modelview, t * 20, 0, 1, 0);
            scale(particles.m_modelview, particles.m_modelview, 0.5, 0.5, 0.5);
            translate(particles.m_modelview, particles.m_modelview, 
                        11 * cos(t * 0.227 * sin(t * 0.113)),
                        7 * -sin(t * 0.122 * (1 + sin(t * 0.213))),
                -35.0 + 4 *  sin(t * 0.345 * (.5 + .5 * cos(t * 0.417)))
            );

            frustum(particles.m_projection, -0.5, .5, -.5, .5, 1.0, 10000.0);

            particles.setTime(t);
            particles.render();
            //quad.render();
        }

        {
            // Step all the particles
            auto fbo = positions.select();
            stepper.setVelocityTex(currentVelocities->getTexture());
            stepper.setColor(vec4{ 1,1,1, dt / 2.0f });
            stepper.render();
        }

        {
            // Step all the accelerations
            auto fbo = otherVelocities->select();
            accelerator.setVelocityTex(currentVelocities->getTexture());
            accelerator.setDT(dt);
            accelerator.render();

            // Swap the velocity FBOs
            FBO* tmp = otherVelocities;
            otherVelocities = currentVelocities;
            currentVelocities = tmp;
        }

        static float pos_data[MAXPARTICLES];
        static float color_data[MAXPARTICLES];
        static float vel_data[MAXPARTICLES];

        positions.getData(pos_data);
        colors.getData(color_data);
        currentVelocities->getData(vel_data);

        for (int p = 0; p < 16000*dt; p++) {
            float ddt = t - dt + (dt * float(p) / 10.0);
            vec3 pos = { sin(ddt * 7) * 20, cos(ddt * 5) * 14 + 10, sin(ddt * 9) * 8 };

            vec3 posdiff = { (pos.x - lastpos.x)  , (pos.y - lastpos.y) , (pos.z - lastpos.z) };
            vec3 velocity = { (pos.x - lastpos.x) / dt , (pos.y - lastpos.y) / dt , (pos.z - lastpos.z) / dt };
            lastpos = pos;
            vec3 color1 = { 0.04, 0.03, 0.005 };
            vec3 color2 = { 0.04, 0.035, 0.04 };

            int n = 1;
            for (int i = 0; i < n; i++) {
                float d = 1;
                float rnd = fmod(ddt * 1000 + p * 0.123, 1) * 4 - 2;
                float rnd2 = fmod(ddt * 997 + p * rnd, 1) * 4 - 2;
                float rnd3 = fmod(ddt * 993 + p * rnd2, 1) * 4 - 2;
                vec3 newpos = { pos.x + (rnd/4.0) + (posdiff.x * d), pos.y + (rnd2/4.0) + (posdiff.y * d), pos.z + (rnd3/4.0) + (posdiff.z * d) };
                vec3 newvelocity = { velocity.x + rnd2 * 5, velocity.y + rnd3 * 5, velocity.z + rnd * 5};
                vec3* color = p % 2 ? &color1 : &color2;

                addParticle(pos_data, color_data, vel_data, tsize, newpos, newvelocity, *color, ddt + (4 * rnd));
            }
        }

        positions.setData(pos_data);
        colors.setData(color_data);
        currentVelocities->setData(vel_data);

        SwapBuffers(gldc);

    }


    exit(0);
//  DestroyWindow(window);
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
