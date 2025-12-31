#include "internal/sgl_shader.h"

#include <cassert>
#include <string>
#include <utility>

#include "glad/glad.h"

#include "internal/sgl_log.h"
#include "internal/sgl_file.h"
#include "internal/sgl_util.h"

namespace sgl {
    // ctors and assignments

    shader::shader(shader &&other) noexcept : m_program{std::exchange(other.m_program, 0)},
                                              m_uniform_cache{std::move(other.m_uniform_cache)} {
    }

    shader &shader::operator=(shader &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        destroy();

        m_program = std::exchange(other.m_program, 0);
        m_uniform_cache = std::move(other.m_uniform_cache);

        return *this;
    }

    shader::~shader() {
        destroy();
    }

    // fabrics

    shader::result shader::create_from_ids(gl_uint vertex_shader, gl_uint fragment_shader) noexcept {
        if (!vertex_shader || !fragment_shader) {
            return unexpected{error::invalid_params};
        }

        const gl_uint program = glCreateProgram();
        if (!program) {
            log_error("glCreateProgram() failed");
            return unexpected{error::gl_create_program_failed};
        }

        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);

        glDetachShader(program, vertex_shader);
        glDetachShader(program, fragment_shader);

        if (!check_link(program)) {
            glDeleteProgram(program);
            return unexpected{error::gl_program_link_failed};
        }

