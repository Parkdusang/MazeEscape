#include <math.h>
#include <gl/glut.h>
//#include<GLUT/GLUT.h>

#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include <iostream>
GLfloat vertices[][3] = { { -1,-1,-1 },{ 1,-1,-1 },{ 1,1,-1},{ -1,1,-1 },
    { -1, -1 , 1 },{ 1, -1 , 1 },{ 1, 1 , 1 },{ -1, 1, 1 } };
GLfloat colors[][3] = { { 0.3, 0.3, 0.3 },{ 1, 1, 1 },{ 0.3,0.3, 0.3 },{ 0.3,0.3, 0.3 },
    { 0.3, 0.3, 0.3 },{ 0.3, 0.3, 0.3 },{ 1,1, 1 },{ 1,1,1 } };

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _texture
{
    GLubyte*        rgb;// rgb array
    int             width;// width
    int             height;// height
} Texture;
void texture_create( Texture* p, int w, int h )
{
    p->rgb    = (GLubyte*)malloc(sizeof(GLubyte)*w*h*3);
    p->width  = w;
    p->height = h;
    memset( p->rgb, 0, sizeof(GLubyte)*w*h*3 );
}
void texture_checkerboard( Texture* p, int px, int py )
{
    for ( int y=0; y<p->width; y++ )
    {
        for ( int x=0; x<p->height; x++ )
        {
            if((y/px)%2){
                
                if ( ( (x/px)+(y/py) ) %4 )
                {
                    p->rgb[(x+y*p->width)*3+0] = 136;
                    p->rgb[(x+y*p->width)*3+1] =   84;
                    p->rgb[(x+y*p->width)*3+2] =   47;
                }
                else
                {
                    p->rgb[(x+y*p->width)*3+0] =   183;
                    p->rgb[(x+y*p->width)*3+1] =   183;
                    p->rgb[(x+y*p->width)*3+2] = 183;
                }
                
                
            }
            else{
                p->rgb[(x+y*p->width)*3+0] =   183;
                p->rgb[(x+y*p->width)*3+1] =   183;
                p->rgb[(x+y*p->width)*3+2] = 183;
            }
        }
    }
}
void texture_destroy( Texture* p )
{
    free( p->rgb );
}

typedef struct {
    GLfloat pos[4];   // position
    GLfloat amb[4];   // ambient
    GLfloat dif[4];   // diffuse
    GLfloat spe[4];   // specular
} Light;

typedef struct {
    GLfloat amb[4];   // ambient
    GLfloat dif[4];   // diffuse
    GLfloat spe[4];   // specular
    GLfloat shi;    // shininess
} Material;
Light light = {
    { 15.0, 15.0, 15.0, 1.0 },   // position
    { 0.8, 0.8, 0.8, 0.8 }, // ambient
    { 1.0, 1.0, 1.0, 1.0 }, // diffuse
    { 1.0, 1.0, 1.0, 1.0 }, // specular
};
//0.05, 1.0, 0.05, 0.5 green
//1.0, 1.0, 0.05, 0.5 yello
//{ 0.05, 0.05, 1.00, 0.5 }, // bluey blue
//    { 0.05, 1.00, 1.00, 0.5 }, // sk
Material mat0 = {
    { 1.00, 0.05, 0.05, 0.5 }, // ambient
    { 0.8, 0.8, 0.8, 1.0 }, // diffuse
    { 1.0, 1.0, 1.0, 1.0 }, // specular
    { 100.0              }, // shininess
};

Material mat2 = {
    { 0.4, 0.4, 0.4, 1.0 }, // ambient
    { 0.8, 0.8, 0.8, 1.0 }, // diffuse
    { 1.0, 1.0, 0.0, 1.0 }, // specular
    { 100.0              }, // shininess
};

Material mat1 = {
    { 0.2, 0.8, 0.2, 1.0 }, // ambient
    { 0.0, 1.0, 0.0, 1.0 }, // diffuse
    { 1.0, 1.0, 0.0, 1.0 }, // specular
    { 100.0              }, // shininess
};
Material start = {
    { 1.0, 0.2, 0.2, 1.0 }, // ambient
    { 1.0, 0.0, 0.0, 1.0 }, // diffuse
    { 1.0, 1.0, 0.0, 1.0 }, // specular
    { 100.0              }, // shininess
};
Material end = {
    { 0.2, 0.2, 1.0, 1.0 }, // ambient
    { 0.0, 0.0, 1.0, 1.0 }, // diffuse
    { 1.0, 1.0, 0.0, 1.0 }, // specular
    { 100.0              }, // shininess
};

Material eye_boll = {
    { 0, 0, 0, 0.0 }, // ambient
    { 0.0, 0.0, 0.0, 1.0 }, // diffuse
    { 1.0, 1.0, 0.0, 1.0 }, // specular
    { 100.0              }, // shininess
};
//여기까지
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool mode = true;//true 는 맨위, false 가까이
//float  ex=8.5,ey=5,ez=8.5,ax=8.5, ay=0.0,az=8.5,ux=0.0,uy=0.0,uz=0.0;
float  ex=-2.0,ey=10.0,ez=0.0,   ax=5.0,ay=0.0,az=1.5,   ux=0.0,uy=0.0,uz=0.0;

FILE *fp;

static GLfloat theta = 0.0;// variable for rotation

int axis = 1;
float checkpoint_x = 3.2;
float checkpoint_z = 3.2;
float angle = 0;
int maze[15][15]; // 미로를 나타내는 변수  2는 벽 1은 길
bool checkroad[15][15];
int startX = 1, startY = 1; // 미로의 시작점입니다.
int endX = 13, endY = 13;     // 미로의 끝나는 지점을 나타냅니다.
int seeangle = 0 ;
int lookatpoint = 1; // 1 up 2 right 3 down 4 left
float jumpwallsize = 0; // 벽 점프 크기용
int jumpcount = 0;  // 애니매이션용
int endcount = 0;
int roatateandroid = 0; // 안드로이드 돌리기용
float height_android = 0;
// robot 관련 코드

static double time1 = 0;
static double time2 = 0;
static double time3 = 0;
static double time4 = 0;



bool checkkeyinput =false;
GLfloat robot_x = 0;
GLfloat robot_z = 0;
GLfloat control_x = 0;
GLfloat control_z = 0;


