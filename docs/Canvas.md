# Canvas
#### *Type: Container*

The canvas is an invisible container type element. Its only purpose is ensuring layout stability. A good use case (except simply as a background) is wrapping another UI element inside to give more padding.
### **Creation**
There are two ways to create this element with TempoUI.

**First, using the default constructor directly**
``` cpp
Canvas(const std::string& id, glm::vec2 pos, glm::vec2 size)
```

**The second and recommended way**, using the CanvasConfig which follows the Curiously recurring template pattern (CRTP) principle

Can be constructed before then utilized inside the constructor of the canvas:
``` cpp
/* 
	CanvasConfig doesn't add anything itself. It only wraps the element
	config base class. Important to set unique ids!
*/
CanvasConfig canvas_config;

Canvas canvas1(canvas_config);
Canvas canvas2(canvas_config);
```
This is better if the goal is to utilize the same config multiple times.

Another way is constructing the config inside the canvas constructor:
``` cpp
Canvas canvas(CanvasConfig().id(/*identifier*/));
```
Generally recommended for creating unique elements.
>[!Info]
>Remember that when utilizing the configs, not all configs have to be set. Most have a default value which will be applied, and when utilizing themes, they will usually also set a lot of them.

>[!Important]
>All setter functions for the config has a getter with the same name, simply remove the arguments.
>
>The CanvasConfig inherits values from a base ElementConfig class.
>`id(string), pos(vec2), size(vec2), radius(vec4), flex(float)`

### Layout JSON Form

#### **Required:**

| Key  | Value              |
| ---- | ------------------ |
| Id   | custom string name |
| Type | "Canvas"           |
> [!CAUTION]
> Failing to set either of these will cause the application to crash!

#### *Other*

| **Key**  |      **Value Type**      |
| :------: | :----------------------: |
|   Pos    |           vec2           |
|   Size   |           vec2           |
| Children | array of element objects |
|   Flex   |          float           |
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



