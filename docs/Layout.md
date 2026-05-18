# Layout
The main intended way to create a UI using TempoUI is through the JSON layout loader system.

>[!Important]
>To use the layout loader system, the macro `LAYOUT_LOADER` has to be defined. For best results and consistency it is highly recommended to utilize preprocessor definitions for this purpose. 
>
>If you still want to define it manually using a macro inside the project, ensure that the `#define` is before any TempoUI includes in the project.
### Functionality
#### Loading Layout
As many other features within TempoUI, the layout functionality is handled through the UIRenderer class. The `load_layout` function inside UIRenderer is simply a wrapper that calls the LayoutLoader's function for loading the layout and automatically binds it to the screen canvas.

If you for some reason want to access the layout's own canvas, it will always get saved as `Layout_Canvas`. 

``` cpp
ui_renderer.load_layout(/*JSON file path (Something like "layouts/main.json")))*/);
```

>[!Warning]
>Do note that this file is relative and might need to be copied during compilation to an executable 

>[!TIP]
>Calling this function without the layout existing will cause it to be created at the appropriate position in regards to where you specified
#### Layout Callback
An advantage with using the UIRenderer's layout loading instead of the LayoutLoader class itself is the fact that you can bind a callback to it. This means that when you hot-reload the layout during run-time the callback can include bindings with buttons etc. so that it does not break during reload.
### Format
#### Getting Started with JSON

| Complex Type | Format                               |
| :----------- | ------------------------------------ |
| Object       | "Name" : {<br>"key": value<br>}      |
| Array        | "Name": `[property1, property2 ...]` |
In JSON every value is a key-value pair, meaning it needs a name (string only) and a value of some kind. Comments are not allowed in standard JSON as well as `key` only values. 

Start by wrapping the entire file in curly brackets.
``` json
{
	"key": value,
	...
}
```
Every aspect of this file needs to be within that scope.

Another important thing to keep in mind is that every value needs to be separated with `,` except the last value of a scope. 
#### TempoUI Specific
Remember that every key is case-sensitive.
Check out individual elements to see specific JSON Features to each element.

### Hot-reloading
One of the main purposes of the layout system is hot-reloading. Hot-reloading is simply the act of loading the file during run-time to update data. 

To achieve this in a concise and efficient way within TempoUI you can use this method:

``` cpp
// Bind a callback to layout change
ui_renderer.bind_layout_callback([](){
	/*
		Elements with special properties
		Always use this for getting and binding properties if hot-reloading!
	*/
});
```
>[!Important]
>Remember to get the element inside the callback, DO NOT rely on pointers as they will be invalidated. The only pointer that is directly stable is the `Screen` Canvas element 

``` cpp
// Bind a key press to a function
ui_renderer.remove_element_from_canvas("Layout_Canvas");
ui_renderer.load_layout(/*json file name*/);
```
Here we first delete the previous layout (stored as `Layout_Canvas` as explained earlier), then we simply load it again. This will also cause the callback set with the `bind_layout_callback` function to trigger as well.