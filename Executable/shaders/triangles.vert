
#version 400 core

layout( location = 0 ) in vec3 vertexPosition;
layout( location = 1 ) in vec3 vertexColour;

uniform mat4 mvp;

out vec4 fragColour;

void
main()
{
    gl_Position = mvp * vec4(vertexPosition, 1.0);
	fragColour = vec4(vertexColour, 1.0);
}