GLfloat R_Arm_x = 0;
GLfloat R_Arm_y = 0;
GLfloat L_Arm_x = 0;
GLfloat L_Arm_y = 0;
GLfloat R_Leg_x = 0;
GLfloat R_Leg_y = 0;
GLfloat L_Leg_x = 0;
GLfloat L_Leg_y = 0;
GLfloat R = 0;
GLfloat R2 = 0;

GLUquadricObj* cyl;

int a = 0;
int b = 0;
int c = 0;

static int flag = 0 ;
static int key =2;

#define RUN 1
#define JAP 2
#define ROCKET 3
#define YUNA 4
#define EXIT 6

// end 로봇관련 코드

void makewall(int,int);
void Run();
void Run2();
void Jump();
void endpoint();

void normal( GLfloat *n )
{
    GLfloat x = sqrt( n[0]*n[0] + n[1]*n[1] + n[2]*n[2] );
    if ( x > 0.0 ) for ( int i=0; i<3; i++ ) n[i] /= x;
}


void ShuffleArray(int array1[])
{
    int index, temp;
    //int random = rand() % 4 ;
    for (int i =3; i > 0; i--)
    {
        index = rand()%i;
        temp = array1[index];
        array1[index] = array1[i];
        array1[i] = temp;
    }
}

void routetop(int y,int x){
    // Log.i("TAG", "1+ "+count + " i , j :"+ y+ " "+ x);
    if (y - 1 != 0) { // 한칸위가 테두리라면 읽지않습니다. ( 테두리는 모두 벽입니다.)
        
        // 만약 한칸왼쪽이 한번도 보지않았던 길이고 (checkroad[y-1][x]) 벽이아니면 그쪽으로 이동합니다.
        if(maze[y-1][x ] == 1 && checkroad[y-1][x] == true){
            makewall(y-1, x);
        }
        else if (maze[y - 1][x] == 2) {
            
            // 만약 왼쪽에 붙어있다면 자기 위쪽과 오른쪽만 비교하고 길을 뚫어주고 한칸 이동합니다.
            if (x - 1 == 0) {
                if (maze[y - 1][x + 1] == 2 && maze[y - 2][x] == 2) {
                    maze[y - 1][x] = 1;
                    makewall(y - 1, x);
                }
            } else if (x + 1 == 14) {
                // 만약 오른쪽 붙어있다면 자기 위쪽과 왼쪽만 비교하고 길을 뚫어주고 한칸 이동합니다.
                if (maze[y - 1][x - 1] == 2 && maze[y - 2][x] == 2) {
                    maze[y - 1][x] = 1;
                    makewall(y - 1, x);
                }
            } else {
                // 만약 주변에 길이있다면  길을 뚫지 않고 다 벽이면 길을 뚫어서 한칸 위로 올라갑니다.
                if (maze[y - 1][x - 1] == 2 && maze[y - 1][x + 1] == 2 && maze[y - 2][x] == 2) {
                    maze[y - 1][x] = 1;
                    makewall(y - 1, x);
                }
            }
        }
    }
}
void routebottom(int y,int x){
    
    if (y + 1 != 14) { // 한칸 아래가 테두리라면 읽지않습니다.
        
        // 만약 한칸왼쪽이 한번도 보지않았던 길이고 (checkroad[y+1][x] == true) 벽이아니면 그쪽으로 이동합니다.
        if(maze[y+1][x] == 1 && checkroad[y+1][x] == true){
            makewall(y+1, x);
        }
        else if (maze[y + 1][x] == 2) {
            // 만약 왼쪽에 붙어있다면 자기 위쪽과 오른쪽만 비교하고 길을 뚫어주고 한칸 이동합니다.
            if (x - 1 == 0) {
                if (maze[y + 1][x + 1] == 2 && maze[y + 2][x] == 2) {
                    maze[y + 1][x] = 1;
                    makewall(y + 1, x);
                }
                
                
            } else if (x + 1 == 14) {
                // 만약 오른쪽 붙어있다면 자기 위쪽과 왼쪽만 비교하고 길을 뚫어주고 한칸 이동합니다.
                if (maze[y + 1][x - 1] == 2 && maze[y + 2][x] == 2) {
                    maze[y + 1][x] = 1;
                    makewall(y + 1, x);
                }
            } else {
                // 만약 주변에 길이있다면  길을 뚫지 않고 다 벽이면 길을 뚫어서 한칸 아래로 내려갑니다.
                if (maze[y + 1][x - 1] == 2 && maze[y + 1][x + 1] == 2 && maze[y + 2][x] == 2) {
                    maze[y + 1][x] = 1;
                    makewall(y + 1,x);
                }
            }
        }
    }
}
void routeright(int y,int x){
    
    if (x + 1 != 14) { // 오른쪽 한칸 오른쪽이 테두리라면 읽지않습니다.  (다시한번 테두리는 모두 벽입니다.)
        
        // 만약 한칸왼쪽이 한번도 보지않았던 길이고 ( checkroad[y][x+1] == true) 벽이아니면 그쪽으로 이동합니다.
        if(maze[y][x + 1] == 1 && checkroad[y][x+1] == true){
            makewall(y, x + 1);
        }
        else if (maze[y][x + 1] == 2) {
            // 만약 위쪽에 붙어있다면 자기 아래쪽, 오른쪽만 비교하고 길을 뚫어주고 한칸 이동합니다.
            if (y - 1 == 0) {
                if (maze[y + 1][x + 1] == 2 && maze[y][x + 2] == 2) {
                    maze[y][x + 1] = 1;
                    makewall(y, x + 1);
                }
            } else if (y + 1 == 14) {
                // 만약 아래쪽에 붙어있다면 자기 위쪽, 오른쪽만 비교하고 길을 뚫어주고 한칸 이동합니다.
                if (maze[y - 1][x + 1] == 2 && maze[y][x + 2] == 2) {
                    maze[y][x + 1] = 1;
                    makewall(y, x + 1);
                }
            } else {
                // 만약 주변에 길이있다면  길을 뚫지 않고 다 벽이면 길을 뚫어서 한칸 오른쪽으로 이동합니다.
                if (maze[y + 1][x + 1] == 2 && maze[y - 1][x + 1] == 2 && maze[y][x + 2] == 2) {
                    maze[y][x + 1] = 1;
                    makewall(y, x + 1);
                }
            }
        }
    }
}
void routeleft(int y,int x){
    
    if (x - 1 != 0) { // 한 칸 왼쪽이 테두리라면 읽지않습니다.
        
        // 만약 한칸왼쪽이 한번도 보지않았던 길이고 (checkroad[y][x-1] == true) 벽이아니면 그쪽으로 이동합니다.
        if(maze[y][x - 1] == 1 && checkroad[y][x-1] == true){
            makewall(y, x - 1);
        }
        else if (maze[y][x - 1] == 2) {
            if (y - 1 == 0) {
                // 만약 위쪽에 붙어있다면 자a기 아래쪽, 오른쪽만 비교하고 길을 뚫어주고 한칸 이동합니다.
                if (maze[y + 1][x - 1] == 2 && maze[y][x - 2] == 2) {
                    maze[y][x - 1] = 1;
                    makewall(y, x - 1);
                }
            } else if (y + 1 == 14) {
                // 만약 아래쪽에 붙어있다면 자기 위쪽, 오른쪽만 비교하고 길을 뚫어주고 한칸 이동합니다.
                if (maze[y - 1][x - 1] == 2 && maze[y][x - 2] == 2) {
                    maze[y][x - 1] = 1;
                    makewall(y, x - 1);
                }
            } else {
                // 만약 주변에 길이있다면 (maze[][] == 1)  길을 뚫지 않고 다 벽이면 ( == 2) 길을 뚫어서 한칸 왼쪽으로 이동합니다.
                if (maze[y + 1][x - 1] == 2 && maze[y - 1][x - 1] == 2 && maze[y][x - 2] == 2) {
                    maze[y][x - 1] = 1;
                    makewall(y, x - 1);
                }
            }
        }
    }
}





