varying vec3 ec_vnormal, ec_vposition;



void main() {
  vec3 P, N, L, V, H;
  vec4 diffuse_color = gl_FrontMaterial.diffuse;
  vec4 specular_color = gl_FrontMaterial.specular;
  float shininess = gl_FrontMaterial.shininess;

  P = ec_vposition;
  N = normalize(ec_vnormal);
  L = normalize(gl_LightSource[0].position - P);
  V = normalize( -P );
  H = normalize(L + V);
  
//  diffuse_color = diffuse_color;
  diffuse_color *= max(dot(N, L), 0.0);
  specular_color *= pow(max(dot(H, N), 0.0), shininess);
  specular_color /= (8.0 * 3.1415926 / (shininess + 2));
  gl_FragColor = diffuse_color + specular_color;
}
