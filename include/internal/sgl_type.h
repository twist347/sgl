#pragma once

#include <cstdint>
#include <cstddef>

namespace sgl {
    using gl_boolean = unsigned char; // GLboolean
    using gl_byte = signed char; // GLbyte
    using gl_ubyte = unsigned char; // GLubyte

    using gl_short = short; // GLshort
    using gl_ushort = unsigned short; // GLushort

    using gl_int = int; // GLint
    using gl_uint = unsigned int; // GLuint

    using gl_sizei = int; // GLsizei

    using gl_enum = unsigned int; // GLenum
    using gl_bitfield = unsigned int; // GLbitfield

    using gl_float = float; // GLfloat
    using gl_double = double; // GLdouble

    using gl_char = char; // GLchar

    using gl_intptr = std::intptr_t; // GLintptr
    using gl_sizeiptr = std::ptrdiff_t; // GLsizeiptr
}
