#include "objects.hpp"
#include <wlr.h>
#include <debug/debug.hpp>

void sk::Global::new_output(sk::Global*, void* data)
{
    Output* output { new Output { static_cast<wlr_output*>(data) } };
    if(!output)
    {
        debug::error(std::source_location::current(), "could not allocate memory");
        return;
    }
    output->set_mode();
    output->set_screens();

    output->init_listeners();
}

void sk::Output::frame(Output* output, void*)
{
    output->render_commit(); 
}

void sk::Output::destroy(Output* output, void*)
{   
    delete output;
    sk::debug::info(std::source_location::current(), "output deleted");
}

void sk::Output::request_state(Output* output, void* data)
{
    const auto* event { static_cast<const wlr_output_event_request_state*>(data) };
    output->commit_state(event->state);
}
