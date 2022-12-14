// Reading Obj file
#define  _CRT_SECURE_NO_WARNINGS
#define NONE 0
#define PAWN 1
#define KNIGHT 2
#define BISHOP 3
#define ROOK 4
#define QUEEN 5
#define KING 6
#define RED 100
#define BLUE 101

#include "shader.h"
#include "objRead.h"

using namespace std;

GLuint g_window_w = 600;
GLuint g_window_h = 600;

GLuint VAO[6];
GLuint VBO_position[6];
GLuint VBO_normal[6];
GLuint VBO_color[6];

GLuint Chess_Board_vao;
GLuint box_vao;

int polygon_mode = 1;

const int num_vertices = 3;
const int num_triangles = 1;

void Display();
void Reshape(int w, int h);
void Keyboard(unsigned char key, int x, int y);
void Skeyboard(int key, int x, int y);
void TimerFunction(int value);
void InitBuffer();
void transform(float tx, float ty, float tz, float rx, float ry, float rz, float sx, float sy, float sz);
void viewTransform();
void projectionTransform();
void ambientlight(float R, float G, float B);
void Chess_Board_ambientlight();
void Chess_obj_draw();
void init_Chess_board();

struct Chess {
	float x, z;
	int p; //piece
	int c; // color
};

struct Chess board[8][8];
struct Chess selected;

