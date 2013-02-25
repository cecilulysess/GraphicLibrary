

//
//  main.cpp
//  PhysicalAnimation
//
//  Created by Julian Wu on 1/22/2013
//  Copyright (c) 2013 Julian Wu. All rights reserved.
//

//  Style comments
//  Cooresponding header file here

//  C stdlib here

//  C++ stdlib here
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "GLCommonHeader.h"
#include "common_data_structure.h"
#include "GraphicUtilities.h"
#include "GraphicObject.h"

#ifdef MAIN_PROG
//#define TESTING_

#ifdef TESTING_
#include "_test_case.h"
#endif

#define WIDTH 1024
#define HEIGHT 768


unsigned int RENDER_MODE = 0;

int AALevel = 4;
bool WantToRedraw = false;
double focus = 6.0;

struct point {
  float x, y, z;
};

Frustum* frustum;
std::vector<GLuint> shaders;
GLuint selected_shader_id = 0;

void setup_the_viewvolume(){
  struct point eye, view, up;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
//  gluPerspective(45, (float)WIDTH / (float)HEIGHT, 0.1 , 100.0);
  frustum = new Frustum(60, (float)WIDTH / (float)HEIGHT, 1.0, 20.0);
//  frustum = new Frustum(-0.2, 0.2, -0.15, 0.15, 0.1, 20);
  glLoadMatrixd(frustum->GetMatrix().Transpose().GetPtr());
//  glFrustum(-0.2, 0.2, -0.15, 0.15, 0.1, 20);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  eye.x = 4.0, eye.y = 4.0, eye.z = 4.0;
  view.x = 0.0, view.y = 0.0, view.z = 0.0;
  up.x = 0.0, up.y = 1.0, up.z = 0.0;
  glRotated(0, 0, 0, 1);
  glRotated(0, 1, 0, 0);
  glRotated(-45, 0, 1, 0);
  glTranslated(-5, -0.5, -5);
//  gluLookAt(eye.x, eye.y, eye.z, view.x, view.y, view.z, up.x, up.y, up.z);
}

void set_uniform_parameters(unsigned int p){
  int location;
  location = glGetUniformLocation(p, "eye_position");
  glUniform3f(location, -5, -0.5, -5);
  location = glGetUniformLocation(p, "light_position");
  glUniform3f(location, 4.0, 4.0, 4.0); 
}


//GLfloat vertices[] = {
//  0.0, 0.0, 0.0,
//  0.0, 1.0, 0.0,
//  1.0, 1.0, 0.0,
//  1.0, 0.0, 0.0,
//  0.0, 0.0, 1.0,
//  0.0, 1.0, 1.0,
//  1.0, 1.0, 1.0,
//  1.0, 0.0, 1.0,
//};
//
//GLubyte face[] = {
//  4, 7, 6, 5, //front
//  0, 3, 2, 1, //back
//  3, 2, 6, 7, //right
//  1, 0, 4, 5, //left
//  5, 6, 2, 1, //top
//  3, 7, 4, 0, //bottom
//};
//
//GLfloat normal[6][3] = {
//  0.0, 0.0, 1.0,
//  0.0, 0.0, -1.0,
//  1.0, 0.0, 0.0,
//  -1.0, 0.0, 0.0,
//  0.0, 1.0, 0.0,
//  0.0, -1.0, 0.0,
//};

GLfloat* vertices;
GLfloat vertices_number;
GLuint* faces;
GLuint faces_length;
GLfloat* normal;
int normal_length;
GLuint vertices_normal_length;
GLfloat* vertices_normal;

void load_object(){
  GraphicObject* obj = new GraphicObject();
  obj->readFile();
  obj->execute();
  vertices = obj->getVertexPointer();
  vertices_normal = obj->getNormalPointer();
  normal_length = obj->getNormalLength();
  obj->PrintAll();
}

