#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/glut.h> // OpenGL ??? ?????
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
float alpx = 45, alpy = 0, alpz = 0;
float pieceOffsets[8] = {0, 0, 0, 0, 0, 0, 0, 0}; 
int selectedPiece = 0; 
void DrawAxes(float l) {
    glBegin(GL_LINES);
    glColor3f(1, 0, 0); glVertex3f(-l, 0, 0); glVertex3f(l, 0, 0);
    glColor3f(0, 1, 0); glVertex3f(0, -l, 0); glVertex3f(0, l, 0);
    glColor3f(0, 0, 1); glVertex3f(0, 0, -l); glVertex3f(0, 0, l);
    glEnd();
}
void DrawPuzzleFloor(int pieces, float radius) {
    float angleStep = 360.0f / pieces;

    for (int i = 0; i < pieces; i++) {
        float startAngle = i * angleStep;
        float endAngle = (i + 1) * angleStep;
        float midAngle = (startAngle + endAngle) / 2.0f;
        
        // ????? ?????? ??????? ?? ??????? ????????? ??????
        float offsetX = cos(midAngle * M_PI / 180.0f) * pieceOffsets[i];
        float offsetZ = sin(midAngle * M_PI / 180.0f) * pieceOffsets[i];

        glPushMatrix();
        glTranslatef(offsetX, 0.0f, offsetZ);

        // ???? ?????????? (?????????? ??????? ?????????)
        if (i == selectedPiece) {
            glColor3f(1.0f, 1.0f, 0.0f); // ??? ?????? ???????
        } else {
            if (i % 2 == 0) glColor3f(0.85f, 0.53f, 0.10f);
            else glColor3f(0.65f, 0.35f, 0.05f);
        }

        // ??????? ?????
        glBegin(GL_POLYGON);
        glVertex3f(0.0f, 0.0f, 0.0f);
        for (float a = startAngle; a <= endAngle; a += 5.0f) {
            float rad = a * M_PI / 180.0f;
            glVertex3f(cos(rad) * radius, 0.0f, sin(rad) * radius);
        }
        glVertex3f(cos(endAngle * M_PI / 180.0f) * radius, 0.0f, sin(endAngle * M_PI / 180.0f) * radius);
        glEnd();

        // ????? ?????
        glColor3f(0.0f, 0.0f, 0.0f);
        glLineWidth(2);
        glBegin(GL_LINE_LOOP);
        glVertex3f(0.0f, 0.0f, 0.0f);
        for (float a = startAngle; a <= endAngle; a += 5.0f) {
            float rad = a * M_PI / 180.0f;
            glVertex3f(cos(rad) * radius, 0.0f, sin(rad) * radius);
        }
        glVertex3f(cos(endAngle * M_PI / 180.0f) * radius, 0.0f, sin(endAngle * M_PI / 180.0f) * radius);
        glEnd();

        glPopMatrix();
    }
}
void render() {
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glPushMatrix();
    glRotatef(alpx, 1, 0, 0);
    glRotatef(alpy, 0, 1, 0);
    glRotatef(alpz, 0, 0, 1);

    DrawAxes(10);
    DrawPuzzleFloor(8, 8.0f);

    glPopMatrix();
    glutSwapBuffers();
}
void keypress(unsigned char c, int a, int b) {
    if (c == 27) exit(0);
    
    if (c >= '1' && c <= '8') {
        selectedPiece = c - '1';
        printf("Hesegh %d songogdloo.\n", selectedPiece + 1);
    }

    if (c == '+') pieceOffsets[selectedPiece] += 0.5f;
    if (c == '-' && pieceOffsets[selectedPiece] > 0.0f) pieceOffsets[selectedPiece] -= 0.5f;

    if (c == ' ') {
        static bool expanded = false;
        expanded = !expanded;
        for(int i=0; i<8; i++) pieceOffsets[i] = expanded ? 2.0f : 0.0f;
    }

    if (c == 'x') alpx -= 5; else if (c == 'X') alpx += 5;
    if (c == 'y') alpy -= 5; else if (c == 'Y') alpy += 5;
    if (c == 'z') alpz -= 5; else if (c == 'Z') alpz += 5;

    glutPostRedisplay();
}
void resize(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-15.0, 15.0, -15.0, 15.0, -15.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Ger Floor Simulation");
    
    glutReshapeFunc(resize);
    glutKeyboardFunc(keypress);
    glutDisplayFunc(render);
    
    glutMainLoop();
    return 0;
}