float Chess_Board_location[] = {
	0.0f, 0.0f, 0.0f,				// A-1
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	1.0f, 0.0f, 0.0f,				// A-2
	2.0f, 0.0f, 0.0f,
	2.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,

	2.0f, 0.0f, 0.0f,				// A-3
	3.0f, 0.0f, 0.0f,
	3.0f, 0.0f, 1.0f,
	2.0f, 0.0f, 1.0f,

	3.0f, 0.0f, 0.0f,				// A-4
	4.0f, 0.0f, 0.0f,
	4.0f, 0.0f, 1.0f,
	3.0f, 0.0f, 1.0f,

	4.0f, 0.0f, 0.0f,				// A-5
	5.0f, 0.0f, 0.0f,
	5.0f, 0.0f, 1.0f,
	4.0f, 0.0f, 1.0f,

	5.0f, 0.0f, 0.0f,				// A-6
	6.0f, 0.0f, 0.0f,
	6.0f, 0.0f, 1.0f,
	5.0f, 0.0f, 1.0f,

	6.0f, 0.0f, 0.0f,				// A-7
	7.0f, 0.0f, 0.0f,
	7.0f, 0.0f, 1.0f,
	6.0f, 0.0f, 1.0f,

	7.0f, 0.0f, 0.0f,				// A-8
	8.0f, 0.0f, 0.0f,
	8.0f, 0.0f, 1.0f,
	7.0f, 0.0f, 1.0f,

	0.0f, 0.0f, 1.0f,				// B-1
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 2.0f,
	0.0f, 0.0f, 2.0f,

	1.0f, 0.0f, 1.0f,				// B-2
	2.0f, 0.0f, 1.0f,
	2.0f, 0.0f, 2.0f,
	1.0f, 0.0f, 2.0f,

	2.0f, 0.0f, 1.0f,				// B-3
	3.0f, 0.0f, 1.0f,
	3.0f, 0.0f, 2.0f,
	2.0f, 0.0f, 2.0f,

	3.0f, 0.0f, 1.0f,				// B-4
	4.0f, 0.0f, 1.0f,
	4.0f, 0.0f, 2.0f,
	3.0f, 0.0f, 2.0f,

	4.0f, 0.0f, 1.0f,				// B-5
	5.0f, 0.0f, 1.0f,
	5.0f, 0.0f, 2.0f,
	4.0f, 0.0f, 2.0f,

	5.0f, 0.0f, 1.0f,				// B-6
	6.0f, 0.0f, 1.0f,
	6.0f, 0.0f, 2.0f,
	5.0f, 0.0f, 2.0f,

	6.0f, 0.0f, 1.0f,				// B-7
	7.0f, 0.0f, 1.0f,
	7.0f, 0.0f, 2.0f,
	6.0f, 0.0f, 2.0f,

	7.0f, 0.0f, 1.0f,				// B-8
	8.0f, 0.0f, 1.0f,
	8.0f, 0.0f, 2.0f,
	7.0f, 0.0f, 2.0f,

	0.0f, 0.0f, 2.0f,				// C-1
	1.0f, 0.0f, 2.0f,
	1.0f, 0.0f, 3.0f,
	0.0f, 0.0f, 3.0f,

	1.0f, 0.0f, 2.0f,				// C-2
	2.0f, 0.0f, 2.0f,
	2.0f, 0.0f, 3.0f,
	1.0f, 0.0f, 3.0f,

	2.0f, 0.0f, 2.0f,				// C-3
	3.0f, 0.0f, 2.0f,
	3.0f, 0.0f, 3.0f,
	2.0f, 0.0f, 3.0f,

	3.0f, 0.0f, 2.0f,				// C-4
	4.0f, 0.0f, 2.0f,
	4.0f, 0.0f, 3.0f,
	3.0f, 0.0f, 3.0f,

	4.0f, 0.0f, 2.0f,				// C-5
	5.0f, 0.0f, 2.0f,
	5.0f, 0.0f, 3.0f,
	4.0f, 0.0f, 3.0f,

	5.0f, 0.0f, 2.0f,				// C-6
	6.0f, 0.0f, 2.0f,
	6.0f, 0.0f, 3.0f,
	5.0f, 0.0f, 3.0f,

	6.0f, 0.0f, 2.0f,				// C-7
	7.0f, 0.0f, 2.0f,
	7.0f, 0.0f, 3.0f,
	6.0f, 0.0f, 3.0f,

	7.0f, 0.0f, 2.0f,				// C-8
	8.0f, 0.0f, 2.0f,
	8.0f, 0.0f, 3.0f,
	7.0f, 0.0f, 3.0f,

	0.0f, 0.0f, 3.0f,				// D-1
	1.0f, 0.0f, 3.0f,
	1.0f, 0.0f, 4.0f,
	0.0f, 0.0f, 4.0f,

	1.0f, 0.0f, 3.0f,				// D-2
	2.0f, 0.0f, 3.0f,
	2.0f, 0.0f, 4.0f,
	1.0f, 0.0f, 4.0f,

	2.0f, 0.0f, 3.0f,				// D-3
	3.0f, 0.0f, 3.0f,
	3.0f, 0.0f, 4.0f,
	2.0f, 0.0f, 4.0f,

	3.0f, 0.0f, 3.0f,				// D-4
	4.0f, 0.0f, 3.0f,
	4.0f, 0.0f, 4.0f,
	3.0f, 0.0f, 4.0f,

	4.0f, 0.0f, 3.0f,				// D-5
	5.0f, 0.0f, 3.0f,
	5.0f, 0.0f, 4.0f,
	4.0f, 0.0f, 4.0f,

	5.0f, 0.0f, 3.0f,				// D-6
	6.0f, 0.0f, 3.0f,
	6.0f, 0.0f, 4.0f,
	5.0f, 0.0f, 4.0f,

	6.0f, 0.0f, 3.0f,				// D-7
	7.0f, 0.0f, 3.0f,
	7.0f, 0.0f, 4.0f,
	6.0f, 0.0f, 4.0f,

	7.0f, 0.0f, 3.0f,				// D-8
	8.0f, 0.0f, 3.0f,
	8.0f, 0.0f, 4.0f,
	7.0f, 0.0f, 4.0f,

	0.0f, 0.0f, 4.0f,				// E-1
	1.0f, 0.0f, 4.0f,
	1.0f, 0.0f, 5.0f,
	0.0f, 0.0f, 5.0f,

	1.0f, 0.0f, 4.0f,				// E-2
	2.0f, 0.0f, 4.0f,
	2.0f, 0.0f, 5.0f,
	1.0f, 0.0f, 5.0f,

	2.0f, 0.0f, 4.0f,				// E-3
	3.0f, 0.0f, 4.0f,
	3.0f, 0.0f, 5.0f,
	2.0f, 0.0f, 5.0f,

	3.0f, 0.0f, 4.0f,				// E-4
	4.0f, 0.0f, 4.0f,
	4.0f, 0.0f, 5.0f,
	3.0f, 0.0f, 5.0f,

	4.0f, 0.0f, 4.0f,				// E-5
	5.0f, 0.0f, 4.0f,
	5.0f, 0.0f, 5.0f,
	4.0f, 0.0f, 5.0f,

	5.0f, 0.0f, 4.0f,				// E-6
	6.0f, 0.0f, 4.0f,
	6.0f, 0.0f, 5.0f,
	5.0f, 0.0f, 5.0f,

	6.0f, 0.0f, 4.0f,				// E-7
	7.0f, 0.0f, 4.0f,
	7.0f, 0.0f, 5.0f,
	6.0f, 0.0f, 5.0f,

	7.0f, 0.0f, 4.0f,				// E-8
	8.0f, 0.0f, 4.0f,
	8.0f, 0.0f, 5.0f,
	7.0f, 0.0f, 5.0f,

	0.0f, 0.0f, 5.0f,				// F-1
	1.0f, 0.0f, 5.0f,
	1.0f, 0.0f, 6.0f,
	0.0f, 0.0f, 6.0f,

	1.0f, 0.0f, 5.0f,				// F-2
	2.0f, 0.0f, 5.0f,
	2.0f, 0.0f, 6.0f,
	1.0f, 0.0f, 6.0f,

	2.0f, 0.0f, 5.0f,				// F-3
	3.0f, 0.0f, 5.0f,
	3.0f, 0.0f, 6.0f,
	2.0f, 0.0f, 6.0f,

	3.0f, 0.0f, 5.0f,				// F-4
	4.0f, 0.0f, 5.0f,
	4.0f, 0.0f, 6.0f,
	3.0f, 0.0f, 6.0f,

	4.0f, 0.0f, 5.0f,				// F-5
	5.0f, 0.0f, 5.0f,
	5.0f, 0.0f, 6.0f,
	4.0f, 0.0f, 6.0f,

	5.0f, 0.0f, 5.0f,				// F-6
	6.0f, 0.0f, 5.0f,
	6.0f, 0.0f, 6.0f,
	5.0f, 0.0f, 6.0f,

	6.0f, 0.0f, 5.0f,				// F-7
	7.0f, 0.0f, 5.0f,
	7.0f, 0.0f, 6.0f,
	6.0f, 0.0f, 6.0f,

	7.0f, 0.0f, 5.0f,				// F-8
	8.0f, 0.0f, 5.0f,
	8.0f, 0.0f, 6.0f,
	7.0f, 0.0f, 6.0f,

	0.0f, 0.0f, 6.0f,				// G-1
	1.0f, 0.0f, 6.0f,
	1.0f, 0.0f, 7.0f,
	0.0f, 0.0f, 7.0f,

	1.0f, 0.0f, 6.0f,				// G-2
	2.0f, 0.0f, 6.0f,
	2.0f, 0.0f, 7.0f,
	1.0f, 0.0f, 7.0f,

	2.0f, 0.0f, 6.0f,				// G-3
	3.0f, 0.0f, 6.0f,
	3.0f, 0.0f, 7.0f,
	2.0f, 0.0f, 7.0f,

	3.0f, 0.0f, 6.0f,				// G-4
	4.0f, 0.0f, 6.0f,
	4.0f, 0.0f, 7.0f,
	3.0f, 0.0f, 7.0f,

	4.0f, 0.0f, 6.0f,				// G-5
	5.0f, 0.0f, 6.0f,
	5.0f, 0.0f, 7.0f,
	4.0f, 0.0f, 7.0f,

	5.0f, 0.0f, 6.0f,				// G-6
	6.0f, 0.0f, 6.0f,
	6.0f, 0.0f, 7.0f,
	5.0f, 0.0f, 7.0f,

	6.0f, 0.0f, 6.0f,				// G-7
	7.0f, 0.0f, 6.0f,
	7.0f, 0.0f, 7.0f,
	6.0f, 0.0f, 7.0f,

	7.0f, 0.0f, 6.0f,				// G-8
	8.0f, 0.0f, 6.0f,
	8.0f, 0.0f, 7.0f,
	7.0f, 0.0f, 7.0f,

	0.0f, 0.0f, 7.0f,				// H-1
	1.0f, 0.0f, 7.0f,
	1.0f, 0.0f, 8.0f,
	0.0f, 0.0f, 8.0f,

	1.0f, 0.0f, 7.0f,				//H-2
	2.0f, 0.0f, 7.0f,
	2.0f, 0.0f, 8.0f,
	1.0f, 0.0f, 8.0f,

	2.0f, 0.0f, 7.0f,				//H-3
	3.0f, 0.0f, 7.0f,
	3.0f, 0.0f, 8.0f,
	2.0f, 0.0f, 8.0f,

	3.0f, 0.0f, 7.0f,				//H-4
	4.0f, 0.0f, 7.0f,
	4.0f, 0.0f, 8.0f,
	3.0f, 0.0f, 8.0f,

	4.0f, 0.0f, 7.0f,				//H-5
	5.0f, 0.0f, 7.0f,
	5.0f, 0.0f, 8.0f,
	4.0f, 0.0f, 8.0f,

	5.0f, 0.0f, 7.0f,				//H-6
	6.0f, 0.0f, 7.0f,
	6.0f, 0.0f, 8.0f,
	5.0f, 0.0f, 8.0f,

	6.0f, 0.0f, 7.0f,				//H-7
	7.0f, 0.0f, 7.0f,
	7.0f, 0.0f, 8.0f,
	6.0f, 0.0f, 8.0f,

	7.0f, 0.0f, 7.0f,				//H-8
	8.0f, 0.0f, 7.0f,
	8.0f, 0.0f, 8.0f,
	7.0f, 0.0f, 8.0f
};

