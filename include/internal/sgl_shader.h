#pragma once

#include <string>

#include "sgl_type.h"
#include "sgl_expected.h"

namespace sgl {
    enum class shader_uniform_type {
        INT = 0,
        INT_VEC2,
        INT_VEC3,
        INT_VEC4,

        UINT,
        UINT_VEC2,
        UINT_VEC3,
        UINT_VEC4,

        FLOAT,
        VEC2,
        VEC3,
        VEC4,

        MAT2,
        MAT3,
        MAT4,

        COUNT
    };

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
            const std::string &vertex_src,
            const std::string &fragment_src
        ) noexcept {
            return create_from_source_or_panic(vertex_src.c_str(), fragment_src.c_str());
        }

        static shader create_from_files_or_panic(const char *vertex_path, const char *fragment_path) noexcept;

        static shader create_from_files_or_panic(
            const std::string &vertex_path,
            const std::string &fragment_path
        ) noexcept {
            return create_from_files_or_panic(vertex_path.c_str(), fragment_path.c_str());
        }

        // api

        void use() const noexcept;

        [[nodiscard]] gl_uint get_id() const noexcept { return m_program; }
        [[nodiscard]] bool is_valid() const noexcept { return m_program != 0; }

        [[nodiscard]] gl_int uniform_location(const char *name) const noexcept;

        void set_uniform(gl_int loc, const void *value, shader_uniform_type type, gl_sizei count = 1) const noexcept;

        void set_uniform(
            const char *name,
            const void *value,
            shader_uniform_type type,
            gl_sizei count = 1
        ) const noexcept;

        constexpr static const char *err_to_str(error err) noexcept;

    private:
        static gl_uint compile_shader(gl_enum type, const char *src, error &out_err) noexcept;

        static bool check_compile(gl_uint shader_id, const char *type_name) noexcept;

        static bool check_link(gl_uint program) noexcept;

        static void set_uniform_impl(
            gl_int loc,
            const void *value,
            shader_uniform_type type,
            gl_sizei count
        ) noexcept;

    private:
        explicit shader(gl_uint program) noexcept : m_program{program} {
        }

        void destroy() noexcept;

        gl_uint m_program = 0;
    };
}
