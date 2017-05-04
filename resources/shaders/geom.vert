#version 410 core

// Uniforms
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

// Ins
in vec3 position;
in vec4 instances; // Each instance will have x,y,z and w (radius)

// Outs
out vec3 vPosition;
out vec3 vNormal;
out vec3 vWorldNormal;
out vec3 vWorldPosition;

void main(void)
{
  vPosition = vec4(ViewMatrix * ModelMatrix * vec4(instances.w * position + instances.xyz, 1.0)).xyz;
  vWorldPosition = instances.w * position + instances.xyz;
  mat3 normalMatrix = transpose(inverse(mat3(ViewMatrix * ModelMatrix)));
  vNormal = normalMatrix * normalize(position);
  vWorldNormal = normalize(position);
  gl_Position = ProjectionMatrix * vec4(vPosition, 1.0);
}