float Chess_Board_nomal[] = {
	-1.0f, 0.0f, 0.0f,				//A-1
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,				//A-2
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,


	-1.0f, 0.0f, 0.0f,				//A-3
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	// 왼

	1.0f, 0.0f, 0.0f,				//A-4
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,				//A-5
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	0.0f, 1.0f, 0.0f,				//A-6
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	// 위
	-1.0f, 0.0f, 0.0f,				//A-7
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,				//A-8
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,				//B-1
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,				//B-2
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,				//B-3
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,				//B-4
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,				//B-5
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,				//B-6
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,				//B-7
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,				//B-8
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,				//C-1
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,				//C-2
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,


	-1.0f, 0.0f, 0.0f,				//C-3
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	// 왼

	1.0f, 0.0f, 0.0f,				//C-4
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,				//C-5
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	0.0f, 1.0f, 0.0f,				//C-6
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	// 위
	-1.0f, 0.0f, 0.0f,				//C-7
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,				//C-8
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,				//D-1
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,				//D-2
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,				//D-3
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,				//D-4
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,				//D-5
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,				//D-6
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,				//D-7
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,				//D-8
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,				//E-1
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,				//E-2
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,


	-1.0f, 0.0f, 0.0f,				//E-3
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	// 왼

	1.0f, 0.0f, 0.0f,				//E-4
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,				//E-5
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	0.0f, 1.0f, 0.0f,				//E-6
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	// 위
	-1.0f, 0.0f, 0.0f,				//E-7
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,				//E-8
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,				//F-1
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,				//F-2
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,				//F-3
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,				//F-4
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,				//F-5
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,				//F-6
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,				//F-7
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,				//F-8
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,				//G-1
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,				//G-2
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,


	-1.0f, 0.0f, 0.0f,				//G-3
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	// 왼

	1.0f, 0.0f, 0.0f,				//G-4
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,				//G-5
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	0.0f, 1.0f, 0.0f,				//G-6
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	// 위
	-1.0f, 0.0f, 0.0f,				//G-7
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,				//G-8
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,				//H-1
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,				//H-2
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,				//H-3
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,				//H-4
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,				//H-5
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,				//H-6
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,				//H-7
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	-1.0f, 0.0f, 0.0f,				//H-8
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
};

