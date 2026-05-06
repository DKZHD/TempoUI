# Toggle Slider
#### *Type: Interactive*

Similar to a check box in a way, when pressed it toggles as a bool. The reason it is called a slider in this instance is the sliding animation of the toggle.
### **Creation**
There are two ways to create this element with TempoUI.

**First, using the default constructor directly**
``` cpp
ToggleSlider(const std::string& id, glm::vec2 pos, glm::vec2 size, glm::vec4 color = glm::vec4(0.f, 0.f, 0.f, 1.f))
```
>[!NOTE]
> When using this method the background color and the hover color can not be set individually.

**The second and recommended way**, using the ToggleSliderConfig which follows the Curiously recurring template pattern (CRTP) principle

Can be constructed before then utilized inside the constructor of the toggle slider:
``` cpp
ToggleSliderConfig toggle_slider_config;
toggle_slider_config
.id(/*identifier*/)
.slider_color(/*vec4*/)
.background_color(/*vec4*/)
.hover_color(/*vec4*/);

ToggleSlider toggle_slider1(toggle_slider_config);
ToggleSlider toggle_slider2(toggle_slider_config);
```
This is better if the goal is to utilize the same config multiple times.

Another way is constructing the config inside the toggle slider constructor:
``` cpp
ToggleSlider toggle_slider1(ToggleSliderConfig()
.id(/*identifier*/)
.slider_color(/*vec4*/)
.background_color(/*vec4*/)
.hover_color(/*vec4*/));
```
Generally recommended for creating unique elements.

>[!Info]
>Remember that when utilizing the configs, not all configs have to be set. Most have a default value which will be applied, and when utilizing themes, they will usually also set a lot of them.

>[!Important]
>All setter functions for the config has a getter with the same name, simply remove the arguments.
>
>The ToggleSliderConfig inherits values from a base ElementConfig class.
>`id(string), pos(vec2), size(vec2), radius(vec4), flex(float)`
### Layout JSON Form

#### **Required:**

| Key  | Value              |
| ---- | ------------------ |
| Id   | custom string name |
| Type | "ToggleSlider"     |
> [!CAUTION]
> Failing to set either of these will cause the application to crash!

#### *Other*

|     **Key**      | **Value Type** |
| :--------------: | :------------: |
|       Pos        |      vec2      |
|       Size       |      vec2      |
|      Radius      |      vec4      |
|       Flex       |     float      |
|   Slider Color   |      vec4      |
| Background Color |      vec4      |
|   Hover Color    |      vec4      |
*None of these needs to be called for the application to work*
### Theme
Key = "ToggleSlider"

| Key              | Type |
| :--------------- | ---- |
| Slider Color     | vec4 |
| Hover Color      | vec4 |
| Background Color | vec4 |
| Radius           | vec4 |


### Usage
The main use of a toggle slider is binding it using the `bind_variable` function.
``` cpp
slider_ptr->bind_variable(bool_ref);
```
If you require the value itself for multiple interactions, a `get_value` function is also provided.
### Inherited Functionality 
[Check Element Base](ElementBase.md)