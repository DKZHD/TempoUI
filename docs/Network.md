# Network
Although limited, TempoUI supports basic networking through a TCP protocol.

> [!IMPORTANT] 
> To utilize any networking features, you must define the `NETWORK` macro before including the library. It is recommended to use preprocessor definitions for consistency in code base.
### Server-Side
To utilize the server side of the NetworkManager you do it like this:
``` cpp
TempoUI::NetworkManager server;
server.start_server(/*port as string*/);
```
The server can at any top be stopped afterwards by using the `stop` function, which also will be called automatically if the NetworkManager get discarded.

#### Sending Messages
To send messages to clients from the server, simply call the `send_message` function. This will change behavior depending on if the NetworkManager represents a server or client. 
``` cpp
server.send_message("Hello World");
```
On the server-side all clients will get the message.
### Client-Side
While quite similar to the server-side, there is one difference. Connecting to the server needs to be called while the server is running (obviously). This means it can not be called in the same non-thinking fashion that the server can.

To connect to a server as a client use the `connect_to_server` function like this:
``` cpp
TempoUI::NetworkManager client;
client.connect_to_server(/*ip as string*/, /*port as string*/);
```
To disconnect simply call `disconnect_from_server`

#### Sending Messages
To send messages to the server, simply call the `send_message` function. This will change behavior depending on if the NetworkManager represents a server or client. 
``` cpp
client.send_message("Hello World");
```
This will only be readable by the server itself.

### Processing Messages
To actually receive the messages and do something with the info utilize the `process_messages` function.

Example:
``` cpp
server.process_messages([](const std::string& content){
	std::cout << "Received: " << content << std::endl;
});
```
These messages gets updated asynchronously and they are thread-safe.