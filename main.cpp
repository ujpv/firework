#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <GL/glut.h>
#include "fireworks_2d_opengl.h"

Fireworks_2D_OpenGL fireWork;

void renderScene() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  fireWork.draw();
  glutSwapBuffers();
}

void idle() {
  fireWork.proceed();
  glutPostRedisplay();
  usleep(30000);
}

int main(int argc, char **argv) {

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100,100);
  glutInitWindowSize(800,800);
  glutCreateWindow("Firework");

  glutIdleFunc(idle);
  glutDisplayFunc(renderScene);

  glutMainLoop();

  return 0;
}
