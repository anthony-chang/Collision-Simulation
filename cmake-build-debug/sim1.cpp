#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <cmath>

#define PI 3.1415296535897
using namespace std;
typedef pair <double, double> pdd;

double k = 1302, m = 0.014123, r = 0.03052, dt = 0.00001;
struct model {
    double x, y, vx, vy, ax, ay;
} obj[2];
double t = 0;
double v1i, v2i, theta1i, theta2i, m1, m2; //initial velocities, initial angles, initial masses
double d, theta; //vector of the line connecting the 2 centre of masses

int temp1, temp2;
double mag(double x, double y) { //magnitude of a vector
    return sqrt(x*x+y*y);
}
double dir(double x, double y){ //direction of a vector (from the +x axis)
    if(x<0 && y<0)
        return atan(y/x)+PI;
    if(x<0)
        return atan(y/x)+PI;
    if(y<0)
        return atan(y/x)+(2*PI);

    return atan(y/x);
}


int main() {
    ofstream file1;
    file1.open("sim1.csv");
    file1 << " , , OBJECT 1, , , , , , , OBJECT 2\n";
    file1 << "t, , x1, y1, vx1, vy1, ax1, ay1, , x2, y2, vx2, vy2, ax2, ay2\n";
    r = 0.6;

    printf("Mass of Object 1 (kg): ");
    scanf("%lf", &m1);
    if(m1 > 0.1)
        m1 /= 1000;
    printf("Initial position of Object 1 (m) <x y>: ");
    scanf("%lf %lf", &obj[0].x, &obj[0].y);
    printf("Initial velocity and angle of Object 1 (m/s deg) <vx vy>: ");
    scanf("%lf %lf", & obj[0].vx, & obj[0].vy);

    printf("Mass of Object 2 (kg): ");
    scanf("%lf", &m2);
    if(m2 > 0.1)
        m2 /= 1000;
    printf("Initial position of Object 2 (m) <x y>: ");
    scanf("%lf %lf", &obj[1].x, &obj[1].y);
    printf("Initial velocity and angle of Object 2 (m/s deg) <vx vy>: ");
    scanf("%lf %lf", &obj[1].vx, &obj[1].vy);

    //scanf("%lf %lf %lf %lf %lf %lf %lf %lf", &obj[0].x, &obj[0].y, &obj[1].x, &obj[1].y, &v1i, &theta1i, &v2i, &theta2i);

    while (t <= 1) {
        d = mag(obj[1].x-obj[0].x, obj[1].y-obj[0].y);
        theta = dir(obj[1].x-obj[0].x, obj[1].y-obj[0].y);
        if (d < 2*r) {
            //cout<<"Compresssion: "<<2*r-d<<endl;
            //cout<<"Force: "<<-k*(2*r-d)<<endl;

            obj[0].ax = -k*(2*r-d)*cos(theta)/(2*m1);
            obj[0].ay = -k*(2*r-d)*sin(theta)/(2*m1);

            //from the POV of the second ball
            obj[1].ax = -k*(2*r-d)*cos(theta+PI)/(2*m2);
            obj[1].ay = -k*(2*r-d)*sin(theta+PI)/(2*m2);
        }
        else {
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

        if(temp1 == temp2) {
            file1 <<t<<" , ,"<<obj[0].x<<", "<<obj[0].y<<", "<<obj[0].vx<<", "<<obj[0].vy<<", "<<obj[0].ax<<", "<<obj[0].ay<<" , , ";
            file1 <<obj[1].x<<", "<<obj[1].y<<", "<<obj[1].vx<<", "<<obj[1].vy<<", "<<obj[1].ax<<", "<<obj[1].ay<<"\n";
            /*printf("OBJECT1 - t: %lf, vx: %lf, vy: %lf, ax: %lf,  ay: %lf", t, obj[0].vx, obj[0].vy, obj[0].ax,
                   obj[0].vy);
            cout << endl;
            printf("OBJECT2 - t: %lf, vx: %lf, vy: %lf, ax: %lf,  ay: %lf", t, obj[1].vx, obj[1].vy, obj[1].ax,
                   obj[1].vy);
            cout << endl;*/
            temp2 += 100;
        }

        //update velocity with v = v_0 + at
        obj[0].vx += obj[0].ax*dt;
        obj[0].vy += obj[0].ay*dt;
        obj[1].vx += obj[1].ax*dt;
        obj[1].vy += obj[1].ay*dt;

        t += dt;
        temp1++;
    }
    printf("OBJECT 1\n");
    printf("vx: %lf vy: %lf\n\n", obj[0].vx, obj[0].vy);
    printf("OBJECT 2\n");
    printf("vx: %lf vy: %lf\n", obj[1].vx, obj[1].vy);

    file1.close();

    return 0;
}
/*
 sim 1
0.6
0 0
3.14 0
1
1.5 0
0 0

sim 2
0.1
0.15 -0.8
4 2.2
0.1
1.5 0
0 0

sim 3
1
1 0
1 0.3
1
2 0.5
-0.5 -0.5
 */