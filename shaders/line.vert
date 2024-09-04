#version 330 core

layout (location = 0) in vec2 initPos;

out vec4 LineColor;

uniform float xOffSet;
uniform float yOffSet;
uniform vec3 rgbColor;
uniform float windWidth;
uniform float windHeight;
uniform mat2 rotMatrix;

void main()
{
    float halfH = windHeight/2;
    float halfW = windWidth/2;
    vec2 rotCord = rotMatrix * initPos;
    vec2 scaledPos = rotCord * vec2(windWidth,windHeight);

    gl_Position = vec4((scaledPos.x + xOffSet)/halfW, (scaledPos.y + yOffSet)/halfH, 0,1);
    // gl_Position = vec4((initPos.x + xOffSet)/halfW, (initPos.y + yOffSet)/halfH, 0,1);
    LineColor = vec4(rgbColor,1);
}