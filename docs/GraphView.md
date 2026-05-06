# Graph View
#### *Type: Monitoring*

The graph view is a monitoring element which stores x values with a y interval between them and displays them in an orderly fashion. 
### **Creation**
There are two ways to create this element with TempoUI.

**First, using the default constructor directly**
``` cpp
GraphView(const std::string& id, glm::vec2 pos, glm::vec2 size, int scope_size, float interval, float min, float max)
```
>[!Note]
> With this approach there is no way of setting the colors related to the graph view. They will be set to a standard

**The second and recommended way**, using the GraphViewConfig which follows the Curiously recurring template pattern (CRTP) principle

Can be constructed before then utilized inside the constructor of the graph view:
``` cpp
GraphViewConfig graph_view_config;
graph_view_config
.id(/*identifier*/)
.scope_size(/*int*/)
.interval(/*float*/)
.min(/*float*/)
.max(/*float*/)
.node_color(/*vec4*/)
.inner_color(/*vec4*/)
.background_color(/*vec4*/);

GraphView graph_view1(graph_view_config);
GraphView graph_view2(graph_view_config);
```
This is better if the goal is to utilize the same config multiple times.

Another way is constructing the config inside the graph view constructor:
``` cpp
GraphView graph_view1(GraphViewConfig()
.id(/*identifier*/)
.scope_size(/*int*/)
.interval(/*float*/)
.min(/*float*/)
.max(/*float*/)
.node_color(/*vec4*/)
.inner_color(/*vec4*/)
.background_color(/*vec4*/));
```
Generally recommended for creating unique elements.
>[!Info]
>Remember that when utilizing the configs, not all configs have to be set. Most have a default value which will be applied, and when utilizing themes, they will usually also set a lot of them.

>[!Important]
>All setter functions for the config has a getter with the same name, simply remove the arguments.
>
>The GraphViewConfig inherits values from a base ElementConfig class.
>`id(string), pos(vec2), size(vec2), radius(vec4), flex(float)`

### Layout JSON Form

#### **Required:**

| Key  | Value              |
| ---- | ------------------ |
| Id   | custom string name |
| Type | "GraphView"        |
> [!CAUTION]
> Failing to set either of these will cause the application to crash!

#### *Other*

|     **Key**      |      **Value Type**      |
| :--------------: | :----------------------: |
|       Pos        |           vec2           |
|       Size       |           vec2           |
|      Radius      |           vec4           |
|     Children     | array of Element objects |
|       Flex       |          float           |
|    Node Color    |           vec4           |
| Background Color |           vec4           |
|   Inner Color    |           vec4           |
|    Scope Size    |           int            |
|     Interval     |          float           |
|       Min        |          float           |
|       Max        |          float           |
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
Key = "GraphView"

| Key              | Type |
| :--------------- | ---- |
| Node Color       | vec4 |
| Background Color | vec4 |
| Inner Color      | vec4 |
| Radius           | vec4 |
### Usage
The only way to track a value as of now is by utilizing the `bind_variable` function.
This function will bind a float value to the graph and will check its value at the set interval. 

``` cpp
graph_view_ptr->bind_variable(float_ref);
```

This does not keep the variable in persistent memory, this is your responsibility! It will crash if the float variable gets deleted at some point.
### Inherited Functionality 
[Check Element Base](ElementBase.md)




