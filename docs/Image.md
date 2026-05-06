# Image
#### Type: Other

An image is exactly what it sounds like, a picture. To utilize this one properly the `ImageHandler` has to have the image in question in its internal memory. Without this the application most likely will simply not showcase the image.
### **Creation**
There are two ways to create this element with TempoUI.

**First, using the default constructor directly**
``` cpp
Image(const std::string& id, std::string tex_name, glm::vec2 pos, glm::vec2 size)
```

**The second and recommended way**, using the ImageConfig which follows the Curiously recurring template pattern (CRTP) principle

Can be constructed before then utilized inside the constructor of the image:
``` cpp
ImageConfig image_config;
image_config
.id(/*identifier*/)
.texture_name(/*Texture Name*/)

Image image1(image_config);
Image image2(image_config);
```
This is better if the goal is to utilize the same config multiple times.

Another way is constructing the config inside the image constructor:
``` cpp
Image image1(ImageConfig()
.id(/*identifier*/)
.texture_name(/*Texture Name*/));
```
Generally recommended for creating unique elements.
>[!Info]
>Remember that when utilizing the configs, not all configs have to be set. Most have a default value which will be applied, and when utilizing themes, they will usually also set a lot of them.

>[!Important]
>All setter functions for the config has a getter with the same name, simply remove the arguments.
>
>The ImageConfig inherits values from a base ElementConfig class.
>`id(string), pos(vec2), size(vec2), radius(vec4), flex(float)`

### Adding texture to ImageHandler
There are two ways of adding images to the image handler.
``` cpp
// 1. add_texture
ImageHandler::get().add_texture(/*texture name*/, /*path of image*/);

// 2. add_texture_from_data
ImageHandler::get().add_texture_from_data(/*texture name*/, /* char* data */, /* Data Length */);
```
>[!Tip]
>To load image from data, stb_image is highly recommended!
### Layout JSON Form

#### **Required:**

| Key  | Value              |
| ---- | ------------------ |
| Id   | custom string name |
| Type | "Image"            |
> [!CAUTION]
> Failing to set either of these will cause the application to crash!

#### *Other*

| **Key** |      **Value Type**      |
| :-----: | :----------------------: |
|   Pos   |           vec2           |
|  Size   |           vec2           |
| Radius  |           vec4           |
|  Flex   |          float           |
| Texture | string (name of texture) |
*None of these needs to be called for the application to work*
### Inherited Functionality 
[Check Element Base](ElementBase.md)

