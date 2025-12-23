#include "internal/sgl_camera.h"

#include <algorithm>
#include <cmath>

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace sgl {
    // fabric

    camera camera::create(float fov_deg, float aspect, float z_near, float z_far) noexcept {
        camera cam;
        cam.set_projection(fov_deg, aspect, z_near, z_far);
        cam.update_vecs();
        return cam;
    }

    // positions and orientation

    void camera::set_yaw_pitch(float yaw, float pitch) noexcept {
        m_yaw = yaw;
        m_pitch = std::clamp(pitch, -89.f, 89.f);
        update_vecs();
    }

    // moves
    void camera::move_forward(float dt) noexcept {
        m_pos += m_front * (m_move_speed * dt);
    }

    void camera::move_right(float dt) noexcept {
        m_pos += m_right * (m_move_speed * dt);
    }

    void camera::move_up_world(float dt) noexcept {
        m_pos += m_world_up * (m_move_speed * dt);
    }

    void camera::move_up_local(float dt) noexcept {
        m_pos += m_up * (m_move_speed * dt);
    }

    void camera::rotate(float yaw_dt, float pitch_dt) noexcept {
        const float scaled_yaw = yaw_dt * m_sens;
        const float scaled_pitch = pitch_dt * m_sens;
        set_yaw_pitch(m_yaw + scaled_yaw, m_pitch + scaled_pitch);
    }

    void camera::set_projection(float fov, float aspect, float z_near, float z_far) noexcept {
        m_fov = fov;
        m_aspect = aspect;
        m_z_near = z_near;
        m_z_far = z_far;
    }

    glm::mat4 camera::view() const noexcept {
        const auto target = m_pos + m_front;
        return glm::lookAt(m_pos, target, m_up);
    }

    glm::mat4 camera::projection() const noexcept {
        return glm::perspective(glm::radians(m_fov), m_aspect, m_z_near, m_z_far);
    }

    // internals

    void camera::update_vecs() noexcept {
        const float yaw_rad = glm::radians(m_yaw);
        const float pitch_rad = glm::radians(m_pitch);

        glm::vec3 front;
        front.x = std::cos(yaw_rad) * std::cos(pitch_rad);
        front.y = std::sin(pitch_rad);
        front.z = std::sin(yaw_rad) * std::cos(pitch_rad);

        m_front = glm::normalize(front);

        m_right = glm::normalize(glm::cross(m_front, m_world_up));
        m_up = glm::normalize(glm::cross(m_right, m_front));
    }
}
