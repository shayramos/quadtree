#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#if defined (__APPLE__) || defined(MACOSX)
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#include "EstruturasDeDados.h"
#include "winGL.h"
#include "quadtree.h"

#define SENTINELA -1;

unsigned char* 	imageGray = NULL;	// image file

int iHeight, 	// altura é o número de linhas
iWidth;		 		// largura é o número de colunas

tNoArvore* raiz = NULL;

bool desenha = false;

// ***********************************************
// ******                                   ******
// ***********************************************
// int inc = 0;
void buscaNivel(tNoArvore** no, int cont){
	if(*no == NULL)
		return;

	if((*no)->q->nivel < cont){
		int cor;
		tPonto p0, p1;
		p0.x = (*no)->filho[0]->q->pBase.x;
		p0.y = (*no)->filho[0]->q->pBase.y;
		p1.x = p0.x + (*no)->filho[0]->q->w;
		p1.y = p0.y + (*no)->filho[0]->q->h;
		cor = (*no)->filho[0]->q->cor;
		desenhaQuadrante(p0, p1, cor);  // desenha um quadrante no canto superior esquerdo

		p0.x = (*no)->filho[1]->q->pBase.x;
		p0.y = (*no)->filho[1]->q->pBase.y;
		p1.x = p0.x + (*no)->filho[1]->q->w;
		p1.y = p0.y + (*no)->filho[1]->q->h;
		cor = (*no)->filho[1]->q->cor;
		desenhaQuadrante(p0, p1, cor);  // desenha um quadrante no canto superior direito

		p0.x = (*no)->filho[2]->q->pBase.x;
		p0.y = (*no)->filho[2]->q->pBase.y;
		p1.x = p0.x + (*no)->filho[2]->q->w;
		p1.y = p0.y + (*no)->filho[2]->q->h;
		cor = (*no)->filho[2]->q->cor;
		desenhaQuadrante(p0, p1, cor);  // desenha um quadrante no canto inferior direito

		p0.x = (*no)->filho[3]->q->pBase.x;
		p0.y = (*no)->filho[3]->q->pBase.y;
		p1.x = p0.x + (*no)->filho[3]->q->w;
		p1.y = p0.y + (*no)->filho[3]->q->h;
		cor = (*no)->filho[3]->q->cor;
		desenhaQuadrante(p0, p1, cor);  // desenha um quadrante no canto inferior esquerdo

		for(int i = 0; i<4; i++){
			buscaNivel(&(*no)->filho[i], cont);
		}
	}else{return;}

}
int cont = 1;
void desenhaQuadtree(tNoArvore** no) {

	printf("Aqui eu vou desenhar a quadtree\n");

	buscaNivel(no, cont);
	cont++;
	if(cont == 9){
		cont = 1;
	}
	// rotina que deve ser implementada para visualizacao da quadtree
	// utilize a rotina desenhaQuadrante(p0, p1, cor)
	// fornecendo os pontos inicial e final do quadrante e a sua cor
	// funcao do valor do pixel ou da regiao que voce quer desenhar
}

/// ***********************************************************************
/// **
/// ***********************************************************************

tNoArvore* createNode(int color) {

	tNoArvore* new = (tNoArvore*)malloc(sizeof(tNoArvore));

	if (new != NULL) {

		new->q = (tQuadrante*) malloc(sizeof(tQuadrante));
		if (new->q != NULL) {
			new->q->h = SENTINELA;
			new->q->w = SENTINELA;
			new->q->nivel = SENTINELA;
			new->q->pBase.x = SENTINELA;
			new->q->pBase.y = SENTINELA;
			new->q->cor = color;
			new->q->erro = SENTINELA;
		}
		new->filho[0] = NULL;
		new->filho[1] = NULL;
		new->filho[2] = NULL;
		new->filho[3] = NULL;
		}

	return new;
}

/// ***********************************************************************
/// **
/// ***********************************************************************

// Calcula a media apenas da raiz
int calcMedia(){

	float cor_med = 0;

	for (int i = 0; i < iWidth; i++){
		for (int j = 0; j < iHeight; j++){
			cor_med += imageGray[(j*iHeight)+i];
			// cor_med += imageGray[(i*iHeight)+j];
		}
	}
	cor_med = cor_med/(iWidth*iHeight);
	// printf("%d\n", cor_med);

	return (int)cor_med;
}

/// ***********************************************************************
/// **
/// ***********************************************************************

// Calcula o erro apenas da raiz
float calcErro(){

	float erro = 0;
	int cor_med = calcMedia();

	for (int i = 0; i < iWidth; i++){
		for (int j = 0; j < iHeight; j++){
			erro += (abs(imageGray[(j*iHeight)+i] - cor_med)/imageGray[(j*iHeight)+i])*100;
		}
	}
	erro = erro/(iWidth*iHeight);
	// printf("%f\n", erro);

	return erro;
}

/// ***********************************************************************
/// **
/// ***********************************************************************

// Preenche a raiz
void initQuadtree(tNoArvore** raiz) {
		*raiz = createNode(calcMedia());
		// if(*raiz == NULL) {

			(*raiz)->q->h = iWidth;
			(*raiz)->q->w = iHeight;
			(*raiz)->q->nivel = 0;
			(*raiz)->q->pBase.x = 0;
			(*raiz)->q->pBase.y = 0;
			(*raiz)->q->cor = calcMedia();
			(*raiz)->q->erro = calcErro();
			(*raiz)->filho[0] = NULL;
			(*raiz)->filho[1] = NULL;
			(*raiz)->filho[2] = NULL;
			(*raiz)->filho[3] = NULL;
			// }
}

