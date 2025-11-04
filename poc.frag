#version 450

layout(set = 0, binding = 0) uniform sampler2D text;

layout(location = 0) in vec2 f_pos;

layout(location = 0) out vec4 colour;

void main() {
  vec2 p = f_pos * 0.5 + 0.5;
  p = texture(text, p).rg;
  colour = vec4(p, 1, 1);
}
