#version 330 core
layout(location = 0) in vec2 Position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec2 size;
layout(location = 3) in vec4 color;
layout(location = 4) in vec4 radius;
layout(location = 5) in vec4 clipping;
layout(location = 6) in float textureID;

out vec2 vPosition;
out vec2 vSize;
out vec2 vTexCoords;
out vec4 vColor;
out vec4 vRadius;
out vec4 vClipping;
flat out float vTextureID;

uniform mat4 Projection;

void main(){
    vPosition = Position;
    vSize = size;
    vTexCoords = texCoords;
    vColor = color;
    vRadius = radius;
    vClipping = clipping;
    vTextureID = textureID;

    gl_Position = Projection * vec4(Position,0.f,1.f);
}