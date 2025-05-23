#include <filesystem>
#include <string_view>
#include <unistd.h>

namespace sk
{
  namespace internal_settings
  {
    inline const std::filesystem::path log_filename { std::string(getenv("HOME")) + "/strike-libwayland/log/log.txt" };
  }
}
