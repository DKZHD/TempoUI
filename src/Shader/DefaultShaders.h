#pragma once

namespace DefaultShaders
{
    inline const char* vertex_shader = R"(
        #version 330 core
        layout(location = 0) in vec2 Position;
        layout(location = 1) in vec2 texCoords;
        layout(location = 2) in vec2 size;
        layout(location = 3) in vec4 color;
        layout(location = 4) in vec4 radius;
        layout(location = 5) in float textureID;
        
        out vec2 vPosition;
        out vec2 vSize;
        out vec2 vTexCoords;
        out vec4 vColor;
        out vec4 vRadius;
        flat out float vTextureID;
        
        uniform mat4 Projection;
        
        void main(){
            vPosition = Position;
            vSize = size;
            vTexCoords = texCoords;
            vColor = color;
            vRadius = radius;
            vTextureID = textureID;
        
            gl_Position = Projection * vec4(Position,0.f,1.f);
        })";

    inline const char* fragment_shader = R"(
        #version 330 core
        in vec2 vPosition;
        in vec2 vSize;
        in vec2 vTexCoords;
        in vec4 vColor;
        in vec4 vRadius;
        flat in float vTextureID;
        
        out vec4 fragColor;
        
        uniform sampler2D uTextures[32];
        
        sampler2D get_sample(int id){
            switch(id){
                case 0: return uTextures[0];
                case 1: return uTextures[1];
                case 2: return uTextures[2];
                case 3: return uTextures[3];
                case 4: return uTextures[4];
                case 5: return uTextures[5];
                case 6: return uTextures[6];
                case 7: return uTextures[7];
                case 8: return uTextures[8];
                case 9: return uTextures[9];
                case 10: return uTextures[10];
                case 11: return uTextures[11];
                case 12: return uTextures[12];
                case 13: return uTextures[13];
                case 14: return uTextures[14];
                case 15: return uTextures[15];
                case 16: return uTextures[16];
                case 17: return uTextures[17];
                case 18: return uTextures[18];
                case 19: return uTextures[19];
                case 20: return uTextures[20];
                case 21: return uTextures[21];
                case 22: return uTextures[22];
                case 23: return uTextures[23];
                case 24: return uTextures[24];
                case 25: return uTextures[25];
                case 26: return uTextures[26];
                case 27: return uTextures[27];
                case 28: return uTextures[28];
                case 29: return uTextures[29];
                case 30: return uTextures[30];
                case 31: return uTextures[31];
                default: return uTextures[0];
            }
        }
        
        float roundedRectSDF(vec2 point, vec2 bounds, vec4 radius) {
        
            vec2 radius_h = (point.y > 0.0) ? radius.zw : radius.xy;
            float thisRadius = (point.x > 0.0) ? radius_h.y : radius_h.x;
        
            vec2 dist = abs(point) - bounds + thisRadius;
            return min(max(dist.x, dist.y), 0.0) + length(max(dist, 0.0)) - thisRadius;
        }
        
        void main(){
            vec2 center = vSize * 0.5;
            vec2 p = (vTexCoords*vSize)-center;
        
            float dist = roundedRectSDF(p,center, vRadius);
            float alpha = 1.0 - smoothstep(-1.0,1.0,dist);
            if(alpha<= 0.0) discard;
        
            int id = int(vTextureID+0.5);
            if(id == 1)
            {
                fragColor = vec4(vColor.rgb, vColor.a * texture(get_sample(id),vTexCoords).r);
            }
            else if(id == -1)
            {
                vec4 top_color = mix(vec4(0.0, 1.0, 0.0, 1.0), vec4(1.0, 1.0, 1.0, 1.0), vTexCoords.x);
                vec4 bot_color = mix(vec4(0.0, 0.0, 1.0, 1.0), vec4(1.0, 0.0, 0.0, 1.0), vTexCoords.x);
                fragColor = mix(top_color, bot_color, vTexCoords.y);
            }
            else
            {
                fragColor = vColor * texture(get_sample(id),vTexCoords);
            }
            fragColor.a *= alpha;
        })";
} // namespace DefaultShaders