#pragma once
#include <wayland-server.h>
#include <memory>
#include <xkbcommon/xkbcommon.h>

#include <wlr.h>

template<typename T, void(*func)(T*)>
struct func_destroyer
{
  void operator()(T* ptr) 
  {
    func(ptr);
  }
};

inline void destroy_display_with_clients(wl_display* display)
{
  wl_display_destroy_clients(display);
  wl_display_destroy(display);
};

#define decl_destroyer(type, func) template <>\
  struct std::default_delete<type> : func_destroyer<type, func>{}

//libwayland
decl_destroyer(wl_display, destroy_display_with_clients);

//wlroots
decl_destroyer(wlr_allocator, wlr_allocator_destroy);
decl_destroyer(wlr_renderer, wlr_renderer_destroy);

//libxkbcommon  
decl_destroyer(xkb_context, xkb_context_unref);
decl_destroyer(xkb_keymap, xkb_keymap_unref);

#undef decl_destroyer
