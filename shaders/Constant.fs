#version 150
uniform vec4 Colour;

/// @brief our output fragment colour
out vec4 fragColour;


void main ()
{
  fragColour = Colour;
}

