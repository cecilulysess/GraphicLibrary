varying vec3 ec_vnormal, ec_vposition;



void main() {
  vec3 P, N, L, V, H;
  vec4 diffuse_color = gl_FrontMaterial.diffuse;
  vec4 specular_color = gl_FrontMaterial.specular;
  float shininess = gl_FrontMaterial.shininess;

  P = ec_vposition;
  N = normalize(ec_vnormal);
  L0 = normalize(gl_LightSource[0].position - P);
  L1 = normalize(gl_LightSource[1].position - P);
  L1 = normalize(gl_LightSource[2].position - P);
  V = normalize( -P );
  H0 = normalize(L0 + V);
  H1 = normalize(L1 + V);
  H2 = normalize(L2 + V);

  vec4 diff_c0 = diffuse_color;
  vec4 diff_c1 = diff_c0, diff_c2 = diff_c0;
  vec4 spec_c0 = specular_color;
  vec4 spec_c1 = spec_c0, spec_c2 = spec_c0;

  diff_c0 *= max(dot(N, L0), 0.0);
  diff_c1 *= max(dot(N, L1), 0.0);
  diff_c1 *= max(dot(N, L2), 0.0);

  spec_c0 *= pow(max(dot(H0, N), 0.0), shininess);
  spec_c1 *= pow(max(dot(H1, N), 0.0), shininess);
  spec_c2 *= pow(max(dot(H2, N), 0.0), shininess);

  diffuse_color = diff_c0 + diff_c1 + diff_c2;
  specular_color = spec_c0 + spec_c1 + spec_c2;
  gl_FragColor = diffuse_color + specular_color;
  gl_FragColor = vec4(0.0, 0.1, 0.2, 0.8);
}
