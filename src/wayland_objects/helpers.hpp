#pragma once

#include <memory>
#include <functional>

#include <wayland-server.h>

namespace sk
{
    template <typename T, typename U = std::default_delete<T>>
    class unique_ptr 
    {
    public:
        unique_ptr(): _std { nullptr }
        {}

        unique_ptr(T* ptr) : _std { ptr }
        {}

        unique_ptr(std::unique_ptr<T,U> ptr) : _std { std::move(ptr) }
        {}

        operator T*() { return _std.get(); }
        T* operator ->() { return _std.get(); }

        auto& get_std() { return _std; }

    private:

        std::unique_ptr<T, U> _std;
    };

    class Listener
    {
    public:
        Listener() = default;
        ~Listener();
        void init(wl_signal* sig, std::function<void(void* data)> callback);
    private:

        static void wrap_func(wl_listener* lis, void* data);

        wl_listener _wl;
        //classes will add their *this* ptr in the lambda
        std::function<void(void* data)> _callback;
    };
}