void makewall(int y, int x) {
    
    // 지나간길을 다시 지나가지않기위해 checkroad를 사용합니다.
    checkroad[y][x] = false;
    
    // 어느 방면의 길부터 탐색할지 랜덤으로 결정합니다.
    // 이 부분을 안할시 미로가 생기긴해도 너무 규칙적인 미로가 나오므로 랜덤으로 돌려줬습니다.
    int solutionArray[4] = { 1, 2, 3, 4};
    ShuffleArray(solutionArray);
    
    // 섞인 순서대로 방향을 찾아봅니다.
    for(int i = 0 ; i< 4 ; i++){
        if(solutionArray[i] == 1){ // 1일시 먼저 위쪽 길을 확인합니다.
            routetop(y ,x); // 북쪽
        }
        else if(solutionArray[i] == 2){// 2일시 먼저 오른쪽 길을 확인합니다.
            routeright(y,x); // 오른쪽
        }
        else if(solutionArray[i] == 3){ // 3일시 먼저 아래쪽 길을 확인합니다.
            routebottom(y ,x); // 아래쪽
        }
        else{ // 4일시 먼저 왼쪽 길을 확인합니다.
            routeleft(y ,x); // 왼쪽
        }
    }
    return ;
}


//미로를 만들어줍니다.
void CreateMaze() {
    int r; // r을 이용해서 랜덤을 받고 미로에서 벽인지 길인지를 구분합니다.
    int i, j;
    //먼저 전부 벽으로 한번 초기화 시켜줍니다.   maze[p][q] 가  2 면 벽 1이면 길입니다.;
    for (int p = 0; p < 15; p++) {
        for (int q = 0; q < 15; q++) {
            maze[p][q] = 2;
            checkroad[p][q] = false;
            
        }
    }
    
    //시작지점과 끝지점을 이어주는 길 하나를 만듭니다.
    maze[startY][startX] = 1;
    maze[endY][endX] = 1;
    i = 1;
    j = 1;
    
    
    while (true) {
        r = rand()%2;
        // 마지막 부분에 도착시 loop를 끝냅니다.
        if (i == endY && j == endX) {
            break;
        }
        if (r == 0) {
            if (i < 13) {
                
                i++;
                // 길인 부분을 로 maze[i][j] == 1 (길) 그리고 wasHere를 true로 바꿔줍니다.
                maze[i][j] = 1;
                checkroad[i][j] = true;
            }
        } else {
            if (j < 13) {
                // 길인 부분을 로 maze[i][j] == 1 (길) 그리고 wasHere를 true로 바꿔줍니다.
                j++;
                maze[i][j] = 1;
                checkroad[i][j] = true;
            }
        }
    }
    // 벽을 미로답게 만들어줍니다.
    makewall(1, 1);
    
}



void init()
{
    glMatrixMode(GL_PROJECTION);// change to projection view
    glClearColor(1.0, 1.0, 1.0, 1.0);
    
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
    glColor3f   ( 0.5, 0.5, 0.5      );
    glEnable(GL_DEPTH_TEST);
    //glMatrixMode(GL_MODELVIEW);// restore to model view
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();// load identity matrix
    if(mode)
        glOrtho(-1, 2.0, -1.0, 2.0, -1.0, 50.0);
    else
        glOrtho(-1, 30.0, -1.0, 31.0, -1.0, 31.0);
    
    
    ///////////////////////////////////////////////////////////////////////////
    //여기부터
    
    glEnable( GL_TEXTURE_2D );
    Texture tex;
    texture_create      ( &tex, 600, 600 );
    texture_checkerboard( &tex,  32,  32 );
    glTexImage2D  ( GL_TEXTURE_2D, 0, GL_RGB, tex.width, tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex.rgb );
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D,0x2800,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,0x2801,GL_LINEAR);
    
    /////////////////////////////////////////////////////여기까지
    
    
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();// load identity matrix
    glLightfv( GL_LIGHT0, GL_POSITION, light.pos );
    glLightfv( GL_LIGHT0, GL_AMBIENT,  light.amb );
    glLightfv( GL_LIGHT0, GL_DIFFUSE,  light.dif );
    glLightfv( GL_LIGHT0, GL_SPECULAR, light.spe );
    
    glShadeModel( GL_SMOOTH   );
    glEnable    ( GL_LIGHTING );
    glEnable    ( GL_LIGHT0   );
    
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//여기부터
void polygon(int a, int b, int c, int d)
{
    glBegin(GL_POLYGON);
    static GLfloat texcoord[4][2] = { { 0, 1 }, { 1, 1 }, { 1, 0 }, { 0, 0 } };
    GLfloat nor[3];
    
    nor[0] = ( vertices[a][0] + vertices[b][0] + vertices[c][0] + vertices[d][0] );
    nor[1] = ( vertices[a][1] + vertices[b][1] + vertices[c][1] + vertices[d][1] );
    nor[2] = ( vertices[a][2] + vertices[b][2] + vertices[c][2] + vertices[d][2] );
    normal( nor );
    
    glBegin(GL_POLYGON);
    glNormal3fv( nor         );
    glColor3fv(colors[a]);
    glTexCoord2fv( texcoord[0] );
    glVertex3fv( vertices[a] );
    glColor3fv(colors[b]);
    glTexCoord2fv( texcoord[1] );
    glVertex3fv( vertices[b] );
    glColor3fv(colors[c]);
    glTexCoord2fv( texcoord[2] );
    glVertex3fv( vertices[c] );
    glColor3fv(colors[d]);
    glTexCoord2fv( texcoord[3] );
    glVertex3fv( vertices[d] );
    glEnd();
}
void polygon2(int a, int b, int c, int d)
{
    glBegin(GL_POLYGON);
    static GLfloat texcoord[4][2] = { { 0, 1 }, { 1, 1 }, { 1, 0 }, { 0, 0 } };
    GLfloat nor[3];
    
    
    nor[0] = ( vertices[a][0] + vertices[b][0] + vertices[c][0] + vertices[d][0] );
    nor[1] = ( vertices[a][1] + vertices[b][1] + vertices[c][1] + vertices[d][1] );
    nor[2] = ( vertices[a][2] + vertices[b][2] + vertices[c][2] + vertices[d][2] );
    normal( nor );
    
    glBegin(GL_POLYGON);
    glNormal3fv( nor         );
    glColor3fv(colors[a]);
    
    glVertex3fv( vertices[a] );
    glColor3fv(colors[b]);
    
    glVertex3fv( vertices[b] );
    glColor3fv(colors[c]);
    
    glVertex3fv( vertices[c] );
    glColor3fv(colors[d]);
    
    glVertex3fv( vertices[d] );
    glEnd();
}
//여기까지
//////////////////////////////////////////////////////////////////////////////////////