float Chess_Board_color[] = {
	1.0f, 1.0f, 1.0f,				// A-1
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				// A-2
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				// A-3
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				// A-4
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				// A-5
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				// A-6
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				// A-7
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				// A-8
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	0.0f, 0.0f, 0.0f,				// B-1
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				//B-2
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				//B-3
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				//B-4
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				//B-5
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				//B-6
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				//B-7
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				//B-8
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	1.0f, 1.0f, 1.0f,				// C-1
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				// C-2
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				// C-3
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				// C-4
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				// C-5
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				// C-6
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				// C-7
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				// C-8
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	0.0f, 0.0f, 0.0f,				//D-1
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				//D-2
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				//D-3
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				//D-4
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				//D-5
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				//D-6
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				//D-7
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				//D-8
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	1.0f, 1.0f, 1.0f,				//E-1
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				//E-2
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				//E-3
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				//E-4
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				//E-5
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				//E-6
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				//E-7
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				//E-8
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	0.0f, 0.0f, 0.0f,				//F-1
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				//F-2
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				//F-3
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				//F-4
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				//F-5
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				//F-6
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				//F-7
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				//F-8
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	1.0f, 1.0f, 1.0f,				//G-1
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				//G-2
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				//G-3
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				//G-4
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				//G-5
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				//G-6
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				//G-7
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				//G-8
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	0.0f, 0.0f, 0.0f,				//H-1
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				//H-2
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				//H-3
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				//H-4
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				//H-5
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				//H-6
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,				//H-7
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 1.0f,				//H-8
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
};

float Move_box[] = {
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f
};

