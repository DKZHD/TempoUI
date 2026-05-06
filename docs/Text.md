# Text
#### Type: Other

The text element is the base of a lot of other elements as well as the main way of displaying custom messages inside the application. 
>[!Caution]
>A font has to be loaded and bound to the name "default_font". Not doing this will make the text non-functional. This also mean that additional fonts can not be used at the same time currently.

### **Creation**
There are two ways to create this element with TempoUI.

**First, using the default constructor directly**
``` cpp
Text(const std::string& id, const std::string& text, glm::vec2 pos, glm::vec3 color = glm::vec3(0.f))
```

**The second and recommended way**, using the TextConfig which follows the Curiously recurring template pattern (CRTP) principle

Can be constructed before then utilized inside the constructor of the text:
``` cpp
TextConfig text_config;
text_config
.id(/*identifier*/)
.text(/*text*/)
.color(/*vec3*/)
.align_vert(/*Custom alignment enum (Top / Center / Bottom)*/)
.align_hori(/*Custom alignment enum (Left / Center / Right)*/);

Text text1(text_config);
Text text2(text_config);
```
This is better if the goal is to utilize the same config multiple times.

Another way is constructing the config inside the text constructor:
``` cpp
Text text1(TextConfig()
.id(/*identifier*/)
.text(/*text*/)
.color(/*vec3*/)
.align_vert(/*Custom alignment enum (Top / Center / Bottom)*/)
.align_hori(/*Custom alignment enum (Left / Center / Right)*/));
```
Generally recommended for creating unique elements.

>[!Info]
>Remember that when utilizing the configs, not all configs have to be set. Most have a default value which will be applied, and when utilizing themes, they will usually also set a lot of them.

>[!Important]
>All setter functions for the config has a getter with the same name, simply remove the arguments.
>
>The TextConfig inherits values from a base ElementConfig class.
>`id(string), pos(vec2), size(vec2), radius(vec4), flex(float)`

### Adding font to TextHandler
There are two ways of adding fonts to the TextHandler.
``` cpp
// UIRenderer has to be initialized first
// 1. add_font
ui_renderer.add_font("default_font", /*Path*/, /*font size*/);
// 2. add_font_from_memory
ui_renderer.add_font_from_memory("default_font", /* char* data */, /* Data Length */, /*font size*/);
```
>[!Tip]
>To load a font into memory, simply read the binary data of a true type font file
>``` cpp
>// Example
>std::ifstream font_file(/*font file*/, std::ios::binary | std::ios::ate);
>std::streamsize size = font_file.tellg();
>font_file.seekg(0, std::ios::beg);
>std::vector<unsigned char> font_buffer(size);
>font_file.read(reinterpret_cast<char*>(font_buffer.data()), size);
>/* 
>	font_buffer now contains the font data
> */
>```
>
>Do note that the font_buffer in this instance needs to exist during the adding of the font, but can be deleted after. 
>
>The add font from memory function will not keep the vector inside persistent memory by itself!

### **Current limitations:**
Font is hard set to one size, it only scales depending on the window size if the correct callbacks are set up correctly.

Only one font at a time is supported.

### Layout JSON Form

#### **Required:**

| Key  | Value              |
| ---- | ------------------ |
| Id   | custom string name |
| Type | "Text"             |
> [!CAUTION]
> Failing to set either of these will cause the application to crash!

#### *Other*

| **Key** |     **Value Type**      |
| :-----: | :---------------------: |
|   Pos   |          vec2           |
|  Size   |          vec2           |
|  Text   |         string          |
|  Color  |          vec3           |
| VAlign  | "Top"/"Center"/"Bottom" |
| HAlign  | "Left"/"Center"/"Right" |
*None of these needs to be called for the application to work*
### Theme
Key = "Text"

| Key   | Type |
| :---- | ---- |
| Color | vec3 |

### Inherited Functionality 
[Check Element Base](ElementBase.md)