void cube()
{
    polygon(0, 3, 2, 1);
    polygon2(2, 3, 7, 6);
    polygon(0, 4, 7, 3);
    polygon(1, 2, 6, 5);
    polygon(4, 5, 6, 7);
    polygon(0, 1, 5, 4);
}
void floor(){
    
    polygon2(0, 3, 2, 1);
    polygon2(2, 3, 7, 6);
    polygon2(0, 4, 7, 3);
    polygon2(1, 2, 6, 5);
    polygon2(4, 5, 6, 7);
    polygon2(0, 1, 5, 4);
}

void display()
{
    //기울기 시간이벤트로 360
    
    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();// load identity matri
    // gluPerspective(5,1.0,-10.0,10.0);
    //
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();// load identity matrix
    
    
    if(mode == true)
        gluLookAt(ex,ey,ez,  ax,ay,az,    0.0,1.0, 0.0);
    else
        gluLookAt(0,10,0,  1.0,0,0,    0.0,1.0, 0.0);
    
    
    //0.7 1.4
    //glTranslatef(2 , 0.0, 1.5 );
    for(int i=0;i<15;i++)
    {
        for(int j=0;j<15;j++){
            ////////////////////////////////////////////////////////////////////////여기부터
            glTranslatef(2, 0.0, 0.0 );
            
            if(i==1 && j == 1){
                glMaterialfv( GL_FRONT, GL_AMBIENT,   start.amb );
                glMaterialfv( GL_FRONT, GL_DIFFUSE,   start.dif );
                glMaterialfv( GL_FRONT, GL_SPECULAR,  start.spe );
                glMaterialf ( GL_FRONT, GL_SHININESS, start.shi );
            }
            else if(i== 13 && j == 13){
                glMaterialfv( GL_FRONT, GL_AMBIENT,   end.amb );
                glMaterialfv( GL_FRONT, GL_DIFFUSE,   end.dif );
                glMaterialfv( GL_FRONT, GL_SPECULAR,  end.spe );
                glMaterialf ( GL_FRONT, GL_SHININESS, end.shi );
            }
            else{
                glMaterialfv( GL_FRONT, GL_AMBIENT,   mat0.amb );
                glMaterialfv( GL_FRONT, GL_DIFFUSE,   mat0.dif );
                glMaterialfv( GL_FRONT, GL_SPECULAR,  mat0.spe );
                glMaterialf ( GL_FRONT, GL_SHININESS, mat0.shi );
            }
            ////////////////////////////////////////////////////////////////////////여기까지
            
            
            floor();
        }
        glTranslatef( -30.0, 0.0, 2 );
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    glTranslatef( 0.0, 2, -30.0 );//2층시작
    
    
    
    int blink=1;
    for(int i=0;i<15;i++)
    {
        for(int j=0;j<15;j++){
            if(maze[i][j] == 2 ){
                glTranslatef( 2, 0.0, 0.0 );
                
                glMaterialfv( GL_FRONT, GL_AMBIENT,   mat2.amb );
                glMaterialfv( GL_FRONT, GL_DIFFUSE,   mat2.dif );
                glMaterialfv( GL_FRONT, GL_SPECULAR,  mat2.spe );
                glMaterialf ( GL_FRONT, GL_SHININESS, mat2.shi );
                
                cube();
                
            }
            else
                glTranslatef( 2, 0.0, 0.0 );
            
        }
        glTranslatef( -30, 0.0, 2 );
        blink=1;
    }
    glTranslatef( 4, 0, -28 );// 로봇위치 조정위해서  현재 를 1.5 1.5라고 잡는다.
    glTexParameterf(GL_TEXTURE_2D,0,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,0,GL_LINEAR);
    
    
    
    glMaterialfv( GL_FRONT, GL_AMBIENT,   mat1.amb );
    glMaterialfv( GL_FRONT, GL_DIFFUSE,   mat1.dif );
    glMaterialfv( GL_FRONT, GL_SPECULAR,  mat1.spe );
    glMaterialf ( GL_FRONT, GL_SHININESS, mat1.shi );
    if(key == RUN){
        glTranslated( control_x, 0,  control_z);
        Run();
        glPopMatrix();
    }
    else if(key == 2){
        glTranslated( control_x, 0, control_z);
        Run2();
        glPopMatrix();
    }
    else if(key == 3){
        if(jumpcount < 10){
            glTranslated( control_x, height_android, control_z);
            Jump();
            glPopMatrix();
            
            
        }
        else if(jumpcount >20){
            glTranslated( control_x, height_android, control_z);
            Jump();
            glPopMatrix();
            if(jumpcount == 30){
                jumpcount = 0;
                key = 2;
                jumpwallsize = 0;
                roatateandroid =0;
                checkkeyinput = false;
            }
            
        }
        else{
            if(lookatpoint == 1){
                control_x += jumpwallsize/10;
                ex+= jumpwallsize/10;
                ax+= jumpwallsize/10;
            }
            else if(lookatpoint == 2){
                control_z += jumpwallsize/10;
                ez += jumpwallsize/10;
                az += jumpwallsize/10;
                
            }
            else if(lookatpoint == 3){
                control_x -= jumpwallsize/10;
                ex -= jumpwallsize/10;
                ax -= jumpwallsize/10;
            }
            else{
                control_z -= jumpwallsize/10;
                ez -= jumpwallsize/10;
                az -= jumpwallsize/10;
                
            }
            glTranslated( control_x, 1.5, control_z);
            Jump();
            glPopMatrix();
            
        }
    }
    else if(key == 4){
        glTranslated( control_x, 1.5, control_z);
        endpoint();
        glPopMatrix();
    }
    
    glFlush();
    glutSwapBuffers();
    
}
void keyboard_handler(unsigned char keyf, int x, int y)
{
    
    if(keyf == 'm')
    {
        if(mode){
            mode=false;
            light.pos[3]=0.3;
            mat2.amb[0]=0.8;
            mat2.amb[1]=0.8;
            mat2.amb[2]=0.8;
            mat2.amb[3]=1;
        }
        else{
            mode=true;
            light.pos[3]=1.0;
            mat2.amb[0]=0.4;
            mat2.amb[1]=0.4;
            mat2.amb[2]=0.4;
            mat2.amb[3]=1.0;
        }
        init();
        glutPostRedisplay();
        
    }
    
    else if(keyf == 'e'){  // jump 입력키 만약 바로 옆에 벽이있다면 실행
        if(key != 3){
            checkkeyinput = true;
            jumpwallsize =0;
            if(lookatpoint == 1){
                if(maze[(int)(checkpoint_x + control_z )/2][(int)(checkpoint_z + control_x + 0.5)/2] == 2){
                    int p;
                    
                    for(p = (int)(checkpoint_z + control_x + 0.5)/2 ; p < 15 ; p++){
                        if(p == 14){
                            jumpwallsize =0;
                            key =2;
                        }
                        else if(maze[(int)(checkpoint_x + control_z )/2][p] == 2){
                            jumpwallsize += 2;
                        }
                        else{
                            jumpwallsize++;
                            key = 3;
                            break;
                        }
                    }
                    
                }
            }
            else if(lookatpoint == 2){ // j가 위로가면서 그린다.
                if(maze[(int)(checkpoint_z + control_z + 0.5)/2][(int)(checkpoint_x + control_x )/2]  == 2){
                    int p;
                    
                    for(p = (int)(checkpoint_z + control_z + 0.5)/2 ; p < 15 ; p++){
                        if(p == 14){
                            jumpwallsize =0;
                            key =2;
                        }
                        else if(maze[p][(int)(checkpoint_x + control_x )/2]  == 2){
                            jumpwallsize += 2;
                        }
                        else{
                            jumpwallsize++;
                            key =3;
                            break;
                        }
                        
                    }
                    
                }
                
            }
            else if(lookatpoint == 3){
                if(maze[(int)(checkpoint_x + control_z )/2][(int)(checkpoint_z + control_x - 0.5)/2]  == 2){
                    int p;
                    
                    for(p = (int)(checkpoint_z + control_x - 0.5)/2 ; p >= 0 ; p--){
                        if(p == 0){
                            jumpwallsize =0;
                            key =2;
                        }
                        else if(maze[(int)(checkpoint_x + control_z )/2][p] == 2){
                            jumpwallsize += 2;
                        }
                        else{
                            jumpwallsize++;
                            key =3;
                            break;
                        }
                        
                    }
                    
                }
            }
            else{
                if(maze[(int)(checkpoint_x + control_z -0.5)/2][(int)(checkpoint_z + control_x )/2]  == 2){
                    int p;
                    
                    for(p = (int)(checkpoint_x + control_z -0.5)/2 ; p >= 0 ; p--){
                        if(p == 0 ){
                            jumpwallsize =0;
                            key =2;
                        }
                        else if(maze[p][(int)(checkpoint_z + control_x )/2] == 2){
                            jumpwallsize += 2;
                        }
                        else{
                            key =3;
                            jumpwallsize++;
                            break;
                            
                        }
                    }
                }
            }
            
            
        }
    }
    
    else if(keyf == 'j') // left
    {
        if(!checkkeyinput){
            //left input
            printf("왼쪽 %d\n",lookatpoint);
            checkkeyinput = true;
            seeangle = 90;
            lookatpoint = 4;
            
            key =1;
        }
        
    }
    else if(keyf == 'k') // down
    {
        
        if(!checkkeyinput){
            
            checkkeyinput = true;
            seeangle =180;
            lookatpoint = 3;
            printf("아래쪽 %d\n",lookatpoint);
            //down input
            key =1;
        }
        
    }
    else if(keyf == 'l') // right
    {
        if(!checkkeyinput){
            // right input
            
            checkkeyinput = true;
            seeangle = -90;
            lookatpoint = 2;
            printf("오른쪽  %d\n", lookatpoint);
            key =1;
        }
        
        
        
    }
    else if(keyf == 'i')
    {
        
        if(!checkkeyinput){
            // input i up
            
            checkkeyinput = true;
            seeangle = 0;
            lookatpoint = 1;
            key =1;
            printf("위쪽 %d\n",lookatpoint);
        }
        
    }
    printf("%d \n",key);
    
}

void keyUp (unsigned char keyf, int x, int y) {
    if((int)keyf == 106)
    {
        if(key != 3 && key != 4){
            //left input
            key =2;
            lookatpoint = 4;
            checkkeyinput = false;
        }
        
    }
    else if((int)keyf == 107)
    {
        if(key != 3 && key != 4){
            //down input
            key =2;
            lookatpoint = 3;
            checkkeyinput = false;
        }
    }
    else if((int)keyf == 108)
    {
        if(key != 3 && key != 4){
            // right input
            key =2;
            lookatpoint = 2;
            
            checkkeyinput = false;
        }
    }
    else if((int)keyf == 105)
    {
        if(key != 3 && key != 4){
            // input i up
            key =2;
            lookatpoint = 1;
            checkkeyinput = false;
        }
    }
}

// start robot



void glInit(){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    GLfloat ambientLight[] = {0.3f , 0.3f , 0.3f ,1.0f};
    GLfloat diffuseLight[] = {0.7f ,0.7f ,0.7f ,1.0f};
    GLfloat specular[] = {1.0f ,1.0f ,1.0f ,1.0f };
    GLfloat specref[] = {1.0f ,1.0f ,1.0f ,1.0f };
    GLfloat position[] = {400.0 , 300.0, -700.0, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT , ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE , diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR , specular);
    glLightfv(GL_LIGHT0, GL_POSITION ,position );
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMateriali(GL_FRONT, GL_SHININESS, 128);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5, 5.0, -0.5, 0.5, -0.5, 0.5);
}

