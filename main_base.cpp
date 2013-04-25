
//
//  main.cpp
//  
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
#include "GraphicCamera.h"
#include "GraphicModel.h"

#ifdef MAIN_PROG

#define WIDTH 320
#define HEIGHT 240


unsigned int RENDER_MODE = 0;

int AALevel = 4;
bool WantToRedraw = false;
bool DrawNormal = false;
bool IsDrawFrame = false;
bool IsDrawGrid = true;
bool IsFillLight = true;
bool IsKeyLight = true;
bool IsBgLight = true;
double focus = 1.2;
float shininess = 10.0;
float l0brightness = 1.2;
GLuint LightSwitch = 0x7;

//=====================Lights=======================
  float light0_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
  float light0_diffuse[] = { l0brightness,l0brightness,l0brightness,0.0};
  float light0_specular[] = { l0brightness,l0brightness,l0brightness,0.0};
  float light0_position[] = { 3, 4, 3, 1.0 };
  float light0_direction[] = { -1.5, -2.0, -2.0, 1.0 };
//==================================================

GraphicModel *model, *ground, *skydome;
GraphicCamera::GraphicCamera *camera, *eye_cam, *light0_cam;

GLuint selected_shader_id = 0;

GLfloat* vertices;
GLfloat vertices_number;
GLuint* faces;
GLuint faces_length;
GLfloat* normal;
int normal_length;
GLuint vertices_normal_length;
GLfloat* vertices_normal;


void SwitchLight(){
  if (IsFillLight) {
    glEnable(GL_LIGHT1);
    LightSwitch |= 0x2;
  } else {
    glDisable(GL_LIGHT1);
    LightSwitch &= (~0x2);
  }
  if (IsKeyLight) {
    glEnable(GL_LIGHT0);
    LightSwitch |= 0x1;
  } else {
    glDisable(GL_LIGHT0);
    LightSwitch &= (~0x1);
  }
  if (IsBgLight) {
    glEnable(GL_LIGHT2);
    LightSwitch |= 0x4;
  } else {
    glDisable(GL_LIGHT2);
    LightSwitch &= (~0x4);
  }
}



void do_lights(){
  // Light0 as key light

  
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0 );
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.2 );
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01);
  
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
  
// Light1 as fill light
  float light1_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
  float light1_diffuse[] = { l0brightness, l0brightness, l0brightness, 0.0 };
  float light1_specular[] = { l0brightness, l0brightness, l0brightness, 0.0 };
  float light1_position[] = { -3, 2.5, 4, 1.0 };
  float light1_direction[] = { -1.5, -2.0, -2.0, 1.0 };
  
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
  glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.8 );
  glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5 );
  glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);
  
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

// Light2 as background light
  float light2_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
  float light2_diffuse[] = { 2.0, 2.0, 2.0, 0.0 };
  float light2_specular[] = { 2.0, 2.0, 2.0, 0.0 };
  float light2_position[] = { 0, 4, -5, 1.0 };
  float light2_direction[] = { -1.5, -2.0, -2.0, 1.0 };
  
  glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
  glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
  glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.8 );
  glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.4 );
  glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.20);
  
  glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
  glEnable(GL_LIGHTING);
  
}

void set_shadow_uniform(int shader_id){
  if (shader_id == 0) return;
  int location = glGetUniformLocation(shader_id, "shadow_map0");
  glUniform1i(location, 7);
}

  unsigned int texid = 6;
  unsigned int framebuf[2];
  unsigned int framebuft[1], fbo_depth;