float Move_box_color[] = {
	1.0f, 1.0f, 0.0f,				//A-1
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f
};

float Move_box_nomal[] = {
	1.0f, 0.0f, 0.0f,				//A-1
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f
};

float dis = 15.0f;
float angle = 0.0f;
float camera_x = -10.0f;
float camera_y = 15.0f;
float camera_z = 4.0f;

float t_box_x = 0.0f;
float t_box_z = 0.0f;
int box_i = 0;
int box_j = 0;

bool sel = false;

//--- load obj related variabales
objRead objReader[6];
GLint Pawn = objReader[0].loadObj_normalize_center("Pawn.obj");
GLint Knight = objReader[1].loadObj_normalize_center("Knight.obj");
GLint Bishop = objReader[2].loadObj_normalize_center("BIshop.obj");
GLint Rook = objReader[3].loadObj_normalize_center("Rook.obj");
GLint Queen = objReader[4].loadObj_normalize_center("Queen.obj");
GLint King = objReader[5].loadObj_normalize_center("King.obj");

int main(int argc, char** argv)
{
	// create window using freeglut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(g_window_w, g_window_h);
	glutInitWindowPosition(0, 0);

	glutCreateWindow("Computer Graphics");

	//////////////////////////////////////////////////////////////////////////////////////
	//// initialize GLEW
	//////////////////////////////////////////////////////////////////////////////////////
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "GLEW OK\n";
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//// Create shader program an register the shader
	//////////////////////////////////////////////////////////////////////////////////////

	GLuint vShader[4];
	GLuint fShader[4];

	vShader[0] = MakeVertexShader("vertex2.glsl", 0);			// Sun
	fShader[0] = MakeFragmentShader("fragment2.glsl", 0);

	// shader Program
	s_program[0] = glCreateProgram();
	glAttachShader(s_program[0], vShader[0]);
	glAttachShader(s_program[0], fShader[0]);
	glLinkProgram(s_program[0]);
	checkCompileErrors(s_program[0], "PROGRAM");

	InitBuffer();
	init_Chess_board();

	// callback functions
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Skeyboard);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	// freeglut 윈도우 이벤트 처리 시작. 윈도우가 닫힐때까지 후한루프 실행.
	glutMainLoop();

	return 0;
}


