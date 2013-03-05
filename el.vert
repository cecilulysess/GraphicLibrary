uniform vec3 eye_pos;
varying vec3 ec_vnormal, ec_vposition;
//Attenuation
varying float attent0, attent1, attent2;
varying vec3 light_dir0, light_dir1, light_dir2;

void main() {
  // gl_NormalMatrix is inverse transpose model-view matrix
  ec_vnormal = gl_NormalMatrix * gl_Normal;
  ec_vposition = gl_ModelViewMatrix * gl_Vertex;
  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
  vec3 Pos = eye_pos; //ec_vposition;
  
  light_dir0 = vec3(gl_LightSource[0].position.xyz - Pos);
  float dis0 = length(light_dir0);
  attent0 = 1.0 / (gl_LightSource[0].constantAttenuation +
    (gl_LightSource[0].linearAttenuation * dis0) + 
    (gl_LightSource[0].quadraticAttenuation * dis0 * dis0 ) );

  light_dir1 = vec3(gl_LightSource[1].position.xyz - Pos);
  float dir1 = length(light_dir1);
  attent1 = 1.0 / (gl_LightSource[1].constantAttenuation +
    (gl_LightSource[1].linearAttenuation * dir1) + 
    (gl_LightSource[1].quadraticAttenuation * dir1 * dir1 ) );

  light_dir2 = vec3(gl_LightSource[2].position.xyz - Pos);
  float dir2 = length(light_dir2);
  attent2 = 1.0 / (gl_LightSource[2].constantAttenuation +
    (gl_LightSource[2].linearAttenuation * dir2) + 
    (gl_LightSource[2].quadraticAttenuation * dir2 * dir2 ) );
}
