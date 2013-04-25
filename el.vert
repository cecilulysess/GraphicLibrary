uniform vec3 eye_pos;
uniform int L0Switch;
varying vec3 ec_vnormal, ec_vposition, ec_vtangent, ec_vbitangent, ec_reflect;
attribute vec3 tangent, bitangent;
varying vec4 tcoord;

void main() {
  vec4 ec_vertex;
  vec3 ec_eyedir;
  tcoord = gl_TextureMatrix[7] * gl_Vertex;

  // gl_NormalMatrix is inverse transpose model-view matrix
  ec_vnormal = gl_NormalMatrix * gl_Normal;
  ec_vposition = gl_ModelViewMatrix * gl_Vertex;
  ec_vtangent = gl_NormalMatrix * tangent;
  ec_vbitangent = gl_NormalMatrix * bitangent;
  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
  gl_TexCoord[0] = gl_MultiTexCoord0;
  ec_eyedir = normalize(-ec_vposition);
  ec_reflect = -ec_eyedir + 
               2.0 * (dot(ec_eyedir, normalize(ec_vnormal))) * normalize(ec_vnormal);

}
