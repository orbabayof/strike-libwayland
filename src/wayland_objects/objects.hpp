#include <wayland-server-core.h>
#include <wayland-server.h>

#include "helpers.hpp"
#include "destroy.hpp"

#include <wlr.h>


namespace sk
{
  struct Global
  {
    static Global& get();
    operator bool();
    sk::unique_ptr<wl_display> display;
    sk::unique_ptr<wlr_backend> backend;
    sk::unique_ptr<wlr_renderer> renderer2d;
    sk::unique_ptr<wlr_allocator> allocator;
    sk::unique_ptr<wlr_scene> scene;
    
    wlr_output_layout* output_layout;
    wlr_scene_output_layout* scene_output_layout;
    wlr_xdg_shell* shell;

    const char* socket_name;

  };
  
  class Output
  {
  public:

    Output(wlr_output* output);

  private:
  };

}

