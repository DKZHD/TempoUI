# Wrapbox
#### Type: Container

The wrap box is a container type element which enables for elements to, as the name implies, wrap to a lower section. In essence it will always work as a horizontal box until it reaches the end where it will add later elements lower and lower.
### **Creation**
There are two ways to create this element with TempoUI.

**First, using the default constructor directly**
``` cpp
Wrapbox(const std::string& id, glm::vec2 pos, glm::vec2 size)
```

**The second and recommended way**, using the WrapboxConfig which follows the Curiously recurring template pattern (CRTP) principle

Can be constructed before then utilized inside the constructor of the wrap box:
``` cpp
WrapboxConfig wrapbox_config;

Wrapbox wrapbox1(wrapbox_config);
Wrapbox wrapbox2(wrapbox_config);
```
This is better if the goal is to utilize the same config multiple times.

Another way is constructing the config inside the wrap box constructor:
``` cpp
Wrapbox wrapbox1(WrapboxConfig()
.id(/*identifier*/));
```
Generally recommended for creating unique elements.

As the wrap box is a container first and foremost, it simply cares about the elements inside of it. It will wrap only based on the combined width of the children inside of it. Because of this the config doesn't really add any special properties upon creation.

>[!Info]
>Remember that when utilizing the configs, not all configs have to be set. Most have a default value which will be applied, and when utilizing themes, they will usually also set a lot of them.

>[!Important]
>All setter functions for the config has a getter with the same name, simply remove the arguments.
>
>The WrapboxConfig inherits values from a base ElementConfig class.
>`id(string), pos(vec2), size(vec2), radius(vec4), flex(float)`
### Layout JSON Form

#### **Required:**

| Key  | Value              |
| ---- | ------------------ |
| Id   | custom string name |
| Type | "Wrapbox"          |
> [!CAUTION]
> Failing to set either of these will cause the application to crash!

#### *Other*

| **Key**  |      **Value Type**      |
| :------: | :----------------------: |
|   Pos    |           vec2           |
|   Size   |           vec2           |
| Children | array of Element objects |
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