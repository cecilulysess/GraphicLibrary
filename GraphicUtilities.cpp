//
//  GraphicUtilities.cpp
//  Graphic
//
//  Copyright (c) 2013 Julian Wu. All rights reserved.
//
//  Author: cosmobserver@gmail.com (Yanxiang Wu <Julian>)
//
//  Created on 1/31/13
//
#include "GLCommonHeader.h"
#include "GraphicUtilities.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

namespace GraphicUtilities{

char* GraphicUtilities::read_shader_program(const char *filename){
  FILE *fp;
  char *content = NULL;
  int fd, count;
  fd = open(filename, O_RDONLY);
  count = (int) lseek(fd, 0, SEEK_END);
  close(fd);
  content = (char *) calloc(1, (count + 1));
  fp = fopen(filename, "r");
  count = (int) fread(content, sizeof(char), count, fp);
  content[count] = '\0';
  fclose(fp);
  return content;
}

bool GraphicUtilities::LoadTexture(char *filename, GLuint &texture){
  FILE *fptr;
  char buf[512], *parse;
  int im_size, im_width, im_height, max_color;
  unsigned char *texture_bytes;

  fptr = fopen(filename, "r");
  if (fptr == NULL) {
    perror("Reading material file with error: ");
    perror(filename);
  }
  fgets(buf, 512, fptr);

  do {
    fgets(buf, 512, fptr);
  } while(buf[0] == '#' || buf[0] == '\n');
  sscanf(buf, "%d %d", &im_width, &im_height);
  fgets(buf, 512, fptr);
  im_size = im_width * im_height;
  texture_bytes = (unsigned char *) calloc(3, im_size);
  fread(texture_bytes, 3, im_size, fptr);
  fclose(fptr);
  
  glBindTexture(GL_TEXTURE_2D, texture /*SetCurrentTexture id*/);
  glTexImage2D(GL_TEXTURE_2D, 
    0, // Mipmap level
    GL_RGB, im_width, im_height, 0/* border size */, GL_RGB,
    GL_UNSIGNED_BYTE, texture_bytes);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  free(texture_bytes);
  printf("Loaded texture: %s\n", filename);
  return true;
}

void GLShortCut::PrintGLErrors(char *file, int line){
  GLenum glErr;
  glErr = glGetError();
//  fprintf(stderr, "Detecting Error...\n");
  if (glErr != GL_NO_ERROR) {
    fprintf(stderr, "glError in file %s @ line %d: %s\n", 
            file, line, gluErrorString(glErr));
  }
}

  
}//GraphicUtilities
