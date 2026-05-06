# Theme
Every default element inside TempoUI will load theme defaults provided by the `ThemeManager` when utilizing `Configs` during creation. This is also the method used when loading elements from layout.

The main way of loading a theme is when creating the UIRenderer.
``` cpp
UIRenderer ui_renderer(/*JSON data*/);
```
When this is done the theme will be loaded to the ThemeManager automatically.

If you still want to load the theme manually, for instance if hot-reloading, this can be achieved like this.
``` cpp
ThemeManager::get().load_theme(/*JSON data (best read from a file)*/);
```
### Format
#### Getting Started with JSON

| Complex Type | Format                               |
| :----------- | ------------------------------------ |
| Object       | "Name" : {<br>"key": value<br>}      |
| Array        | "Name": `[property1, property2 ...]` |
In JSON every value is a key-value pair, meaning it needs a name (string only) and a value of some kind. Comments are not allowed in standard JSON as well as `key` only values. 

Start by wrapping the entire file in curly brackets.
``` json
{
	"key": value,
	...
}
```
Every aspect of this file needs to be within that scope.

Another important thing to keep in mind is that every value needs to be separated with `,` except the last value of a scope. 
#### TempoUI Specific
Remember that every key is case-sensitive.
Check out individual elements to see specific JSON Features to each element.

### Custom JSON functionality
This JSON system is not completely standard. A quality of life feature is added to give access to global variables. To utilize this simply create a `"Globals"` object in JSON. 
``` json
"Globals":{
	"key": value
}
```
>[!Remember]
>When using this system remember to use the appropriate value type!

To use the global variables at a later stage simply input the key name as well as a `$` sign in front.
``` json
"Globals":{
	"example vec3": [0,0,0]
}
"ExampleElement":{
	"Color": "$example vec3"
}
```
>[!Warning]
>`Globals` need to be defined in the outer most scope and does not support being used except for one scope in, like in the example above! 

### Hot-reloading
Hot-reloading the theme is very simple.
``` cpp
// Simply load the theme again
ThemeManager::get().load_theme(/*JSON data (best read from a file)*/);
ui_renderer.get_element<Canvas>("Screen")->theme_updated();
```
`theme_updated` will recursively ensure each element in the hierarchy gets updated. 
### Elements
To see the individual theme applicable aspects of the different elements, check out their individual files.