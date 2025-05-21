#include <exception>
#include <fstream>
#include <iostream>
#include <source_location>
#include <string_view>
#include <syncstream>
#include <iostream>

#include <debug/debug.hpp>
#include <thread>

#include <format>
#include <iostream>
#include <utility>


int main()
{
  sk::debug::error(std::source_location::current(), "{}", "k");
}
