# Color Picker
#### *Type: Interactive*

The color picker is an advanced interactive element, which displays a gradient mapping over a square and includes the ability to move a cursor to select a color on the gradient. If you want to use this as a child of a smaller element in any way, use a canvas to ensure the color picker circle gets displayed fully. 
### **Creation**
There are two ways to create this element with TempoUI.

**First, using the default constructor directly**
``` cpp
ColorPicker(const std::string& id, glm::vec2 pos, glm::vec2 size)
```
>[!Caution]
> The diameter of the color picker circle will not be changeable with this approach.

**The second and recommended way**, using the ColorPickerConfig which follows the Curiously recurring template pattern (CRTP) principle

Can be constructed before then utilized inside the constructor of the color picker:
``` cpp
TempoUI::ColorPickerConfig color_picker_config;
color_picker_config
.id(/*identifier*/)
.selector_inner_diameter(/*float*/)
.selector_outer_diameter(/*float*/)
.selector_background_color(/*vec4*/);
/* 
	Note that the background color is simply the border. The middle of the
	selector will get updated with the color that is selected 
*/
TempoUI::ColorPicker color_picker1(color_picker_config);
TempoUI::ColorPicker color_picker2(color_picker_config);
```
This is better if the goal is to utilize the same config multiple times.

Another way is constructing the config inside the color picker constructor:
``` cpp
TempoUI::ColorPicker color_picker1(TempoUI::ColorPickerConfig()
.id(/*identifier*/)
.selector_inner_diameter(/*float*/)
.selector_outer_diameter(/*float*/)
.selector_background_color(/*vec4*/));
```
Generally recommended for creating unique elements.
>[!Info]
>Remember that when utilizing the configs, not all configs have to be set. Most have a default value which will be applied, and when utilizing themes, they will usually also set a lot of them.

>[!Important]
>All setter functions for the config has a getter with the same name, simply remove the arguments.
>
>The ColorPickerConfig inherits values from a base ElementConfig class.
>`id(string), pos(vec2), size(vec2), radius(vec4), flex(float)`

### Layout JSON Form

#### **Required:**

| Key  | Value              |
| ---- | ------------------ |
| Id   | custom string name |
| Type | "ColorPicker"      |
> [!CAUTION]
> Failing to set either of these will cause the application to crash!

#### *Other*

|          **Key**          | **Value Type** |
| :-----------------------: | :------------: |
|            Pos            |      vec2      |
|           Size            |      vec2      |
|           Flex            |     float      |
|  Selector Inner Diameter  |     float      |
|  Selector Outer Diameter  |     float      |
| Selector Background Color |      vec4      |
*None of these needs to be called for the application to work*
### Theme
Key = "ColorPicker"

| Key                       | Type  |
| :------------------------ | ----- |
| Selector Background Color | vec4  |
| Selector Inner Diameter   | float |
| Selector Outer Diameter   | float |
### Usage
To retrieve the currently selected color, use the `get_color` function:
``` cpp
color = color_picker_ptr->get_color();
```
Color picker does not have a binding function at this point in time.
### Inherited Functionality 
[Check Element Base](ElementBase.md)



