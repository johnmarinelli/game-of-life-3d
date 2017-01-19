#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 in_color;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;
uniform mat4 mv_matrix;

uniform vec3 camera_position;

out vec4 vs_color;

out vec3 position_worldspace;
out vec3 normal_viewspace;
out vec3 eyedirection_viewspace;
out vec3 lightdirection_viewspace;

uniform vec3 light_pos_worldspace = vec3(100,100,100);

void main() {
  mat4 mvp = proj_matrix * view_matrix * model_matrix;
  
  // final output position of vertex in clip space
  gl_Position = mvp * vec4(position, 1.0);
  
  // position of vertex in world space
  position_worldspace = (model_matrix * vec4(position, 1.0)).xyz;
  
  // position of vertex in view space (after camera)
  vec3 position_viewspace = (view_matrix * model_matrix * vec4(position, 1.0)).xyz;
  
  // -P
  eyedirection_viewspace = vec3(0,0,0) - position_viewspace;
  
  // vector from position to light
  vec3 lightposition_viewspace = (view_matrix * vec4(light_pos_worldspace, 1.0)).xyz;
  lightdirection_viewspace = lightposition_viewspace + eyedirection_viewspace;
  
  
  // Normal of the the vertex, in camera space
  // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
  // normal of the vertex in camera space
  normal_viewspace = (view_matrix * model_matrix * vec4(normal, 0)).xyz;
  
  //vs_color = vec4(in_color,1.0);
  vs_color = vec4(normal, 1);
}
