
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
#include "Camera.h"

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
bool DrawNormal = false;
bool IsDrawFrame = false;
bool IsDrawGrid = true;
bool IsFillLight = true;
bool IsKeyLight = true;
bool IsBgLight = true;
double focus = 0.42;
float shininess = 2.0;
float l0brightness = 1.5;

Camera *camera;

Frustum* frustum;
std::vector<GLuint> shaders;
GLuint selected_shader_id = 0;

void setup_the_viewvolume(){
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  frustum = new Frustum(60, (float)WIDTH / (float)HEIGHT, 0.02, 20.0);
  glLoadMatrixd(frustum->GetMatrix().Transpose().GetPtr());
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glRotated(0, 0, 0, 1);
  glRotated(10, 1, 0, 0);
  glRotated(0, 0, 1, 0);
  glTranslated(0, -0.15, -0.4);
}


GLfloat* vertices;
GLfloat vertices_number;
GLuint* faces;
GLuint faces_length;
GLfloat* normal;
int normal_length;
GLuint vertices_normal_length;
GLfloat* vertices_normal;

void load_object(const char* path){
  GraphicObject* obj = new GraphicObject();
  obj->readFile(path);
  obj->execute();
  vertices = obj->getVertexPointer();
  vertices_normal = obj->getNormalPointer();
  vertices_number = obj->getVertexNumber();
  normal_length = obj->getFaceNumber();
  faces_length = obj->getFaceNumber();
  normal = obj->getFaceNormal();
//  obj->PrintAll();
  //validate_object();
}

void LightSwitch(){
  if (IsFillLight) {
    glEnable(GL_LIGHT1);
  } else {
    glDisable(GL_LIGHT1);
  }
  if (IsKeyLight) {
    glEnable(GL_LIGHT0);
  } else {
    glDisable(GL_LIGHT0);
  }
  if (IsBgLight) {
    glEnable(GL_LIGHT2);
  } else {
    glDisable(GL_LIGHT2);
  }
}

void draw_stuff(){
  LightSwitch();
  int draw_nu = 6;
  glEnable(GL_DEPTH_TEST);
  glUseProgram(0);
  glClearColor(0.35, 0.35, 0.35, 0.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  if (IsDrawGrid)
    GraphicUtilities::DrawGrid(10, 1);
  // draw normal
  glUseProgram(0);
  glDisable(GL_LIGHTING);
  glClearColor(0,0,0,0);
  float* v = normal;
  float* vn = vertices_normal;
  glColor4f(0, 0, 1.0, 1.0);
  if ( DrawNormal ) {
    for (int i = 0; i < faces_length * 3; i++) {
      glBegin(GL_LINES);
        glVertex3fv(&vertices[i*3]);
        //glVertex3f(vertices[i*3] + v[i/3 * 3] ,vertices[i*3 + 1] + v[i/3* 3+1], vertices[i*3 + 2]+ v[i/3 * 3+2]);
	glVertex3f(vertices[i*3] + vn[i*3] ,vertices[i*3 + 1] + vn[i* 3+1], vertices[i*3 + 2]+ vn[i * 3+2]);
      glEnd();
    }
  }
  glEnable(GL_LIGHTING);
//  frustum->DrawFrustum(50, 4.0/3.0, 0.1, 20);
  glUseProgram(selected_shader_id);
  printf("Using shader %d\n", selected_shader_id);
  glEnableClientState(GL_VERTEX_ARRAY); 
  glEnableClientState(GL_NORMAL_ARRAY);
  //glScalef(10, 10, 10);
  glVertexPointer(3,GL_FLOAT, 3 * sizeof(GLfloat), vertices); 
  glNormalPointer(GL_FLOAT, 3 * sizeof(GLfloat), vertices_normal);
  //for (int i = 0; i < faces_length; ++i) {
    //glNormal3fv(&normal[i * 3]);    
  //  glDrawArrays(GL_TRIANGLES, 3 * i, 3); 
  //} 
  glDrawArrays(GL_TRIANGLES, 0, 3 * faces_length);
  
  glPushMatrix();
  glTranslated(0.15, 0, -0.3);
  glRotated(45, 0, 1, 0);
  glDrawArrays(GL_TRIANGLES, 0, 3 * faces_length);
  glPopMatrix();
  glPushMatrix();
  glTranslated(-0.3, 0, -0.3);
  glScalef(0.1,0.1,0.1);
  glutSolidTeapot (0.5);
  glPopMatrix();
  /*for (i = 0; i < faces_length; ++i) {
    glNormal3fv(&normal[i]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * i) );
  }
  glFlush();*/
}

void do_lights(){
  // Light0 as key light
  float light0_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
  float light0_diffuse[] = { l0brightness,l0brightness,l0brightness,0.0};
  float light0_specular[] = { l0brightness,l0brightness,l0brightness,0.0};
  float light0_position[] = { 3, 4, 3, 1.0 };
  float light0_direction[] = { -1.5, -2.0, -2.0, 1.0 };
  
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light0_ambient);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
  
  glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
  glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 1.0);
  glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0);
  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5 );
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.2 );
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.015);
  
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_direction);
  
