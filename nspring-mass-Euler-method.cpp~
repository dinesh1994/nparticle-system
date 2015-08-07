#include <GL/freeglut.h>
#include <GL/gl.h>

#include "Polygon.h"
#include <cmath>
#define BOXSIZE 3
#include <cmath>
using namespace std;

//Globals
int ww=500,wh=500;
bool manualDraw = true;
bool picking = false;
bool moving = false;
bool simulation = false;
float K = 0.01;
float L = 100.0;
float STEPSIZE = 0.1;
float energyLoss = 0.8;
float u = 0.1;                  //coefficient of friction
float camx=0.0,camy=0.0,camz=0.0,lx=0.0,ly=0.0,lz=-1.0;
float camAngle=0.0,speed=0.1;
typedef vector<Vertex>::iterator vertexIt;
vector<vertexIt> pickList;

Polygon p1;

void update(int value)
{ //press s key for starting n stopping simulation
  if(simulation) {
  int index = 0;
  int size = p1.vertices.size();
  while(index!=size && size>1)
  {
    int adjVer1,adjVer2;
    index == 0 ? adjVer1 = size-1 : adjVer1 = index-1;
    index == size-1 ? adjVer2 = 0 : adjVer2 = index+1;
    Vec3f force1,force2,friction;
    Vec3f f1dir,f2dir;
    float l1,l2;

    f1dir = Vec3f(((p1.vertices[index].pos.v[0]-p1.vertices[adjVer1].pos.v[0])),
                   ((p1.vertices[index].pos.v[1]-p1.vertices[adjVer1].pos.v[1])),
                    0.0);
    f2dir = Vec3f(((p1.vertices[index].pos.v[0]-p1.vertices[adjVer2].pos.v[0])),
                   ((p1.vertices[index].pos.v[1]-p1.vertices[adjVer2].pos.v[1])),
                   0.0);

    l1 = f1dir.magnitude();     //current length of spring1
    l2 = f2dir.magnitude();     //current length of spring2

    f1dir = f1dir.normalize();   //converting to unit dir vector of force1
    f2dir = f2dir.normalize();   //converting to unit dir vector of force2

    //calculating force i.e k*(dx)*unitVector
    force1 = (-K*(l1 - L))*(f1dir);
    force2 = (-K*(l2 - L))*(f2dir);

    //calculating friction force friction = -k*velocity
    Vec3f dVelo1 = Vec3f(p1.vertices[index].velocity-p1.vertices[adjVer1].velocity);
    Vec3f dVelo2 = Vec3f(p1.vertices[index].velocity-p1.vertices[adjVer2].velocity);

    friction = (((-u*dVelo1.dot(f1dir))*f1dir)+((-u*dVelo2.dot(f2dir))*f2dir));

    p1.vertices[index].forces.push_back(force1);
    p1.vertices[index].forces.push_back(force2);
    p1.vertices[index].forces.push_back(friction);

    vector<Vec3f>::iterator it = p1.vertices[index].forces.begin();
    while(it!=p1.vertices[index].forces.end())
    {
      p1.vertices[index].netForce += *it;
      it++;
    }

    //updating velocity v = u + at
    p1.vertices[index].velocity += STEPSIZE*p1.vertices[index].netForce*energyLoss;

    //updating position of Vertex
    p1.vertices[index].pos += STEPSIZE*p1.vertices[index].velocity;

    index++;
  }
  }
  glutPostRedisplay();

  glutTimerFunc(30, update, 0);
}

void myInit()
{

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable( GL_DEPTH_TEST );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    //gluPerspective(45.0, (GLfloat)ww/(GLfloat)wh, 1, 200);
    gluOrtho2D(0.0, ww, 0.0, wh);
}

void reshapeFunc(int w, int h)
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	ww = w;
	wh = h;
	glViewport(0.0, 0.0, ww, wh);
	//gluPerspective(45.0, (GLfloat)ww/(GLfloat)wh, 1, 200);
	gluOrtho2D(0.0, ww, 0.0, wh);
}


