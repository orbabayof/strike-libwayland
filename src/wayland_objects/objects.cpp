#include <wayland-server-core.h>
#include <wayland_objects/objects.hpp>

sk::Global& sk::Global::get()
{
    static Global global {};
    return global;
}

sk::Global::operator bool()
{
    return display && backend && renderer2d && allocator && socket_name;
}