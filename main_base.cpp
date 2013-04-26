
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
#include "GraphicCamera.h"
#include "GraphicModel.h"

#define WIDTH 1024
#define HEIGHT 768

unsigned int RENDER_MODE = 0;
int light_switch[3] = {1, 1, 1};
int AALevel = 4;
bool WantToRedraw = false, DrawNormal = false;
double focus = 1.2;

GraphicModel *model, *ground, *skydome;
GraphicCamera::GraphicCamera *camera;

GLuint selected_shader_id = 0;

float eye_loc[] = {0.0, 2.0, 4.0}, eye_aim[] = {0.0, 1.0, 0.0};


typedef struct LightSource{
  float diffuse[4];
  float specular[4];
  float position[4];
  float const_atten, linear_atten, quadra_atten; 
} LightSource;

LightSource lights[3] = {
  { 1, 1, 1, 0,
    1, 1, 1, 0,
    3, 4, 3, 1.0,
    0, 0.2, 0.01   },
  { 1, 1, 1, 0,
    1, 1, 1, 0,
    -3, 2.5, 4, 1.0,
    0.8, 0.5, 0.2   },
  { 2, 2, 2, 0,
    2, 2, 2, 0,
    0, 4, -5, 1.0,
    0.8, 0.4, 0.2    }
};


void SwitchLight(){
  for (int i = 0; i < 3; ++i)   {
    if (light_switch[i])
      glEnable(GL_LIGHT0 + i);
    else
      glDisable(GL_LIGHT0 + i);
  }
}

void do_lights(){
  for (int i = 0; i < 3; ++i) {
    glLightfv(GL_LIGHT0, GL_POSITION, lights[i].position);
    glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, lights[i].diffuse);
    glLightfv(GL_LIGHT0 + i, GL_SPECULAR, lights[i].specular);
    glLightf(GL_LIGHT0 + i, GL_CONSTANT_ATTENUATION, lights[i].const_atten);
    glLightf(GL_LIGHT0 + i, GL_LINEAR_ATTENUATION, lights[i].linear_atten);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, lights[i].quadra_atten);
  }
  glEnable(GL_LIGHTING);
}

void draw_stuff(){
  SwitchLight();
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.4, 0.4, 0.4, 0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  do_lights();
  glUseProgram(selected_shader_id);
  if (selected_shader_id) {
   glUniform1iv(glGetUniformLocation(
                    selected_shader_id, "LtSwitch"), 3, light_switch); 
  }
  model->DrawModel((int) DrawNormal, selected_shader_id);
  ground->DrawModel((int) DrawNormal, selected_shader_id);
  skydome->DrawModel((int) DrawNormal, selected_shader_id);

}

void PerspectiveDisplay(int width, int height) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  gluPerspective(60, (float) width/ (float) height,
                 0.01, 30.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(eye_loc[0], eye_loc[1], eye_loc[2],
            eye_aim[0], eye_aim[1], eye_aim[2],
            0.0, 1.0, 0.0);
}

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
  glShaderSource(pv, 1, (const char**)&vs, NULL);
  glShaderSource(pf, 1, (const char**)&fs, NULL);
  free(vs);
  free(fs);
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

  float updef[3] = {0.0, 1.0, 0.0};
  try{
    camera = new GraphicCamera::GraphicCamera(eye_loc, eye_aim, updef, focus);
  } catch (...){
    fprintf(stderr, "error init cam: pos: %f %f %f, aim: %f %f %f\n",
            eye_loc[0], eye_loc[1], eye_loc[2], eye_aim[0], eye_aim[1], eye_aim[2] );
  }  
  camera->PerspectiveDisplay(WIDTH, HEIGHT);
  do_lights();
  SetShadersOrDie(selected_shader_id, vshader_path, fshader_path);
  model->InitModelData(selected_shader_id);
  ground->InitModelData(selected_shader_id);
  skydome->InitModelData(selected_shader_id);
}

void KeyBoardHandler(unsigned char key, int x, int y){
  switch (key) {
    case 'q':
      exit(1);
      break;
    case 'a':
      if ((RENDER_MODE & (~GL_CONTROL_DEF::KAA_MARKER)) != 0 )
        RENDER_MODE = 0;
      RENDER_MODE ^= GL_CONTROL_DEF::KAA_MARKER;
      printf("Set AA to %d \n", (RENDER_MODE & GL_CONTROL_DEF::KAA_MARKER) > 0);
      break;
    case 'w':
      AALevel = (AALevel + 4)%16;
      if (AALevel == 0) {
        AALevel = 16;
      }
      break;
    case 'b':
      if ((RENDER_MODE & (~GL_CONTROL_DEF::KDOF_MARKER)) != 0 )
        RENDER_MODE = 0;
      RENDER_MODE ^= GL_CONTROL_DEF::KDOF_MARKER;
      printf("Set DoF to %d \n", (RENDER_MODE & GL_CONTROL_DEF::KDOF_MARKER) > 0);
      break;
    case 'z':
      if(camera->focus() > 0.2) camera->focus() -= 0.03;
      break;
    case 'x':
      if(camera->focus() < 100.0) camera->focus() += 0.03;
      break;
    case 'n':
      DrawNormal = !DrawNormal;
      break;
    case '1':
    case '2':
    case '3':
      light_switch[key - '1'] = !light_switch[key - '1'];
      break;
    default:
      return;
  }
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
      PerspectiveDisplay(WIDTH, HEIGHT);
      draw_stuff();
  }
  glFlush();
  glutSwapBuffers();
  WantToRedraw = false;
}

void Redraw(){
  if (WantToRedraw)  RenderScene();
}


int main(int argc, char* argv[]){
  if(argc != 6){
    fprintf(stderr, "usage: show_bunny vertex_shader frag_shader model1.obj model2.obj sky.obj\n");
    exit(1);
  }
  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH |
                       GLUT_DOUBLE |GLUT_ACCUM );
  glutInitWindowSize(WIDTH, HEIGHT);
  //  glutInitWindowPosition(50, 50);
  glutCreateWindow("Porcelain teapot with cracks");
  
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
  glutKeyboardFunc(KeyBoardHandler);
  glutIdleFunc(Redraw);
  glutMainLoop();
  // Routine that loops forever looking for events. It calls the registered
  // callback routine to handle each event that is detected
  return 0;
}
