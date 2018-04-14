/*
    Autores: Anderson Guerra, Juliana Leal
    Versão: 0.2

    * O tanque já faz a movimentação básica necessária e também
    consegue atirar;
    * A rotação do tanque e do canhão são independentes entre si;

*/


// necessário pegar o SOIL

#include <gl/glut.h>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <SOIL.h>

using namespace std;

GLfloat xf, yf, win;
GLfloat r, g, b, velocidade=10;
GLint view_w, view_h;
float rotat = 0;
float rotatC = 0;
float posX = 0, posY = 0, posZ = 0;
float posXt = 0, posYt = 0, posZt = 0;
int movimento = false;


static GLuint LoadPNG(char* filename){
    GLuint texture = SOIL_load_OGL_texture
    (
        filename,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    if( 0 == texture )
    {
        printf( "erro do SOIL: '%s'\n", SOIL_last_result() );
    }

    glBindTexture (GL_TEXTURE_2D, texture);

    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glColor3f (1.0,1.0,1.0);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_REPEAT);

    return texture;
}

void DesenhaQuadrado(int fator){
    glEnable(GL_TEXTURE_2D);
    GLuint tex = LoadPNG("C:\\Users\\andgu\\dev\\tanque\\tanque.jpg");
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);glVertex2f(0+fator, 0+fator);
        glTexCoord2f(1.0, 0.0);glVertex2f(xf+fator, 0+fator);
        glTexCoord2f(1.0, 1.0);glVertex2f(xf+fator, yf+fator);
        glTexCoord2f(0.0, 1.0);glVertex2f(0+fator, yf+fator);
     glEnd();
    glDisable(GL_TEXTURE_2D);
}

void DesenhaCanhao(int fator){

    glEnable(GL_TEXTURE_2D);
    GLuint tex = LoadPNG("C:\\Users\\andgu\\dev\\tanque\\canhao.jpg");
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);glVertex2f(-5, -25);
        glTexCoord2f(1.0, 0.0);glVertex2f(5, -25);
        glTexCoord2f(1.0, 1.0);glVertex2f(5, 50);
        glTexCoord2f(0.0, 1.0);glVertex2f(-5, 50);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void DesenhaBala(int fator){
    glBegin(GL_POLYGON);
       glVertex2f(0+fator, 0+fator);
       glVertex2f(10+fator, 0+fator);
       glVertex2f(10+fator, 10+fator);
       glVertex2f(0+fator, 10+fator);
     glEnd();
}


void imprime(int x, int y,int z, char *string)
{
    glRasterPos2f(x,y);

    int len = (int) strlen(string);

    for (int i = 0; i < len; i++)    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,string[i]);
    }
}


void Desenha(void)
{

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(r,g,b);

    //imprime(90, 10, 0, "F1 - Zoom");
    //imprime(90, -30, 0, "-> - Move");
    //imprime(90, -70, 0, "Clique - MAGIC!!!");

    glPushMatrix();
    glTranslatef(posXt,posYt,posZt);
    glRotatef(rotatC,0.0,0.0,1.0);
    DesenhaBala(-5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(posX,posY,posZ);
    glRotatef(rotat,0.0,0.0,1.0);
    DesenhaQuadrado(-25);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(posX,posY,posZ);
    glRotatef(rotatC,0.0,0.0,1.0);
    DesenhaCanhao(-25);
    glPopMatrix();

    glFlush();

}


void Inicializa (void)
{
    glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    r = 0.0f;
    g = 0.0f;
    b = 1.0f;
    xf=50.0f;
    yf=50.0f;
    win=250.0f;
}

void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
    view_w = w;
    view_h = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D (-win, win, -win, win);
}

void MenuCor(int op)
{
   switch(op) {
    case 0:
        r = 1.0f;
        g = 0.0f;
        b = 0.0f;
        break;
    case 1:
        r = 0.0f;
        g = 1.0f;
        b = 0.0f;
        break;
    case 2:
        r = 0.0f;
        g = 0.0f;
        b = 1.0f;
        break;
}
    glutPostRedisplay();
}

void MenuPrincipal(int op){}

void CriaMenu()
{
    int menu,submenu1;

    submenu1 = glutCreateMenu(MenuCor);
    glutAddMenuEntry("Vermelho",0);
    glutAddMenuEntry("Verde",1);
    glutAddMenuEntry("Azul",2);

    menu = glutCreateMenu(MenuPrincipal);
    glutAddSubMenu("Cor",submenu1);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void GerenciaMouse(int button, int state, int x, int y)
{
    if (button == GLUT_RIGHT_BUTTON)
         if (state == GLUT_DOWN)
            CriaMenu();

    glutPostRedisplay();
}


void TeclasEspeciais(int key, int x, int y)
{
    if(key == GLUT_KEY_F1) {
           win -= 20;
           glMatrixMode(GL_PROJECTION);
           glLoadIdentity();
           gluOrtho2D (-win, win, -win, win);
    }
    if(key == GLUT_KEY_UP) {
        posX -= 10 * sin(rotat * 3.14159 / 180);
        posY += 10 * cos(rotat * 3.14159 / 180);
        posXt = posX;
        posYt = posY;
        movimento = false;
    }
    if(key == GLUT_KEY_DOWN) {
        posX += 10 * sin(rotat * 3.14159 / 180);
        posY -= 10 * cos(rotat * 3.14159 / 180);
        posXt = posX;
        posYt = posY;
        movimento = false;
        //printf("%f", posX);
    }
    if(key == GLUT_KEY_RIGHT){
        rotat-=10;

    }
    if(key == GLUT_KEY_LEFT){
        rotat+=10;
    }

    glutPostRedisplay();
}

void ControlaTeclado(unsigned char key, int x, int y){
    switch(key){
    case 'a':
    case 'A':
        rotatC+=10;
        break;
    case 'd':
    case 'D':
        rotatC-=10;
        break;
    case 's':
    case 'S':
        posXt = posX;
        posYt = posY;
        if(movimento){
            movimento = false;
        } else {
            movimento = true;
        }
        break;
    }

    glutPostRedisplay();
}

void Movimento(int value){

    if (movimento){
        posXt -= 10 * sin(rotatC * 3.14159 / 180);
        posYt += 10 * cos(rotatC * 3.14159 / 180);
    }

    glutPostRedisplay();
    glutTimerFunc(velocidade,Movimento, 1);
}


int main(int argc, char *argv[])
{
     glutInit(&argc, argv);
     glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
     glutInitWindowSize(350,300);
     glutInitWindowPosition(10,10);
     glutCreateWindow("Exemplo de Movimento");
     glutDisplayFunc(Desenha);
     glutReshapeFunc(AlteraTamanhoJanela);
     glutMouseFunc(GerenciaMouse);
     glutSpecialFunc(TeclasEspeciais);
     glutKeyboardFunc(ControlaTeclado);
     glutTimerFunc(velocidade, Movimento, 1);
     Inicializa();
     glutMainLoop();
}
