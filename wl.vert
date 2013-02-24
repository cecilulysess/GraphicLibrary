varying vec3 wc_normal, wc_position;

void main() {
  wc_normal = gl_Normal;
  wc_position = gl_Vertex;
  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}
