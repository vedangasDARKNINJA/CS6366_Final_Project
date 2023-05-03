#pragma once 
#include <glm/glm.hpp>

namespace RendererPBR
{
    class Transform
    {
    public:
        Transform();
        ~Transform() = default;
        void SetPosition(glm::vec3 position);
        void SetScale(glm::vec3 scale);
        void SetRotation(glm::vec3 eulerAngles);
    
        void OnImGuiDraw(const char* label);

        glm::mat4 GetModelMatrix() const;

        glm::vec3 GetPosition()const;
        glm::vec3 GetScale()const;
        glm::vec3 GetEulerAngles()const;
    private:
        void CalculateModelMatrix()const;

    private:
        glm::vec3 m_Translation;
        glm::vec3 m_Scale;
        glm::vec3 m_EulerAngles;

        mutable glm::mat4 m_CachedModelMatrix;
        mutable bool m_IsDirty = false;

    };
}