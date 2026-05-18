![TempoUI Banner](docs/images/TempoUIBanner.png)
# TEMPO UI
### Premise
TempoUI was created as a middle ground between the user interface giants Qt and Dear ImGui. It is created as a retained-mode UI library similar to Qt, but adopts some aspects from ImGui alongside custom features designed for rapid iteration and development. In essence this library tries to bridge the runtime speed of a compiled Qt application and the development speed of an ImGui application.
### *Important Notes*
To get started using this library there are some smaller features that have to be explained. Some features are disabled by default.

>[!TIP]
>To enable optional features, add the relevant Preprocessor Definitions to your project
>
>* `GLFW_UI`:  Enables glfw for the inputs of the UI. 
>* `OPENGL`: Enables OpenGL as rendering backend.
>* `LAYOUT_LOADER`: Enables JSON layout loading.
>* `NETWORK`: Enables TCP networking features. [Networking Documentation](docs/Network.md)
>
>This can also be achieved by defining a macro with the same names before the inclusion of "TempoUI.h". It is still recommended to use preprocessor definitions for consistency. 
###### *Planned:*
- [ ] Improving Drag and Drop
- [ ] Adding support for **Vulkan** backend
- [ ] Adding support for **SDL2**

### *Getting Started*

TempoUI itself doesn't include the capabilities of window management, therefore this has to be handled by you. TempoUI works out of its UIRenderer class and only really requires 4 function calls to work itself. 

---
**Minimal Working Example**:

``` cpp
#include <TempoUI/TempoUI.h>

// Window context already created

TempoUI::UIRenderer ui_renderer(/*JSON theme data, if wanted (raw data)*/);
ui_renderer.init(window_width, window_height);
// Multiple input bindings will be added later, change GLFW with wanted handler
TempoUI::UI::GLFW::init_input(window_context);

// In run loop
if(ui_renderer.draw(delta_time))
{
	// swap buffers and other wanted logic
}
```

For more details on hooking it up to your application, check out:
[CMake Integration](docs/CMakeIntegration.md)
##### **General Element Management**

*Element Creation:*
``` cpp
ui_renderer.add_element<Type>(Arguments);
```
Element Removal:
``` cpp
ui_renderer.remove_element_from_canvas(identifier);
```
Element Access:
``` cpp
ui_renderer.get_element<Type>(identifier); // Gets a pointer to the element
```

---
##### **Element-Element interaction**
When rendering the UI, children will always render on top of its parents.

*Element Creation:*
``` cpp
element_ptr->create_child<Type>(Arguments);
```
or
``` cpp
element_ptr->add_child(/*Unique Pointer to a element*/);
```

*Element Removal:*
``` cpp
element_ptr->remove_element(identifier);
```

*Element Access:*
``` cpp
element_ptr->get_element<Type>(identifier); // Gets a pointer to the element
```

---
##### **Element Types**
- **Containers:** [Canvas](docs/Canvas.md), [Bordered Box](docs/BorderedBox.md), [Horizontal Box](docs/HorizontalBox.md), [Vertical Box](docs/VerticalBox.md), [Scroll Box](docs/ScrollBox.md), [Wrap box](docs/Wrapbox.md), 
[Combo box](docs/Combobox.md).
- **Interactive:** [Button](docs/Button.md), [Checkbox](docs/Checkbox.md), [Slider](docs/Slider.md), [Toggle Slider](docs/ToggleSlider.md), [Text Area](docs/TextArea.md), [Input Box](docs/InputBox.md), [Color Picker](docs/ColorPicker.md). 
- **Monitoring:** [Progress Bar](docs/ProgressBar.md), [Graph View](docs/GraphView.md).
- **Other:** [Image](docs/Image.md), [Text](docs/Text.md).

---
#### **JSON**
##### *Layout*
The main way of handling UI within TempoUI is by utilizing its JSON layout functionality.

``` cpp
// This will load the layout if it exists or create it
ui_renderer.load_layout(/*json file name*/);
```
---
*Hot-reloading Layout:*
If you want to hot-reload the layout of your UI, this is the way that it is intended.

``` cpp
// Bind a callback to layout change
ui_renderer.bind_layout_callback([](){
	/* 
		Elements with special properties
		Always use this for getting and binding properties if hot-reloading!
	*/
});
```

``` cpp
// Bind a key press to a function
ui_renderer.remove_element_from_canvas("Layout_Canvas");
ui_renderer.load_layout(/*json file name*/);
```
[Layout Documentation](docs/Layout.md)
##### *Theme*

It is highly recommended to utilize a theme for your UI. Without a theme, every color needs to be coded in manually which is a hassle.

*Loading theme initially*
``` cpp
// During initialization
TempoUI::UIRenderer ui_renderer(/*JSON theme data, if wanted (raw data)*/);
```

*Hot-reloading theme*
``` cpp
// Before element creation
TempoUI::ThemeManager::get().load_theme(/*JSON theme data*/);
ui_renderer.get_element<TempoUI::Canvas>("Screen")->theme_updated();
```
[Theme Documentation](docs/Theme.md)