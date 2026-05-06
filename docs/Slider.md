# Slider
#### *Type: Interactive*

The slider can be dragged between a minimum and maximum value. A variable can also be bound so that it gets updated together with the slider in real-time.
### **Creation**
There are two ways to create this element with TempoUI.

**First, using the default constructor directly**
``` cpp
Slider(const std::string& id, glm::vec2 pos, glm::vec2 size, glm::vec4 color = glm::vec4(1.f), glm::vec4 radius = glm::vec4(0.f), float min = 0.f, float max = 1.f)
```

**The second and recommended way**, using the SliderConfig which follows the Curiously recurring template pattern (CRTP) principle

Can be constructed before then utilized inside the constructor of the slider:
``` cpp
SliderConfig slider_config;
slider_config
.id(/*identifier*/)
.min(/*float*/)
.max(/*float*/)
.slider_size(/*float*/)
.color(/*vec4*/);

Slider slider1(slider_config);
Slider slider2(slider_config);
```
This is better if the goal is to utilize the same config multiple times.

Another way is constructing the config inside the slider constructor:
``` cpp
Slider slider1(SliderConfig()
.id(/*identifier*/)
.min(/*float*/)
.max(/*float*/)
.slider_size(/*float*/)
.color(/*vec4*/));
```
Generally recommended for creating unique elements.

>[!Info]
>Remember that when utilizing the configs, not all configs have to be set. Most have a default value which will be applied, and when utilizing themes, they will usually also set a lot of them.

>[!Important]
>All setter functions for the config has a getter with the same name, simply remove the arguments.
>
>The SliderConfig inherits values from a base ElementConfig class.
>`id(string), pos(vec2), size(vec2), radius(vec4), flex(float)`
### Layout JSON Form

#### **Required:**

| Key  | Value              |
| ---- | ------------------ |
| Id   | custom string name |
| Type | "Slider"           |
> [!CAUTION]
> Failing to set either of these will cause the application to crash!

#### *Other*

|   **Key**   | **Value Type** |
| :---------: | :------------: |
|     Pos     |      vec2      |
|    Size     |      vec2      |
|   Radius    |      vec4      |
|    Flex     |     float      |
|     Min     |     float      |
|     Max     |     float      |
| Slider Size |     float      |
|    Color    |      vec4      |
*None of these needs to be called for the application to work*
### Theme
Key = "Slider"

| Key    | Type  |
| :----- | ----- |
| Min    | vec4  |
| Max    | float |
| Color  | vec4  |
| Radius | vec4  |

### Usage
The main use of a slider is binding it using the `bind_variable` function.
``` cpp
slider_ptr->bind_variable(float_ref);
```
If you require the value itself for multiple interactions, a `get_value` function is also provided.

If for any reason you need to set the value at any time use the `set_value` function for this purpose.
### Inherited Functionality 
[Check Element Base](ElementBase.md)



