//#ifndef MAIN_PROG
//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
//
//#include "GLCommonHeader.h"
//#include "common_data_structure.h"
//#include "GraphicUtilities.h"
//
//#define BOX 1
//
//void graphics_init() {
//  int i, j;
//  glClearColor(0.7, 0, 0, 0);
//  
//  glNewList(BOX, GL_COMPILE);
//  for(i = 0; i < 100; ++i) {
//    glBegin(GL_QUAD_STRIP);
//    for(j = 0; j < 110; ++j) {
//      glColor3f(0.0, 0.01 * j, 0.01 * j);
//      glVertex2f(-0.5 + j * 0.01, -0.5 + i * 0.01);
//      glVertex2f(-0.5 + j * 0.01, -0.5 + (i + 1) * 0.01);
//      
//    }
//    glEnd();
//  }
//  glEndList();
//}
//
//int p;
//
//void do_stuff(){
//  float twist_value;
//  int location;
//  location = glGetUniformLocation(p, "twist");
//  for (twist_value = 0.0; twist_value <= 3.14; twist_value += 0.001) {
//    glUniform1f(location, twist_value);
//    glClear(GL_COLOR_BUFFER_BIT);
//    glCallList(BOX) ;
//    glutSwapBuffers() ;
//  }
//  for (twist_value = 3.14; twist_value >= 0.0; twist_value -= 0.001) {
//    glUniform1f(location, twist_value);
//    glClear(GL_COLOR_BUFFER_BIT);
//    glCallList(BOX);
//    glutSwapBuffers();
//  }
//}
//
//unsigned int set_shaders(){
//  GLint vertCompiled, fragCompiled;
//  char *vs, *fs;
//  GLuint v, f;
//  
//  v = glCreateShader(GL_VERTEX_SHADER);
//  f = glCreateShader(GL_FRAGMENT_SHADER);
//  vs = GraphicUtilities::read_shader_program(VERT_SHADER_FILE_DIR);
//  fs = GraphicUtilities::read_shader_program(FRAG_SHADER_FILE_DIR);
//  // shader, # of string, array of string and array of tring length
//  glShaderSource(v, 1, (const char**)&vs, NULL);
//  glShaderSource(f, 1, (const char**)&fs, NULL);
//  free(vs);
//  free(fs);
//  glCompileShader(v);
//  glCompileShader(f);
//  p = glCreateProgram();
//  glAttachShader(p, f);
//  glAttachShader(p, v);
//  glLinkProgram(p);
//  glUseProgram(p);
//  cout<<"Finished Set up of Shaders: "<<p<<endl;
//  return p;
//}
//
//int main(int argc, char **argv){
//  
//  glutInit(&argc, argv);
//  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
//  glutInitWindowSize(1024, 768);
//  glutInitWindowPosition(100, 50);
//  glutCreateWindow("Hurricane");
//  p = set_shaders();
//  graphics_init();
//  glutDisplayFunc(do_stuff);
//  glutIdleFunc(do_stuff);
//  glutMainLoop();
//  return 0;
//}
//#endif