void Draw_Color(int i){ // 색상을 그리는 함수
    if(i == RUN){
        glColor3f(0.0, 1.0 ,0.0);
    }
    else if(i == JAP){
        glColor3f(1.0, 1.0 ,0.0);
    }
    else if(i == ROCKET){
        glColor3f(0.0, 1.0 ,1.0);
    }
    else if(i == YUNA){
        glColor3f(1.0, 0.0 ,1.0);
    }
    else if(i == EXIT){
        glColor3f(0.2, 0.2 ,0.2);
    }
    else if(i == 7)
        glColor3f(0.0, 1.0 ,0.0);
}

void Change_Wire_Or_Solid(int i){
    if(flag == 1)
        gluQuadricDrawStyle(cyl, GLU_LINE);
}


//로봇 왼팔을 그리는 함수

void DrawL_Arm(int x,int a ,int b,int c){
    glPushMatrix();
    Draw_Color(key);
    cyl = gluNewQuadric();
    glRotated(x, a, b, c);
    glRotated(90.0, 1.0, 0.0, 0.0);
    
    glTranslatef(0.25 ,0.0 ,0.0);
    glRotated(15.0, 0.0, 1.0, 0.0);
    Change_Wire_Or_Solid(flag);
    gluCylinder(cyl, 0.05, 0.05, 0.2, 50, 1);  //실린더를 그림
}

