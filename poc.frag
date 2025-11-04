#version 450

layout(location = 0) in vec2 f_pos;

layout(location = 0) out vec4 colour;

void main() {
  vec2 p = f_pos * 0.5 + 0.5;
  colour = vec4(p, 1, 1);
}
