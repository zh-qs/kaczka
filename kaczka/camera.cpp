#include "camera.h"
#include <glm/gtx/transform.hpp>

void kaczka::Camera::zoom(float scale)
{
    distance += scale;
    if (distance < 0.1f)
        distance = 0.1f;
}

void kaczka::Camera::move(const glm::vec3& v)
{
    target += v;
}

void kaczka::Camera::rotate(float angx, float angy)
{
    rotx += angx;
    roty += angy;
}

glm::vec3 kaczka::Camera::getPosition() const
{
    glm::mat3 rotation = glm::rotate(rotx, glm::vec3{ 0.0f,1.0f,0.0f }) * glm::rotate(roty, glm::vec3{ 1.0f,0.0f,0.0f });
    glm::vec3 dir = rotation * glm::vec3{ 0.0f,0.0f,1.0f };
    return target + distance * dir;
}

glm::mat4 kaczka::Camera::getViewMatrix() const
{
    glm::mat3 rotation = glm::rotate(rotx, glm::vec3{ 0.0f,1.0f,0.0f }) * glm::rotate(roty, glm::vec3{ 1.0f,0.0f,0.0f });
    glm::vec3 up = rotation * glm::vec3{ 0.0f,1.0f,0.0f };
    glm::vec3 dir = rotation * glm::vec3{ 0.0f,0.0f,-1.0f };
    return glm::lookAt(target + distance * dir, target, up);
}

glm::mat4 kaczka::Camera::getProjMatrix() const
{
    return glm::perspective(fov, static_cast<float>(width) / height, near, far);
}