void DrawL_Hand(int y,int a, int b, int c){
    glPushMatrix();
    Draw_Color(key);
    cyl = gluNewQuadric();
    glTranslatef(0.0, 0.0, 0.22);
    glRotated(y, a, b, c);
    Change_Wire_Or_Solid(flag);
    if(key == YUNA) gluCylinder(cyl, 0.05, 0.02, 0.2, 15, 1);
    else gluCylinder(cyl, 0.05, 0.05, 0.2, 15, 1);
    glPopMatrix();
}


void DrawL_HandRocket(){
    glPushMatrix();
    Draw_Color(key);
    cyl = gluNewQuadric();
    glTranslatef(0, 0, R);
    Change_Wire_Or_Solid(flag);
    gluCylinder(cyl, 0.05, 0.05, 0.2, 50, 1);
    glPopMatrix();
}

void DrawR_Arm(int x,int a,int b,int c){
    glPushMatrix();
    Draw_Color(key);
    cyl = gluNewQuadric();
    glRotated(x, a, b, c);
    glRotated(90.0, 1.0, 0.0, 0.0);
    glTranslatef(-0.25, 0.0, 0.0);
    glRotated(-15.0, 0.0, 1.0, 0.0);
    Change_Wire_Or_Solid(flag);
    gluCylinder(cyl, 0.05, 0.05, 0.2, 50, 1);
}

void DrawR_Hand(int y,int a,int b,int c){
    glPushMatrix();
    Draw_Color(key);
    cyl = gluNewQuadric();
    glTranslatef(0.0,0.0,0.22);
    glRotated(y, a, b, c);
    Change_Wire_Or_Solid(flag);
    if(key == YUNA) gluCylinder(cyl, 0.05, 0.02, 0.2, 50, 1);
    else gluCylinder(cyl, 0.05, 0.05, 0.2, 50, 1);
    glPopMatrix();
}


void DrawR_HandRocket(){
    glPushMatrix();
    Draw_Color(key);
    cyl = gluNewQuadric();
    glTranslatef(0 , 0 , R2);
    Change_Wire_Or_Solid(flag);
    gluCylinder(cyl, 0.05, 0.05, 0.2, 50, 1);
    glPopMatrix();
}


void Drawbody(int x ,int a,int b,int c){
    glPushMatrix();
    Draw_Color(key);
    cyl = gluNewQuadric();
    glRotated(90.0, 1.0, 0.0, 0.0);
    glRotated(x, a, b, c);
    Change_Wire_Or_Solid(flag);
    gluCylinder(cyl, 0.2, 0.2, 0.45, 50, 1);
    glPopMatrix();
}


void DrawL_Legs(int x,int a,int b,int c){
    glPushMatrix();
    Draw_Color(key);
    cyl = gluNewQuadric();
    glRotated(90.0, 1.0, 0.0, 0.0);
    glTranslatef(0.1 , 0.0, 0.42);
    glRotated(x, a, b, c);
    Change_Wire_Or_Solid(flag);
    gluCylinder(cyl, 0.05, 0.05, 0.15, 50, 1);
}

void DrawL_foot(int y,int a,int b,int c){
    glPushMatrix();
    Draw_Color(key);
    cyl = gluNewQuadric();
    glTranslatef(0.0, 0.0, 0.18);
    glRotated(y, a, b, c);
    Change_Wire_Or_Solid(flag);
    if(key == YUNA) gluCylinder(cyl, 0.05, 0.03, 0.2,50 , 1);
    else gluCylinder(cyl, 0.05, 0.05, 0.2, 50, 1);
    glPopMatrix();
}


void DrawR_Legs(int x,int a,int b,int c){
    
    glPushMatrix();
    Draw_Color(key);
    cyl = gluNewQuadric();
    glRotated(90.0, 1.0, 0.0, 0.0);
    glTranslatef(-0.1, 0.0, 0.42);
    glRotated(x, a, b, c);
    Change_Wire_Or_Solid(flag);
    gluCylinder(cyl, 0.05, 0.05, 0.15, 50, 1);
}


