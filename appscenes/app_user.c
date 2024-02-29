#include "app_user.h"

//--------------------------------------------------------------------------
//  To costum the callbacks
//--------------------------------------------------------------------------

static bool app_scene_costum_callback(void* context, uint32_t costum_event) {
    furi_assert(context);
    App* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, costum_event);
}

//--------------------------------------------------------------------------
//  To get the events back from the scenes
//--------------------------------------------------------------------------

static bool app_scene_back_event(void* context) {
    furi_assert(context);
    App* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

//--------------------------------------------------------------------------
//  This function Initialize the app
//--------------------------------------------------------------------------

static App* app_alloc() {
    App* app = malloc(sizeof(App));
    app->scene_manager = scene_manager_alloc(&app_scene_handlers, app);
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(app->view_dispatcher);
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, app_scene_costum_callback);
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_navigation_event_callback(app->view_dispatcher, app_scene_back_event);

    app->widget = widget_alloc(); // we initialize the widget
    view_dispatcher_add_view(
        app->view_dispatcher,
        ViewWidget,
        widget_get_view(app->widget)); // we add the widgets to the view dispatcher
    return app;
}

//--------------------------------------------------------------------------
//  To close the app
//--------------------------------------------------------------------------

static void app_free(App* app) {
    furi_assert(app);
    view_dispatcher_remove_view(app->view_dispatcher, ViewWidget);
    scene_manager_free(app->scene_manager);
    view_dispatcher_free(app->view_dispatcher);
    widget_free(app->widget);
    free(app);
}

//--------------------------------------------------------------------------
//  Main
//--------------------------------------------------------------------------

int app_main(void* p) {
    UNUSED(p);

    App* app = app_alloc();

    Gui* gui = furi_record_open(RECORD_GUI);

    view_dispatcher_attach_to_gui(app->view_dispatcher, gui, ViewDispatcherTypeFullscreen);

    scene_manager_next_scene(app->scene_manager, AppSceneHelloWorld);

    view_dispatcher_run(app->view_dispatcher);

    app_free(app);

    return 0;
}