// Light1 as fill light
  float light1_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
  float light1_diffuse[] = { 1.0, 1.0, 1.0, 0.0 };
  float light1_specular[] = { 1.0, 1.0, 1.0, 0.0 };
  float light1_position[] = { -3, 2.5, 4, 1.0 };
  float light1_direction[] = { -1.5, -2.0, -2.0, 1.0 };
  
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light1_ambient);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
  
  glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
  glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 1.0);
  glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180.0);
  glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.8 );
  glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5 );
  glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);
  
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_direction);


// Light2 as background light
  float light2_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
  float light2_diffuse[] = { 1.0, 1.0, 1.0, 0.0 };
  float light2_specular[] = { 1.0, 1.0, 1.0, 0.0 };
  float light2_position[] = { 0, 4, -5, 1.0 };
  float light2_direction[] = { -1.5, -2.0, -2.0, 1.0 };
  
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light2_ambient);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
  
  glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
  glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
  glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 1.0);
  glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 180.0);
  glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.8 );
  glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.4 );
  glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.20);
  
  glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
  glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_direction);


  glEnable(GL_LIGHTING);
  
}

void do_material(){

  float mat_ambient[] = {1.0, 1.0, 1.0, 1.0};
  float mat_diffuse[] = {255.0/255, 200.0/255, 100.0/255, 1.0};
  float mat_specular[] = {255.0/255, 240.0/255, 215.0/255, 1};
  float mat_shininess[] = {shininess};
  
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
  selected_shader_id = p;
  cout<<"Finished Set up of Shaders: "<<p<<endl;
  return ;
}


void init(const char* model_path) {
  //camera = new Camera(Vector3d(0, 0.15, 0.4), Vector3d(0, 0, 0), 
  //          Vector3d(0, 1 , 0.1));
  //camera->PerspectiveDisplay(WIDTH, HEIGHT);
  setup_the_viewvolume();
  do_lights();
  do_material();
  load_object(model_path);
  /*glBindBuffer(GL_ARRAY_BUFFER, mybuf[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  
  //final arg is bte offset, not address
  glVertexPointer(3, GL_FLOAT, 3 * sizeof(GLfloat), 0);
  glEnableClientState(GL_VERTEX_ARRAY);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mybuf[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces_length * sizeof(GLfloat),
               faces,
              GL_STATIC_DRAW);
  */
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
      if(focus > 0.2) focus -= 0.03;
      WantToRedraw = true;
      break;
    case 'x':
      if(focus < 100.0) focus += 0.03;
      WantToRedraw = true;
      break;
    case 's':
      (++selected_shader_id) %= 2;
      break;
    case 'n':
      DrawNormal = !DrawNormal;
      break;
    case 'f':
      IsDrawFrame = !IsDrawFrame;
      break;
    case 'g':
      IsDrawGrid = !IsDrawGrid;
      break;
    case '2':
      IsFillLight = !IsFillLight;
      break;
    case '1':
      IsKeyLight = !IsKeyLight;
      break;
    case '3':
      IsBgLight = !IsBgLight;
      break;
    case 'i':
      shininess += 0.1f;
      do_material();
      break;
    case 'o':
      shininess -= 0.1f;
      do_material();
      break;
    case 'k':
      l0brightness +=0.1f;
      do_lights();
      break;
    case 'l':
      l0brightness -=0.1f;
      do_lights();
      break;
    default:
      break;
  }
  printf("Shininess: %f\n", shininess);
  WantToRedraw = true;
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
     //camera->PerspectiveDisplay(WIDTH, HEIGHT);
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
  //glutPostRedisplay();
}

void mouseEventHandler(int button, int state, int x, int y) {
    // let the camera handle some specific mouse events (similar to maya)
      camera->HandleMouseEvent(button, state, x, y);
}

void motionEventHandler(int x, int y) {
    // let the camera handle some mouse motions if the camera is to be moved
      camera->HandleMouseMotion(x, y);
      glutPostRedisplay();
}

/*
 Main program to draw the square, change colors, and wait for quit
 */
int main(int argc, char* argv[]){
#ifdef TESTING_
  test_Vector();
  test_Matrix();
#endif
  if(argc != 2){
    fprintf(stderr, "usage: show_object object.ply\n");
    exit(1);
  }
  //LoadParameters(argv[1]);
  //parafile = argv[1];
  
  
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
  init(argv[1]);
  
  // set up the callback routines to be called when glutMainLoop() detects
  // an event
  //  glutReshapeFunc(doReshape);
  
  SetShadersOrDie(shaders); 
  glutDisplayFunc(RenderScene);
  //glutMouseFunc(mouseEventHandler);
  //glutMotionFunc(motionEventHandler);
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