void InitBuffer()
{
	glGenVertexArrays(6, VAO);
	glGenBuffers(6, VBO_position);
	glGenBuffers(6, VBO_normal);
	glGenBuffers(6, VBO_color);

	GLint cAttribute = glGetAttribLocation(s_program[0], "aColor");
	GLint pAttribute = glGetAttribLocation(s_program[0], "aPos");
	GLint nAttribute = glGetAttribLocation(s_program[0], "aNormal");
	for (int i = 0; i < 6; i++)
	{
		glUseProgram(s_program[0]);
		glBindVertexArray(VAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_position[i]);
		glBufferData(GL_ARRAY_BUFFER, objReader[i].outvertex.size() * sizeof(glm::vec3), objReader[i].outvertex.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_normal[i]);
		glBufferData(GL_ARRAY_BUFFER, objReader[i].outnormal.size() * sizeof(glm::vec3), objReader[i].outnormal.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(nAttribute);
	}

	GLuint Chess_Board_vbo[3];
	glGenVertexArrays(1, &Chess_Board_vao);
	glGenBuffers(3, Chess_Board_vbo);
	glBindVertexArray(Chess_Board_vao);
	glBindBuffer(GL_ARRAY_BUFFER, Chess_Board_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Chess_Board_location), Chess_Board_location, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, Chess_Board_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Chess_Board_color), Chess_Board_color, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(cAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, Chess_Board_vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Chess_Board_nomal), Chess_Board_nomal, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(nAttribute);

	GLuint Chess_Board_vbo2[3];
	glGenVertexArrays(1, &box_vao);
	glGenBuffers(3, Chess_Board_vbo2);
	glBindVertexArray(box_vao);
	glBindBuffer(GL_ARRAY_BUFFER, Chess_Board_vbo2[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Move_box), Move_box, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, Chess_Board_vbo2[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Move_box_color), Move_box_color, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(cAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, Chess_Board_vbo2[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Move_box_nomal), Move_box_nomal, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(nAttribute);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO_color[0]);
	//glBufferData(GL_ARRAY_BUFFER, objReader.outvertex.size() * sizeof(glm::vec3), &objReader.outvertex[0], GL_STATIC_DRAW);
	//glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	//glEnableVertexAttribArray(cAttribute);

	glEnable(GL_DEPTH_TEST);
}


void Display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//*************************************************************************
	// Drawing circle
	glUseProgram(s_program[0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	transform(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
	viewTransform();
	projectionTransform();
	ambientlight(1.0, 1.0, 1.0);
	glBindVertexArray(Chess_Board_vao);
	glDrawArrays(GL_QUADS, 0, 32 * 8);
	glBindVertexArray(box_vao);
	transform(t_box_x, 0.01, t_box_z, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
	ambientlight(1.0, 1.0, 0.0);
	glDrawArrays(GL_QUADS, 0, 4);

	Chess_obj_draw();

	glutSwapBuffers();
}


void Reshape(int w, int h)
{
	g_window_w = w;
	g_window_h = h;
	glViewport(0, 0, w, h);
}

int temp_i = -1;
int temp_j = -1;

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '[':
		if (board[box_i][box_j].p != NONE) {
			sel = true;
			selected.p = board[box_i][box_j].p;
			selected.c = board[box_i][box_j].c;
			temp_i = box_i;
			temp_j = box_j;
		}
		break;

	case ']':
		if (sel == true) {
			sel = false;
			board[box_i][box_j].p = selected.p;
			board[box_i][box_j].c = selected.c;
			board[temp_i][temp_j].p = NONE;
			board[temp_i][temp_j].c = NONE;
		}
		break;

	case 'p':
		if (sel == true) {
			sel = false;
		}
		break;
	}

	glutPostRedisplay();
}

void Skeyboard(int key, int x, int y) {
	//if (sel == true) {
	//	if (key == GLUT_KEY_LEFT) {
	//		if (box_j - 1 >= 0 /*&& board[box_i][box_j - 1].p == NONE*/) {
	//			t_box_z -= 1.0f;
	//			box_j--;
	//			//board[box_i][box_j].p = selected.p;
	//			//board[box_i][box_j].c = selected.c;
	//			//board[box_i][box_j + 1].p = NONE;
	//			//board[box_i][box_j + 1].c = NONE;
	//		}
	//	}

	//	else if (key == GLUT_KEY_RIGHT) {
	//		if (box_j + 1 <= 7 /*&& board[box_i][box_j + 1].p == NONE*/) {
	//			t_box_z += 1.0f;
	//			box_j++;
	//			/*board[box_i][box_j].p = selected.p;
	//			board[box_i][box_j].c = selected.c;
	//			board[box_i][box_j - 1].p = NONE;
	//			board[box_i][box_j - 1].c = NONE;*/
	//		}
	//	}

	//	else if (key == GLUT_KEY_UP) {
	//		if (box_i + 1 <= 7 /*&& board[box_i + 1][box_j].p == NONE*/) {
	//			t_box_x += 1.0f;
	//			box_i++;
	//			/*board[box_i][box_j].p = selected.p;
	//			board[box_i][box_j].c = selected.c;
	//			board[box_i - 1][box_j].p = NONE;
	//			board[box_i - 1][box_j].c = NONE;*/
	//		}
	//		cout << "i " << box_i << "\n";
	//	}

	//	else if (key == GLUT_KEY_DOWN) {
	//		if (box_i - 1 >= 0 /*&& board[box_i - 1][box_j].p == NONE*/) {
	//			t_box_x -= 1.0f;
	//			box_i--;
	//			/*board[box_i][box_j].p = selected.p;
	//			board[box_i][box_j].c = selected.c;
	//			board[box_i + 1][box_j].p = NONE;
	//			board[box_i + 1][box_j].c = NONE;*/
	//		}
	//		cout << "i " << box_i << "\n";
	//	}
	//}

	//else {
	if (key == GLUT_KEY_LEFT) {
		if (box_j - 1 >= 0) {
			t_box_z -= 1.0f;
			box_j--;
		}
	}

	else if (key == GLUT_KEY_RIGHT) {
		if (box_j + 1 <= 7) {
			t_box_z += 1.0f;
			box_j++;
		}
	}

	else if (key == GLUT_KEY_UP) {
		if (box_i + 1 <= 7) {
			t_box_x += 1.0f;
			box_i++;
		}
	}

	else if (key == GLUT_KEY_DOWN) {
		if (box_i - 1 >= 0) {
			t_box_x -= 1.0f;
			box_i--;
		}
	}
		

	glutPostRedisplay();
}

void init_Chess_board() {
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board[i][j].x = 0.5 + 1.0 * i;
			board[i][j].z = 0.5 + 1.0 * j;
			if(i == 1 or i == 6)
				board[i][j].p = PAWN;
			else
				board[i][j].p = NONE;
			if (i < 2)
				board[i][j].c = RED;
			else if (i > 5)
				board[i][j].c = BLUE;
			else
				board[i][j].c = NONE;
		}
	}
	board[0][0].p = ROOK;
	board[0][1].p = KNIGHT;
	board[0][2].p = BISHOP;
	board[0][3].p = QUEEN;
	board[0][4].p = KING;
	board[0][5].p = BISHOP;
	board[0][6].p = KNIGHT;
	board[0][7].p = ROOK;

	board[7][0].p = ROOK;
	board[7][1].p = KNIGHT;
	board[7][2].p = BISHOP;
	board[7][3].p = KING;
	board[7][4].p = QUEEN;
	board[7][5].p = BISHOP;
	board[7][6].p = KNIGHT;
	board[7][7].p = ROOK;

}

