#pragma once
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

    Listener new_output_listen;

    const char* socket_name;

    static void new_output(sk::Global* global, void* data);

  };
  
  class Output
  {
  public:

    Output(wlr_output* output);

    void commit_state(const wlr_output_state* output_state);

    void set_mode();
    void set_screens();
    void init_listeners();
    void render_commit();

  private:

    static void frame(Output* output, void* data); 
    static void destroy(Output* output, void* data);
    static void request_state(Output* output, void* data); 

    wlr_output* _wlr;
    Listener _frame_listener;
    Listener _destroy_listener;
  };

}