void draw_stuff(){
  int i;
  glEnable(GL_DEPTH_TEST);
  glUseProgram(0);
  glClearColor(0.35, 0.35, 0.35, 0.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  GraphicUtilities::DrawGrid(10, 1);
//  frustum->DrawFrustum(50, 4.0/3.0, 0.1, 20);
  glUseProgram(selected_shader_id);
  
  /*glPushMatrix();
  glTranslatef(0.03,-0.1,0.0);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glNormalPointer(GL_FLOAT, 0, vertices_normal);
  glDrawArrays(GL_TRIANGLES, 0, normal_length);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);

  glPopMatrix();
  glFlush();*/
 // glPushMatrix();
 // glTranslated(2.5, 0, 2.5);
 // glRotated(45, 0, 1, 0);
 // glutSolidTeapot (0.5);
 // glPopMatrix();
  /*for (i = 0; i < faces_length; ++i) {
    glNormal3fv(&normal[i]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * i) );
  }
  glFlush();*/
}

void do_lights(){
  float light0_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
  float light0_diffuse[] = { 1.0, 1.0, 1.0, 0.0 };
  float light0_specular[] = { 1.0, 1.0, 1.0, 0.0 };
  float light0_position[] = { 3, 4, 4, 1.0 };
  float light0_direction[] = { -1.5, -2.0, -2.0, 1.0 };
  
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light0_ambient);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
  
  glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
  glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 1.0);
  glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0);
  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5 );
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1 );
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01);
  
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_direction);
  
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}

void do_material(){
  float mat_ambient[] = {0.0, 0.0, 0.0, 1.0};
  float mat_diffuse[] = {0.9, 0.9, 0.1, 1.0};
  float mat_specular[] = {1.0, 1.0, 1.0, 1.0};
  float mat_shininess[] = {2.0};
  
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

unsigned int mybuf[2] = { 1, 2 };

void GLSL_LogReport(int shader, bool is_print){
  GLint maxLength = 0;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

  // max length includes the NULL character
  std::vector<GLchar> infoLog(maxLength);
  glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
  
  if (is_print){
    for(int i = 0; i < infoLog.size(); ++i) {
      cout<<infoLog[i];
    }
  }
  cout<<endl;
}

bool CompileSuccess(int obj) {
  int status;
  glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    printf("Compile Status: %d\n", status);
    GLSL_LogReport(obj, true);
  }
  return status == GL_TRUE;
}

bool LinkSuccessful(int obj) {
  int status;
  glGetProgramiv(obj, GL_LINK_STATUS, &status);
  return status == GL_TRUE;
}
// set up shaders for using GLSL
void SetShadersOrDie(std::vector<GLuint>& shaders){
  GLint vertCompiled, fragCompiled;
  char *vs, *fs, *bpfs;
  GLuint pv, pf, p, bpv, bpf, bp;
  
  pv = glCreateShader(GL_VERTEX_SHADER);
  pf = glCreateShader(GL_FRAGMENT_SHADER);
  vs = GraphicUtilities::read_shader_program(VERT_SHADER_FILE_DIR);
  fs = GraphicUtilities::read_shader_program(
    BLINN_PHONG_FRAG_SHADER_FILE_DIR);
  bpfs = GraphicUtilities::read_shader_program(
    BLINN_PHONG_FRAG_SHADER_FILE_DIR);
  // shader, # of string, array of string and array of tring length
  glShaderSource(pv, 1, (const char**)&vs, NULL);
  glShaderSource(pf, 1, (const char**)&fs, NULL);
  glShaderSource(bpv, 1, (const char**)&vs, NULL);
  glShaderSource(bpf, 1, (const char**)&bpfs, NULL);
  free(vs);
  free(fs);
  free(bpfs);
  glCompileShader(pv);
  assert(CompileSuccess(pv));
  glCompileShader(pf);
  assert(CompileSuccess(pf));
  //glCompileShader(bpv);
  //assert(CompileSuccess(bpv));
  //glCompileShader(bpf);
  //assert(CompileSuccess(bpf));
  p = glCreateProgram();
  //bp = glCreateProgram();
  glAttachShader(p, pf);
  glAttachShader(p, pv);
  //glAttachShader(bp, bpf);
  //glAttachShader(bp, pv);
  glLinkProgram(p);
  assert(LinkSuccessful(p));
  //glLinkProgram(bp);
  //assert(LinkSuccessful(bp));
  shaders.push_back(p);
  //shaders.push_back(bp);
  glUseProgram(p);
  cout<<"Finished Set up of Shaders: "<<p<<endl;
  return ;
}


