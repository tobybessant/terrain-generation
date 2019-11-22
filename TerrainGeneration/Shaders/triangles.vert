
#version 400 core

layout( location = 0 ) in vec3 vertexPosition;

uniform mat4 mvp;

out vec4 fragColour;

void
main()
{
    gl_Position = mvp * vec4(vertexPosition, 1.0);
	fragColour = vec4(0.5, 0.4, 0.8, 1.0);
}
