#version 450

layout(set = 0, binding = 0) uniform sampler2D text;

layout(location = 0) in vec2 f_pos;

layout(location = 0) out vec4 colour;

void main() {
  vec2 uv = f_pos * vec2(0.5, 1) * 0.5 + 0.5;
  vec4 tcol = texture(text, uv);
  colour = vec4(tcol.r, 0, 1, 1);
}