void DrawR_foot(int y,int a ,int b,int c){
    glPushMatrix();
    Draw_Color(key);
    cyl = gluNewQuadric();
    glTranslatef(0.0, 0.0, 0.18);
    glRotated(y, a, b, c);
    Change_Wire_Or_Solid(flag);
    if(key) gluCylinder(cyl, 0.05, 0.03, 0.2, 15, 1);
    else gluCylinder(cyl, 0.05, 0.05, 0.2, 15, 1);
    glPopMatrix();
    
}

void DrawNeck(){
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    cyl = gluNewQuadric();
    glRotated(90.0, 1.0, 0.0, 0.0);
    
    glTranslatef(0.0, 0.0, -0.045);
    Change_Wire_Or_Solid(flag);
    gluCylinder(cyl, 0.2, 0.2, 0.025, 100, 1);
    glPopMatrix();
}

void DrawGroung(){
    Draw_Color(flag);
    glTranslatef(0.0, -2.73, 0.0);
    glRotated(45, 0.0, 1.0, 0.0);
    glRotated(0.0+time4 -15, 0.0, 0.0, 1);
    Change_Wire_Or_Solid(flag);
    gluSphere(cyl, 2.5, 30, 90);
    
}

void DrawHead(){
    glTranslatef(0.0 , 0.02 , 0.0);
    glPushMatrix();
    cyl = gluNewQuadric();
    Change_Wire_Or_Solid(flag);
    Draw_Color(key);
    gluSphere(cyl, 0.20, 30, 10);
    
    
    //왼쪽 뿔 그리기
    glRotated(90.0, 1.0, 0.0, 0.0);
    
    glTranslatef(-0.16, 0.0,-0.22);
    glRotated(35.0, 0.0, 1.0, 0.0);
    gluCylinder(cyl, 0.005, 0.008, 0.1, 3, 1);
    glPopMatrix();
    glPushMatrix();
    
    //오른쪽 뿔 그리기
    glRotated(90.0, 1.0, 0.0, 0.0);
    
    glTranslatef(0.16, 0.0,-0.22);
    glRotated(-35.0, 0.0, 1.0, 0.0);
    gluCylinder(cyl, 0.005, 0.008, 0.1, 3, 1);
    glPopMatrix();
    glPushMatrix();
    
    
    //왼쪽 눈 그리기
    glTranslatef(-0.1 , 0.1, 0.13);
    glColor3f(0.0, 0.0, 0.0);
    glMaterialfv( GL_FRONT, GL_AMBIENT,   eye_boll.amb );
    glMaterialfv( GL_FRONT, GL_DIFFUSE,   eye_boll.dif );
    glMaterialfv( GL_FRONT, GL_SPECULAR,  eye_boll.spe );
    glMaterialf ( GL_FRONT, GL_SHININESS, eye_boll.shi );
    gluSphere(cyl, 0.03, 10, 10);
    glPopMatrix();
    glPushMatrix();
    
    //오른쪽눈 그리기
    glTranslatef(0.1 , 0.1, 0.13);
    gluSphere(cyl, 0.03, 10, 10);
    glPopMatrix();
    glMaterialfv( GL_FRONT, GL_AMBIENT,   mat1.amb );
    glMaterialfv( GL_FRONT, GL_DIFFUSE,   mat1.dif );
    glMaterialfv( GL_FRONT, GL_SPECULAR,  mat1.spe );
    glMaterialf ( GL_FRONT, GL_SHININESS, mat1.shi );
    
}

void DrawAndroid(){
    Drawbody(0, 0, 0, 0);
    DrawNeck();
    DrawHead();
    DrawR_Arm(R_Arm_x, 1.0, 0.0, 0.0);
    if(key == ROCKET) DrawR_HandRocket();
    else DrawR_Hand(R_Arm_y, 1, 0, 0);
    DrawL_Arm(L_Arm_x, 1, 0, 0);
    if(key == ROCKET) DrawL_HandRocket();
    else DrawL_Hand(L_Arm_y, 1, 0, 0);
    
    DrawL_Legs(L_Leg_x, 1, 0, 0);
    DrawL_foot(L_Leg_y, 1, 0, 0);
    DrawR_Legs(R_Leg_x, 1, 0, 0);
    DrawR_foot(R_Leg_y, 1, 0, 0);
    
}


void Run(){
    //glLoadIdentity();
    glRotatef(-30+ seeangle, 0, 1, 0);
    // glRotated(90, 1, 1, 0);
    glColor3f(0.0, 1.0 ,0.0);
    L_Arm_x = sin(time1)*80;
    R_Arm_x = -L_Arm_x;
    R_Arm_y = -abs(sin(time1)*60+50);
    
    L_Arm_y = -abs(-sin(time1)*60+50);
    R_Arm_y = abs(-sin(time1)*30 -30);
    L_Leg_y = abs(sin(time1)*30 -30);
    R_Leg_x = sin(time1)*60;
    L_Leg_x = -R_Leg_x;
    
    
    cyl =gluNewQuadric();
    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //glMatrixMode(GL_MODELVIEW);
    
    // DrawGroung();
    //glLoadIdentity();
    
    glRotatef(-230.0, 0, 1, 0);
    glRotatef(abs(sin(time1)*16),1,0,0);
    glRotatef(sin(time1)*16, 0, 1, 0);
    
    
    float i =0 ;
    i = abs(sin(time1)*0.08);
    
    glPushMatrix();
    //gluLookAt( 0, -0.9, 0, 0, 0, 0, 0, 0, 0 );
    glTranslatef(0.0, i, 0);
    glTranslatef(0.0,0.5,0.0);
    DrawAndroid();
    // glutSwapBuffers();
    
}
void Run2(){
    //glLoadIdentity();
    glRotatef(-30 + seeangle, 0, 1, 0);
    L_Arm_x = 0;
    R_Arm_x = 0;
    R_Arm_y = 0;
    
    L_Arm_y = 0;
    R_Arm_y = 0;
    L_Leg_y = 0;
    R_Leg_x = 0;
    L_Leg_x = 0;
    
    
    cyl =gluNewQuadric();
    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    // glMatrixMode(GL_MODELVIEW);
    
    //DrawGroung();
    // glLoadIdentity();
    
    glRotatef(-230.0, 0, 1, 0);
    glRotatef(abs(sin(time1)*16),1,0,0);
    glRotatef(sin(time1)*16, 0, 1, 0);
    
    
    float i =0 ;
    i = abs(sin(time1)*0.08);
    
    glPushMatrix();
    //gluLookAt( 0, -0.9, 0, 0, 0, 0, 0, 0, 0 );
    glTranslatef(0.0, i, 0);
    glTranslatef(0.0,0.5,0.0);
    DrawAndroid();
    // glutSwapBuffers();
    
}

