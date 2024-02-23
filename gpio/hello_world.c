#include <furi.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/widget.h>
#include <gui/modules/submenu.h>
#include <gui/modules/text_input.h>

//  ------------------------------------------------------------------------
//
//  This program has the goal to watch that we understood how to do scenes and screens
//
//  -------------------------------------------------------------------------

typedef enum {
    hello_world_scene,
    second_scene,
    count_scene,
} appScene;

typedef enum {
    widgetViews,
} scenesViews;

typedef struct App {
    SceneManager* scene_manager;
    ViewDispatcher* view_dispatcher;
    Widget* widget;
} App;
// ------------------------------------------------------------------------------------
//
//
// ------------------------------------------------------------------------------------
void hello_world_scene_on_enter(void* context) {
    App* app = context;
    widget_reset(app->widget);

    widget_add_string_element(
        app->widget, 25, 15, AlignLeft, AlignCenter, FontPrimary, "Hello World");

    view_dispatcher_switch_to_view(app->view_dispatcher, widgetViews);
}

bool hello_world_scene_on_event(void* context, SceneManagerEvent event) {
    UNUSED(event);
    App* app = context;
    bool consumed = false;
    return consumed;
}

void hello_world_scene_on_exit(void* context) {
    UNUSED(context);
}

// ------------------------------------------------------------------------------------

void second_scene_on_enter(void* context) {
    App* app = context;
    widget_reset(app->widget);

    widget_add_string_element(
        app->widget, 25, 15, AlignLeft, AlignCenter, FontPrimary, "WELCOME!!");

    view_dispatcher_switch_to_view(app->view_dispatcher, widgetViews);
}

bool second_scene_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);

    return false;
    //App* app = context;
    //bool consumed = false;

    //return consumed;
}

void second_scene_on_exit(void* context) {
    UNUSED(context);

    //App* app = context;
}
// ------------------------------------------------------------------------------------
//
//
//
// -----------------------------------------------------------------------------------
void (*const app_scenes_on_enter[])(void*) = {
    hello_world_scene_on_enter,
    second_scene_on_enter,
};

bool (*const app_scenes_on_event[])(void*, SceneManagerEvent) = {
    hello_world_scene_on_event,
    second_scene_on_event,
};

void (*const app_scenes_on_exit[])(void*) = {
    hello_world_scene_on_exit,
    second_scene_on_exit,
};

static const SceneManagerHandlers app_scenes_handlers = {
    .on_enter_handlers = app_scenes_on_enter,
    .on_event_handlers = app_scenes_on_event,
    .on_exit_handlers = app_scenes_on_exit,
    .scene_num = count_scene,
};
// ------------------------------------------------------------------------------------
//
//
// -----------------------------------------------------------------------------------
static bool app_scene_costum_callback(void* context, uint32_t costum_event) {
    furi_assert(context);
    App* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, costum_event);
}

static bool app_scene_back_event(void* context) {
    furi_assert(context);
    App* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}
// ------------------------------------------------------------------------------------
//
//
// ------------------------------------------------------------------------------------

App* app_alloc() {
    App* app = malloc(sizeof(App));
    app->scene_manager = scene_manager_alloc(&app_scenes_handlers, app);

    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(app->view_dispatcher);
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, app_scene_costum_callback);
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_navigation_event_callback(app->view_dispatcher, app_scene_back_event);

    app->widget = widget_alloc();
    view_dispatcher_add_view(app->view_dispatcher, widgetViews, widget_get_view(app->widget));

    return app;
}

// ------------------------------------------------------------------------------------
//
//
// ------------------------------------------------------------------------------------

static void app_free(App* app) {
    furi_assert(app);
    view_dispatcher_remove_view(app->view_dispatcher, widgetViews);
    scene_manager_free(app->scene_manager);
    view_dispatcher_free(app->view_dispatcher);
    widget_free(app->widget);
    free(app);
}

// ------------------------------------------------------------------------------------
//
//
// ------------------------------------------------------------------------------------

int32_t app_main(void* p) {
    UNUSED(p);

    App* app = app_alloc();

    Gui* gui = furi_record_open(RECORD_GUI);

    view_dispatcher_attach_to_gui(app->view_dispatcher, gui, ViewDispatcherTypeFullscreen);

    scene_manager_next_scene(app->scene_manager, hello_world_scene);

    view_dispatcher_run(app->view_dispatcher);

    app_free(app);

    return 0;
}
