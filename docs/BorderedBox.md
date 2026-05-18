# Bordered Box
#### *Type: Container*

The bordered box class contains a two mesh element which in turn allows you to set a inset.
This also means if the inset is set to 0 for some reason, the box will only be one color and the outline will not be visible.
### **Creation**
There are two ways to create this element with TempoUI.

**First, using the default constructor directly**
``` cpp
BorderedBox(const std::string& id, glm::vec2 pos, glm::vec2 size, float inset = 0.f, glm::vec4 radius = glm::vec4(0.f))
```
>[!NOTE]
> This will set the border to black and main section to white. Call setter functions afterwards to set them individually

**The second and recommended way**, using the BorderedBoxConfig which follows the Curiously recurring template pattern (CRTP) principle

Can be constructed before then utilized inside the constructor of the bordered box:
``` cpp
TempoUI::BorderedBoxConfig bordered_box_config;
bordered_box_config
.id(/*identifier*/)
.inset(/*inset*/)
.inner_color(/*vec4 inner color*/)
.outer_color(/*vec4 outer color*/);

TempoUI::BorderedBox box1(bordered_box_config);
TempoUI::BorderedBox box2(bordered_box_config);
```
This is better if the goal is to utilize the same config multiple times.

Another way is constructing the config inside the bordered box constructor:
``` cpp
TempoUI::BorderedBox box1(TempoUI::BorderedBoxConfig()
.id(/*identifier*/)
.inset(/*inset*/)
.inner_color(/*vec4 inner color*/)
.outer_color(/*vec4 outer_color*/));
```
Generally recommended for creating unique elements.
>[!Info]
>Remember that when utilizing the configs, not all configs have to be set. Most have a default value which will be applied, and when utilizing themes, they will usually also set a lot of them.

>[!Important]
>All setter functions for the config has a getter with the same name, simply remove the arguments.
>
>The BorderedBoxConfig inherits values from a base ElementConfig class.
>`id(string), pos(vec2), size(vec2), radius(vec4), flex(float)`

### Layout JSON Form

#### **Required:**

| Key  | Value              |
| ---- | ------------------ |
| Id   | custom string name |
| Type | "BorderedBox"      |
> [!CAUTION]
> Failing to set either of these will cause the application to crash!

#### *Other*

|   **Key**   |      **Value Type**      |
| :---------: | :----------------------: |
|     Pos     |           vec2           |
|    Size     |           vec2           |
|   Radius    |           vec4           |
|    Flex     |          float           |
| Inner Color |           vec4           |
| Outer Color |           vec4           |
|    Inset    |          float           |
|  Children   | array of element objects |
Example for adding children to element.
```json
"Children":[
	{
		"Id": "identifier",
		"Type": "Some Type",
		// Other Values
	},
	// ...
]
```

*None of these needs to be called for the application to work*

### Theme
Key = "BorderedBox"

| Key         | Type  |
| :---------- | ----- |
| Outer Color | vec4  |
| Inner Color | vec4  |
| Inset       | float |
| Radius      | vec4  |
| Size        | vec2  |
### Inherited Functionality 
[Check Element Base](ElementBase.md)



