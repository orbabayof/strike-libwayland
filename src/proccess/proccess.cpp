#include <proccess/proccess.hpp>

#include <string>
#include <filesystem>

std::filesystem::path sk::proccess::binary_path_of(std::string_view pname)
{
    constexpr std::string_view unix_binary_path { "/usr/bin" };
    std::filesystem::path binary_file { unix_binary_path };

    return binary_file /= pname;
}