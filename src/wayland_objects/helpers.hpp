#include <memory>

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

        auto& get_std() { return _std; }

    private:

        std::unique_ptr<T, U> _std;
    };
}
