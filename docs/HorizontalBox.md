# Horizontal Box
#### *Type: Container*

The horizontal box container is one of the layout container elements within TempoUI. When adding elements to the horizontal box they will be organized and stacked horizontally with a set spacing between them. If more spacing is wanted a Canvas can be utilized for this purpose.
### **Creation**
There are two ways to create this element with TempoUI.

**First, using the default constructor directly**
``` cpp
HorizontalBox(const std::string& id, glm::vec2 pos, glm::vec2 size, float spacing)
```

**The second and recommended way**, using the HorizontalBoxConfig which follows the Curiously recurring template pattern (CRTP) principle

Can be constructed before then utilized inside the constructor of the horizontal box:
``` cpp
TempoUI::HorizontalBoxConfig horizontal_box_config;
horizontal_box_config.spacing(/*float*/);

TempoUI::HorizontalBox horizontal_box1(horizontal_box_config);
TempoUI::HorizontalBox horizontal_box2(horizontal_box_config);
```
This is better if the goal is to utilize the same config multiple times.

Another way is constructing the config inside the horizontal box constructor:
``` cpp
TempoUI::HorizontalBox horizontal_box1(TempoUI::HorizontalBoxConfig()
.id(/*identifier*/)
.spacing(/*float*/));
```
Generally recommended for creating unique elements.
>[!Info]
>Remember that when utilizing the configs, not all configs have to be set. Most have a default value which will be applied, and when utilizing themes, they will usually also set a lot of them.

>[!Important]
>All setter functions for the config has a getter with the same name, simply remove the arguments.
>
>The HorizontalBoxConfig inherits values from a base ElementConfig class.
>`id(string), pos(vec2), size(vec2), radius(vec4), flex(float)`

### Layout JSON Form

#### **Required:**

| Key  | Value              |
| ---- | ------------------ |
| Id   | custom string name |
| Type | "HorizontalBox"    |
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



