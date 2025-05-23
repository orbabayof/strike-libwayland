#include <wayland-server-core.h>
#include <wayland_objects/objects.hpp>
#include <debug/debug.hpp>

sk::Global& sk::Global::get()
{
    static Global global {};
    return global;
}

sk::Global::operator bool()
{
    return display && backend && renderer2d && allocator && socket_name;
}

sk::Output::Output(wlr_output* wlr): _wlr { wlr }
{}

void sk::Output::commit_state(const wlr_output_state* output_state)
{
    wlr_output_commit_state(_wlr, output_state);
}

void sk::Output::set_mode()
{
    Global& global { sk::Global::get() };
    wlr_output_init_render(_wlr, global.allocator, global.renderer2d);

    struct wlr_output_state state;
	wlr_output_state_init(&state);
	wlr_output_state_set_enabled(&state, true);

    if(wlr_output_mode* mode = wlr_output_preferred_mode( _wlr ))
    {
        wlr_output_state_set_mode(&state, mode);
        debug::info(std::source_location::current(), "output mode is set");
    }
    else
    {
        debug::info(std::source_location::current(), "mode is not needed for output");
    }

    commit_state(&state);
    wlr_output_state_finish(&state);


}

void sk::Output::set_screens()
{
    Global& global { sk::Global::get() };
    wlr_output_layout_output* out_lay_out { wlr_output_layout_add_auto(global.output_layout, _wlr) };
    wlr_scene_output* scene_output { wlr_scene_output_create(global.scene, _wlr) };

    wlr_scene_output_layout_add_output(global.scene_output_layout, out_lay_out, scene_output);

    debug::info(std::source_location::current() ,"screens are set");
}


void sk::Output::init_listeners()
{
  namespace ph = std::placeholders;
  std::function<void(void*)> frame_callback { std::bind(sk::Output::frame, this, ph::_1) };
  _frame_listener.init(&_wlr->events.frame, frame_callback);

  std::function<void(void*)> destroy_callback { std::bind(sk::Output::destroy, this, ph::_1) };
  _destroy_listener.init(&_wlr->events.destroy, destroy_callback);
}

void sk::Output::render_commit()
{
    Global& global { Global::get() };
    wlr_scene_output* scene_out { wlr_scene_get_scene_output(global.scene, _wlr) };
    wlr_scene_output_commit(scene_out, nullptr);
    debug::info(std::source_location::current(), "scene output commited");

    std::timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    wlr_scene_output_send_frame_done(scene_out, &now);
}