void drawScene()
{
	glBegin( GL_LINE_LOOP );

	vector<Vertex>::iterator it = p1.vertices.begin();
	float *vtemp;
	if(p1.vertices.size() > 1)
	{
		vtemp = it->pos.v;
		it++;
		while(it!=p1.vertices.end())
		{
		glVertex3fv(vtemp);
		glVertex3fv(it->pos.v);
		vtemp = it->pos.v;
		it++;
		}
	}
	else if(p1.vertices.size() ==1 )
	{
		glVertex3fv(p1.vertices[0].pos.v);
	}

	glEnd();
}

void displayFunc()
{
  glClear( GL_COLOR_BUFFER_BIT );
	glColor3f(0.0, 0.0, 1.0);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

  	drawScene();
	glFlush();
  glutSwapBuffers();
}

bool compare(vertexIt a, vertexIt b)
{
  return (a->getId() < b->getId());
}

void generatePickList(int x, int y)
{
   vector<Vertex>::iterator it = p1.vertices.begin();
    while(it!=p1.vertices.end())
    {
      if((x>=(it->pos.v[0] - BOXSIZE)) && (y>= (it->pos.v[1] - BOXSIZE))
        &&(x<=(it->pos.v[0] + BOXSIZE)) && (y<= (it->pos.v[1] + BOXSIZE)))
      {

        pickList.push_back(it);
        /*Sort the pickList so that all the duplicate entries
          are setup consequtively and we can use unique() to erase it */
        sort(pickList.begin(), pickList.end(), compare);
        /* since unique() method return iterator followed by last
            unremoved entry therefore we erase from there till end */
        pickList.erase( unique( pickList.begin(), pickList.end()), pickList.end());
      }
      it++;
    }

}

void mouseFunc(int btn, int state, int x, int y)
{
	y = wh - y;		//remember must be inverted with height of window
	if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !picking && !moving && manualDraw )
	{
		Vertex v(x, y, 0.0);
		p1.vertices.push_back(v);
	}
	else 	if(btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN && !picking && !moving && manualDraw )
	{
		picking = true;
    moving = true;
    generatePickList(x, y);
  }
	glutPostRedisplay();
}

void motionFunc( int x, int y)
{
  if(picking && moving && !pickList.empty()){
  y = wh - y;
  int index = pickList.size()-1;
  pickList[index]->pos = Vec3f(x, y, 0);
  glutPostRedisplay();
  }
}

void clearScene()
{
  picking = false;
  moving = false;
  simulation = false;
  p1.vertices.erase(p1.vertices.begin(),p1.vertices.end());
  pickList.erase(pickList.begin(), pickList.end());
  cout<<"picking = "<<picking<<"\t moving = "<<moving<<"\t simulation = "<<simulation<<"\n";
  cout<<"scene cleared !!"<<"\n";
  glutPostRedisplay();
}

void keyboardFunc(unsigned char key, int x, int y)
{
    switch(key)
    {


      case 'b':
              if(pickList.empty()) break;
              for(int i=0;i<pickList.size();i++)
              {
                cout<<pickList[i]->getId()<<"\n";
              }
              break;
      case 'z':
                picking = false;
                moving = false;
              if(!pickList.empty())
              pickList.erase(pickList.begin(),pickList.end());
              cout<<"pickList Cleared !"<<"\n";
              cout<<"exiting moving Vertice mode \n";
              break;
      case 'x':
              cout<<pickList.size();
              break;
      case 'v':
              simulation ? simulation=false : simulation=true;
              cout<<simulation<<"\n";
              break;
      case 'c':
              clearScene();
              break;
      case 27:
              exit(0);
    }
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(ww,wh);
	glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
	glutCreateWindow("spring system using Euler method");
	myInit();

	  glutDisplayFunc(displayFunc);
	  glutReshapeFunc(reshapeFunc);
	  glutMouseFunc(mouseFunc);
	  glutMotionFunc(motionFunc);
    glutKeyboardFunc(keyboardFunc);
  //  glutSpecialFunc(specialKey);
    glutTimerFunc(25, update, 0);

    glutMainLoop();

return 1;
}
