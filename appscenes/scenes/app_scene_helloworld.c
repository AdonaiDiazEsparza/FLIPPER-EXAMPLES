#include "../app_user.h"

void app_scene_hello_world_on_enter(void* context) {
    App* app = context;
    widget_reset(app->widget);

    widget_add_string_element(
        app->widget, 25, 15, AlignLeft, AlignCenter, FontPrimary, "Hello World");

    view_dispatcher_switch_to_view(app->view_dispatcher, ViewWidget);
}

bool app_scene_hello_world_on_event(void* context, SceneManagerEvent event) {
    UNUSED(event);
    UNUSED(context);
    bool consumed = false;
    return consumed;
}

void app_scene_hello_world_on_exit(void* context) {
    UNUSED(context);
}