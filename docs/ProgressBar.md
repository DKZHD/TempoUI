# Progress Bar
#### *Type: Monitoring*

The progress bar is a monitoring element which displays what percentage a value is in the scope of zero to a set max value.  
### **Creation**
There are two ways to create this element with TempoUI.

**First, using the default constructor directly**
``` cpp
ProgressBar(const std::string& id, glm::vec2 pos, glm::vec2 size, float max, float inset = 0.f, glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4 borderColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4 radius = glm::vec4(0.f))
```

**The second and recommended way**, using the ProgressBarConfig which follows the Curiously recurring template pattern (CRTP) principle

Can be constructed before then utilized inside the constructor of the progress bar:
``` cpp
ProgressBarConfig progress_bar_config;
progress_bar_config
.id(/*identifier*/)
.percent(/*float 0-1*/)
.max(/*float*/)
.inset(/*float*/)
.color(/*vec4*/)
.border_color(/*vec4*/);

ProgressBar progress_bar1(progress_bar_config);
ProgressBar progress_bar2(progress_bar_config);
```
This is better if the goal is to utilize the same config multiple times.

Another way is constructing the config inside the progress bar constructor:
``` cpp
ProgressBar progress_bar1(ProgressBarConfig()
.id(/*identifier*/)
.percent(/*float 0-1*/)
.max(/*float*/)
.inset(/*float*/)
.color(/*vec4*/)
.border_color(/*vec4*/));
```
Generally recommended for creating unique elements.
>[!Info]
>Remember that when utilizing the configs, not all configs have to be set. Most have a default value which will be applied, and when utilizing themes, they will usually also set a lot of them.

>[!Important]
>All setter functions for the config has a getter with the same name, simply remove the arguments.
>
>The ProgressBarConfig inherits values from a base ElementConfig class.
>`id(string), pos(vec2), size(vec2), radius(vec4), flex(float)`

### Layout JSON Form

#### **Required:**

| Key  | Value              |
| ---- | ------------------ |
| Id   | custom string name |
| Type | "ProgressBar"      |
> [!CAUTION]
> Failing to set either of these will cause the application to crash!

#### *Other*

|   **Key**    |      **Value Type**      |
| :----------: | :----------------------: |
|     Pos      |           vec2           |
|     Size     |           vec2           |
|    Radius    |           vec4           |
|     Flex     |          float           |
|    Inset     |          float           |
|     Max      |          float           |
|   Percent    |       float (0-1)        |
|    Color     |           vec4           |
| Border Color |           vec4           |
*None of these needs to be called for the application to work*
### Theme
Key = "ProgressBar"

| Key          | Type  |
| :----------- | ----- |
| Border Color | vec4  |
| Color        | vec4  |
| Inset        | float |
| Radius       | vec4  |

### Usage
The only way to track a value is by utilizing the `bind_variable` function.
This function will bind a float value to the progress bar and update accordingly. 

``` cpp
progress_bar_ptr->bind_variable(float_ref, max_value);
```

This does not keep the variable in persistent memory, this is your responsibility! It will crash if the float variable gets deleted at some point.
### Inherited Functionality 
[Check Element Base](ElementBase.md)
