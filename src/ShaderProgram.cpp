#include "ShaderProgram.h"


red::Vector4 StaticShader::VertexShader(const red::Vector3 &vertex, const red::Vector3 &normal, red::Vector3 &outNormal, red::Vector4 &outWorldPosition) {
    outWorldPosition = (*m_transformationMatrix) * red::Vector4(vertex, 1.0f);
    out_vertex = (*m_projectionMatrix) * (*m_viewMatrix) * outWorldPosition;
    outNormal = ((*m_viewMatrix) * (*m_transformationMatrix) * red::Vector4(normal, 0.0f)).getVector3();
    return out_vertex;
}

float StaticShader::PixelShader(const red::Vector3 &fragCoord, const red::Vector3 &normal, const red::Vector4 &inWorldPosition) {
    out_frag = std::max(lightDir.dot(normal), 0.1f);
    return out_frag;
}

void StaticShader::loadMatrices(red::Matrix4 *projectionMatrix, red::Matrix4 *transformationMatrix, red::Matrix4 *viewMatrix) {
    m_projectionMatrix = projectionMatrix;
    m_transformationMatrix = transformationMatrix;
    m_viewMatrix = viewMatrix;
}
