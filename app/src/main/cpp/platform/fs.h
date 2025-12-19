#pragma once
#include <string>
#include <cstddef>

bool write_text_file(const std::string& path, const std::string& content);
bool write_bytes_file(const std::string& path, const void* data, size_t size);
