varying vec3 ec_vnormal, ec_vposition;
//Attenuation
varying float attent;
varying vec3 light_dir;

void main() {
  // gl_NormalMatrix is inverse transpose model-view matrix
  ec_vnormal = gl_NormalMatrix * gl_Normal;
  ec_vposition = gl_ModelViewMatrix * gl_Vertex;
  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
  
  light_dir = vec3(gl_LightSource[0].position.xyz - ec_vposition);
  float dis = length(light_dir);
  attent = 1.0 / (gl_LightSource[0].constantAttenuation +
    (gl_LightSource[0].linearAttenuation * dis) + 
    (gl_LightSource[0].quadraticAttenuation * dis * dis ) );

}
