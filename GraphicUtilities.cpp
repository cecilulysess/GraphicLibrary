//
//  GraphicUtilities.cpp
//  Graphic
//
//  Created by Julian Wu on 1/31/13.
//  Copyright (c) 2013 Julian Wu. All rights reserved.
//
#include "GLCommonHeader.h"
#include "GraphicUtilities.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

float frustum[6][4];

void ExtractFrustum()
{
  float proj[16];
  float modl[16];
  float clip[16];
  float t;
  /* Get the current PROJECTION matrix from OpenGL */
  glGetFloatv( GL_PROJECTION_MATRIX, proj );
  /* Get the current MODELVIEW matrix from OpenGL */
  glGetFloatv( GL_MODELVIEW_MATRIX, modl );
  /* Combine the two matrices (multiply projection by modelview)    */
  clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] +    modl[ 3] * proj[12];
  clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] +    modl[ 3] * proj[13];
  clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] +    modl[ 3] * proj[14];
  clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] +    modl[ 3] * proj[15];
  clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4]    + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
  clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] +    modl[ 7] * proj[13];
  clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] +    modl[ 7] * proj[14];
  clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] +    modl[ 7] * proj[15];
  clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4]    + modl[10] * proj[ 8] + modl[11] * proj[12];
  clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] +    modl[11] * proj[13];
  clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] +    modl[11] * proj[14];
  clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] +    modl[11] * proj[15];
  clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4]    + modl[14] * proj[ 8] + modl[15] * proj[12];
  clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] +    modl[15] * proj[13];
  clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] +    modl[15] * proj[14];
  clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] +    modl[15] * proj[15];
  /* Extract the numbers for the RIGHT plane */
  frustum[0][0] = clip[ 3] - clip[ 0];
  frustum[0][1] = clip[ 7] - clip[ 4];
  frustum[0][2] = clip[11] - clip[ 8];
  frustum[0][3] = clip[15] - clip[12];
  /* Normalize the result */
  t = sqrt( frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] + frustum[0][2]    * frustum[0][2] );
  frustum[0][0] /= t;
  frustum[0][1] /= t;
  frustum[0][2] /= t;
  frustum[0][3] /= t;
  /* Extract the numbers for the LEFT plane */
  frustum[1][0] = clip[ 3] + clip[ 0];
  frustum[1][1] = clip[ 7] + clip[ 4];
  frustum[1][2] = clip[11] + clip[ 8];
  frustum[1][3] = clip[15] + clip[12];
  /* Normalize the result */
  t = sqrt( frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] + frustum[1][2]    * frustum[1][2] );
  frustum[1][0] /= t;
  frustum[1][1] /= t;
  frustum[1][2] /= t;
  frustum[1][3] /= t;
  /* Extract the BOTTOM plane */
  frustum[2][0] = clip[ 3] + clip[ 1];
  frustum[2][1] = clip[ 7] + clip[ 5];
  frustum[2][2] = clip[11] + clip[ 9];
  frustum[2][3] = clip[15] + clip[13];
  /* Normalize the result */
  t = sqrt( frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] + frustum[2][2]    * frustum[2][2] );
  frustum[2][0] /= t;
  frustum[2][1] /= t;
  frustum[2][2] /= t;
  frustum[2][3] /= t;
  /* Extract the TOP plane */
  frustum[3][0] = clip[ 3] - clip[ 1];
  frustum[3][1] = clip[ 7] - clip[ 5];
  frustum[3][2] = clip[11] - clip[ 9];
  frustum[3][3] = clip[15] - clip[13];
  /* Normalize the result */
  t = sqrt( frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] + frustum[3][2]    * frustum[3][2] );
  frustum[3][0] /= t;
  frustum[3][1] /= t;
  frustum[3][2] /= t;
  frustum[3][3] /= t;
  /* Extract the FAR plane */
  frustum[4][0] = clip[ 3] - clip[ 2];
  frustum[4][1] = clip[ 7] - clip[ 6];
  frustum[4][2] = clip[11] - clip[10];
  frustum[4][3] = clip[15] - clip[14];
  /* Normalize the result */
  t = sqrt( frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] + frustum[4][2]    * frustum[4][2] );
  frustum[4][0] /= t;
  frustum[4][1] /= t;
  frustum[4][2] /= t;
  frustum[4][3] /= t;
  /* Extract the NEAR plane */
  frustum[5][0] = clip[ 3] + clip[ 2];
  frustum[5][1] = clip[ 7] + clip[ 6];
  frustum[5][2] = clip[11] + clip[10];
  frustum[5][3] = clip[15] + clip[14];
  /* Normalize the result */
  t = sqrt( frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] + frustum[5][2]    * frustum[5][2] );
  frustum[5][0] /= t;
  frustum[5][1] /= t;
  frustum[5][2] /= t;
  frustum[5][3] /= t;
}


char* GraphicUtilities::read_shader_program(char *filename){
  FILE *fp;
  char *content = NULL;
  int fd, count;
  fd = open(filename, O_RDONLY);
  count = lseek(fd, 0, SEEK_END);
  close(fd);
  content = (char *) calloc(1, (count + 1));
  fp = fopen(filename, "r");
  count = fread(content, sizeof(char), count, fp);
  content[count] = '\0';
  fclose(fp);
  return content;
}

float jitterTable[][2] = {
  {0.5625, 0.4375},
  {0.0625, 0.9375},
  {0.3125, 0.6875},
  {0.6875, 0.8124},
  {0.8125, 0.1875},
  {0.9375, 0.5625},
  {0.4375, 0.0625},
  {0.1875, 0.3125}
};

void GraphicUtilities::AntiAlias(int level, render_callback render_frame){
  glClearAccum(0, 0, 0, 0);
  glClear(GL_ACCUM_BUFFER_BIT);
  ExtractFrustum();
  for (int i = 0 ; i < level; ++i) {
    jitterCamera(jitterTable[i][0], jitterTable[i][1]);
    render_frame();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glAccum(GL_ACCUM, 1.0/(float)level);
  }
  glAccum(GL_RETURN, 1.0);
}

// jitter a camera by pixels in x and y
void GraphicUtilities::jitterCamera(GLfloat pix_x, GLfloat pix_y) {
  ExtractFrustum();
  GLint viewport_params[4];
  // from OpenGL doc: When used with non-indexed variants of glGet
  //  (such as glGetIntegerv), params returns four values: the x
  //  and y window coordinates of the viewport, followed by its
  //  width and height. Initially the  x and y window coordinates
  //  are both set to 0, and the width and height are set to the
  //  width and height of the window into which the GL will do its rendering
  glGetIntegerv(GL_VIEWPORT, viewport_params);
  GLfloat wndWidth=viewport_params[2];
  GLfloat WndHeight=viewport_params[3];
  GLfloat frustumWidth=0.5;
  GLfloat frustumHeight=0.5;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  GLfloat dx, dy;
  dx = pix_x*frustumWidth/wndWidth;
  dy = pix_y*frustumHeight/WndHeight;
  printf("world delta = %f %f\n",dx,dy);
  glFrustum(frustum[1][0] + dx, frustum[0][0] - dx, frustum[2][0] + dy,frustum[3][0]  + dy, 0.1, 20.0 );
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}