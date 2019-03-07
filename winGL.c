#include <stdio.h>
#include <stdlib.h>

#if defined (__APPLE__) || defined(MACOSX)
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#include "winGL.h"
#include "SOIL/SOIL.h"
#include "quadtree.h"

extern int 	iHeight, 
			iWidth;
						
/// ***********************************************************************
/// ** 
/// ***********************************************************************

unsigned char* leImagem(char* filename) {
	
	unsigned char* 	image 		= NULL;
	int channels;
	int i, j;
	unsigned char aux;
	int force_channels = SOIL_LOAD_L;
	
	/* load an image file */
	image = SOIL_load_image(filename, &iWidth, &iHeight, &channels, force_channels);
	
	/* check for an error during the load process */
	if( image == NULL ) {
		printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
		printf("Erro carregando imagem %s\n", filename);
		exit(0);
	}

	printf("Imagem => %s\n", filename);
	printf("Resolucao => %d x %d\n", iHeight, iWidth);
	
	// vira a imagem de cabeça para baixo
	for(i=0 ; i < iWidth ; i++) 
		for(j=0 ; j < iHeight / 2.0 ; j++) {
			aux								= image[i+iWidth*j];  
			image[i+iWidth*j] 				= image[i+iWidth*(iHeight-1-j)];  
			image[i+iWidth*(iHeight-1-j)] 	= aux;  
		}
	
	return image;	
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

						
/// ***********************************************************************
/// ** 
/// ***********************************************************************

void desenhaQuadrante(tPonto p0, tPonto p1, unsigned char c) {
		
	glColor3ub(c, c, c);
    glBegin(GL_QUADS);
		glVertex2f(p0.x, p0.y);
		glVertex2f(p1.x, p0.y);
		glVertex2f(p1.x, p1.y);
		glVertex2f(p0.x, p1.y);
    glEnd(); 

}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void initOpenGL (void) {
	glClearColor(0.0, 0.0, 0.0, 0.0); 
	glShadeModel(GL_FLAT);
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void reshape (int w, int h){

   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0, w, 0.0, h);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void criaJanela(int argc, char** argv) {
	
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (iWidth, iHeight);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Quadtree - Estrutura de Dados");
}


/// ***********************************************************************
/// ** 
/// ***********************************************************************

void initEventos() {
	
    glutDisplayFunc(desenho);
    glutKeyboardFunc(teclado);
	glutReshapeFunc(reshape);
	
	glutMainLoop();

}


