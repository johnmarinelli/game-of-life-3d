#version 410 core

in vec4 vs_color;
in vec3 position_worldspace;
in vec3 normal_viewspace;
in vec3 eyedirection_viewspace;
in vec3 lightdirection_viewspace;
out vec4 color;

uniform mat4 mv_matrix;
uniform vec3 light_pos_worldspace = vec3(100,100,100);

void main() {
  // light emission properties
  /*vec3 lightcolor = vec3(1.0,1.0,1.0);
   vec3 lightambient = vec3(0.9, 0.9, 0.9);
   vec3 lightdiffuse = lightcolor * 0.3;
   vec3 lightspec = vec3(0.6, 0.6, 0.6);
   float lightpower = 50.0;*/
  vec3 diffusealbedo = vec3(0.5, 0.0, 0.0);
  vec3 specalbedo = vec3(0.7);
  vec3 ambient = vec3(0.1);
  
  // material properties
  vec3 materialDiffuseColor = vs_color.rgb;
  vec3 materialAmbientColor = vec3(0.5, 0.5, 0.5) * materialDiffuseColor;
  vec3 materialSpecularColor = vec3(0.7);
  float materialSpecularPower = 128.0;
  
  // distance from light
  float distance = length(light_pos_worldspace - position_worldspace);
  
  // normal of the computed fragment in view space
  vec3 n = normalize(normal_viewspace);
  
  // direction of light (from fragment to light)
  vec3 l = normalize(lightdirection_viewspace);
  
  // cosine of angle between normal and light direction
  // clamped above 0
  float costheta = clamp(dot(l,n), 0, 1);
  
  // eye vector
  vec3 e = normalize(eyedirection_viewspace);
  
  // direction that light is reflected
  vec3 r = reflect(-l, n);
  
  // cosine of angle between eye and reflect vector
  float cosalpha = clamp(dot(r,e), 0, 1);
  
  float dissq = distance * distance;
  
  //vec3 ambient = materialAmbientColor * lightambient;
  //vec3 diffuse = (materialDiffuseColor * costheta * lightdiffuse);
  vec3 diffuse = max(dot(n,l), 0.0) * diffusealbedo;
  //vec3 spec = (materialSpecularColor * (pow(cosalpha, materialSpecularPower)) * lightspec);
  vec3 spec = pow(max(dot(r,e), 0.0), materialSpecularPower) * specalbedo;
  
  color = vec4(ambient + diffuse + spec, 1.0);
}
