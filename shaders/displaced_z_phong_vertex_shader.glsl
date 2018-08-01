#version 130

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;

in vec3 vertexPosition;//in model space.
in vec3 vertexNormal;//in model space.
in vec3 vertexColor;
in float vertexZDisplacement;//relative to the (constant) vertexPosition in model space.

//output variables are sent to the fragment shader and are automatically interpolated between vertices.
out vec3 fragmentPosition;//in camera space.
out vec3 fragmentNormalVector;//in camera space.
out vec3 fragmentDiffuseColor;//diffuse reflection coefficient per color component (r, g, b).

/**
 * Implements Phong shading, see https://en.wikipedia.org/wiki/Phong_shading
 * Allows displacement of z coordinate per vertex.
 * This can be used for example to change the shape of a horizontal fluid surface every frame.
 */
void main() {
    vec3 displacedVertexPosition = vec3(vertexPosition.xy, vertexPosition.z + vertexZDisplacement);
    gl_Position = modelViewProjectionMatrix * vec4(displacedVertexPosition, 1);

    vec4 displacedVertexPositionInCameraSpace = modelViewMatrix * vec4(displacedVertexPosition, 1);
    vec4 vertexNormalInCameraSpace = modelViewMatrix * vec4(vertexNormal, 0);
    fragmentPosition = displacedVertexPositionInCameraSpace.xyz;
    fragmentNormalVector = vertexNormalInCameraSpace.xyz;
    fragmentDiffuseColor = vertexColor;
}
