# Scroll Box
#### *Type: Container*

The scroll box container is one of the layout container elements within TempoUI. When adding elements to the scroll box they will be organized and aligned with a predefined direction with a set spacing between them. If more spacing is wanted a Canvas can be utilized for this purpose.
This also means the scroll box can be utilized for both horizontal and vertical layouts. 
### **Creation**
There are two ways to create this element with TempoUI.

**First, using the default constructor directly**
``` cpp
ScrollBox(const std::string& id, glm::vec2 pos, glm::vec2 size, float spacing, Direction direction = Direction::Vertical)
```

**The second and recommended way**, using the ScrollBoxConfig which follows the Curiously recurring template pattern (CRTP) principle

Can be constructed before then utilized inside the constructor of the scroll box:
``` cpp
TempoUI::ScrollBoxConfig scroll_box_config;
scroll_box_config.spacing(/*float*/)
.direction(/*Direction::Vertical or Direction::Horizontal*/);

TempoUI::ScrollBox scroll_box1(scroll_box_config);
TempoUI::ScrollBox scroll_box2(scroll_box_config);
```
This is better if the goal is to utilize the same config multiple times.

Another way is constructing the config inside the scroll box constructor:
``` cpp
TempoUI::ScrollBox scroll_box1(TempoUI::ScrollBoxConfig()
.id(/*identifier*/)
.spacing(/*float*/)
.direction(/*Direction::Vertical or Direction::Horizontal*/));
```
Generally recommended for creating unique elements.
>[!Info]
>Remember that when utilizing the configs, not all configs have to be set. Most have a default value which will be applied, and when utilizing themes, they will usually also set a lot of them.

>[!Important]
>All setter functions for the config has a getter with the same name, simply remove the arguments.
>
>The ScrollBoxConfig inherits values from a base ElementConfig class.
>`id(string), pos(vec2), size(vec2), radius(vec4), flex(float)`

### Layout JSON Form

#### **Required:**

| Key  | Value              |
| ---- | ------------------ |
| Id   | custom string name |
| Type | "ScrollBox"    |
> [!CAUTION]
> Failing to set either of these will cause the application to crash!

#### *Other*

| **Key**  |      **Value Type**      |
| :------: | :----------------------: |
|   Pos    |           vec2           |
|   Size   |           vec2           |
| Children | array of Element objects |
|   Flex   |          float           |
| Spacing  |          float           |
| Vertical |           bool           |
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
No particular settings.
### Inherited Functionality 
[Check Element Base](ElementBase.md)




