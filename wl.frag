niform vec3 eye_position, light_position;

varying vec3 wc_normal, wc_position;

void main() {
  vec3 P, N, L, V, H;
  vec4 diffuse_color = gl_FrontMaterial.diffuse;
  vec4 specular_color = gl_FrontMaterial.specular;
  float shininess = gl_FrontMaterial.shininess;

  P = wc_position;
  N = normalize(wc_normal);
  L = normalize(light_position - P);
  V = normalize(eye_position - P);
  H = normalize(L + V);
  
  diffuse_color *= max(dot(N, L), 0.0);
  specular_color *= pow(max(dot(H, N), 0.0), shininess);
  gl_FragColor = diffuse_color + specular_color;
}
