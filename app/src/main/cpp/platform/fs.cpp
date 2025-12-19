#include "fs.h"
#include <cstdio>

bool write_text_file(const std::string& path, const std::string& content) {
    FILE* f = fopen(path.c_str(), "wb");
    if (!f) return false;
    const size_t n = fwrite(content.data(), 1, content.size(), f);
    fclose(f);
    return n == content.size();
}

bool write_bytes_file(const std::string& path, const void* data, size_t size) {
    FILE* f = fopen(path.c_str(), "wb");
    if (!f) return false;
    const size_t n = fwrite(data, 1, size, f);
    fclose(f);
    return n == size;
}
