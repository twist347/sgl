#pragma once

#include <string>
#include <unordered_map>

#include "sgl_type.h"
#include "sgl_expected.h"

namespace sgl {
    enum class shader_error {
        invalid_params = 0,
        file_io_failed,
        gl_create_shader_failed,
        gl_create_program_failed,
        gl_vertex_compile_failed,
        gl_fragment_compile_failed,
        gl_program_link_failed,
        count
    };

    class shader {
    public:
        using error = shader_error;
        using result = expected<shader, error>;

        // ctors and assignments

        shader(const shader &) = delete;

        shader &operator=(const shader &) = delete;

        shader(shader &&other) noexcept;

        shader &operator=(shader &&other) noexcept;

        ~shader();

        // fabrics

        static result create_from_ids(gl_uint vertex_shader, gl_uint fragment_shader) noexcept;

        static result create_from_source(const char *vertex_src, const char *fragment_src) noexcept;

        static result create_from_source(const std::string &vertex_src, const std::string &fragment_src) noexcept {
            return create_from_source(vertex_src.c_str(), fragment_src.c_str());
        }

        static result create_from_files(const char *vertex_path, const char *fragment_path) noexcept;

        static result create_from_files(const std::string &vertex_path, const std::string &fragment_path) noexcept {
            return create_from_files(vertex_path.c_str(), fragment_path.c_str());
        }

        // or panic wrappers

        static shader create_from_ids_or_panic(gl_uint vertex_shader, gl_uint fragment_shader) noexcept;

        static shader create_from_source_or_panic(const char *vertex_src, const char *fragment_src) noexcept;

        static shader create_from_source_or_panic(
            const std::string &vertex_src, const std::string &fragment_src
        ) noexcept {
            return create_from_source_or_panic(vertex_src.c_str(), fragment_src.c_str());
        }

        static shader create_from_files_or_panic(const char *vertex_path, const char *fragment_path) noexcept;

        static shader create_from_files_or_panic(
            const std::string &vertex_path, const std::string &fragment_path
        ) noexcept {
            return create_from_files_or_panic(vertex_path.c_str(), fragment_path.c_str());
        }

        // api

        void use() const noexcept;

        [[nodiscard]] gl_uint get_id() const noexcept { return m_program; }
        [[nodiscard]] bool is_valid() const noexcept { return m_program != 0; }

        [[nodiscard]] gl_int get_uniform_loc(const char *name) const noexcept;

        [[nodiscard]] gl_int get_uniform_loc(const std::string &name) const noexcept {
            return get_uniform_loc(name.c_str());
        }

        // scalars

        bool set_uniform(gl_int loc, gl_int v) const noexcept;
        bool set_uniform(gl_int loc, gl_uint v) const noexcept;
        bool set_uniform(gl_int loc, gl_float v) const noexcept;

        // vecs

        bool set_uniform_vec2(gl_int loc, const gl_float *v, gl_sizei count = 1) const noexcept;
        bool set_uniform_vec3(gl_int loc, const gl_float *v, gl_sizei count = 1) const noexcept;
        bool set_uniform_vec4(gl_int loc, const gl_float *v, gl_sizei count = 1) const noexcept;

        // mats (column major)

        bool set_uniform_mat3(gl_int loc, const gl_float *m, gl_boolean transpose = false, gl_sizei count = 1) const noexcept;
        bool set_uniform_mat4(gl_int loc, const gl_float *m, gl_boolean transpose = false, gl_sizei count = 1) const noexcept;

        // scalars

        bool set_uniform(const char *name, gl_int v) const noexcept;
        bool set_uniform(const char *name, gl_uint v) const noexcept;
        bool set_uniform(const char *name, gl_float v) const noexcept;

        // vecs

        bool set_uniform_vec2(const char *name, const gl_float *v, gl_sizei count = 1) const noexcept;
        bool set_uniform_vec3(const char *name, const gl_float *v, gl_sizei count = 1) const noexcept;
        bool set_uniform_vec4(const char *name, const gl_float *v, gl_sizei count = 1) const noexcept;

        // mats (column major)

        bool set_uniform_mat3(const char *name, const gl_float *m, gl_boolean transpose = false, gl_sizei count = 1) const noexcept;
        bool set_uniform_mat4(const char *name, const gl_float *m, gl_boolean transpose = false, gl_sizei count = 1) const noexcept;

        constexpr static const char *err_to_str(error err) noexcept;

    private:
        static gl_uint compile_shader(gl_enum type, const char *src, error &out_err) noexcept;

        static bool check_compile(gl_uint shader_id, const char *type_name) noexcept;

        static bool check_link(gl_uint program) noexcept;

    private:
        explicit shader(gl_uint program) noexcept : m_program{program} {
        }

        void destroy() noexcept;

        [[nodiscard]] bool validate_uniform_loc(gl_int loc) const noexcept;

        gl_uint m_program = 0;
        mutable std::unordered_map<std::string, gl_int> m_uniform_cache;
    };
}
