#pragma once

#include <string>
#include <filesystem>

namespace sgl::detail {
    bool read_text_file(const char *path, std::string &out_src) noexcept;

    inline bool read_text_file(const std::string &path, std::string &out_src) noexcept {
        return read_text_file(path.c_str(), out_src);
    }

    inline bool read_text_file(const std::filesystem::path &path, std::string &out_src) noexcept {
        return read_text_file(path.string().c_str(), out_src);
    }
}