void init() {
  setup_the_viewvolume();
  do_lights();
  do_material();
  load_object();
  glBindBuffer(GL_ARRAY_BUFFER, mybuf[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  
  //final arg is bte offset, not address
  glVertexPointer(3, GL_FLOAT, 3 * sizeof(GLfloat), 0);
  glEnableClientState(GL_VERTEX_ARRAY);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mybuf[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces_length * sizeof(GLfloat),
               faces,
               GL_STATIC_DRAW);
}


void KeyBoardHandler(unsigned char key, int x, int y){
  switch (key) {
    case 'q':
      glDeleteBuffers(2, /*(GLuint*)*/ mybuf);
      exit(1);
      break;
    case 'a':
      if ((RENDER_MODE & (~GL_CONTROL_DEF::KAA_MARKER)) != 0 )
        RENDER_MODE = 0;
      RENDER_MODE ^= GL_CONTROL_DEF::KAA_MARKER;
      printf("Set AA to %d \n", (RENDER_MODE & GL_CONTROL_DEF::KAA_MARKER) > 0);
      WantToRedraw = true;
      break;
    case 'w':
      AALevel = (AALevel + 4)%16;
      if (AALevel == 0) {
        AALevel = 16;
      }
      WantToRedraw = true;
      break;
    case 't':
      GraphicUtilities::JitterCamera(0.0, 0.0, 0.1 ,0.0 , 5.0, frustum);
      WantToRedraw = true;
      break;
    case 'b':
      if ((RENDER_MODE & (~GL_CONTROL_DEF::KDOF_MARKER)) != 0 )
        RENDER_MODE = 0;
      RENDER_MODE ^= GL_CONTROL_DEF::KDOF_MARKER;
      printf("Set DoF to %d \n", (RENDER_MODE & GL_CONTROL_DEF::KDOF_MARKER) > 0);
      WantToRedraw = true;
      break;
    case 'z':
      if(focus > 3.0) focus -= 0.1;
      WantToRedraw = true;
      break;
    case 'x':
      if(focus < 100.0) focus += 0.1;
      WantToRedraw = true;
      break;
    case 's':
      (++selected_shader_id) %= 2;
      break;
    default:
      break;
  }
}

/*
 On Redraw request, erase the window and redraw everything
 */
void RenderScene(){
  switch (RENDER_MODE) {
    case GL_CONTROL_DEF::KRM_AAONLY:
      cout<<"AA only"<<endl;
      GraphicUtilities::AntiAlias(AALevel, draw_stuff, frustum);
      break;
    case GL_CONTROL_DEF::KRM_DOF_ONLY:
      cout<<"DoF at focus:"<<focus<<endl;
      glEnable(GL_MULTISAMPLE);
      GraphicUtilities::DoFScene(draw_stuff, frustum, focus, 8);
      break;
    default:
     
      draw_stuff();
  }
  glFlush();
  glutSwapBuffers();
//  printf("\tDraw\n");
  WantToRedraw = false;
}

void Redraw(){
  if (WantToRedraw)
    RenderScene();
  
//  glutPostRedisplay();
}



/*
 Main program to draw the square, change colors, and wait for quit
 */
int main(int argc, char* argv[]){
#ifdef TESTING_
  test_Vector();
  test_Matrix();
#endif
  //  if(argc != 2){
  //    fprintf(stderr, "usage: bounce paramfile\n");
  //    exit(1);
  //  }
  //  LoadParameters(argv[1]);
  //  parafile = argv[1];
  
  
  // start up the glut utilities
  glutInit(&argc, argv);
  
  
  // make GLUT select a double buffered display that uses RGBA colors
  // Julian: Add GLUT_DEPTH when in 3D program so that 3D objects drawed
  // correctly regardless the order they draw
  glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH |
                       GLUT_DOUBLE |GLUT_ACCUM ); // | GLUT_ACCUM |
                       //GLUT_MULTISAMPLE );
  // glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH );
  glutInitWindowSize(WIDTH, HEIGHT);
  //  glutInitWindowPosition(50, 50);
  glutCreateWindow("New Animation");
  
  // initialize the camera and such
  init();
  
  // set up the callback routines to be called when glutMainLoop() detects
  // an event
  //  glutReshapeFunc(doReshape);
  
  SetShadersOrDie(shaders); 
  glutDisplayFunc(RenderScene);
//  glutMouseFunc(mouseEventHandler);
//  glutMotionFunc(motionEventHandler);
  glutKeyboardFunc(KeyBoardHandler);
  glutIdleFunc(Redraw);
  
  glutMainLoop();
  /* Set shading to flat shading */
  //  glShadeModel(GL_FLAT);
  
  //  MakeMenu();
  
  // Routine that loops forever looking for events. It calls the registered
  // callback routine to handle each event that is detected
  return 0;
}

#endif
