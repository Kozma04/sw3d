#include "window.h"
#include "utils.h"
#include <stdio.h>


const static char CLASS_NAME[] = "SoftwareRendererWindowClass";


static uint8_t *g_bmp_a, *g_bmp_b;
uint8_t *g_bmp;
static uint8_t *g_draw_bmp;
int32_t g_width, g_height;
static int32_t g_pixelscale;


static BITMAPINFO bmi;
static HWND hwnd;
static HDC hdc;
static HBITMAP hBitmap;
static HBITMAP hOldBitmap;

static int32_t prev_mouse_x, prev_mouse_y;
static int32_t curr_mouse_x, curr_mouse_y;
static uint8_t mouse_move_active;
static uint32_t last_mouse_move;


static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
        case WM_MOUSEMOVE:
            curr_mouse_x = LOWORD(lParam);
            curr_mouse_y = HIWORD(lParam);
            last_mouse_move = time_ms();
            return 0;
        case WM_LBUTTONDOWN:
            mouse_move_active = 1;
            prev_mouse_x = curr_mouse_x;
            prev_mouse_y = curr_mouse_y;
            return 0;
        case WM_LBUTTONUP:
            mouse_move_active = 0;
            return 0;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

static HWND CreateRenderWindow(HINSTANCE hInstance, int width, int height, int pixelscale) {
    // Step 1: Register the window class
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    RECT wr = { 0, 0, width * pixelscale, height * pixelscale};
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    // Step 2: Create the window
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles
        CLASS_NAME,                     // Window class
        "Software Renderer",           // Window title
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,

        NULL,       // Parent window
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );
    if(hwnd == NULL) {
        return NULL;
    }
    ShowWindow(hwnd, SW_SHOWDEFAULT);
    return hwnd;
}

bool window_create(HINSTANCE inst, uint32_t width, uint32_t height, uint32_t pixelscale) {
    hwnd = CreateRenderWindow(inst, width, height, pixelscale);
    if(hwnd == NULL) {
        return false;
    }
    hdc = GetDC(hwnd);
    hBitmap = CreateCompatibleBitmap(hdc, width, height);

    bmi = {};
    bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;  // Negative height to ensure top-down drawing
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;  // Assuming 32-bit color (ARGB)
    bmi.bmiHeader.biCompression = BI_RGB;

    hOldBitmap = (HBITMAP)SelectObject(hdc, hBitmap);

    g_width = width;
    g_height = height;
    g_pixelscale = pixelscale;
    g_bmp_a = new uint8_t[width * height * 4];
    g_bmp_b = new uint8_t[width * height * 4];

    g_bmp = g_bmp_a;

    g_draw_bmp = g_bmp_b;
    return true;
}

void window_clear_draw_buffer() {
    memset(g_bmp, 0, g_width * g_height * 4);
}

void window_swap_buffers() {
    if(g_bmp == g_bmp_a) {
        g_bmp = g_bmp_b;
        g_draw_bmp = g_bmp_a;
    }
    else {
        g_bmp = g_bmp_a;
        g_draw_bmp = g_bmp_b;
    }
}

void window_mousepos_delta(int32_t* x, int32_t* y) {
    if (!mouse_move_active || time_ms() - last_mouse_move > 20) *x = 0, * y = 0;
    else {
        *x = curr_mouse_x - prev_mouse_x;
        *y = curr_mouse_y - prev_mouse_y;
        prev_mouse_x = curr_mouse_x;
        prev_mouse_y = curr_mouse_y;
    }
}

void window_mousepos(int32_t* x, int32_t* y) {
    *x = prev_mouse_x;
    *y = prev_mouse_y;
}

void window_destroy() {
    SelectObject(hdc, hOldBitmap);
    DeleteObject(hBitmap);
    ReleaseDC(hwnd, hdc);
    DestroyWindow(hwnd);
    UnregisterClass(CLASS_NAME, GetModuleHandle(NULL));
}

void window_loop(window_render_fn_t render) {
    MSG msg = {};
    bool quit = false;
    while(!quit) {
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if(msg.message == WM_QUIT) {
                quit = true;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        render();
        //SetDIBitsToDevice(hdc, 0, 0, g_width, g_height, 0, 0, 0, g_height, g_bmp, &bmi, DIB_RGB_COLORS);

        StretchDIBits(hdc, 0, 0, g_width * g_pixelscale, g_height * g_pixelscale, 0, 0, g_width, g_height, g_bmp, &bmi, DIB_RGB_COLORS, SRCCOPY);
        //SetDIBits(hdc, NULL, 0, g_height, g_bmp, &bmi, DIB_RGB_COLORS);
    }
    window_destroy();
}