        return shader{program};
    }

    shader::result shader::create_from_source(const char *vertex_src, const char *fragment_src) noexcept {
        if (!vertex_src || !fragment_src) {
            return unexpected{error::invalid_params};
        }

        auto err = error::invalid_params;

        const gl_uint vs = compile_shader(GL_VERTEX_SHADER, vertex_src, err);
        if (vs == 0) {
            return unexpected{err};
        }

        const gl_uint fs = compile_shader(GL_FRAGMENT_SHADER, fragment_src, err);
        if (fs == 0) {
            glDeleteShader(vs);
            return unexpected{err};
        }

        auto prog_res = create_from_ids(vs, fs);

        glDeleteShader(vs);
        glDeleteShader(fs);

        return prog_res;
    }

    shader::result shader::create_from_files(const char *vertex_path, const char *fragment_path) noexcept {
        if (!vertex_path || !fragment_path) {
            return unexpected{error::invalid_params};
        }

        std::string vs_src;
        std::string fs_src;

        if (!detail::read_text_file(vertex_path, vs_src)) {
            log_error("failed to read vertex shader file: {}", vertex_path);
            return unexpected{error::file_io_failed};
        }

        if (!detail::read_text_file(fragment_path, fs_src)) {
            log_error("failed to read fragment shader file: {}", fragment_path);
            return unexpected{error::file_io_failed};
        }

        log_info(
            "shader: loaded files vs='{}' ({} bytes), fs='{}' ({} bytes)",
            vertex_path, vs_src.size(), fragment_path, fs_src.size()
        );

        return create_from_source(vs_src, fs_src);
    }

    // try wrappers

    shader shader::create_from_ids_try(gl_uint vertex_shader, gl_uint fragment_shader) noexcept {
        auto res = create_from_ids(vertex_shader, fragment_shader);
        if (!res) {
            const auto err = res.error();
            log_fatal("failed to create shader program from shaders: {}", shader::err_to_str(err));
        }
        return std::move(*res);
    }

    shader shader::create_from_source_try(const char *vertex_src, const char *fragment_src) noexcept {
        auto res = create_from_source(vertex_src, fragment_src);
        if (!res) {
            const auto err = res.error();
            log_fatal("failed to create shader from source: {}", err_to_str(err));
        }
        return std::move(*res);
    }

    shader shader::create_from_files_try(const char *vertex_path, const char *fragment_path) noexcept {
        auto res = create_from_files(vertex_path, fragment_path);
        if (!res) {
            const auto err = res.error();
            log_fatal(
                "failed to create shader from files ('{}', '{}'): {}",
                vertex_path, fragment_path, err_to_str(err)
            );
        }
        return std::move(*res);
    }

    // api

    void shader::use() const noexcept {
        assert(m_program);

        glUseProgram(m_program);
    }

    gl_int shader::uniform_loc(const char *name) const noexcept {
        if (!m_program || !name) {
            return -1;
        }

        if (const auto it = m_uniform_cache.find(name); it != m_uniform_cache.end()) {
            return it->second;
        }

        const gl_int loc = glGetUniformLocation(m_program, name);
        m_uniform_cache.emplace(name, loc); // cache -1 too

        return loc;
    }

    // scalars

    bool shader::set_uniform(gl_int loc, gl_int v) const noexcept {
        assert(m_program);

        if (!validate_uniform_loc(loc)) {
            return false;
        }
        glUniform1i(loc, v);
        return true;
    }

    bool shader::set_uniform(gl_int loc, gl_uint v) const noexcept {
        assert(m_program);

        if (!validate_uniform_loc(loc)) {
            return false;
        }
        glUniform1ui(loc, v);
        return true;
    }

    bool shader::set_uniform(gl_int loc, gl_float v) const noexcept {
        assert(m_program);

        if (!validate_uniform_loc(loc)) {
            return false;
        }
        glUniform1f(loc, v);
        return true;
    }

    // vecs

    bool shader::set_uniform_vec2(gl_int loc, const gl_float *v, gl_sizei count) const noexcept {
        assert(m_program);

        if (!validate_uniform_loc(loc) || !v) {
            return false;
        }
        glUniform2fv(loc, count, v);
        return true;
    }

    bool shader::set_uniform_vec3(gl_int loc, const gl_float *v, gl_sizei count) const noexcept {
        assert(m_program);

        if (!validate_uniform_loc(loc) || !v) {
            return false;
        }
        glUniform3fv(loc, count, v);
        return true;
    }

    bool shader::set_uniform_vec4(gl_int loc, const gl_float *v, gl_sizei count) const noexcept {
        assert(m_program);

        if (!validate_uniform_loc(loc) || !v) {
            return false;
        }
        glUniform4fv(loc, count, v);
        return true;
    }

    // mats (column major)

    bool shader::set_uniform_mat3(gl_int loc, const gl_float *m, gl_boolean transpose, gl_sizei count) const noexcept {
        assert(m_program);

        if (!validate_uniform_loc(loc) || !m) {
            return false;
        }
        glUniformMatrix3fv(loc, count, transpose, m);
        return true;
    }

    bool shader::set_uniform_mat4(gl_int loc, const gl_float *m, gl_boolean transpose, gl_sizei count) const noexcept {
        assert(m_program);

        if (!validate_uniform_loc(loc) || !m) {
            return false;
        }
        glUniformMatrix4fv(loc, count, transpose, m);
        return true;
    }

    // scalars

    bool shader::set_uniform(const char *name, gl_int v) const noexcept {
        assert(m_program);

        if (!name) {
            return false;
        }

        const gl_int loc = uniform_loc(name);
        if (loc < 0) {
            log_error("uniform '{}' not found", name);
            return false;
        }

        return set_uniform(loc, v);
    }

    bool shader::set_uniform(const char *name, gl_uint v) const noexcept {
        assert(m_program);

        if (!name) {
            return false;
        }

        const gl_int loc = uniform_loc(name);
        if (loc < 0) {
            log_error("uniform '{}' not found", name);
            return false;
        }

        return set_uniform(loc, v);
    }

    bool shader::set_uniform(const char *name, gl_float v) const noexcept {
        assert(m_program);

        if (!name) {
            return false;
        }

        const gl_int loc = uniform_loc(name);
        if (loc < 0) {
            log_error("uniform '{}' not found", name);
            return false;
        }

        return set_uniform(loc, v);
    }

    // vecs

    bool shader::set_uniform_vec2(const char *name, const gl_float *v, gl_sizei count) const noexcept {
        assert(m_program);

        if (!name || !v) {
            return false;
        }

        const gl_int loc = uniform_loc(name);
        if (loc < 0) {
            log_error("uniform '{}' not found", name);
            return false;
        }

        return set_uniform_vec2(loc, v, count);
    }

    bool shader::set_uniform_vec3(const char *name, const gl_float *v, gl_sizei count) const noexcept {
        assert(m_program);

        if (!name || !v) {
            return false;
        }

        const gl_int loc = uniform_loc(name);
        if (loc < 0) {
            log_error("uniform '{}' not found", name);
            return false;
        }

        return set_uniform_vec3(loc, v, count);
    }

    bool shader::set_uniform_vec4(const char *name, const gl_float *v, gl_sizei count) const noexcept {
        assert(m_program);

        if (!name || !v) {
            return false;
        }

        const gl_int loc = uniform_loc(name);
        if (loc < 0) {
            log_error("uniform '{}' not found", name);
            return false;
        }

        return set_uniform_vec4(loc, v, count);
    }

    // mats (column major)

    bool shader::set_uniform_mat3(const char *name, const gl_float *m, gl_boolean transpose,
                                  gl_sizei count) const noexcept {
        assert(m_program);

        if (!name || !m) {
            return false;
        }

        const gl_int loc = uniform_loc(name);
        if (loc < 0) {
            log_error("uniform '{}' not found", name);
            return false;
        }

        return set_uniform_mat3(loc, m, transpose, count);
    }

    bool shader::set_uniform_mat4(const char *name, const gl_float *m, gl_boolean transpose,
                                  gl_sizei count) const noexcept {
        assert(m_program);

        if (!name || !m) {
            return false;
        }

        const gl_int loc = uniform_loc(name);
        if (loc < 0) {
            log_error("uniform '{}' not found", name);
            return false;
        }

        return set_uniform_mat4(loc, m, transpose, count);
    }

    // internal

    gl_uint shader::compile_shader(gl_enum type, const char *src, error &out_err) noexcept {
        SGL_VERIFY(src);

        const gl_uint shader = glCreateShader(type);
        if (!shader) {
            log_error("glCreateShader() failed");
            out_err = error::gl_create_shader_failed;
            return 0;
        }

        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        auto type_name = "SHADER";
        switch (type) {
            case GL_VERTEX_SHADER: type_name = "VERTEX";
                break;
            case GL_FRAGMENT_SHADER: type_name = "FRAGMENT";
                break;
#ifdef GL_GEOMETRY_SHADER
            case GL_GEOMETRY_SHADER: type_name = "GEOMETRY";
                break;
#endif
            default: break;
        }

        if (!check_compile(shader, type_name)) {
            glDeleteShader(shader);

            if (type == GL_VERTEX_SHADER) {
                out_err = error::gl_vertex_compile_failed;
            } else if (type == GL_FRAGMENT_SHADER) {
                out_err = error::gl_fragment_compile_failed;
            }

            return 0;
        }

        return shader;
    }

    bool shader::check_compile(gl_uint shader_id, const char *type_name) noexcept {
        gl_int success = GL_FALSE;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

        if (success == GL_TRUE) {
            return true;
        }

        gl_int log_len = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_len);
        if (log_len < 1) {
            log_len = 1;
        }

        std::string log;
        log.resize(static_cast<std::size_t>(log_len));

        gl_sizei written = 0;
        glGetShaderInfoLog(shader_id, log_len, &written, log.data());

        log_error("'{}' compilation failed:\n{}", type_name, log);
        return false;
    }

    bool shader::check_link(gl_uint program) noexcept {
        gl_int success = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &success);

        if (success == GL_TRUE) {
            return true;
        }

        gl_int log_len = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);
        if (log_len < 1) {
            log_len = 1;
        }

        std::string log;
        log.resize(static_cast<std::size_t>(log_len));

        gl_sizei written = 0;
        glGetProgramInfoLog(program, log_len, &written, log.data());

        log_error("shader program linking failed:\n{}", log);
        return false;
    }

    void shader::destroy() noexcept {
        if (m_program != 0) {
            glDeleteProgram(m_program);
            m_program = 0;
        }
    }

    bool shader::validate_uniform_loc(gl_int loc) const noexcept {
        assert(m_program);

        if (loc < 0) {
            log_warn("shader::set_uniform: location < 0");
            return false;
        }

        if (!is_valid()) {
            log_error("shader::set_uniform: called on invalid program");
            return false;
        }

#ifndef NDEBUG
        GLint current = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &current);
        assert(static_cast<gl_uint>(current) == m_program);
#endif

        return true;
    }
}
