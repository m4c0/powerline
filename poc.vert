#version 450

layout(constant_id = 0) const float aspect = 1.0;

layout(location = 0) in vec2 pos;

layout(location = 0) out vec2 f_pos;

void main() {
  vec2 p = pos * 2.0 - 1.0;
  gl_Position = vec4(p, 0, 1);
  f_pos = p * vec2(aspect, -1);
}
