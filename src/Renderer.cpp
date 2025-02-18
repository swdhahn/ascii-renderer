#include "Renderer.h"

LRESULT CALLBACK ConsoleWindowCallback(
  _In_ HWND   hwnd,
  _In_ UINT   uMsg,
  _In_ WPARAM wParam,
  _In_ LPARAM lParam
) {
    
}

Renderer::Renderer() {
    // Create a thread for handling input
    keyboardThread = new std::thread(handleKeyInputs);

    // Console rows and columns
    CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	m_windowWidth = (int)(csbi.srWindow.Right - csbi.srWindow.Left);
	m_windowHeight = (int)(csbi.srWindow.Bottom - csbi.srWindow.Top);

    m_pixels = new float[m_windowWidth * m_windowHeight];
    m_depths = new float[m_windowWidth * m_windowHeight];
	memset(m_pixels, 0, m_windowHeight * m_windowWidth * sizeof(float));
    memset(m_depths, 0, m_windowHeight * m_windowWidth * sizeof(float));
    
}

Renderer::Renderer(int windowWidth, int windowHeight) {
    // Create a thread for handling input
    keyboardThread = new std::thread(handleKeyInputs);
    
    // Setting Console size
    HWND console = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(console, &ConsoleRect);
    MoveWindow(console, ConsoleRect.left, ConsoleRect.top, windowWidth, windowHeight, TRUE);
    for(int i = 0; i < 10000; i++) {std::cout << " ";}
    // Console rows and columns
    CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	m_windowWidth = (int)(csbi.srWindow.Right - csbi.srWindow.Left);
	m_windowHeight = (int)(csbi.srWindow.Bottom - csbi.srWindow.Top);

    m_pixels = new float[m_windowWidth * m_windowHeight];
    m_depths = new float[m_windowWidth * m_windowHeight];
	memset(m_pixels, 0, m_windowHeight * m_windowWidth * sizeof(float));
    memset(m_depths, 0, m_windowHeight * m_windowWidth * sizeof(float));
    
}

Renderer::~Renderer() {
    endKeyBoardHandling = true;

    delete [] m_pixels;
    delete [] m_depths;

    keyboardThread->join();
    delete keyboardThread;
}

void Renderer::RenderScreen() {
    std::stringstream stream;

	for(int y = 0; y < m_windowHeight; y++) {
		for(int x = 0; x < m_windowWidth; x++) {
			stream << DEPTH_VALUES[(int)std::clamp((int)(m_pixels[y * m_windowWidth + x] * (DEPTH_VALUE_LENGTH - 1)), 0, DEPTH_VALUE_LENGTH)];
		}
		stream << "\n";
	}
	memset(m_pixels, 0, m_windowHeight * m_windowWidth * sizeof(float));
    memset(m_depths, 0, m_windowHeight * m_windowWidth * sizeof(float));

	std::cout << stream.str().c_str();
}

void Renderer::RenderRectangle(const float &x, const float &y, const float &width, const float &height, const float &intensity) {
    for(int yy = y; yy < y + height; yy++) {
		for(int xx = x; xx < x + width; xx++) {
			PlacePixel(xx, yy, 0, intensity);
		}
    }
}

void Renderer::PlacePixel(const int &x, const int &y, const float &z, const float &intensity) {
    if(x < 0 || y < 0 || x >= m_windowWidth || y >= m_windowHeight) return;
    if(z < m_depths[y * m_windowWidth + x]) return;
    m_depths[y * m_windowWidth + x] = z;
    m_pixels[y * m_windowWidth + x] = std::clamp(intensity, 0.1f, 1.0f);
}

void Renderer::RenderModel(const Model *model) {
    for(int i = 0; i < model->indicesLength; i+=3) {
        //std::cout << i << " " << model->indicesLength << std::endl;
        //std::cout << model->verticesLength << " " << model->normalsLength << "\n" << std::endl;
        RenderTriangle(model->vertices[model->indices[i]], model->vertices[model->indices[i + 1]], model->vertices[model->indices[i + 2]], model->normals[model->indices[i]], model->normals[model->indices[i + 1]], model->normals[model->indices[i + 2]]);
    }
}

