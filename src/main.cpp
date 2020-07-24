#include<bits/stdc++.h>
#include <GL/freeglut.h>

#define INF_MASS 10000
#define SCR_SIZE 700
#define G 0.005

using namespace std;

class Vector;
class Body;
class Sun;
class Planet;

void timer(int = 0);
void display();
void mouse(int, int, int, int);
void mouseMotion(int, int);
void removeUniverse();
void RenderString(float x, float y, void *font, string s);
void keyboard(unsigned char, int, int);

int Mx, My, WIN;
bool PRESSED_LEFT = false, PRESSED_RIGHT = false,
     PRESSED_MIDDLE = false;

vector<Body*> universe; // Contains all Suns and Planets

/**
 * Class Vector
 * A class for 2D cartisian vector
 * 
 * @note
 * This includes all basic operations between
 * two vectors that are required by the program
 * **/
class Vector {

    float x, y;

public:

    Vector();
    Vector(float, float);
    Vector(int, int);
    void setX(float x);                             // Update x
    void setY(float y);                             // Update y
    pair<float, float> cords();                     // Return x and y
    friend Vector operator + (Vector a, Vector b);  // Vector addition
    friend Vector operator * (Vector a, float f);   // Scalar multiplication
    friend float distance (Vector a, Vector b);     // Distance between Vectors

};

/**
 * Class Body
 * A circular object's representation in a 2D cartesian space
 * 
 * @note
 * This class includes all properties that are required for
 * representation of a body in space.
 * **/
class Body {

protected:

    Vector pos;         // Position Components (Coordinates)
    Vector v;           // Velocity Components
    float radius;       // Radius of body
    float mass;         // Mass of the body
    float color[3];     // Color channels of the body

public:
    Body(Vector pos, Vector v, float radius, float mass, float color[3]);
    Vector* position();
    Vector* velocity();
    void draw(int i);
    friend Vector field(Body* a, Body* b);    // Update Velocity of *this due to field of body
    float getMass();
    float getRadius();
    virtual char type() = 0;
};

/**
 * Class Planet
 * A Planet's representation in our universe (2D cartisian space)
 * 
 * @note
 * This class includes all properties that are required for
 * representation of a Planet in space.
 * **/
class Planet: public Body {
public:
    Planet(Vector pos, Vector v, float color[3], float radius = 5, float mass = 1000)
    :Body(pos, v, radius, mass, color){};
    char type();
};

/**
 * Class Sun
 * A Sun's representation in our universe (2D cartisian space)
 * 
 * @note
 * This class includes all properties that are required for
 * representation of a Sun in space.
 * **/
class Sun: public Body {
public:
    Sun(Vector pos, Vector v, float color[3], float radius = 20, float mass = INF_MASS)
    :Body(pos, v, radius, mass, color){};
    char type();
};

Vector::Vector() {
    x = 0; y = 0;
}
Vector::Vector(float a, float b) {
    x = a; y = b;
}
void Vector::setX(float x) {
    this->x = x;
}
void Vector::setY(float y) {
    this->y = y;
}

pair<float, float> Vector::cords() {
    return pair<float, float>(x, y);
}

Vector operator + (Vector a, Vector b) {
    Vector c(a.x + b.x, a.y + b.y);
    return c;
}

Vector operator * (Vector a, float f) {
    Vector c(a.x * f, a.y * f);
    return c;
}

