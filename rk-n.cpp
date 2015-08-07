#include <GL/freeglut.h>
#include <GL/gl.h>
#include <stdio.h>
#include "Polygon.h"
#include <cmath>
#define BOXSIZE 3

using namespace std;

//Globals
int ww=500,wh=500,N;
bool manualDraw = true;
bool picking = false;
bool moving = false;
bool simulation = false;
float K = 0.01;
float L = 100.0;
float STEPSIZE = 0.1;
float energyLoss = 0.8;
float u = 0.01,*b,**array;                  //coefficient of friction
typedef vector<Vertex>::iterator vertexIt;
vector<vertexIt> pickList;

Polygon p1;

Vec3f computeForce(int index, int size, Vertex *vtemp)
{

    int adjVer1,adjVer2;
    index == 0 ? adjVer1 = size-1 : adjVer1 = index-1;
    index == size-1 ? adjVer2 = 0 : adjVer2 = index+1;
    Vec3f force1,force2,friction;
    Vec3f f1dir,f2dir;
    float l1,l2;

    f1dir = Vec3f(((vtemp->pos.v[0]-p1.vertices[adjVer1].pos.v[0])),
                   ((vtemp->pos.v[1]-p1.vertices[adjVer1].pos.v[1])),
                    0.0);
    f2dir = Vec3f(((vtemp->pos.v[0]-p1.vertices[adjVer2].pos.v[0])),
                   ((vtemp->pos.v[1]-p1.vertices[adjVer2].pos.v[1])),
                   0.0);

    l1 = f1dir.magnitude();     //current length of spring1
    l2 = f2dir.magnitude();     //current length of spring2

    f1dir = f1dir.normalize();   //converting to unit dir vector of force1
    f2dir = f2dir.normalize();   //converting to unit dir vector of force2

    //calculating force i.e -k*(dx)*unitVector
    force1 = (-K*(l1 - L))*(f1dir);
    force2 = (-K*(l2 - L))*(f2dir);

    //calculating friction force friction = -k*velocity
    Vec3f dVelo1 = Vec3f(vtemp->velocity-p1.vertices[adjVer1].velocity);
    Vec3f dVelo2 = Vec3f(vtemp->velocity-p1.vertices[adjVer2].velocity);

    friction = (((-u*dVelo1.dot(f1dir))*f1dir)+((-u*dVelo2.dot(f2dir))*f2dir));

    //saving the forces
    vtemp->forces.push_back(force1);
    vtemp->forces.push_back(force2);
    vtemp->forces.push_back(friction);
    vector<Vec3f>::iterator it = vtemp->forces.begin();

    while(it!=vtemp->forces.end())
    {
      vtemp->netForce += *it;
      it++;
    }
    return vtemp->netForce;
}

void update(int value)
{ //press s key for starting n stopping simulation
  if(simulation) {
  int index = 0;
  int size = p1.vertices.size();
  while(index<size && size>1)
  {
      Vec3f Y[N],F[n];
      Y[0] = ;
  }
  }
  else if(picking && moving){
    vector<Vertex>::iterator it = p1.vertices.begin();
    while(it!=p1.vertices.end())
    {
      it->netForce = Vec3f(0.0, 0.0, 0.0);
      it->velocity = Vec3f(0.0, 0.0, 0.0);
      it++;
    }
  }
  glutPostRedisplay();

  glutTimerFunc(25, update, 0);
}

void myInit()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

void reshapeFunc(int w, int h)
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	ww = w;
	wh = h;
	glViewport(0.0, 0.0, ww, wh);
	gluOrtho2D(0.0, ww, 0.0, wh);
}

void drawScene()
{
	glColor3f(0.0, 0.0, 1.0);
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
	else
	{
		glVertex3fv(vtemp);
	}

	glEnd();
}

void displayFunc()
{
	glClear( GL_COLOR_BUFFER_BIT );

	glColor3f(0.0, 0.0, 1.0);
	drawScene();
	glFlush();
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
      case 'a':
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
      case 's':
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

void getMethodDetail()
{
    cout<<"enter Nth order of RK to apply"<<endl;
    cin>>N;
    b = new float[N];
    array = new float*[N];
    for(int i=0;i<N;i++)
        *(array+i) = new float[N];

    for(int i=0;i<N;i++)
        for(int j=i;j<N;j++)
            array[i][j] = 0;

    cout<<"enter value of b[i] st sum(bi)=1 "<<endl;
    for(int i=0;i<N;i++)
    {
        printf("\nenter the b[%d]:",i);
        cin>>b[i];
    }

    for(int i=0;i<N;i++)
    {
        for(int j=0;j<i;j++)
        {
            printf("\nenter a[%d][%d]:",i,j);
            cin>>array[i][j];
        }
    }

    for(int i=0;i<N;i++)
    {
        cout<<b[i]<<" ";
    }

    for(int i=0;i<N;i++)
    {
        for(int j=0;j<i;j++)
            cout<<array[i][j]<<"";
        cout<<"\n";
    }
}

int main(int argc, char** argv)
{
    getMethodDetail();
	glutInit(&argc, argv);
	glutInitWindowSize(ww,wh);
	glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
	glutCreateWindow("spring system using Rune Kutta 4");
	myInit();
	glutReshapeFunc(reshapeFunc);
	glutMouseFunc(mouseFunc);
	glutDisplayFunc(displayFunc);
	glutMotionFunc(motionFunc);
  glutKeyboardFunc(keyboardFunc);

  glutTimerFunc(15, update, 0);

  glutMainLoop();

return 1;
}