void Renderer::RenderTriangle(const red::Vector3 &vv1, const red::Vector3 &vv2, const red::Vector3 &vv3, const red::Vector3 &n1, const red::Vector3 &n2, const red::Vector3 &n3) {
    red::Vector4 wp1, wp2, wp3;
    red::Vector3 norm1, norm2, norm3;
    red::Vector4 vsh1 = m_shader->VertexShader(vv1, n1, norm1, wp1);
    red::Vector4 vsh2 = m_shader->VertexShader(vv2, n2, norm2, wp2);
    red::Vector4 vsh3 = m_shader->VertexShader(vv3, n3, norm3, wp3);

    red::Vector3 v1 = RasterizePoint(vsh1);
    red::Vector3 v2 = RasterizePoint(vsh2);
    red::Vector3 v3 = RasterizePoint(vsh3);

    //std::cout << v1.x << std::endl;

    int maxX = std::max(v1.x, std::max(v2.x, v3.x));
    int minX = std::min(v1.x, std::min(v2.x, v3.x));
    int maxY = std::max(v1.y, std::max(v2.y, v3.y));
    int minY = std::min(v1.y, std::min(v2.y, v3.y));

    red::Vector2 vs1(v2.x - v1.x, v2.y - v1.y);
    red::Vector2 vs2(v3.x - v1.x, v3.y - v1.y);

    for(int x = minX; x <= maxX; x++) {
        for(int y = minY; y <= maxY; y++) {

            if(x < 0 || y < 0 || x >= m_windowWidth || y >= m_windowHeight) continue;

            red::Vector2 q(x - v1.x, y - v1.y);

            float s = q.cross(vs2) / vs1.cross(vs2);
            float t = vs1.cross(q) / vs1.cross(vs2);

            if((s >= 0) && (t >= 0) && (s + t <= 1)) {
                // barrycentric weights
                float w1 = ((v2.y - v3.y) * (x - v3.x) + (v3.x - v2.x) * (y - v3.y)) / ((v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.y - v3.y));
                float w2 = ((v3.y - v1.y) * (x - v3.x) + (v1.x - v3.x) * (y - v3.y)) / ((v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.y - v3.y));
                float w3 = 1 - w1 - w2;
                // z value from weights and coords
                float z = v1.z * w1 + v2.z * w2 + v3.z * w3;

                //std::cout << "Z: " << z << "\nV1.z: " << v1.z << "\nW1: " << w1 << std::endl;
                if(z > m_depths[y * m_windowWidth + x]) continue;
                m_depths[y * m_windowWidth + x] = z;

                // Other weight finding
                red::Vector4 worldPosition(wp1.x * w1 + wp2.x * w2 + wp3.x * w3, wp1.y * w1 + wp2.y * w2 + wp3.y * w3, wp1.z * w1 + wp2.z * w2 + wp3.z * w3, wp1.w * w1 + wp2.w * w2 + wp3.w * w3);
                red::Vector3 normalDir(norm1.x * w1 + norm2.x * w2 + norm3.x * w3, norm1.y * w1 + norm2.y * w2 + norm3.y * w3, norm1.z * w1 + norm2.z * w2 + norm3.z * w3);

                // Finally Render from the pixel shader
                
                m_pixels[y * m_windowWidth + x] = std::clamp(m_shader->PixelShader(red::Vector3(x, y, z), normalDir, worldPosition), 0.0f, 1.0f);
            }
        }
    }

}

red::Vector3 Renderer::RasterizePoint(const red::Vector4 &point) {
    //std::cout << point.x << " " << point.y << " " << point.z << " " << point.w << std::endl;
    //std::cout << (point.x / point.w) << " " << (point.y / point.w) << " " << (point.z / point.w) << std::endl;
    return red::Vector3((1.0f + point.x / point.w) / 2.0f * m_windowWidth, (1.0f + point.y / point.w) / 2.0f * m_windowHeight, (1.0f + point.z / point.w) / 2.0f * getDepth());
}

void Renderer::SetShader(ShaderProgram *program) {
    m_shader = program;
}