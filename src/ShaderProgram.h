#ifndef SHADERPROGRAM
#define SHADERPROGRAM

#include <algorithm>
#include "Maths.h"

// See StaticShader for example of a ShaderProgram
class ShaderProgram {
public:
    // vertex: in vertex positions without any transformations-directly from models
    // normals: in normals from a Model
    // outWorldPosition: vertex in world position that goes to pixel shader
    virtual red::Vector4 VertexShader(const red::Vector3 &vertex, const red::Vector3 &normal, red::Vector3 &outNormal, red::Vector4 &outWorldPosition) = 0;
    // fragCoord: the current fragCoord, fragment positions
    // normal: normals with interpolation (normalize them here for more accuracy, but not neccessary)
    // inWorldPosition: position in world space interpolated from vertex shader
    virtual float PixelShader(const red::Vector3 &fragCoord, const red::Vector3 &normal, const red::Vector4 &inWorldPosition) = 0;

protected:
    red::Vector4 out_vertex;
    float out_frag;
};

class StaticShader : public ShaderProgram {
public:
    red::Vector4 VertexShader(const red::Vector3 &vertex, const red::Vector3 &normal, red::Vector3 &outNormal, red::Vector4 &outWorldPosition);
    float PixelShader(const red::Vector3 &fragCoord, const red::Vector3 &normal, const red::Vector4 &inWorldPosition);

    void loadMatrices(red::Matrix4 *projectionMatrix, red::Matrix4 *transformationMatrix, red::Matrix4 *viewMatrix);
private:
    red::Vector3 lightDir = red::Vector3(-0.2f, -1, 0);

    red::Matrix4 *m_projectionMatrix = nullptr;
    red::Matrix4 *m_transformationMatrix = nullptr;
    red::Matrix4 *m_viewMatrix = nullptr;
};

#endif // SHADERPROGRAM