void Jump(){
    //glLoadIdentity();
    glRotatef(-30 + seeangle + roatateandroid, 0, 1, 0);
    L_Arm_x = 0;
    R_Arm_x = 0;
    R_Arm_y = 0;
    
    L_Arm_y = 0;
    R_Arm_y = 0;
    L_Leg_y = 0;
    R_Leg_x = 0;
    L_Leg_x = 0;
    
    
    cyl =gluNewQuadric();
    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    // glMatrixMode(GL_MODELVIEW);
    
    //DrawGroung();
    // glLoadIdentity();
    
    glRotatef(-230.0, 0, 1, 0);
    glRotatef(abs(sin(time1)*16),1,0,0);
    glRotatef(sin(time1)*16, 0, 1, 0);
    
    
    float i =0 ;
    i = abs(sin(time1)*0.08);
    
    glPushMatrix();
    //gluLookAt( 0, -0.9, 0, 0, 0, 0, 0, 0, 0 );
    glTranslatef(0.0, i, 0);
    glTranslatef(0.0,0.5,0.0);
    DrawAndroid();
    // glutSwapBuffers();
    
}

void endpoint(){
    //glLoadIdentity();
    glRotatef(-30+ seeangle + roatateandroid, 0, 1, 0);
    // glRotated(90, 1, 1, 0);
    glColor3f(0.0, 1.0 ,0.0);
    L_Arm_x = 180;
    R_Arm_x = L_Arm_x;
    R_Arm_y = -abs(180);
    L_Arm_y = -abs(180);
    
    R_Leg_y = abs(sin(time1)*30 -10);
    L_Leg_y = abs(sin(time1)*30 -10);
    R_Leg_x = sin(time1)*20;
    L_Leg_x = R_Leg_x;
    
    
    cyl =gluNewQuadric();
    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //glMatrixMode(GL_MODELVIEW);
    
    // DrawGroung();
    //glLoadIdentity();
    
    glRotatef(-230.0, 0, 1, 0);
    glRotatef(abs(sin(time1)*16),1,0,0);
    glRotatef(sin(time1)*16, 0, 1, 0);
    
    
    float i =0 ;
    i = abs(sin(time1)*0.08);
    
    glPushMatrix();
    //gluLookAt( 0, -0.9, 0, 0, 0, 0, 0, 0, 0 );
    glTranslatef(0.0, i, 0);
    glTranslatef(0.0,0.5,0.0);
    DrawAndroid();
    // glutSwapBuffers();
    
}

void MyTimer(int Value) {
    if(key == 4){
        checkkeyinput = true;
        endcount++;
        time2 += 0.3;
        roatateandroid += 20;
        if(endcount > 5){
            
            control_x -= 0.15;
            control_z -= 0.15;
            ex-=0.15;
            ax-=0.15;
            ez-=0.15;
            az-=0.15;
            printf("%f   ",control_x);
            printf("%f \n",control_z);
            
        }
        if(control_x  < 0.5){
            CreateMaze();
            ex=-2.0;ey=10.0;ez=0.0;   ax=5.0;ay=0.0;az=1.5;
            roatateandroid = 0;
            key = 2;
            checkkeyinput = false;
            endcount = 0;
            time2 = 0;
            control_z = 0 ; control_x = 0;
            
        }
    }
    
    else if(key == 3 ){
        jumpcount++;
        roatateandroid += 20;
        if(jumpcount < 10)
            height_android += 0.15;
        else if(jumpcount > 20)
            height_android -= 0.15;
        
    }
    else{
        time1 = time1 + 0.1;
        if((int)(checkpoint_x + control_z )/2 == 13 && (int)(checkpoint_z + control_x)/2 == 13 ){
            key =4;
        }
        else{
            
            if(checkkeyinput){
                if(lookatpoint == 1){ // 위쪽  i , j  j 가 위로가면서 그린다.
                    if(maze[(int)(checkpoint_x + control_z )/2][(int)(checkpoint_z + control_x + 0.5)/2]  == 2){
                        checkkeyinput = false;
                    }
                    else{
                        control_x += 0.3;
                        ex+=0.3;
                        ax+=0.3;
                    }
                    
                }
                else if(lookatpoint == 2){ // 오른쪽
                    if(maze[(int)(checkpoint_x + control_z + 0.5)/2][(int)(checkpoint_z + control_x )/2]  == 2){
                        checkkeyinput = false;
                    }
                    else{
                        control_z += 0.3;
                        ez+=0.3;
                        az+=0.3;
                        
                    }
                    
                }
                else if(lookatpoint == 3){ // 아래쪽
                    if(maze[(int)(checkpoint_x + control_z )/2][(int)(checkpoint_z + control_x - 0.5)/2]  == 2){
                        checkkeyinput = false;
                    }
                    else{
                        control_x -= 0.3;
                        ex-=0.3;
                        ax-=0.3;
                    }
                    
                }
                else if(lookatpoint == 4){ // 왼쪽
                    if(maze[(int)(checkpoint_x + control_z -0.5)/2][(int)(checkpoint_z + control_x )/2]  == 2){
                        checkkeyinput = false;
                    }
                    else{
                        control_z -= 0.3;
                        az-=0.3;
                        ez-=0.3;
                    }
                }
            }
            
        }
    }
    
    //display();
    glutPostRedisplay();
    glutTimerFunc(40,MyTimer ,1);
    
}

void MyMainMenu(int entryID){
    key = entryID;
}




//end robot


int main(int argc, char* argv[])
{
    
    CreateMaze();
    for(int i  = 0 ;i < 15 ; i++){
        for(int  j = 0 ; j < 15 ; j++){
            printf("%d ",maze[i][j]);
            
        }
        printf("\n");
    }
    glutInit(&argc, (char**)argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(700, 800);
    glutCreateWindow("Maze");
    glutKeyboardFunc(keyboard_handler);// add keyboard handler
    glutKeyboardUpFunc(keyUp);
    glutDisplayFunc(display);
    
    // glutIdleFunc(MyTimer);
    glutDisplayFunc(display);
    glutTimerFunc(40, MyTimer, 1);
    init();
    glutMainLoop();
    return 0;
}