void build_shadowmap(){
  glGenTextures(1, framebuft);
  glGenFramebuffers(2, framebuf);
  glActiveTexture(GL_TEXTURE7);

  glBindTexture(GL_TEXTURE_2D, framebuft[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WIDTH, HEIGHT, 0, 
    GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  // glBindTexture(GL_TEXTURE_2D, 6);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuf[1]);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, 
                            GL_TEXTURE_2D, framebuft[0], 0);
  
  glBindTexture(GL_TEXTURE_2D, 0);

  GLenum FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
  if(FBOstatus != GL_FRAMEBUFFER_COMPLETE_EXT)
    printf("GL_FRAMEBUFFER_COMPLETE_EXT failed, CANNOT use FBO\n");

    // switch back to window-system-provided framebuffer
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

}
void save_matrix(GraphicCamera::GraphicCamera *cam) {
  glMatrixMode(GL_TEXTURE);
  glActiveTextureARB(GL_TEXTURE7);
  glEnable(GL_TEXTURE_2D);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -0.005);
  glScalef(0.5, 0.5, 0.5);
  glTranslatef(1.0,1.0,1.0);
  gluPerspective(60, (float)WIDTH/(float) HEIGHT, 0.1, 20.0);
  gluLookAt(light0_position[0], light0_position[1], light0_position[2],
    light0_position[0] + light0_direction[0], 
    light0_position[1] + light0_direction[1], 
    light0_position[2] + light0_direction[2], 0.0, 1.0, 0.0);
  // cam->PerspectiveDisplay(WIDTH, HEIGHT);
}
float edg = 5.0;
float surface[] = {
  -edg, 0, -edg/2,
  edg, 0, -edg,
  edg, 0, edg/2,
  -edg, 0, edg
};
void draw_stuff(){

  SwitchLight();
  glClearColor(0.4, 0.4, 0.4, 1.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  //=================do shadows=====================
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuf[1]);
  glUseProgram(0);
  camera = light0_cam;
  camera->PerspectiveDisplay(WIDTH, HEIGHT);
  // gluPerspective(60, 1024.0/768.0, 0.1, 20.0);
  // gluLookAt(light0_position[0], light0_position[1], light0_position[2],
  //   light0_position[0] + light0_direction[0], 
  //   light0_position[1] + light0_direction[1], 
  //   light0_position[2] + light0_direction[2], 0.0, 1.0, 0.0);
  model->DrawModel((int) DrawNormal, 0);
  // ground->DrawModel((int) DrawNormal, 0);
  // glBegin(GL_TRIANGLES);
  //   for (int i = 0; i < 3; ++i)
  //   {
  //     glVertex3fv(&surface[i * 3]);
  //   }
  // glEnd();
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  save_matrix(camera);
  //================================================
  do_lights();
  // selected_shader_id = 0;
  glUseProgram(selected_shader_id);
  set_shadow_uniform(selected_shader_id);
  glActiveTexture(GL_TEXTURE7);
  glBindTexture(GL_TEXTURE_2D, framebuft[0]);

  camera = eye_cam;
  camera->PerspectiveDisplay(WIDTH, HEIGHT);
  if (selected_shader_id) {
   // if not using fixed shading
   GLint light_switch_loc = glGetUniformLocation(
                         selected_shader_id, "LtSwitch");
   glUniform1i(light_switch_loc, LightSwitch); 
  }
  ground->DrawModel((int) DrawNormal, selected_shader_id);
  model->DrawModel((int) DrawNormal, selected_shader_id);
  // skydome->DrawModel((int) DrawNormal, selected_shader_id);
  printf("Using shader %d\n", selected_shader_id);
  glutSwapBuffers();

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
// set up shaders for using GLSL. This version only load one shader
void SetShadersOrDie(GLuint &shader, const char* vshader,
                     const char* fshader){
  printf("Loading Vertex Shader: %s\nLoading Fragment Shader: %s\n",
          vshader, fshader);
  //GLint vertCompiled, fragCompiled;
  char *vs, *fs, *bpfs;
  GLuint pv, pf, p;//, bpv, bpf, bp;
  
  pv = glCreateShader(GL_VERTEX_SHADER);
  pf = glCreateShader(GL_FRAGMENT_SHADER);
  vs = GraphicUtilities::GraphicUtilities::
        read_shader_program(vshader);
  fs = GraphicUtilities::GraphicUtilities::
        read_shader_program(fshader);
  bpfs = GraphicUtilities::GraphicUtilities::
        read_shader_program(fshader);
  // shader, # of string, array of string and array of tring length
  glShaderSource(pv, 1, (const char**)&vs, NULL);
  glShaderSource(pf, 1, (const char**)&fs, NULL);
  free(vs);
  free(fs);
  free(bpfs);
  glCompileShader(pv);
  assert(CompileSuccess(pv));
  glCompileShader(pf);
  assert(CompileSuccess(pf));
  p = glCreateProgram();
  glAttachShader(p, pf);
  glAttachShader(p, pv);
  glLinkProgram(p);
  assert(LinkSuccessful(p));
  shader = p;
  glUseProgram(p);
  selected_shader_id = p;
  cout<<"Finished Set up of Shaders: "<<p<<endl;
  return ;
}


void init(const char* model_path, const char* vshader_path, 
    const char* fshader_path) {
  eye_cam = new GraphicCamera::GraphicCamera(Vec3d(0, 2, 4),
                                            Vec3d(0, 1, 0),
                          Vec3d(0, 1 , 0), 0.02, 20, 60, focus);
  camera = eye_cam;
  camera->PerspectiveDisplay(WIDTH, HEIGHT);
  light0_cam = new GraphicCamera::GraphicCamera(
    Vec3d(light0_position[0], light0_position[1], light0_position[2]),
    Vec3d(light0_direction[0], light0_direction[1], light0_direction[2]),
    Vec3d(0, 1, 0), 0.02, 20, 60, focus
    );
    
  do_lights();
  build_shadowmap();
  SetShadersOrDie(selected_shader_id, vshader_path, fshader_path);
  model->InitModelData(selected_shader_id);
  ground->InitModelData(selected_shader_id);
  skydome->InitModelData(selected_shader_id);
  glEnable(GL_TEXTURE_2D);
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
    case 'b':
      if ((RENDER_MODE & (~GL_CONTROL_DEF::KDOF_MARKER)) != 0 )
        RENDER_MODE = 0;
      RENDER_MODE ^= GL_CONTROL_DEF::KDOF_MARKER;
      printf("Set DoF to %d \n", (RENDER_MODE & GL_CONTROL_DEF::KDOF_MARKER) > 0);
      WantToRedraw = true;
      break;
    case 'z':
      if(camera->focus() > 0.2) camera->focus() -= 0.03;
      WantToRedraw = true;
      break;
    case 'x':
      if(camera->focus() < 100.0) camera->focus() += 0.03;
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
      // do_material();
      break;
    case 'o':
      shininess -= 0.1f;
      // do_material();
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
//  printf("Shininess: %f\n", shininess);
  WantToRedraw = true;
}

/*
 On Redraw request, erase the window and redraw everything
 */
void RenderScene(){
  switch (RENDER_MODE) {
    case GL_CONTROL_DEF::KRM_AAONLY:
      cout<<AALevel<<"x AA"<<endl;
      camera->AAPerspectiveDisplay(WIDTH, HEIGHT, AALevel, draw_stuff);
      break;
    case GL_CONTROL_DEF::KRM_DOF_ONLY:
      cout<<"DoF at focus:"<<camera->focus()<<endl;
      glEnable(GL_MULTISAMPLE);
      camera->DoFPerspectiveDisplay(WIDTH, HEIGHT, 10, draw_stuff);
      break;
    default:
      camera->PerspectiveDisplay(WIDTH, HEIGHT);
      draw_stuff();
  }
  glFlush();
  glutSwapBuffers();
  WantToRedraw = false;
}

void Redraw(){
  if (WantToRedraw)
    RenderScene();
}

void mouseEventHandler(int button, int state, int x, int y) {
    // let the camera handle some specific mouse events (similar to maya)
      camera->MouseEventHandler(button, state, x, y);
}

void motionEventHandler(int x, int y) {
    // let the camera handle some mouse motions if the camera is to be moved
      camera->MouseMotionEventHandler(x, y);
      glutPostRedisplay();
}

/*
 Main program to draw the square, change colors, and wait for quit
 */
int main(int argc, char* argv[]){
  if(argc != 6){
    fprintf(stderr, "usage: show_bunny vertex_shader frag_shader model1.obj model2.obj sky.obj\n");
    exit(1);
  }
  glutInit(&argc, argv);

  // make GLUT select a double buffered display that uses RGBA colors
  // Julian: Add GLUT_DEPTH when in 3D program so that 3D objects drawed
  // correctly regardless the order they draw
  glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH |
                       GLUT_DOUBLE |GLUT_ACCUM );
  glutInitWindowSize(WIDTH, HEIGHT);
  //  glutInitWindowPosition(50, 50);
  glutCreateWindow("Golden Bunny");
  
  model = new GraphicModel(0, false);
  ground = new GraphicModel(2, false);
  skydome = new GraphicModel(4, true);
  model->LoadObject(argv[3]);
  ground->LoadObject(argv[4]);
  skydome->LoadObject(argv[5]);

  // initialize the camera and such
  init(argv[3], argv[1], argv[2]);
  //glutReshapeFunc(doReshape);
  glutDisplayFunc(RenderScene);
  glutMouseFunc(mouseEventHandler);
  glutMotionFunc(motionEventHandler);
  glutKeyboardFunc(KeyBoardHandler);
  glutIdleFunc(Redraw);
  glutMainLoop();
  // Routine that loops forever looking for events. It calls the registered
  // callback routine to handle each event that is detected
  return 0;
}

#endif
