#include <debug/debug.hpp>
#include <proccess/proccess.hpp>
#include <wayland-server-core.h>
#include <wayland_objects/objects.hpp>

#include <format>
#include <iostream>
#include <utility>
#include <cstdlib>


#include <wlr.h>

#include <wayland-server.h>


int main()
{
  wlr_log_init(WLR_DEBUG, NULL);

  auto& global { sk::Global::get() };

  global.display = wl_display_create();
  global.backend = wlr_backend_autocreate(wl_display_get_event_loop(global.display), nullptr);
  global.renderer2d = wlr_renderer_autocreate(global.backend);
  wlr_renderer_init_wl_display(global.renderer2d, global.display);
  global.allocator = wlr_allocator_autocreate(global.backend, global.renderer2d);

  wlr_compositor_create(global.display, 5, global.renderer2d);
  wlr_subcompositor_create(global.display);
  wlr_data_device_manager_create(global.display);

  global.output_layout = wlr_output_layout_create(global.display);

  global.scene = wlr_scene_create();
  global.scene_output_layout = wlr_scene_attach_output_layout(global.scene, global.output_layout);

  global.shell = wlr_xdg_shell_create(global.display, 3);

  global.socket_name = wl_display_add_socket_auto(global.display);

  if(global && wlr_backend_start(global.backend))
  {
    sk::debug::info(std::source_location::current(), "initialized global vars successfully"); 
    sk::debug::info(std::source_location::current(), "running on socket {}", global.socket_name);
    setenv("WAYLAND_DISPLAY", global.socket_name, true);  
    wl_display_run(global.display);
  }
  else
  {
    sk::debug::error(std::source_location::current(), "could not init globals, aborting");
    return EXIT_FAILURE;
  }


  return EXIT_SUCCESS;
}