void Chess_obj_draw()
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	viewTransform();
	projectionTransform();
	
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j].c == RED)
				ambientlight(1.0, 0.0, 0.0);
			else if (board[i][j].c == BLUE)
				ambientlight(0.0, 0.0, 1.0);
			switch (board[i][j].p)
			{
			case PAWN:
				glBindVertexArray(VAO[0]);
				transform(board[i][j].x, 0.6, board[i][j].z, 0.0, 0.0, 0.0, 0.3, 0.6, 0.3);
				glDrawArrays(GL_TRIANGLES, 0, Pawn);
				break;

			case KNIGHT:
				glBindVertexArray(VAO[1]);
				transform(board[i][j].x, 0.6, board[i][j].z, 0.0, 0.0, 0.0, 0.3, 0.6, 0.3);
				glDrawArrays(GL_TRIANGLES, 0, Knight);
				break;

			case BISHOP:
				glBindVertexArray(VAO[2]);
				transform(board[i][j].x, 0.6, board[i][j].z, 0.0, 0.0, 0.0, 0.3, 0.6, 0.3);
				glDrawArrays(GL_TRIANGLES, 0, Bishop);
				break;
			case ROOK:
				glBindVertexArray(VAO[3]);
				transform(board[i][j].x, 0.6, board[i][j].z, 0.0, 0.0, 0.0, 0.3, 0.6, 0.3);
				glDrawArrays(GL_TRIANGLES, 0, Rook);
				break;
			case QUEEN:
				glBindVertexArray(VAO[4]);
				transform(board[i][j].x, 0.6, board[i][j].z, 0.0, 0.0, 0.0, 0.3, 0.6, 0.3);
				glDrawArrays(GL_TRIANGLES, 0, Queen);
				break;

			case KING:
				glBindVertexArray(VAO[5]);
				transform(board[i][j].x, 0.6, board[i][j].z, 0.0, 0.0, 0.0, 0.3, 0.6, 0.3);
				glDrawArrays(GL_TRIANGLES, 0, King);
				break;
			case NONE:
				break;
			}
		}
	}
	if (sel == true) {
		if (selected.c == RED)
			ambientlight(1.0, 0.0, 0.0);
		else if (selected.c == BLUE)
			ambientlight(0.0, 0.0, 1.0);
		switch (selected.p)
		{
		case PAWN:
			glBindVertexArray(VAO[0]);
			transform(board[box_i][box_j].x, 0.6, board[box_i][box_j].z, 0.0, 0.0, 0.0, 0.3, 0.6, 0.3);
			glDrawArrays(GL_TRIANGLES, 0, Pawn);
			break;

		case KNIGHT:
			glBindVertexArray(VAO[1]);
			transform(board[box_i][box_j].x, 0.6, board[box_i][box_j].z, 0.0, 0.0, 0.0, 0.3, 0.6, 0.3);
			glDrawArrays(GL_TRIANGLES, 0, Knight);
			break;

		case BISHOP:
			glBindVertexArray(VAO[2]);
			transform(board[box_i][box_j].x, 0.6, board[box_i][box_j].z, 0.0, 0.0, 0.0, 0.3, 0.6, 0.3);
			glDrawArrays(GL_TRIANGLES, 0, Bishop);
			break;
		case ROOK:
			glBindVertexArray(VAO[3]);
			transform(board[box_i][box_j].x, 0.6, board[box_i][box_j].z, 0.0, 0.0, 0.0, 0.3, 0.6, 0.3);
			glDrawArrays(GL_TRIANGLES, 0, Rook);
			break;
		case QUEEN:
			glBindVertexArray(VAO[4]);
			transform(board[box_i][box_j].x, 0.6, board[box_i][box_j].z, 0.0, 0.0, 0.0, 0.3, 0.6, 0.3);
			glDrawArrays(GL_TRIANGLES, 0, Queen);
			break;

		case KING:
			glBindVertexArray(VAO[5]);
			transform(board[box_i][box_j].x, 0.6, board[box_i][box_j].z, 0.0, 0.0, 0.0, 0.3, 0.6, 0.3);
			glDrawArrays(GL_TRIANGLES, 0, King);
			break;
		case NONE:
			break;
		}
	}
}

