#include "internal/sgl_io.h"

#include <fstream>

namespace sgl::detail {
    bool read_text_file(const char *path, std::string &out_src) noexcept {
        if (!path) {
            return false;
        }

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
}