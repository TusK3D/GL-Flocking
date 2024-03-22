#version 150
uniform bool Normalize;
/// @brief the vertex passed in
in vec3 inVert;
uniform mat4 MVP;

void main(void)
{
  gl_Position = MVP*vec4(inVert,1);
}
