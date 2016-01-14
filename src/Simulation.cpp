//
//  Simulation.cpp
//  traceSumilation
//
//  Created by Yumiko Kokubu on 12/9/15.
//
//

#include "Simulation.h"

// static変数はどこでも使えるようにクラスの前で宣言
ofVec2f Simulation::velocity;
double Simulation::area, Simulation::Cd, Simulation::Cl, Simulation::theta, Simulation::m, Simulation::rho;
double Simulation::g;


Simulation::Simulation() {
    n = 0;
}

void Simulation::Runge_Kutta(double (*f[])(double t, double *x), double t0, double *x, double tn, int div, int num)
{
    
    double k1[num], k2[num], k3[num], k4[num], temp[num];
    
    //　ここから計算スタート
    double h = (tn - t0);
    if(div) h /= div;
    
    double t = t0;
    
    for(int i=0; i<div; i++){
        for(int j=0; j<num; j++){
            k1[j] = (*f[j])(t, x);
            temp[j] = x[j] + h*k1[j]/2;
        }
        for(int j=0; j<num; j++){
            k2[j] = (*f[j])(t+h/2, temp);
        }
        for(int j=0; j<num; j++){
            temp[j] = x[j] + h*k2[j]/2;
        }
        for(int j=0; j<num; j++){
            k3[j] = (*f[j])(t+h/2, temp);
        }
        for(int j=0; j<num; j++){
            temp[j] = x[j] + h*k3[j];
        }
        for(int j=0; j<num; j++){
            k4[j] = (*f[j])(t+h, temp);
            x[j] += (k1[j] + 2*k2[j] + 2*k3[j] + k4[j])*h/6;
        }
        t += h;
    }
    
}

double Simulation::setValue(double _m, double _area ,double _cd, double _cl, double _theta, ofVec2f _velocity, double _rho)
{
    m = _m; //kg
    area = _area; //m2
    Cd = _cd; //
    Cl = _cl;
    theta = _theta;
    rho = _rho; //
    
    velocity = _velocity;
    
}

double Simulation::setGravity(double _g)
{
    g = _g;
}

double Simulation::f1(double t, double *x)
{
    return -(rho*(x[0]*x[0]+x[1]*x[1])*(Cd*cos(theta)+Cl*sin(theta))*area)/(2*m);
}

double Simulation::f2(double t, double *x)
{
    return -(rho*(x[0]*x[0]+x[1]*x[1])*(Cd*sin(theta)-Cl*cos(theta))*area)/(2*m)-m*g;
}

double Simulation::f3(double t, double *x)
{
    return x[0];
}

double Simulation::f4(double t, double *x)
{
    return x[1];
}



void Simulation::update()
{
    
    double t=0, tn=1; //1s
    double x[4];
//    if (points.empty()) {
        x[0] = velocity.x;
        x[1] = velocity.y;
        x[2] = x[3] = 0;
//    }else {
//        x[0] = velocity.x;
//        x[1] = velocity.y;
//        x[2] = points.end()->x;
//        x[3] = points.end()->y;
//    }
    
    
    double h;
    
    double (*f[4])(double , double*);
    
    f[0] = f1;
    f[1] = f2;
    f[2] = f3;
    f[3] = f4;
    
    h=(tn - t)/LOOP;
    points.clear();
    for(int i=0; i<LOOP; i++){
        //Runge_Kutta(double (*f[])(double t, double *x), double t0, double *x, double tn, int div, int num)
        Runge_Kutta(f, t, x, t+h, 1, 4);
        ofVec2f p = ofVec2f(x[2], x[3]);
        points.push_back(p);
        //        printf("%f %f \n",  x[2], x[3]);
        t+=h;
    }
    
}

void Simulation::draw()
{
    ofSetColor(ofColor::blue);
    
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(2);
    ofSetColor(255);
    ofBeginShape();
    for(int i=0; i<LOOP; i++){
        ofVertex(points[i].x, points[i].y); //アレ？
    }
    ofEndShape();
    ofPopStyle();
    
//    ofSetColor(ofColor::yellow);
//    ofCircle(points[n].x*100, points[n].y*100, 10);
//    n+=40;
}
