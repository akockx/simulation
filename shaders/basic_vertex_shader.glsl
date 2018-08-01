#version 130

uniform mat4 modelViewProjectionMatrix;

in vec3 vertexPosition;//in model space.
in vec3 vertexColor;

//output variables are sent to the fragment shader and are automatically interpolated between vertices.
out vec3 color;

void main() {
    gl_Position = modelViewProjectionMatrix * vec4(vertexPosition, 1);

    color = vertexColor;
}
