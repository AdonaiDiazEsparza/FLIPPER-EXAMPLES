## HELLO!!
In this program I used .h files and .c files to create something like a library, you can take them from the folder "scenes" and the "app_user.h", these files have the codes to define and name the functions for every scene you want to use in your app.

## NOTE:
For every scene you add, they must be in order and the name for every function by scene have to be named like the code indicate.

for example: 
In the file "app_scene_config.h" there you put ADD(prefix,name,id), this is to add a scene and if I want to call one scene like "HelloWorld" the prefix can be "app", name "hello_world", and id "HelloWorld".
```
                ADD(app,hello_world,HelloWorld)
```
So the name in the enum is define for 

```
#define ADD_SCENE(prefix, name, id) AppScene##id,
typedef enum {
    #include "app_scene_config.h"
    AppSceneEnum,
} Appscenes;
#undef ADD_SCENE
```

The name will be: AppSceneHelloWorld

```
#define ADD_SCENE(prefix, name, id) void prefix##_scene_##name##_on_enter(void*);

#define ADD_SCENE(prefix, name, id) bool prefix##_scene_##name##_on_event(void* context, SceneManagerEvent event);

#define ADD_SCENE(prefix, name, id) void prefix##_scene_##name##_on_exit(void* context);

```

For every function of my scene will be call like

```
void app_scene_hello_world_on_enter(void*);
bool app_scene_hello_world_on_enter(void* context, SceneManagerEvent event);
void app_scene_hello_world_on_enter(void* context);
```

And every function will be write in other file or in the same main file to define what will do every function of the scene.
