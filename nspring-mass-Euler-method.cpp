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
float STEPSIZE = 0.001;
float energyLoss = 0.8;
float u = 0.1;                  //coefficient of friction
typedef vector<Vertex>::iterator vertexIt;
vector<vertexIt> pickList;

Polygon p1;
//functions
bool checkEdge(int ,int );
void insertEdges();

Vec3f computeForce(int index, int size, Vertex *vtemp)
{
    //calculating forces for every adjacent vertices
    for(int i=0;i<p1.edges[index].size();i++)
    {
      int adj = p1.edges[index][i]-1;
      Vec3f force,friction;
      Vec3f fdir;
      float l;

      fdir = vtemp->pos - p1.vertices[adj].pos;
      l = fdir.magnitude();     //current length of spring
      fdir = fdir.normalize();   //converting to unit dir vector of force1

      //calculating force i.e -k*(dx)*unitVector
      force = (-K*(l - L))*(fdir);

      //calculating friction force friction = -k*velocity
      Vec3f dVelo = vtemp->velocity - p1.vertices[adj].velocity;

      friction = (-u*dVelo.dot(fdir))*fdir;

      //saving the forces

      vtemp->forces.push_back(force);
      vtemp->forces.push_back(friction);
    }

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
  if(simulation) 
  {
    int index = 0;
    int size = p1.vertices.size();
    while(index!=size && size>1)
    {

      Vec3f force = computeForce(index,size,&p1.vertices[index]);
      //updating velocity v = u + at
      p1.vertices[index].velocity += STEPSIZE*force*energyLoss;

      //updating position of Vertex
      p1.vertices[index].pos += STEPSIZE*p1.vertices[index].velocity;

      index++;
    }
  }
  else if(picking && moving)
  {
    vector<Vertex>::iterator it = p1.vertices.begin();
    while(it!=p1.vertices.end())
    {
      it->netForce = Vec3f(0.0, 0.0, 0.0);
      it->velocity = Vec3f(0.0, 0.0, 0.0);
      it++;
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
  glColor3f(0.0, 0.0, 1.0);
  glBegin( GL_LINES );

        for(int i=0;i<p1.edges.size();i++)
        {
            for(int j=0;j<p1.edges[i].size();j++)
           {
                int edge = p1.edges[i][j]-1;
                glVertex3fv(p1.vertices[i].pos.v);
                glVertex3fv(p1.vertices[edge].pos.v);
            }
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
  y = wh - y;   //remember must be inverted with height of window
  if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !picking && !moving && manualDraw )
  {
    Vertex v(x, y, 0.0);
    cout<<(cout,v.pos)<<endl;
    p1.vertices.push_back(v);
    p1.edges.push_back(vector<int> (0));
    cout<<p1.edges.back().size()<<endl;

    do   //insert atleast one edges to particles
    {
            if(p1.edges.size()<2)
                break;
            insertEdges();
            if(p1.edges.back().empty())
                cout<<"Insert Atleast 1 edge for new vertex "<<p1.vertices.size()<<endl;
            else
                break;
    }while(1);
  }
  else if(btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN && !picking && !moving && manualDraw )
  {
    picking = true;
        moving = true;
        generatePickList(x, y);
    }
  glutPostRedisplay();
}

void motionFunc( int x, int y)
{
  if(picking && moving && !pickList.empty())
  {
    y = wh - y;
    int index = pickList.size()-1;
    pickList[index]->pos = Vec3f(x, y, 0);
    glutPostRedisplay();
  }
}

void clearScene()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glutSwapBuffers();
  glFlush();
  picking = false;
  moving = false;
  simulation = false;
  p1.vertices.erase(p1.vertices.begin(),p1.vertices.end());
  p1.edges.erase(p1.edges.begin(),p1.edges.end());
  pickList.erase(pickList.begin(), pickList.end());
  cout<<"picking = "<<picking<<"\t moving = "<<moving<<"\t simulation = "<<simulation<<"\n";
  cout<<"scene cleared !!"<<"\n";
  glutPostRedisplay();
}

bool checkEdge(int a, int b)
{
  vector<int>::iterator it1 = find(p1.edges[a-1].begin(), p1.edges[a-1].end(), b);
  vector<int>::iterator it2 = find(p1.edges[b-1].begin(), p1.edges[b-1].end(), a);

  if(it1==p1.edges[a-1].end() && it2==p1.edges[b-1].end())
    return false;

  return true;
}

void insertEdges()
{
    if(p1.vertices.size()<2)
              {
                cout<<"Insufficient or no vertices to insert an edge"<<endl;
                return;
              }
              cout<<"Enter two vertices out of following:(enter 0 0 for exit)"<<endl;
              int vertices = p1.vertices.size();
              for(int i=1;i<=vertices;i++)
                  cout<<i<<" ";
              cout<<endl;
              int a,b;
              cin.clear();
              cin>>a>>b;
              while(a!=0&&b!=0)
              {
                if(a>vertices||b>vertices||a<1||b<1||a==b)
                  cout<<"Enter vertices form 1 to"<<vertices<<endl;
                else
                {   //checking if edge already exists
                    if(!checkEdge(a,b))
                    {
                      p1.edges[a-1].push_back(b);
                      p1.edges[b-1].push_back(a);
                      cout<<"Edge inserted btwn "<<a<<"-"<<b<<endl;
                      glutPostRedisplay();          //displaying the inserted edges
                    }
                    else
                      cout<<"edge already exists"<<endl;
                }
                cin>>a>>b;
              }
              cout<<"Exiting loop"<<endl;
}


void keyboardFunc(unsigned char key, int x, int y)
{
    switch(key)
    {
      case 'e'://enter edges in polygon
            {
                simulation = false;
                insertEdges();
            break;
                }
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
