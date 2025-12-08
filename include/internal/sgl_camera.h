#pragma once

#include "glm/glm.hpp"

namespace sgl {
    class camera {
    public:
        // fabrics
        static camera create(float fov, float aspect, float z_near, float z_far) noexcept;

        // positions and orientation

        void set_pos(const glm::vec3 &pos) noexcept { m_pos = pos; }
        [[nodiscard]] const glm::vec3 &get_pos() const noexcept { return m_pos; }

        void set_yaw_pitch(float yaw, float pitch) noexcept;

        [[nodiscard]] float get_yaw() const noexcept { return m_yaw; }
        [[nodiscard]] float get_pitch() const noexcept { return m_pitch; }

        [[nodiscard]] const glm::vec3 &get_front() const noexcept { return m_front; }
        [[nodiscard]] glm::vec3 right() const noexcept { return glm::normalize(glm::cross(m_front, m_world_up)); }
        [[nodiscard]] glm::vec3 up() const noexcept { return m_up; }

        // moves
        void move_forward(float dt) noexcept;

        void move_right(float dt) noexcept;

        void move_up(float dt) noexcept;

        void rotate(float yaw_dt, float pitch_dt) noexcept;

        void set_projection(float fov, float aspect, float z_near, float z_far) noexcept;

        [[nodiscard]] float get_fov() const noexcept { return m_fov; }
        [[nodiscard]] float get_aspect() const noexcept { return m_aspect; }
        [[nodiscard]] float get_z_near() const noexcept { return m_z_near; }
        [[nodiscard]] float get_z_far() const noexcept { return m_z_far; }

        [[nodiscard]] glm::mat4 get_view_mat() const noexcept;

        [[nodiscard]] glm::mat4 get_projection_mat() const noexcept;

        [[nodiscard]] glm::mat4 get_view_projection_mat() const noexcept {
            return get_projection_mat() * get_view_mat();
        }

    private:
        void update_vecs() noexcept;

        glm::vec3 m_pos{0.f, 0.f, 3.f};
        glm::vec3 m_front{0.f, 0.f, -1.f};
        glm::vec3 m_world_up{0.f, 1.f, 0.f};
        glm::vec3 m_up{0.f, 1.f, 0.f};

        float m_yaw = -90.f;
        float m_pitch = 0.f;

        float m_fov = 45.f;
        float m_aspect = 16.f / 9.f;
        float m_z_near = 1.f;
        float m_z_far = 100.f;
    };
}
