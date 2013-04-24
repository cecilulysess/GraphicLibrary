varying vec3 ec_vnormal, ec_vposition, ec_vtangent, ec_vbitangent, ec_reflect;
uniform int LtSwitch, IsNormalMap, IsEnvLightSrc;
varying float attent0, attent1, attent2;
varying vec3 light_dir0, light_dir1, light_dir2;
uniform sampler2D mytexture;
uniform sampler2D mynormalmap;
uniform sampler2D diffuse_irr_map;
uniform sampler2D specular_irr_map;

void main() {
  vec2 d_irr_idx, s_irr_idx;
  vec2 flipped_texcoord = vec2(gl_TexCoord[0].x, 1.0 - gl_TexCoord[0].y);
  
  vec3 P, N, L0, L1, L2, V, H0, H1, H2, mapN, R, tcolor, d_irr, s_irr;
  vec4 diffuse_color = gl_FrontMaterial.diffuse;
  vec4 specular_color = gl_FrontMaterial.specular;
  vec4 ambient_color = 
    (gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) 
    + (gl_LightSource[0].ambient * gl_FrontMaterial.ambient) * attent0
    + (gl_LightSource[1].ambient * gl_FrontMaterial.ambient) * attent1
    + (gl_LightSource[2].ambient * gl_FrontMaterial.ambient) * attent2;
  float shininess = gl_FrontMaterial.shininess;

  P = ec_vposition;
  //N = normalize(ec_vnormal);
  R = 0.99 * normalize(ec_reflect);
  L0 = normalize(gl_LightSource[0].position - P);
  L1 = normalize(gl_LightSource[1].position - P);
  L2 = normalize(gl_LightSource[2].position - P);
  V = normalize(-P );
  H0 = normalize(L0 + V);
  H1 = normalize(L1 + V);
  H2 = normalize(L2 + V);

  if (IsNormalMap) {
    mat3 tform = mat3(ec_vtangent, ec_vbitangent, ec_vnormal); 
    //---------------handling normal map-----------
    mapN = vec3(texture2D(mynormalmap, flipped_texcoord));
    mapN.xy = 2.0 * mapN.xy - vec2(1.0, 1.0);
    N = normalize(tform * normalize(mapN));
    //=============================================
  } else {
    N = 0.99 * normalize(ec_vnormal);
  }
  d_irr_idx = 0.5 * (N.st + vec2(1.0, 1.0));
  s_irr_idx = 0.5 * (R.st + vec2(1.0, 1.0));
  //-------------handling texture---------------
  tcolor = vec3(texture2D(mytexture, flipped_texcoord));
  diffuse_color = vec4(tcolor, 1.0);
  // diffuse_color = 0.5 * diffuse_color + 0.5 *vec4(tcolor, 1.0);
  // diffuse_color *= max(dot(N, L0), 0.0);
  //============================================
  float coef = 0.8;
  d_irr = vec3(texture2D(diffuse_irr_map, d_irr_idx));
  s_irr = vec3(texture2D(specular_irr_map, s_irr_idx));
  // diffuse_color = diffuse_color * coef + (1 - coef) * vec4(d_irr, 1.0);
  // specular_color = specular_color * coef + (1 - coef) * vec4(s_irr, 1.0);
  

  vec4 diff_c0 = diffuse_color ;
  vec4 diff_c1 = diff_c0, diff_c2 = diff_c0;
  vec4 spec_c0 = specular_color;
  vec4 spec_c1 = spec_c0, spec_c2 = spec_c0;

  diff_c0 *= (max(dot(N, L0), 0.0) * gl_LightSource[0].diffuse * attent0);
  diff_c1 *= (max(dot(N, L1), 0.0) * gl_LightSource[1].diffuse * attent1);
  diff_c2 *= (max(dot(N, L2), 0.0) * gl_LightSource[2].diffuse * attent2);

  spec_c0 *= (pow(max(dot(H0, N), 0.0), shininess) / 8.0 / 3.14159 * 
              (shininess + 2.0) * attent0 * gl_LightSource[0].specular);
  spec_c1 *= pow(max(dot(H1, N), 0.0), shininess)  / 8.0 / 3.14159 * 
              (shininess + 2.0)* attent1 * gl_LightSource[1].specular;
  spec_c2 *= pow(max(dot(H2, N), 0.0), shininess)  / 8.0 / 3.14159 * 
              (shininess + 2.0)* attent2 * gl_LightSource[2].specular;

  if (! (LtSwitch & 0x1) ) {
    diff_c0 *= 0.0;
    spec_c0 *= 0.0; 
  }
  if (! (LtSwitch & 0x2) ) {
    diff_c1 *= 0.0;
    spec_c1 *= 0.0;

  }
  if (! (LtSwitch & 0x4) ) {
    diff_c2 *= 0.0;
    spec_c2 *= 0.0;
  }

  if (IsEnvLightSrc) {
    gl_FragColor = vec4(tcolor, 1.0);
  } else {
    diff_c0 = diff_c0 * coef + (1 - coef) * vec4(tcolor * 0.7 + 0.3 * d_irr, 1.0);
    // diff_c1 = diff_c1 * coef + (1 - coef) * vec4(d_irr, 1.0);
    // diff_c2 = diff_c2 * coef + (1 - coef) * vec4(d_irr, 1.0);

    diffuse_color = diff_c0 + diff_c1 + diff_c2 ;
    specular_color = spec_c0 + spec_c1 + spec_c2 ;

    gl_FragColor =  diffuse_color;
    gl_FragColor += 0.5 * specular_color;
    gl_FragColor += ambient_color;
    //gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
  }

}
