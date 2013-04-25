varying vec3 ec_vnormal, ec_vposition, ec_vtangent, ec_vbitangent, ec_reflect;
uniform int LtSwitch, IsNormalMap, IsEnvLightSrc;
uniform sampler2D mytexture;
uniform sampler2D mynormalmap;
uniform sampler2D diffuse_irr_map;
uniform sampler2D specular_irr_map;

void main() {
  vec2 d_irr_idx, s_irr_idx;
  float attent[3] = {0.0, 0.0, 0.0};
  vec2 flipped_texcoord = vec2(gl_TexCoord[0].x, 1.0 - gl_TexCoord[0].y);
  
  vec3 P, N, L[3], V, H[3], mapN, R, tcolor, d_irr, s_irr;
  vec4 diffuse_color = gl_FrontMaterial.diffuse;
  vec4 specular_color = gl_FrontMaterial.specular;
  float shininess = gl_FrontMaterial.shininess;

  P = ec_vposition;
  //N = normalize(ec_vnormal);
  V = normalize(-P);
  R = 0.99 * normalize(ec_reflect);
  vec3 light_dir;
  float light_dist;
  for (int i = 0; i < 3; ++i) {
    light_dir = vec3(gl_LightSource[i].position.xyz - ec_vposition);
    light_dist = length(light_dir);
    attent[i] = 1.0 / (gl_LightSource[i].constantAttenuation +
                (gl_LightSource[i].linearAttenuation * light_dist) + 
                (gl_LightSource[i].quadraticAttenuation * light_dist * light_dist ) );
    L[i] = normalize(gl_LightSource[i].position - P);
    H[i] = normalize(L[i] + V);
  }
  
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
  float coef = 0.99;
  d_irr = vec3(texture2D(diffuse_irr_map, d_irr_idx));
  s_irr = vec3(texture2D(specular_irr_map, s_irr_idx));
  

  vec4 diff[3], spec[3];
  for (int i = 0; i < 3; ++i){
    diff[i] = diffuse_color;
    spec[i] = specular_color;
    diff[i] *= (max(dot(N, L[i]), 0.0) * gl_LightSource[0].diffuse * attent[i]);
    spec[i] *= (pow(max(dot(H[i], N), 0.0), shininess) / 8.0 / 3.14159 * 
              (shininess + 2.0) * attent[i] * gl_LightSource[0].specular);
    diff[i] = diff[i] * coef + (1 - coef) * vec4(tcolor * coef + (1-coef) * d_irr, 1.0);
  }

  if (! (LtSwitch & 0x1) ) {
    diff[0] *= 0.0;
    spec[0] *= 0.0; 
  }
  if (! (LtSwitch & 0x2) ) {
    diff[1] *= 0.0;
    spec[1] *= 0.0;

  }
  if (! (LtSwitch & 0x4) ) {
    diff[2] *= 0.0;
    spec[2] *= 0.0;
  }

  if (IsEnvLightSrc) {
    gl_FragColor = vec4(tcolor, 1.0);
  } else {
    diffuse_color = diff[0];
    specular_color = spec[0];
    for (int i = 1; i < 3; ++i) {
      diffuse_color += diff[i];
      specular_color += spec[i];
    }

    gl_FragColor =  diffuse_color;
    gl_FragColor += 0.5 * specular_color;
    //gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
  }

}
