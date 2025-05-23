#include "helpers.hpp"

#include <wayland-server.h>

void sk::Listener::wrap_func(wl_listener* l, void* data)
{
    Listener* listener = wl_container_of(l, listener, _wl);
    listener->_callback(data);
}

void sk::Listener::init(wl_signal* sig, std::function<void(void* data)> callback)
{
    //listeners will be wrapped to call the callback std::function
    _wl = { .notify = wrap_func };
    _callback = callback;

    wl_signal_add(sig, &_wl);
}

sk::Listener::~Listener()
{
    wl_list_remove(&_wl.link);
}