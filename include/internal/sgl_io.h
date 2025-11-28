#pragma once

#include <string>

namespace sgl::detail {
    bool read_text_file(const char *path, std::string &out_src) noexcept;

    inline bool read_text_file(const std::string &path, std::string &out_src) noexcept {
        return read_text_file(path.c_str(), out_src);
    }
}