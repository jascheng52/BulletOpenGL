#version 330 core

layout (location = 0) in vec2 initPos;


out vec4 PlayerColor;


uniform float xPos;
uniform float yPos;
uniform mat2 rotMat;
uniform float scale;


uniform float windWidth;
uniform float windHeight;
uniform vec3  rgbColor;

void main()
{
    float halfH = windHeight/2;
    float halfW = windWidth/2;
    vec2 scaledPos = rotMat * initPos;
    scaledPos = scaledPos * vec2(scale,scale);
    gl_Position = vec4((scaledPos.x + xPos)/halfW, (scaledPos.y + yPos)/halfH, 0 ,1);
    
PlayerColor = vec4(rgbColor, 1.0);

}