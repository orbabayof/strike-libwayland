#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <format>
#include <fstream>
#include <source_location>
#include <sstream>
#include <string_view>
#include <syncstream>
#include <internal_settings.hpp>
#include <iostream>
#include <utility>

namespace sk
{
  namespace debug
  {



    namespace console
    {
      namespace paint
      {
        enum Type
        {
          red = 31,
          green = 32,
          yellow = 33,
          blue = 34,

          no_color = 39,
        };

        static constexpr std::string color_code(Type t) { return (std::ostringstream{} << "\033[" << t << "m").str(); }

        static constexpr std::string_view reset { "\033[0m" };
      };
    }

    enum class Severity
    {
      info,
      warning,
      error,
    };

    class Log
    {
    public:

    Log(std::filesystem::path path): _log_file { path } {}
      
    template <typename... Args>
    void message(const std::source_location& location, Severity severity, std::format_string<Args...> fmt, Args&&... args)
    {
      namespace cp = console::paint;
      auto msg_color { cp::color_code( color_escape_code_of(severity) ) };
      auto msg { std::format(fmt, std::forward<Args>(args)...) };
      auto msg_nocolor { (std::ostringstream{} << cp::reset << msg << msg_color) };

      std::ostringstream location_in_file;
      location_in_file << location.file_name() << '(' << location.line() << ':' << location.column() << ')';

      std::ostringstream func;
      func << "<" << location.function_name() << ">: ";

      std::osyncstream{ _log_file } << func.str() << msg << ' ' << location_in_file.str() << '\n';
      std::osyncstream{ std::clog } << msg_color << func.str() << msg_nocolor.str() << ' ' << location_in_file.str() << cp::reset << '\n';
    }

    void close() { _log_file.close(); }

    private:

    std::ofstream _log_file;

    constexpr static console::paint::Type color_escape_code_of(Severity severity)
    {
      using enum Severity;
      using color_t = console::paint::Type;
      switch(severity)
      {
        case error: return color_t::red;
        case warning : return color_t::yellow;
        case info : return color_t::green;

        default: return color_t::no_color;
      }
    }
    };
    
    inline auto& global_log()
    {

      static Log log { sk::internal_settings::log_filename };
      return log;
    }
    
    template <typename... Args>
    void warning(const std::source_location& location, std::format_string<Args...> fmt, Args... args)
    {
      global_log().message(location, Severity::warning, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void error(const std::source_location& location, std::format_string<Args...> fmt, Args... args)
    {
      global_log().message(location, Severity::error, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void info(const std::source_location& location, std::format_string<Args...> fmt, Args... args)
    {
      global_log().message(location, Severity::info, fmt, std::forward<Args>(args)...);
    }

    inline void global_log_close() { global_log().close(); }
  }
}
