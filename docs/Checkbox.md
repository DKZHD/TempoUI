# Checkbox
#### *Type: Interactive*

The Checkbox element contains two meshes. One is for the background, and the other one turns visible if the checkbox is checked off.
### **Creation**
There are two ways to create this element with TempoUI.

**First, using the default constructor directly**
``` cpp
Checkbox(const std::string& id, glm::vec2 pos, glm::vec2 size, glm::vec4 radius = glm::vec4(5.f), bool default_state = false)
```
>[!Note]
>When doing it this way, the background will be white, the checkmark will be black and the hover color will be gray. There is no way to change it using this approach

**The second and recommended way**, using the CheckboxConfig which follows the Curiously recurring template pattern (CRTP) principle

Can be constructed before then utilized inside the constructor of the checkbox:
``` cpp
TempoUI::CheckboxConfig checkbox_config;
checkbox_config.id(/*identifier*/)
.check_color(/*vec4*/)
.background_color(/*vec4*/)
.hover_color(/*vec4*/);

TempoUI::Checkbox checkbox1(checkbox_config);
TempoUI::Checkbox checkbox2(checkbox_config);
```
This is better if the goal is to utilize the same config multiple times.

Another way is constructing the config inside the checkbox constructor:
``` cpp
TempoUI::Checkbox checkbox(TempoUI::CheckboxConfig()
.id(/*identifier*/)
.check_color(/*vec4*/)
.background_color(/*vec4*/)
.hover_color(/*vec4*/));
```
Generally recommended for creating unique elements.
>[!Info]
>Remember that when utilizing the configs, not all configs have to be set. Most have a default value which will be applied, and when utilizing themes, they will usually also set a lot of them.

>[!Important]
>All setter functions for the config has a getter with the same name, simply remove the arguments.
>
>The CheckboxConfig inherits values from a base ElementConfig class.
>`id(string), pos(vec2), size(vec2), radius(vec4), flex(float)`
### Layout JSON Form

#### **Required:**

| Key  | Value              |
| ---- | ------------------ |
| Id   | custom string name |
| Type | "Checkbox"           |
> [!CAUTION]
> Failing to set either of these will cause the application to crash!

#### *Other*

|     **Key**      |      **Value Type**      |
| :--------------: | :----------------------: |
|       Pos        |           vec2           |
|       Size       |           vec2           |
|      Radius      |           vec4           |
|       Flex       |          float           |
|   Check Color    |           vec4           |
|   Hover Color    |           vec4           |
| Background Color |           vec4           |
*None of these needs to be called for the application to work*

### Theme
Key = "Checkbox"

| Key              | Type |
| :--------------- | ---- |
| Check Color      | vec4 |
| Hover Color      | vec4 |
| Background Color | vec4 |
| Radius           | vec4 |
### Usage
The main intended behavior of the checkbox is to bind a variable to it, like this:
``` cpp
checkbox_ptr->bind_variable(bool_ref);
```
If this is done, the variable will be automatically changed according to the state of the checkbox. This does not keep the variable in persistent memory, this is your responsibility! It will crash if the bool variable gets deleted at some point.

If multiple values rely on this checkbox, it is also possible to get the value from the checkbox:
``` cpp
// Note, if this is in a run loop, make sure it doesnt run all the time of course
if(checkbox_ptr->get_value()){
	// Code to execute
}
```

The value can also be manually set like this:
``` cpp
checkbox_ptr->set_value(bool_val);
```
### Inherited Functionality 
[Check Element Base](ElementBase.md)