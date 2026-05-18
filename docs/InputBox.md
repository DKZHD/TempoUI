# Input Box
#### Type: Interactive

The input box is a simple interactive element which grabs focus on mouse click and allows the user to type. It only utilizes ASCII at the moment, meaning that special characters will not work properly.
It can also be navigated using arrow keys.
### **Creation**
There are two ways to create this element with TempoUI.

**First, using the default constructor directly**
``` cpp
InputBox(const std::string& id, glm::vec2 pos, glm::vec2 size)
```
>[!NOTE]
> When using this method, the text color, background color and cursor color will be set to a default

**The second and recommended way**, using the InputBoxConfig which follows the Curiously recurring template pattern (CRTP) principle

Can be constructed before then utilized inside the constructor of the input box:
``` cpp
TempoUI::InputBoxConfig input_box_config;
input_box_config
.id(/*identifier*/)
.background_color(/*vec4*/)
.cursor_color(/*vec4*/)
.text_color(/*vec4*/);

TempoUI::InputBox input_box1(input_box_config);
TempoUI::InputBox input_box2(input_box_config);
```
This is better if the goal is to utilize the same config multiple times.

Another way is constructing the config inside the input box constructor:
``` cpp
TempoUI::InputBox input_box1(TempoUI::InputBoxConfig()
.id(/*identifier*/)
.background_color(/*vec4*/)
.cursor_color(/*vec4*/)
.text_color(/*vec4*/));
```
Generally recommended for creating unique elements.
>[!Info]
>Remember that when utilizing the configs, not all configs have to be set. Most have a default value which will be applied, and when utilizing themes, they will usually also set a lot of them.

>[!Important]
>All setter functions for the config has a getter with the same name, simply remove the arguments.
>
>The InputBoxConfig inherits values from a base ElementConfig class.
>`id(string), pos(vec2), size(vec2), radius(vec4), flex(float)`
### Layout JSON Form

#### **Required:**

| Key  | Value              |
| ---- | ------------------ |
| Id   | custom string name |
| Type | "InputBox"         |
> [!CAUTION]
> Failing to set either of these will cause the application to crash!

#### *Other*

|     **Key**      | **Value Type** |
| :--------------: | :------------: |
|       Pos        |      vec2      |
|       Size       |      vec2      |
|      Radius      |      vec4      |
|       Flex       |     float      |
|   Cursor Color   |      vec4      |
| Background Color |      vec4      |
|    Text Color    |      vec4      |
*None of these needs to be called for the application to work*
### Theme
Key = "InputBox"

| Key              | Type |
| :--------------- | ---- |
| Background Color | vec4 |
| Cursor Color     | vec4 |
| Text Color       | vec4 |
| Radius           | vec4 |

### Usage
The main functionality of the input box works only one way, but there is something that can be done to tweak the behavior. This is by using the `limit_input` function.

`limit input` takes in a variadic pack with chars. Whatever you do not want the user to input can be restricted here. 

``` cpp
// Example
input_box_ptr->limit_input('H', 'E', 'L', 'O');
// Here the user would not be able to type those exact letters afterwards. This is case-sensitive, meaning this would ban only uppercase H, E, L and O.
```
#### Binding callback to enter press 
Functionality can be bound to the enter press when focusing the input box.
``` cpp
input_box_ptr->on_enter([](std::string content){
	// Execute code
	// "content" is the current text within the input box.
});
```

If wanted, the text can also be gotten through the `get_text` function as well.
### Inherited Functionality 
[Check Element Base](ElementBase.md)
