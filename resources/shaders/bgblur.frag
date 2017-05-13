#version 410 core

out vec3 fColor;

in vec2 vTexCoords;

uniform sampler2D tInputBG;
uniform vec2 uResolution;

void main() {
  // vec3 color = vec3(0.0);
  // vec2 off1 = vec2(1.3333333333333333);
  // color += texture2D(tInputBG, vTexCoords).rgb * 0.29411764705882354;
  // color += texture2D(tInputBG, vTexCoords + (off1 / resolution)).rgb * 0.35294117647058826;
  // color += texture2D(tInputBG, vTexCoords - (off1 / resolution)).rgb * 0.35294117647058826;





  vec3 color = vec3(0.0);
  vec2 off1 = vec2(1.411764705882353);
  vec2 off2 = vec2(3.2941176470588234);
  vec2 off3 = vec2(5.176470588235294);
  color += texture2D(tInputBG, vTexCoords).rgb * 0.1964825501511404;
  color += texture2D(tInputBG, vTexCoords + (off1 / uResolution)).rgb * 0.2969069646728344;
  color += texture2D(tInputBG, vTexCoords - (off1 / uResolution)).rgb * 0.2969069646728344;
  color += texture2D(tInputBG, vTexCoords + (off2 / uResolution)).rgb * 0.09447039785044732;
  color += texture2D(tInputBG, vTexCoords - (off2 / uResolution)).rgb * 0.09447039785044732;
  color += texture2D(tInputBG, vTexCoords + (off3 / uResolution)).rgb * 0.010381362401148057;
  color += texture2D(tInputBG, vTexCoords - (off3 / uResolution)).rgb * 0.010381362401148057;


  color = texture2D(tInputBG, vTexCoords).rgb;










  fColor = color;
}
