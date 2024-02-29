#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/widget.h>
#include <gui/modules/submenu.h>
#include <gui/modules/text_input.h>

#include "scenes/app_scene_functions.h"

//--------------------------------------------------------------------------
// This code works to LOG or show in the LOG the messages you want to show
//--------------------------------------------------------------------------

#define LOG_TAG "HWSPACE"
#define LOG_SHOW true

#define log_info(format, ...) \
    if(LOG_SHOW) FURI_LOG_I(LOG_TAG, format, ##__VA_ARGS__)

#define log_exception(format, ...) \
    if(LOG_SHOW) FURI_LOG_E(LOG_TAG, format, ##__VA_ARGS__)

#define log_warning(format, ...) \
    if(LOG_SHOW) FURI_LOG_W(LOG_TAG, format, ##__VA_ARGS__)

//--------------------------------------------------------------------------
// Here you create the enum but you can put it in the main file
//--------------------------------------------------------------------------

typedef struct {
    SceneManager* scene_manager;
    ViewDispatcher* view_dispatcher;
    Widget* widget;
} App;

//--------------------------------------------------------------------------
//  Here you enum the Views, like menus, widgets, etc..
//--------------------------------------------------------------------------

typedef enum {
    ViewWidget,
} scenesViews;