/// ***********************************************************************
/// **
/// ***********************************************************************
// ordem dos quadrantes
//*************
//	0	 *	1	 //
//	3	 *	2	 //
//*************

void insereNodo(tNoArvore** no, int Indfilho){
	int cor_med=0;
	float ERRO=0;

	tPonto Pbase;
	int nivel = (*no)->q->nivel + 1;

	switch (Indfilho) {
		case 0:
		Pbase.x = (*no)->q->pBase.x;
		Pbase.y = (*no)->q->pBase.y + (((*no)->q->h)/2);
		break;
		case 1:
		Pbase.x = (*no)->q->pBase.x + (((*no)->q->w)/2);
		Pbase.y = (*no)->q->pBase.y + (((*no)->q->h)/2);
		break;
		case 2:
		Pbase.x = (*no)->q->pBase.x + (((*no)->q->w)/2);
		Pbase.y = (*no)->q->pBase.y;
		break;
		case 3:
		Pbase.x = (*no)->q->pBase.x;
		Pbase.y = (*no)->q->pBase.y;
		break;
	}

	//calcular a média

	for (int i = Pbase.x; i < Pbase.x + (((*no)->q->w)/2); i++){
		for (int j = Pbase.y; j < Pbase.y + (((*no)->q->h)/2); j++){
			cor_med += imageGray[(j*iHeight)+i];
		}
	}
	cor_med = (cor_med/((iWidth/pow(2,nivel))*(iHeight/pow(2,nivel))));
	// printf("Cor Media: %d\n", (int)cor_med);

	//calcular o erro
	for (int i = Pbase.x; i < Pbase.x + (((*no)->q->w)/2); i++){
		for (int j = Pbase.y; j < Pbase.y + (((*no)->q->h)/2); j++){
				float numerador = abs(imageGray[(j*iHeight)+i] - cor_med);
				float  denominador = imageGray[(j*iHeight)+i];
				ERRO += (numerador/denominador)*100;
		}
	}
	ERRO = ERRO/((iWidth/pow(2,nivel))*(iHeight/pow(2,nivel)));
	// printf("Erro: %.2f ", ERRO);

	(*no)->filho[Indfilho] = createNode((int)cor_med);
	(*no)->filho[Indfilho]->q->nivel = nivel;
	(*no)->filho[Indfilho]->q->pBase.x = Pbase.x;
	(*no)->filho[Indfilho]->q->pBase.y = Pbase.y;
	(*no)->filho[Indfilho]->q->w = iWidth/pow(2,nivel);
	(*no)->filho[Indfilho]->q->h = iHeight/pow(2,nivel);
	(*no)->filho[Indfilho]->q->erro = ERRO;
}

void montaQuadtree(tNoArvore** no){
	// printf("Aqui eu vou montar a quadtree\n");

	if(*no == NULL)
		return;

	if((*no)->q->w > 1){
		insereNodo(&(*no), 0);

		insereNodo(&(*no), 1);

		insereNodo(&(*no), 2);

		insereNodo(&(*no), 3);

		montaQuadtree(&((*no)->filho[0]));
		montaQuadtree(&((*no)->filho[1]));
		montaQuadtree(&((*no)->filho[2]));
		montaQuadtree(&((*no)->filho[3]));
	}else{return;}

//-------DESCOMENTE AQUI PARA VISUALIZAR AS INFORMAÇÕES DOS NÓS

	// for(int i=0; i<4; i++){
	// 	printf("---------------\n");
	// 	printf("Ponto Base_filho[%d]: (%d,%d)\n",i,(*no)->filho[i]->q->pBase.x,
	// 	(*no)->filho[i]->q->pBase.y);
	// 	printf("Nível: %d\n", (*no)->filho[i]->q->nivel);
	// 	printf("Cor Media: %d\n", (*no)->filho[i]->q->cor);
	// 	printf("Erro: %.2f\n", (*no)->filho[i]->q->erro);
	// 	printf("Width: %d\nHeight: %d\n", (*no)->filho[i]->q->w, (*no)->filho[i]->q->w);
	// 	printf("---------------\n");
	// }

//-------DESCOMENTE AQUI PARA VISUALIZAR AS INFORMAÇÕES DOS NÓS
}

/// ***********************************************************************
/// **
/// ***********************************************************************

void teclado(unsigned char key, int x, int y) {

	bool quad;
	switch (key) {
		case 27		: 	exit(0);
			break;
		case 'q'	:
		case 'Q'	: 	montaQuadtree(&raiz);
			break;
		case 'i'	:
		case 'I'	: 	desenha = !desenha;
			break;
		}
	glutPostRedisplay();
}

/// ***********************************************************************
/// **
/// ***********************************************************************

void desenho(void) {

    glClear (GL_COLOR_BUFFER_BIT);

    glColor3f (1.0, 1.0, 1.0);

    if (desenha)
    	desenhaQuadtree(&raiz);
    else
    	glDrawPixels(iWidth, iHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, imageGray);

    glutSwapBuffers ();
}

/// ***********************************************************************
/// **
/// ***********************************************************************

int main(int argc, char** argv) {

char* filename = "IMGs/lena.png";

  if (argc > 1){
		filename = argv[1];
	}
	imageGray = leImagem(filename);

	initQuadtree(&raiz);	//cria a raiz
	if(raiz != NULL){
	}else{
		printf("raiz nula\n");
	}
	criaJanela(argc, argv);

	initOpenGL();

	initEventos();

	return 0;
}
