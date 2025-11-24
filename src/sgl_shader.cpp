#include "internal/sgl_shader.h"

#include <cassert>
#include <fstream>
#include <string>

#include "glad/glad.h"

#include "internal/sgl_log.h"

namespace sgl {
    // ctors

    shader::~shader() {
        if (m_program != 0) {
            glDeleteProgram(m_program);
            m_program = 0;
        }
    }

    shader::shader(shader &&other) noexcept : m_program(other.m_program) {
        other.m_program = 0;
    }

    shader &shader::operator=(shader &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        if (m_program != 0) {
            glDeleteProgram(m_program);
        }

        m_program = other.m_program;
        other.m_program = 0;

        return *this;
    }

    // fabrics

    shader::result shader::create_from_shaders(gl_uint vertex_shader, gl_uint fragment_shader) noexcept {
        if (!vertex_shader || !fragment_shader) {
            return unexpected(error::INVALID_PARAMS);
        }

        const gl_uint program = glCreateProgram();

        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);

        glDetachShader(program, vertex_shader);
        glDetachShader(program, fragment_shader);

        if (!check_link(program)) {
            glDeleteProgram(program);
            return unexpected(error::PROGRAM_LINK_FAILED);
        }

        return shader{program};
    }

    shader::result shader::create_from_source(const char *vertex_src, const char *fragment_src) noexcept {
        if (!vertex_src || !fragment_src) {
            return unexpected(error::INVALID_PARAMS);
        }

        auto err = error::INVALID_PARAMS;

        const gl_uint vs = compile_shader(GL_VERTEX_SHADER, vertex_src, err);
        if (vs == 0) {
            return unexpected(err);
        }

        const gl_uint fs = compile_shader(GL_FRAGMENT_SHADER, fragment_src, err);
        if (fs == 0) {
            glDeleteShader(vs);
            return unexpected(err);
        }

        auto prog_res = create_from_shaders(vs, fs);

        glDeleteShader(vs);
        glDeleteShader(fs);

        return prog_res;
    }

    shader::result shader::create_from_files(const char *vertex_path, const char *fragment_path) noexcept {
        if (!vertex_path || !fragment_path) {
            return unexpected(error::INVALID_PARAMS);
        }

        std::string vs_src;
        std::string fs_src;

        if (!read_text_file(vertex_path, vs_src)) {
            SGL_LOG_ERROR("failed to read vertex shader file: %s", vertex_path);
            return unexpected(error::FILE_IO_FAILED);
        }

        if (!read_text_file(fragment_path, fs_src)) {
            SGL_LOG_ERROR("failed to read fragment shader file: %s", fragment_path);
            return unexpected(error::FILE_IO_FAILED);
        }

        return create_from_source(vs_src, fs_src);
    }

    // or panic wrappers

    shader shader::create_from_shaders_or_panic(gl_uint vertex_shader, gl_uint fragment_shader) noexcept {
        auto res = create_from_shaders(vertex_shader, fragment_shader);
        if (!res) {
            const auto err = res.error();
            SGL_LOG_FATAL("failed to create shader program from shaders: %s", shader::err_to_str(err));
        }
        return std::move(*res);
    }

    shader shader::create_from_source_or_panic(const char *vertex_src, const char *fragment_src) noexcept {
        auto res = create_from_source(vertex_src, fragment_src);
        if (!res) {
            const auto err = res.error();
            SGL_LOG_FATAL("failed to create shader from source: %s", shader::err_to_str(err));
        }
        return std::move(*res);
    }

    shader shader::create_from_files_or_panic(const char *vertex_path, const char *fragment_path) noexcept {
        auto res = create_from_files(vertex_path, fragment_path);
        if (!res) {
            const auto err = res.error();
            SGL_LOG_FATAL(
                "failed to create shader from files ('%s', '%s'): %s",
                vertex_path, fragment_path,
                shader::err_to_str(err)
            );
        }
        return std::move(*res);
    }

    // api

    void shader::use() const noexcept {
        glUseProgram(m_program);
    }

    gl_int shader::uniform_location(const char *name) const noexcept {
        if (!m_program || !name) {
            return -1;
        }
        return glGetUniformLocation(m_program, name);
    }

    void shader::set_uniform(gl_int loc, const void *value, shader_uniform_type type, gl_sizei count) const noexcept {
        set_uniform_impl(m_program, loc, value, type, count);
    }

    void shader::set_uniform(
        const char *name,
        const void *value,
        shader_uniform_type type,
        gl_sizei count
    ) const noexcept {
        if (!m_program || !name) {
            return;
        }

        const gl_int loc = glGetUniformLocation(m_program, name);
        if (loc < 0) {
            SGL_LOG_WARN("uniform '%s' not found", name);
            return;
        }

        set_uniform_impl(m_program, loc, value, type, count);
    }

    const char *shader::err_to_str(error err) noexcept {
        switch (err) {
            case error::INVALID_PARAMS: return "invalid params";
            case error::FILE_IO_FAILED: return "file I/O failed";
            case error::VERTEX_COMPILE_FAILED: return "vertex shader compile failed";
            case error::FRAGMENT_COMPILE_FAILED: return "fragment shader compile failed";
            case error::PROGRAM_LINK_FAILED: return "shader program link failed";
            default: return "unknown shader_error_e";
        }
    }

    // internal

    gl_uint shader::compile_shader(gl_enum type, const char *src, error &out_err) noexcept {
        assert(src);

        const gl_uint shader = glCreateShader(type);
        if (!shader) {
            SGL_LOG_ERROR("glCreateShader() failed");
            out_err = error::VERTEX_COMPILE_FAILED;
            return 0;
        }

        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        const char *type_name = "SHADER";
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
                out_err = error::VERTEX_COMPILE_FAILED;
            } else if (type == GL_FRAGMENT_SHADER) {
                out_err = error::FRAGMENT_COMPILE_FAILED;
            } else {
                out_err = error::VERTEX_COMPILE_FAILED;
            }

            return 0;
        }

        return shader;
    }

    bool shader::read_text_file(const char *path, std::string &out_src) noexcept {
        assert(path);

        std::ifstream f(path, std::ios::binary);
        if (!f) {
            return false;
        }

        f.seekg(0, std::ios::end);
        const auto len = f.tellg();
        if (len < 0) {
            return false;
        }

        out_src.resize(len);

        f.seekg(0, std::ios::beg);
        if (len > 0) {
            f.read(out_src.data(), len);
            if (!f) {
                return false;
            }
        }

        return true;
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

        SGL_LOG_ERROR("'%s' compilation failed:\n%s", type_name, log.c_str());
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

        SGL_LOG_ERROR("shader program linking failed:\n%s", log.c_str());
        return false;
    }

    void shader::set_uniform_impl(
        gl_uint program,
        gl_int loc,
        const void *value,
        shader_uniform_type type,
        gl_sizei count
    ) noexcept {
        if (loc < 0) {
            SGL_LOG_WARN("uniform location < 0");
            return;
        }

        if (!value) {
            SGL_LOG_WARN("uniform value is null");
            return;
        }

        glUseProgram(program);

        switch (type) {
            case shader_uniform_type::INT:
                glUniform1iv(loc, count, static_cast<const gl_int *>(value));
                break;
            case shader_uniform_type::INT_VEC2:
                glUniform2iv(loc, count, static_cast<const gl_int *>(value));
                break;
            case shader_uniform_type::INT_VEC3:
                glUniform3iv(loc, count, static_cast<const gl_int *>(value));
                break;
            case shader_uniform_type::INT_VEC4:
                glUniform4iv(loc, count, static_cast<const gl_int *>(value));
                break;

            case shader_uniform_type::UINT:
                glUniform1uiv(loc, count, static_cast<const gl_uint *>(value));
                break;
            case shader_uniform_type::UINT_VEC2:
                glUniform2uiv(loc, count, static_cast<const gl_uint *>(value));
                break;
            case shader_uniform_type::UINT_VEC3:
                glUniform3uiv(loc, count, static_cast<const gl_uint *>(value));
                break;
            case shader_uniform_type::UINT_VEC4:
                glUniform4uiv(loc, count, static_cast<const gl_uint *>(value));
                break;

            case shader_uniform_type::FLOAT:
                glUniform1fv(loc, count, static_cast<const gl_float *>(value));
                break;
            case shader_uniform_type::VEC2:
                glUniform2fv(loc, count, static_cast<const gl_float *>(value));
                break;
            case shader_uniform_type::VEC3:
                glUniform3fv(loc, count, static_cast<const gl_float *>(value));
                break;
            case shader_uniform_type::VEC4:
                glUniform4fv(loc, count, static_cast<const gl_float *>(value));
                break;

            case shader_uniform_type::MAT2:
                glUniformMatrix2fv(loc, count, GL_FALSE, static_cast<const gl_float *>(value));
                break;
            case shader_uniform_type::MAT3:
                glUniformMatrix3fv(loc, count, GL_FALSE, static_cast<const gl_float *>(value));
                break;
            case shader_uniform_type::MAT4:
                glUniformMatrix4fv(loc, count, GL_FALSE, static_cast<const gl_float *>(value));
                break;
        }
    }
} // namespace sgl