void transform(float tx, float ty, float tz, float rx, float ry, float rz, float sx, float sy, float sz) {
	glm::mat4 T = glm::mat4(1.0f);
	glm::mat4 Rx = glm::mat4(1.0f);
	glm::mat4 Ry = glm::mat4(1.0f);
	glm::mat4 Rz = glm::mat4(1.0f);
	glm::mat4 S = glm::mat4(1.0f);
	glm::mat4 TR = glm::mat4(1.0f);
	T = glm::translate(T, glm::vec3(tx, ty, tz));
	Rx = glm::rotate(Rx, glm::radians(rx), glm::vec3(1.0, 0.0, 0.0));;
	Ry = glm::rotate(Rx, glm::radians(ry), glm::vec3(0.0, 1.0, 0.0));;
	Rz = glm::rotate(Rx, glm::radians(rz), glm::vec3(0.0, 0.0, 1.0));;
	S = glm::scale(S, glm::vec3(sx, sy, sz));
	TR = T * Rx * Ry * Rz * S;
	unsigned int modelLocation = glGetUniformLocation(s_program[0], "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
}

void TimerFunction(int value) {

}

void viewTransform() {
	//float sin = glm::sin(angle);
	//float cos = glm::cos(angle);
	glm::vec3 cameraPos = glm::vec3(camera_x, camera_y, camera_z); //--- 카메라 위치
	glm::vec3 cameraDirection = glm::vec3(4.0f, 0.0f, 4.0f); //--- 카메라 바라보는 방향
	glm::vec3 cameraUp = glm::vec3(1.0f, 0.0f, 0.0f); //--- 카메라 위쪽 방향
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	unsigned int viewLocation = glGetUniformLocation(s_program[0], "viewTransform"); //--- 뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
}

void projectionTransform() {
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
	//projection = glm::translate(projection, glm::vec3(0.0, 0.0, 0.0)); //--- 공간을 약간 뒤로 미뤄줌
	unsigned int projectionLocation = glGetUniformLocation(s_program[0], "projectionTransform"); //--- 투영 변환 값 설정
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	//glm:: mat4 glm::perspective (float fovy, float aspect, float near, float far)
}

void ambientlight(float R, float G, float B) {
	glUseProgram(s_program[0]);

	unsigned int lightPosLocation = glGetUniformLocation(s_program[0], "lightPos"); //--- lightPos 값 전달
	glUniform3f(lightPosLocation, 0.0f, 5.0f, 0.0f);
	unsigned int lightColorLocation = glGetUniformLocation(s_program[0], "lightColor"); //--- lightColor 값 전달
	glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
	unsigned int objColorLocation = glGetUniformLocation(s_program[0], "objectColor"); //--- object Color값 전달
	glUniform3f(objColorLocation, R, G, B);
	unsigned int viewPosLocation = glGetUniformLocation(s_program[0], "viewPos"); //--- viewPos 값 전달: 카메라 위치
	glUniform3f(viewPosLocation, camera_x, camera_y, camera_z);

}

void Chess_Board_ambientlight() {
	glUseProgram(s_program[0]);

	unsigned int lightPosLocation = glGetUniformLocation(s_program[0], "lightPos"); //--- lightPos 값 전달
	glUniform3f(lightPosLocation, 0.0f, 5.0f, 0.0f);
	unsigned int lightColorLocation = glGetUniformLocation(s_program[0], "lightColor"); //--- lightColor 값 전달
	glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
	//unsigned int objColorLocation = glGetUniformLocation(s_program[0], "objectColor"); //--- object Color값 전달
	//glUniform3f(objColorLocation, 1.0, 1.0, 1.0);
	unsigned int viewPosLocation = glGetUniformLocation(s_program[0], "viewPos"); //--- viewPos 값 전달: 카메라 위치
	glUniform3f(viewPosLocation, camera_x, camera_y, camera_z);

}






