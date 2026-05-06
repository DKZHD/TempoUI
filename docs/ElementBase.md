# Element Base
*The foundation of all elements within TempoUI.*

### Absolute vs Local Position
- Absolute position is the position on the screen a element is and can be accessed by utilizing `get_position`. 

- The local position, is the position of the element relative to its parenting elements and can be accessed using the `get_local_position` function. The local position can be set as well utilizing the `set_local_position` function.

### Recursive Functions
All of these functions will call the entire child hierarchy as well as its own version of a specific function. The relevant functions are split into two type `self_some_func` and `some_func` where the `some_func` version is the recursive one.

``` cpp
update_buffers() // The core of the retained-mode dirty flag system
// Whenever the mark_dirty function is called (which is recursive), the object parented to this object then the parent of that one etc. This function will only called whenever it has a dirty flag.
```

``` cpp
theme_updated() // A recursive function which updates all elements based on current theme. Core of hot-reloading themes.
```

``` cpp
scale_updated(vec2 new_scale) // A recursive function which updates all elements based on current resolution / original resolution. Required for resizing window.
```
#### Input functions
These functions are what needs to be called for the input to propagate from the root to the leaf elements.
``` cpp
// Core Functions
on_key(UI::Key key, UI::Action action); // Used when using keybinds for a element.
on_mouse_button(UI::MouseButton key, UI::Action action); // Mouse input
on_char(char c); // Used for typing mostly.
on_scroll(float yOffset); // Used for scrolling behavior. 
```

### Hover Functions
These function are here so that one can easily define the hover behavior of a specific element without the class needing to change behavior.
``` cpp
some_element_ptr->on_hover([](){
	// Functionality
});

some_element_ptr->on_unhover([](){
	// Functionality
});
```

### Update
The update function of the elements are quite important for some elements to function properly. This function needs to be called every frame for it to function properly. 

>[!Note]
>Utilizing the UIRenderer's draw function already calls this for the main screen canvas, meaning every element that is a child of it in some fashion already is handled.

### Element States
There is a bitflag defining certain properties for a element. These are:
``` cpp
VISIBLE, IS_HOVERED, CLICKABLE, HOVERABLE, IS_CONTAINER, CLIP_CHILDREN, IS_OVERLAY
```
As a developer there are only 3 you probably need to know about.
`VISIBLE`: If this flag is not set, the entire element and its children will not be rendered.
`IS_HOVERED`: Relevant if you want to check if it hovered for some reason.
`IS_OVERLAY`: If set, the element will render above all non-overlay elements. 

These flags can all be set using the `set_state` and the `toggle_state` functions. The main difference being that set makes the value something and toggle inverses it.
``` cpp
element_ptr->set_state(VISIBLE, false);

element_ptr->toggle_state(VISIBLE);
```

These states can also be checked by utilizing the `check_state` function.
``` cpp
if(element_ptr->check_state(VISIBLE)==false){
	// Execute
}
```

### Default Element Config
The config contains:

| Name   | Type   | Notes                                                                                                                                                                               |
| ------ | ------ | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| id     | string | identifier of element                                                                                                                                                               |
| pos    | vec2   | local position of element                                                                                                                                                           |
| size   | vec2   | absolute size on screen                                                                                                                                                             |
| radius | vec4   | rounding of corners. `[Top Left, Top Right, Bottom Left, Bottom Right]`                                                                                                             |
| flex   | int    | If placed inside certain containers will fill available space.<br>Similar to a priority. If one element has 3 flex and another one has 1 flex<br>the first element will take up 75% |
