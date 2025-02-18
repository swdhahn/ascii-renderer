#ifndef RENDERER
#define RENDERER

#include <windows.h>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <thread>
#include "Maths.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "Keyboard.h"

#undef max
#undef min

class Renderer {
public:
    Renderer();
    Renderer(int windowWidth, int windowHeight);
    ~Renderer();

    void SetShader(ShaderProgram *program);

    void RenderRectangle(const float &x, const float &y, const float &width, const float &height, const float &intensity);
    void RenderTriangle(const red::Vector3 &v1, const red::Vector3 &v2, const red::Vector3 &v3, const red::Vector3 &n1, const red::Vector3 &n2, const red::Vector3 &n3);

    void RenderModel(const Model *model);

    void RenderScreen();

    int getWidth() { return m_windowWidth; };
    int getHeight() { return m_windowHeight; };
    float getDepth() { return 80.0f; };
private:
    red::Vector3 RasterizePoint(const red::Vector4 &point);
    void PlacePixel(const int &x, const int &y, const float &z);
    void PlacePixel(const int &x, const int &y, const float &z, const float &intensity);

    int m_windowWidth = -1, m_windowHeight = -1;
    float *m_pixels = nullptr;
    float *m_depths = nullptr;
    const char *DEPTH_VALUES = " .,\'^\":;/#%@"; //" .,:;/#%@";
    const int DEPTH_VALUE_LENGTH = 12;

    std::thread *keyboardThread = nullptr;

    ShaderProgram *m_shader = nullptr;
};

LRESULT CALLBACK ConsoleWindowCallback(
  _In_ HWND   hwnd,
  _In_ UINT   uMsg,
  _In_ WPARAM wParam,
  _In_ LPARAM lParam
);

#endif // RENDERER