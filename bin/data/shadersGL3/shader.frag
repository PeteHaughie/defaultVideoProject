OF_GLSL_SHADER_HEADER

precision mediump float;

uniform sampler2D u_texture;
uniform vec2 u_resolution;

out vec4 outputColor;

void main() {
  // Calculate UV coordinates based on the screen resolution
  vec2 uv = gl_FragCoord.xy / u_resolution.xy;
  outputColor = texture(u_texture, uv);
}
