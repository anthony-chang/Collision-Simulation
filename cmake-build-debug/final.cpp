#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <cmath>

#define PI 3.1415296535897
using namespace std;

double k = 1302, m = 0.014123, r = 0.03052; //measured spring constant, mass, and radius
const double dt = 0.00001; //temporal resolution

struct model {//an object
    double x, y, vx, vy, ax, ay; //x&y components of position, velocity, and acceleration
} obj[2];
double t = 0; //current time
double m1, m2; //masses
double d, theta; //vector of the line connecting the 2 centre of masses

int temp1, temp2; //temporary, used to output every 100th computation

double mag(double x, double y) { //magnitude of a vector
    return sqrt(x*x+y*y);
}
double dir(double x, double y){ //direction of a vector (from the +x axis)
    if(x<0 && y<0) //quadrant 3
        return atan(y/x)+PI;
    if(x<0) //quadrant 2
        return atan(y/x)+PI;
    if(y<0) //quadrant 4
        return atan(y/x)+(2*PI);

    return atan(y/x); //quadrant 1
}


int main() {
    ofstream file1; //output to a .csv file
    file1.open("sim.csv"); //file name
    //file headers
    file1 << " , , OBJECT 1, , , , , , , OBJECT 2\n";
    file1 << "t, , x1, y1, vx1, vy1, ax1, ay1, , x2, y2, vx2, vy2, ax2, ay2\n";
    
    //object 1 input
    printf("Mass of Object 1 (kg): "); //mass
    scanf("%lf", &m1);
    if(m1 > 0.1) //for accuracy reasons. Otherwise acceleration computation will be too small
        m1 /= 1000;
    printf("Initial position of Object 1 (m) <x y>: "); //position
    scanf("%lf %lf", &obj[0].x, &obj[0].y);
    printf("Initial velocity of Object 1 (m/s) <vx vy>: "); //velocity
    scanf("%lf %lf", & obj[0].vx, & obj[0].vy);
    //object 2 input
    printf("Mass of Object 2 (kg): "); //mass
    scanf("%lf", &m2);
    if(m2 > 0.1) //for accuracy reasons. Otherwise acceleration computation will be too small
        m2 /= 1000;
    printf("Initial position of Object 2 (m) <x y>: "); //position
    scanf("%lf %lf", &obj[1].x, &obj[1].y);
    printf("Initial velocity of Object 2 (m/s) <vx vy>: "); //velocity
    scanf("%lf %lf", &obj[1].vx, &obj[1].vy);

    while (t <= 1) { //run the sim for 1 second
        d = mag(obj[1].x-obj[0].x, obj[1].y-obj[0].y); //magnitude of distance vector
        theta = dir(obj[1].x-obj[0].x, obj[1].y-obj[0].y); //direction of distance vector
        if (d < 2*r) { //if the 2 balls are in contact
            obj[0].ax = -k*(2*r-d)*cos(theta)/(2*m1); //x acceleration computation
            obj[0].ay = -k*(2*r-d)*sin(theta)/(2*m1); //y acceleration computation

            //from the POV of the second ball
            obj[1].ax = -k*(2*r-d)*cos(theta+PI)/(2*m2); //x acceleration computation (+PI because the reference point changes)
            obj[1].ay = -k*(2*r-d)*sin(theta+PI)/(2*m2); //y acceleration computation (+PI because the reference point changes)
        }
        else { //balls not in contact, so no change in speed
            obj[0].ax = 0;
            obj[0].ay = 0;
            obj[1].ax = 0;
            obj[1].ay = 0;
        }
        //update position with s = s_0 + v_0 t + 0.5at^2
        obj[0].x += obj[0].vx*dt + 0.5*obj[0].ax*dt*dt;
        obj[0].y += obj[0].vy*dt + 0.5*obj[0].ay*dt*dt;
        obj[1].x += obj[1].vx*dt + 0.5*obj[1].ax*dt*dt;
        obj[1].y += obj[1].vy*dt + 0.5*obj[1].ay*dt*dt;

        //update velocity with v = v_0 + at
        obj[0].vx += obj[0].ax*dt;
        obj[0].vy += obj[0].ay*dt;
        obj[1].vx += obj[1].ax*dt;
        obj[1].vy += obj[1].ay*dt;

        if(temp1 == temp2) { //only output every 100th loop
            //write t, x, y, vx, vy, ax, ay to csv file
            file1 <<t<<" , ,"<<obj[0].x<<", "<<obj[0].y<<", "<<obj[0].vx<<", "<<obj[0].vy<<", "<<obj[0].ax<<", "<<obj[0].ay<<" , , ";
            file1 <<obj[1].x<<", "<<obj[1].y<<", "<<obj[1].vx<<", "<<obj[1].vy<<", "<<obj[1].ax<<", "<<obj[1].ay<<"\n";
            temp2 += 100; //increment so this if statement runs 100 loops after
        }

        t += dt; //increment time
        temp1++; //count
    }
    //output final velocity components of both objects
    printf("OBJECT 1\n");
    printf("vx: %lf vy: %lf\n\n", obj[0].vx, obj[0].vy);n
    printf("OBJECT 2\n");
    printf("vx: %lf vy: %lf\n", obj[1].vx, obj[1].vy);

    //close file
    file1.close();

    return 0;
}
