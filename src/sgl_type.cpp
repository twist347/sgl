#include "internal/sgl_type.h"

#include "glad/glad.h"

#include <type_traits>

namespace {
    static_assert(std::is_same_v<sgl::gl_boolean, GLboolean>);
    static_assert(std::is_same_v<sgl::gl_byte, GLbyte>);
    static_assert(std::is_same_v<sgl::gl_ubyte, GLubyte>);

    static_assert(std::is_same_v<sgl::gl_short, GLshort>);
    static_assert(std::is_same_v<sgl::gl_ushort, GLushort>);
    static_assert(std::is_same_v<sgl::gl_int, GLint>);
    static_assert(std::is_same_v<sgl::gl_uint, GLuint>);

    static_assert(std::is_same_v<sgl::gl_sizei, GLsizei>);
    static_assert(std::is_same_v<sgl::gl_enum, GLenum>);
    static_assert(std::is_same_v<sgl::gl_bitfield, GLbitfield>);

    static_assert(std::is_same_v<sgl::gl_float, GLfloat>);
    static_assert(std::is_same_v<sgl::gl_double, GLdouble>);

    static_assert(std::is_same_v<sgl::gl_char, GLchar>);

    static_assert(std::is_same_v<sgl::gl_intptr, GLintptr>);
    static_assert(std::is_same_v<sgl::gl_sizeiptr, GLsizeiptr>);
}
