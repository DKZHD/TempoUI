# Button
#### *Type: Interactive*

The button class holds a mesh for visualization and interactivity with hovering and clicking
### **Creation**
There are two ways to create this element with TempoUI.

**First, using the default constructor directly**
``` cpp
Button(const std::string& id, glm::vec2 pos, glm::vec2 size, glm::vec4 color = glm::vec4(1.f), glm::vec4 radius = glm::vec4(0.f))
```
>[!NOTE]
> When using this method, the hover color and clicked color can not be changed individually. It will only change based on the main color, even with the setter function.

**The second and recommended way**, using the ButtonConfig which follows the Curiously recurring template pattern (CRTP) principle

Can be constructed before then utilized inside the constructor of the button:
``` cpp
TempoUI::ButtonConfig button_config;
button_config
.id(/*identifier*/)
.color(/*vec4 main color*/)
.hover_color(/*vec4 hover color*/)
.clicked_color(/*vec4 clicked color*/);

TempoUI::Button button1(button_config);
TempoUI::Button button2(button_config);
```
This is better if the goal is to utilize the same config multiple times.

Another way is constructing the config inside the button constructor:
``` cpp
TempoUI::Button button1(TempoUI::ButtonConfig()
.id(/*identifier*/)
.color(/*vec4 main color*/)
.hover_color(/*vec4 hover color*/)
.clicked_color(/*vec4 clicked color*/));
```
Generally recommended for creating unique elements.
>[!Info]
>Remember that when utilizing the configs, not all configs have to be set. Most have a default value which will be applied, and when utilizing themes, they will usually also set a lot of them.

>[!Important]
>All setter functions for the config has a getter with the same name, simply remove the arguments.
>
>The ButtonConfig inherits values from a base ElementConfig class.
>`id(string), pos(vec2), size(vec2), radius(vec4), flex(float)`
### Layout JSON Form

#### **Required:**

| Key  | Value              |
| ---- | ------------------ |
| Id   | custom string name |
| Type | "Button"           |
> [!CAUTION]
> Failing to set either of these will cause the application to crash!

#### *Other*

|    **Key**    |      **Value Type**      |
| :-----------: | :----------------------: |
|      Pos      |           vec2           |
|     Size      |           vec2           |
|    Radius     |           vec4           |
|     Flex      |          float           |
| Default Color |           vec4           |
| Clicked Color |           vec4           |
|  Hover Color  |           vec4           |
|   Children    | array of element objects |
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
Key = "Button"

| Key           | Type |
| :------------ | ---- |
| Default Color | vec4 |
| Hover Color   | vec4 |
| Click Color   | vec4 |
| Radius        | vec4 |

### Usage
The main way of achieving button clicking functionality is by using the `set_on_click` function inside the button class. 
``` cpp
button_ptr->set_on_click([](){
	std::cout << "Hello World!" << std::endl;
});
```
This function can later be force called with the `trigger_on_click` function as well. 

>[!Important]
>If you are loading the button through a layout, you can still get the button with the identifier and bind functionality as usual!
### Inherited Functionality 
[Check Element Base](ElementBase.md)



