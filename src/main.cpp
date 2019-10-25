#include<bits/stdc++.h>
#include <GL/glut.h>

#define INF_MASS 10000
#define SCR_SIZE 600

using namespace std;

class Vector;
class Body;
class Sun;
class Planet;

class Vector {

    float x, y;

public:

    Vector();
    Vector(float, float);
    void setX(float x);                          // Update x
    void setY(float y);                          // Update y
    pair<float, float> cords();                     // Return x and y
    friend Vector operator + (Vector a, Vector b);  // Vector addition
    friend Vector operator * (Vector a, float f);   // Scalar multiplication
    friend float distance (Vector a, Vector b);     // Distance between Vectors

};

class Body {

protected:

    Vector pos;         // Position Components (Coordinates)
    Vector v;           // Velocity Components
    float radius;       // Radius of body
    float mass;         // Mass of the body
    float color[3];     // Color channels of the body

public:

    Vector* position();
    Vector* velocity();
    set_position(Vector a);
    set_velocity(Vector a);
    float fieldDueTo(Body body);    // Update Velocity of *this due to field of body

};

class Planet: public Body {
    public:
        Planet(Vector pos, Vector v, float radius = 1, float mass = 100);
        void draw();  // Draws Planet
};

class Sun: public Body {
    public:
        Sun(Vector pos, Vector v, float radius = 10, float mass = INF_MASS);
        void draw();  // Draws Suns
};

vector<Body*> universe; // Contains all Suns and Planets

int main(int argc, char const *argv[])
{
    universe.push_back(new Sun(Vector(2, 3), Vector(3, 4)));
    universe.push_back(new Planet(Vector(2, 5), Vector(0, 0)));
    return 0;
}

Vector::Vector() {
    x = 0; y = 0;
}
Vector::Vector(float a, float b) {
    x = a; y = b;
}
void Vector::setX(float x) {
    this -> x = x;
}
void Vector::setY(float y) {
    this -> y = y;
}

pair<float, float> Vector::cords() {
    return {x, y};
}

Vector Vector::operator + (Vector a, Vector b) {
    Vector c(a.x + b.x, a.y + b.y);
    return c;
}

Vector Vector::operator * (Vector a, float f) {
    Vector c(a.x * f, a.y * f);
    return c;
}

float Vector::distance (Vector a, Vector b){
    float c = (a.x-b.x)*(a.x-b.x) + (a.y - b.y)*(a.y - b.y);
    return(sqrt(c));
}

Vector * Body::position(){

}
