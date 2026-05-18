# Text Area
#### Type: Interactive

The text area is a multi-line interactive element which grabs focus on mouse click and allows the user to type. It only utilizes ASCII at the moment, meaning that special characters will not work properly.
It can also be navigated using arrow keys similar to input box but also supports up and down traversal as well.

>[!Caution]
>The text area will not work if the default font is not set. Check out [Text](Text.md) for more info on how to set it up.

>[!Tip]
>The text area itself is not a text box, meaning there will not natively be a background to it. This can be fixed by making a bordered box its parent and using that as the background.
### **Creation**
There are two ways to create this element with TempoUI.

**First, using the default constructor directly**
``` cpp
TextArea(const std::string& id, glm::vec2 pos, glm::vec2 size)
```
>[!Note]
> When using this method, the text color will be set to a default and can not be changed.

**The second and recommended way**, using the TextAreaConfig which follows the Curiously recurring template pattern (CRTP) principle

Can be constructed before then utilized inside the constructor of the text area:
``` cpp
TempoUI::TextAreaConfig text_area_config;
text_area_config
.id(/*identifier*/)
.text_color(/*vec4*/);

TempoUI::TextArea text_area1(text_area_config);
TempoUI::TextArea text_area2(text_area_config);
```
This is better if the goal is to utilize the same config multiple times.

Another way is constructing the config inside the text area constructor:
``` cpp
TempoUI::TextArea text_area1(TempoUI::TextAreaConfig()
.id(/*identifier*/)
.text_color(/*vec4*/));
```
Generally recommended for creating unique elements.
>[!Info]
>Remember that when utilizing the configs, not all configs have to be set. Most have a default value which will be applied, and when utilizing themes, they will usually also set a lot of them.

>[!Important]
>All setter functions for the config has a getter with the same name, simply remove the arguments.
>
>The TextAreaConfig inherits values from a base ElementConfig class.
>`id(string), pos(vec2), size(vec2), radius(vec4), flex(float)`
### Layout JSON Form

#### **Required:**

| Key  | Value              |
| ---- | ------------------ |
| Id   | custom string name |
| Type | "TextArea"         |
> [!CAUTION]
> Failing to set either of these will cause the application to crash!

#### *Other*

|     **Key**      | **Value Type** |
| :--------------: | :------------: |
|       Pos        |      vec2      |
|       Size       |      vec2      |
|      Radius      |      vec4      |
|       Flex       |     float      |
|    Text Color    |      vec4      |
*None of these needs to be called for the application to work*
### Theme
Key = "TextArea"

| Key        | Type |
| :--------- | ---- |
| Text Color | vec4 |
### Usage
The main functionality of the text area works only one way, and the current text will be accessible through the `get_text` function at any time.
### Inherited Functionality 
[Check Element Base](ElementBase.md)
