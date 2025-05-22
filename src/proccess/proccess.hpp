#pragma once

#include <debug/debug.hpp>

#include <string_view>
#include <filesystem>
#include <unistd.h>
#include <cstdlib>

namespace sk
{
    namespace proccess
    {

        std::filesystem::path binary_path_of(std::string_view pname);
        
        template <typename... Args>
        requires(std::is_convertible_v<Args, const char*> && ...) 
        void spawn_detach(std::string_view pname, Args... args) 
        {
            if(fork() == 0)
            {
                auto binary_path = binary_path_of(pname);
                const char* binary_file = binary_path.c_str();
                sk::debug::info(std::source_location::current(), "spawning {}", binary_file);
                execl(binary_file, binary_file, std::forward(args)..., nullptr);
                std::_Exit(EXIT_SUCCESS);
            }
        }
    }
}