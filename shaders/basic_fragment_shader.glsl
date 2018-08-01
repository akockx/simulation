#version 130

//input variables are automatically interpolated between vertices.
in vec3 color;

//the color that is used to draw this fragment on the screen.
out vec4 fragmentColor;

void main() {
    fragmentColor = vec4(color, 1);
}
