# Combo Box
#### *Type: Container*

The combo box is a container type element which displays a button showcasing a default option. When pressed it opens a dropdown overlay that displays all added options.
### **Creation**
There are two ways to create this element with TempoUI.

**First, using the default constructor directly**
``` cpp
Combobox(const std::string& id, glm::vec2 pos, glm::vec2 size, float spacing = 0.f)
```
>[!Note]
> With this approach there is no way of setting the colors related to the combo box. They will be set to a standard

**The second and recommended way**, using the ComboboxConfig which follows the Curiously recurring template pattern (CRTP) principle

Can be constructed before then utilized inside the constructor of the combo box:
``` cpp
TempoUI::ComboboxConfig combobox_config;
combobox_config
.id(/*identifier*/)
.spacing(/*float*/)
.main_color(/*vec4*/)
.sub_color(/*vec4*/);

TempoUI::Combobox combobox1(combobox_config);
TempoUI::Combobox combobox2(combobox_config);
```
This is better if the goal is to utilize the same config multiple times.

Another way is constructing the config inside the combo box constructor:
``` cpp
TempoUI::Combobox combobox1(TempoUI::ComboboxConfig()
.id(/*identifier*/)
.spacing(/*float*/)
.main_color(/*vec4*/)
.sub_color(/*vec4*/));
```
Generally recommended for creating unique elements.
>[!Info]
>Remember that when utilizing the configs, not all configs have to be set. Most have a default value which will be applied, and when utilizing themes, they will usually also set a lot of them.

>[!Important]
>All setter functions for the config has a getter with the same name, simply remove the arguments.
>
>The ComboboxConfig inherits values from a base ElementConfig class.
>`id(string), pos(vec2), size(vec2), radius(vec4), flex(float)`

### Layout JSON Form

#### **Required:**

| Key  | Value              |
| ---- | ------------------ |
| Id   | custom string name |
| Type | "Combobox"         |
> [!CAUTION]
> Failing to set either of these will cause the application to crash!

#### *Other*

|  **Key**   |      **Value Type**      |
| :--------: | :----------------------: |
|    Pos     |           vec2           |
|    Size    |           vec2           |
|   Radius   |           vec4           |
|  Children  | array of element objects |
|    Flex    |          float           |
| Main Color |           vec4           |
| Sub Color  |           vec4           |
|  Spacing   |          float           |
|  Options   |     array of strings     |
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
Key = "Combobox"

| Key        | Type |
| :--------- | ---- |
| Main Color | vec4 |
| Sub Color  | vec4 |
| Radius     | vec4 |
### Usage
In C++, options can be added utilizing the `add_option` or `add_multiple_options` functions.
Both functions take in strings, with the singular taking in one as an argument, and the multiple one taking in an initializer list of strings.
``` cpp
combobox_ptr->add_multiple_options(/*option1*/, /*option2*/, /*option3...*/);
```

Accessing the current values is easily done through the `get_value` or `get_value_as_int` functions, with the normal version returning the string of the option, and the as int function returning the string as a int if that is applicable.

### Inherited Functionality 
[Check Element Base](ElementBase.md)




