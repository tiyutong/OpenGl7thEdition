#include <stdlib.h>
#include <stdio.h>
#define GLEW_STATIC
#include <GL/glew.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>


#define BUFFER_OFFSET(offset) ((const GLubyte *)NULL + (offset))

#pragma comment(lib, "glew32s.lib")


#define XStart -0.8
#define XEnd 0.8
#define YStart -0.8
#define YEnd 0.8
#define NumXPoints 11
#define NumYPoints 11
#define NumPoints (NumXPoints * NumYPoints)
#define NumPointsPerStrip (2 * NumXPoints)
#define NumStrips (NumYPoints - 1)
#define RestartIndex 0xffff
GLfloat color[6][3] = {
	{ 1.0,1.0,1.0 },{ 1.0,0.0,0.0 },{ 1.0,1.0,0.0 },
	{ 0.0,1.0,0.0 },{ 0.0,1.0,1.0 },{ 0.0,0.0,1.0 }
};


void init() {
	GLuint vbo = 0;
	GLuint ebo = 0;
	GLfloat* vertices = NULL;
	GLushort* indices = NULL;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 6 * NumPoints * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	vertices = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	if (vertices == NULL) {
		exit(EXIT_FAILURE);
	}
	else {
		int i;
		int j;
		GLfloat dx = (GLfloat)(XEnd - XStart) / (NumXPoints - 1);
		GLfloat dy = (GLfloat)(YEnd - YStart) / (NumYPoints - 1);
		GLfloat* tmp = vertices;
		int n = 0;

		for (j = 0; j < NumYPoints; ++j) {
			GLfloat y = (GLfloat)(YStart + j * dy);
			for (i = 0; i < NumXPoints; ++i) {
				GLfloat x = (GLfloat)(XStart + i * dx);
				*tmp++ = color[(i + j) % 6][0];
				*tmp++ = color[(i + j) % 6][1];
				*tmp++ = color[(i + j) % 6][2];
				*tmp++ = x;
				*tmp++ = y;
				*tmp++ = 0;
			}
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glVertexPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), BUFFER_OFFSET(3 * sizeof(GLfloat)));
		glColorPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), BUFFER_OFFSET(0));
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		//glInterleavedArrays(GL_C3F_V3F, 0, 0);
	}

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, NumStrips * (NumPointsPerStrip + 1) * sizeof(GLushort), NULL, GL_STATIC_DRAW);
	indices = (GLushort*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (indices == NULL) {
		exit(EXIT_FAILURE);
	}
	else {
		int i;
		int j;
		GLushort* index = indices;
		for (j = 0; j < NumStrips; ++j) {
			GLushort bottomRow = j * NumYPoints;
			GLushort topRow = bottomRow + NumYPoints;
			for (i = 0; i < NumXPoints; ++i) {
				*index++ = topRow + i;
				*index++ = bottomRow + i;
			}
			*index++ = RestartIndex;
		}
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}
	glPrimitiveRestartIndex(RestartIndex);
	glEnable(GL_PRIMITIVE_RESTART);

}


void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1, 1, 1);
	glDrawElements(GL_TRIANGLE_STRIP, NumStrips * (NumPointsPerStrip + 1), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
	glutSwapBuffers();
}
void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, w, 0.0, h);
}
void keyboardfunc(unsigned char key, int x, int y) {
	//std::cout << key << x << y << std::endl;
}
void mouse(int button, int state, int x, int y) {
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			glutIdleFunc(NULL);
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			glutIdleFunc(NULL);
		}
		break;
	default:
		break;
	}
}
void move(int x, int y) {
	//std::cout << x << y << std::endl;
}


int main(int argc, char** argv) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(250, 250);

	glutInitWindowPosition(100, 100);
	glutInitContextVersion(3, 1);

	glutCreateWindow("hello");
	glewInit();
	init();
	glutDisplayFunc(display);
	//glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	//glutMotionFunc(move);
	//glutKeyboardFunc(keyboardfunc);
	glutMainLoop();

	return 0;
}