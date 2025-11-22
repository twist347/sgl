#pragma once

#include <string>

#include "sgl_type.h"
#include "sgl_expected_facade.h"

namespace sgl {
    enum class shader_uniform_type_e {
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
    };

    enum class shader_error_e {
        INVALID_PARAMS = 0,
        FILE_IO_FAILED,
        VERTEX_COMPILE_FAILED,
        FRAGMENT_COMPILE_FAILED,
        PROGRAM_LINK_FAILED,
    };

    class shader_t {
    public:
        // ctors

        using error_t = shader_error_e;
        using expected_t = expected<shader_t, error_t>;

        shader_t(const shader_t &) = delete;

        shader_t &operator=(const shader_t &) = delete;

        shader_t(shader_t &&other) noexcept;

        shader_t &operator=(shader_t &&other) noexcept;

        ~shader_t();

        // fabrics

        static expected_t create_from_shaders(gl_uint vertex_shader, gl_uint fragment_shader) noexcept;

        static expected_t create_from_source(const char *vertex_src, const char *fragment_src) noexcept;

        static expected_t create_from_source(const std::string &vertex_src, const std::string &fragment_src) noexcept {
            return create_from_source(vertex_src.c_str(), fragment_src.c_str());
        }

        static expected_t create_from_files(const char *vertex_path, const char *fragment_path) noexcept;

        static expected_t create_from_files(const std::string &vertex_path, const std::string &fragment_path) noexcept {
            return create_from_files(vertex_path.c_str(), fragment_path.c_str());
        }

        // or panic wrappers

        static shader_t create_from_shaders_or_panic(gl_uint vertex_shader, gl_uint fragment_shader) noexcept;

        static shader_t create_from_source_or_panic(const char *vertex_src, const char *fragment_src) noexcept;

        static shader_t create_from_source_or_panic(
            const std::string &vertex_src,
            const std::string &fragment_src
        ) noexcept {
            return create_from_source_or_panic(vertex_src.c_str(), fragment_src.c_str());
        }

        static shader_t create_from_files_or_panic(const char *vertex_path, const char *fragment_path) noexcept;

        static shader_t create_from_files_or_panic(
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

        void set_uniform(gl_int loc, const void *value, shader_uniform_type_e type, gl_sizei count = 1) const noexcept;

        void set_uniform(
            const char *name,
            const void *value,
            shader_uniform_type_e type,
            gl_sizei count = 1
        ) const noexcept;

        static const char *err_to_str(error_t err) noexcept;

    private:
        explicit shader_t(gl_uint program) noexcept : m_program(program) {
        }

        static gl_uint compile_shader(gl_enum type, const char *src, error_t &out_err) noexcept;

        static bool read_text_file(const char *path, std::string &out_src) noexcept;

        static bool check_compile(gl_uint shader_id, const char *type_name) noexcept;

        static bool check_link(gl_uint program) noexcept;

        static void set_uniform_impl(
            gl_uint program,
            gl_int loc,
            const void *value,
            shader_uniform_type_e type,
            gl_sizei count
        ) noexcept;

    private:
        gl_uint m_program = 0;
    };
} // namespace sgl
