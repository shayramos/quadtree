#ifndef __QUADTREE__
#define __QUADTREE__ 1

void desenhaQuadtree();

tNoArvore* createNode(int color);

int calcMedia();

float calcErro();

void initQuadtree();

void montaQuadtree(tNoArvore** no);

void teclado(unsigned char key, int x, int y);

void desenho(void);

void mouse(int button, int button_state, int x, int y );


#endif	// __QUADTREE__
