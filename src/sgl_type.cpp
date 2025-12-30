#include "internal/sgl_type.h"

#include <type_traits>

#include "glad/glad.h"

namespace {
    static_assert(std::is_same_v<sgl::gl_boolean, GLboolean>);
    static_assert(std::is_same_v<sgl::gl_byte, GLbyte>);
    static_assert(std::is_same_v<sgl::gl_ubyte, GLubyte>);

    static_assert(std::is_same_v<sgl::gl_short, GLshort>);
    static_assert(std::is_same_v<sgl::gl_ushort, GLushort>);
    static_assert(std::is_same_v<sgl::gl_int, GLint>);
    static_assert(std::is_same_v<sgl::gl_uint, GLuint>);

    static_assert(std::is_same_v<sgl::gl_int64, GLint64>);
    static_assert(std::is_same_v<sgl::gl_uint64, GLuint64>);

    static_assert(std::is_same_v<sgl::gl_sizei, GLsizei>);
    static_assert(std::is_same_v<sgl::gl_enum, GLenum>);
    static_assert(std::is_same_v<sgl::gl_bitfield, GLbitfield>);

    static_assert(std::is_same_v<sgl::gl_float, GLfloat>);
    static_assert(std::is_same_v<sgl::gl_double, GLdouble>);

    static_assert(std::is_same_v<sgl::gl_char, GLchar>);

    static_assert(std::is_same_v<sgl::gl_intptr, GLintptr>);
    static_assert(std::is_same_v<sgl::gl_sizeiptr, GLsizeiptr>);

    static_assert(sizeof(sgl::gl_intptr) == sizeof(void *));
    static_assert(sizeof(sgl::gl_sizeiptr) == sizeof(void *));

    static_assert(std::is_same_v<sgl::gl_void, GLvoid>);

    using under = std::underlying_type_t<sgl::idx_type>;

    static_assert(static_cast<under>(sgl::idx_type::u8) == static_cast<under>(GL_UNSIGNED_BYTE));
    static_assert(static_cast<under>(sgl::idx_type::u16) == static_cast<under>(GL_UNSIGNED_SHORT));
    static_assert(static_cast<under>(sgl::idx_type::u32) == static_cast<under>(GL_UNSIGNED_INT));
}
