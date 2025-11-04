#version 450
#extension GL_GOOGLE_include_directive : require
#include "../glslinc/inigo.glsl"

layout(push_constant) uniform upc {
  float time;
};

layout(set = 0, binding = 0) uniform sampler2D text;

layout(location = 0) in vec2 f_pos;

layout(location = 0) out vec4 colour;

vec4 background(vec2 coord);

vec4 textbox(vec2 p, vec4 bgcol) {
  vec2 uv = f_pos * vec2(0.5, 1) * 0.5 + 0.5;
  float d = sd_box(f_pos, vec2(1.5, 0.9));
  bgcol *= smoothstep(-0.01, 0.01, d) * 0.7 + 0.3;
  
  vec4 tcol = texture(text, uv);
  return mix(bgcol, vec4(1), tcol.r);
}

void main() {
  vec4 bgcol = background(f_pos * vec2(1, -1));
  
  vec2 p = f_pos;
  colour = textbox(p, bgcol);
}

#include "neonwave_sunrise.glsl"