float distance (Vector a, Vector b){
    float c = (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
    return(sqrt(c));
}

Body::Body(Vector pos, Vector v, float radius, float mass, float* color) {
    this->pos = pos ;
    this->v = v;
    this->mass = mass;
    this->radius = radius;
    for (int i = 0; i < 3; i++) {
        this->color[i] = color[i];
    }
}

Vector * Body::position(){
   return & pos;
}

Vector * Body::velocity(){
    return &v;
}

void Body::draw(int i){
    glColor3f(color[0], color[1], color[2]);
    glBegin(GL_POLYGON);
    for(float a = 0; a < 2*M_PI; a+=0.2) {
        glVertex2f(radius*cos(a) + position() -> cords().first, radius*sin(a) + position() -> cords().second);
        // cout << radius*cos(a) + pos.cords().first << " " << radius*sin(a) + pos.cords().second << "\n";
    }
    glEnd();
}

Vector field(Body* a, Body* b) {
    Vector E;
    float d = distance(*(a -> position()), *(b -> position()));
    float ax = a -> position() -> cords().first;
    float ay = a -> position() -> cords().second;
    float bx = b -> position() -> cords().first;
    float by = b -> position() -> cords().second;
    E.setX(G*(b -> mass)/(d*d) * (bx - ax)/d);
    E.setY(G*(b -> mass)/(d*d) * (by - ay)/d);
    return E;
}

float Body::getMass() {
    return mass;
}
float Body::getRadius() {
    return radius;
}

char Planet::type() {
    return 'p';
}

char Sun::type() {
    return 's';
}

void display()
{
    int mass = 0;
    int Suns = 0, Planets = 0;
    for (auto b : universe){
        mass += b -> getMass();
        if (b -> type() == 's') Suns++;
        else Planets++;
    }

        glClear(GL_COLOR_BUFFER_BIT);
        RenderString(20 - SCR_SIZE/2, 50 - SCR_SIZE/2, GLUT_BITMAP_HELVETICA_18, "Planet Simulation");
        RenderString(-200 + SCR_SIZE/2, 50 - SCR_SIZE/2, GLUT_BITMAP_HELVETICA_12, "Statistics ");
        RenderString(-200 + SCR_SIZE/2, 65 - SCR_SIZE/2, GLUT_BITMAP_HELVETICA_10, "Bodies in Universe: " + to_string(universe.size()));
        RenderString(-200 + SCR_SIZE/2, 80 - SCR_SIZE/2, GLUT_BITMAP_HELVETICA_10, "Total Mass: " + to_string(mass));
        RenderString(-200 + SCR_SIZE/2, 95 - SCR_SIZE/2, GLUT_BITMAP_HELVETICA_10, "Number of Suns: " + to_string(Suns));
        RenderString(-200 + SCR_SIZE/2, 110 - SCR_SIZE/2, GLUT_BITMAP_HELVETICA_10, "Number of Planets: " + to_string(Planets));


        for(int i = 0; i < universe.size(); i++)
            universe[i] -> draw(i+1);
 
        glFlush();
        glutSwapBuffers();
}

void timer(int)
{
    display();
    if(PRESSED_LEFT)
    {
        float color[3];
        color[0] = rand()%200 / 200.0;
        color[1] = rand()%200 / 200.0;
        color[2] = rand()%200 / 200.0;

        Vector pos((float)Mx, (float)My), v((float)0.5, (float)0);
        Planet* p = new Planet(pos, v, color);
        universe.push_back(p);
        // cout << "P "<< universe.size() << " ";
        PRESSED_LEFT = false;
    }

    if(PRESSED_RIGHT)
    {
        float color[3] = {1, 1, 0};

        Vector pos((float)Mx, (float)My), v((float)0, (float)0);
        Sun* s = new Sun(pos, v, color);
        universe.push_back(s);
        // cout << "S " << universe.size() << " ";

        PRESSED_RIGHT = false;
    }

    if(PRESSED_MIDDLE)
        removeUniverse(); //remove all universe

    for(int i = 0; i < universe.size(); i++)
    {
        Body* b = universe[i];
        bool not_fall = true;
        for(int j = 0; j < universe.size(); j++)
        {
            if(j == i || b -> getMass() >= 10000) // we consider the 10000 as infinit (big mass) so this universe won't move
                continue;   

            Body* p = universe[j];
            float d = distance(*(b -> position()), *(p -> position()));
            if(d > p -> getRadius() || d > SCR_SIZE)
            {
                pair<float, float> E = field(b, p).cords();
                pair<float, float> v = b -> velocity() -> cords();
                b -> velocity() -> setX(v.first + E.first);
                b -> velocity() -> setY(v.second + E.second);
            }
            else not_fall = false;
                // cout << "\n" << d << " " << p -> getRadius()
        }

        if(not_fall)   
        {
            pair<float, float> v = b -> velocity() -> cords();
            pair<float, float> p = b -> position() -> cords();
            b -> position() -> setX(p.first + v.first);
            b -> position() -> setY(p.second + v.second);
        }
        else {
            free(universe[i]);
            universe.erase(universe.begin()+i);
        }
    }

    glutTimerFunc(1, timer, 0);
}

void mouse(int button, int state, int x, int y)
{
        //set the coordinates
        Mx = x - (SCR_SIZE/2);
        My = y - (SCR_SIZE/2);
        // cout << Mx << " " << My << "\n";
        //check which button is pressed
        if(button == GLUT_LEFT_BUTTON)
                PRESSED_LEFT = true;
        else if(button == GLUT_RIGHT_BUTTON)
                PRESSED_RIGHT = true;
        else if(button == GLUT_MIDDLE_BUTTON)
                PRESSED_MIDDLE = true;
}
 
void mouseMotion(int x, int y)
{
    Mx = x - (SCR_SIZE/2);
    My = y - (SCR_SIZE/2);
}

void removeUniverse()
{
    for(auto b: universe) free(b);
    universe.erase(universe.begin(), universe.end());
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'e':
                removeUniverse();
                glutDestroyWindow(WIN);
                exit(0);
                break;
        case 'r':
                removeUniverse();
                break;
    }
}

void RenderString(float x, float y, void *font, string s)
{  
  char *c;

  glColor3f(1, 1, 1); 
  glRasterPos2f(x, y);
  glutBitmapString(font, (const unsigned char *)s.c_str());
}

int main(int argc, char **argv)
{
    float color[3] = {1, 1, 0};
    Vector pos((float)0, (float)0), v((float)0, (float)0);
    Sun* s = new Sun(pos, v, color);
    universe.push_back(s);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(SCR_SIZE, SCR_SIZE);
    glutInitWindowPosition(50, 50);
    WIN = glutCreateWindow("Gravity");

    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-(SCR_SIZE/2), (SCR_SIZE/2), (SCR_SIZE/2), -(SCR_SIZE/2), 0, 1);
    
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);
    timer();
    glutMainLoop();
    removeUniverse();
    return 0;
}