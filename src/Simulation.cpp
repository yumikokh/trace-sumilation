//
//  Simulation.cpp
//  traceSumilation
//
//  Created by Yumiko Kokubu on 12/9/15.
//
//

#include "Simulation.h"

// static変数はどこでも使えるようにクラスの前で宣言
ofVec3f Simulation::v0, Simulation::v, Simulation::pos0;
double Simulation::theta;
double Simulation::area, Simulation::theta0, Simulation::m, Simulation::rho;
double Simulation::g;

CoefficientSpline Simulation::*coePtr;


Simulation::Simulation() {
}

void Simulation::Runge_Kutta(double (*f[])(double t, double *x, CoefficientSpline *_coePtr), double t0, double *x, double tn, int div, int num)
{
    
    double k1[num], k2[num], k3[num], k4[num], temp[num];
    
    double h = (tn - t0);
    //    if(div) h /= div;
    
    double t = t0;
    
    //    for(int i=0; i<div; i++){
    for(int j=0; j<num; j++){
        k1[j] = (*f[j])(t, x, coePtr);
        temp[j] = x[j] + h*k1[j]/2;
    }
    for(int j=0; j<num; j++){
        k2[j] = (*f[j])(t+h/2, temp, coePtr);
    }
    for(int j=0; j<num; j++){
        temp[j] = x[j] + h*k2[j]/2;
    }
    for(int j=0; j<num; j++){
        k3[j] = (*f[j])(t+h/2, temp, coePtr);
    }
    for(int j=0; j<num; j++){
        temp[j] = x[j] + h*k3[j];
    }
    for(int j=0; j<num; j++){
        k4[j] = (*f[j])(t+h, temp, coePtr);
        x[j] += (k1[j] + 2*k2[j] + 2*k3[j] + k4[j])*h/6;
    }
    //        t += h;
    //    }
}

double Simulation::setValue(double _m, double _area ,double _cd, double _cl, double _theta, ofVec3f _velocity, double _rho, ofVec3f _initPos)
{
    m = _m; //kg
    area = _area; //m2
//        Cd = _cd; 
//        Cl = _cl;
    theta0 = _theta;
    rho = _rho; //
    
    v0 = _velocity;
    pos0 = _initPos;
    
}

double Simulation::setGravity(double _g)
{
    g = _g;
}

// dv/dt =
double Simulation::f1(double t, double *x, CoefficientSpline *_coePtr)
{
        theta =  ofRadToDeg( atan2f(x[1], x[0]) );
        if (theta < 0) {
            theta *= -1;
        }
        float Cd_val = _coePtr->getInterpolation( theta, Cd);
        float Cl_val = _coePtr->getInterpolation( theta, Cl);
//        return -( rho*(x[0]*x[0] + x[1]*x[1])*area*(Cd_val*cos(theta) + Cl_val*sin(theta)) )/(2*m);//vxa
    return 0;
}

// dw/dt =
double Simulation::f2(double t, double *x, CoefficientSpline *_coePtr)
{
        theta =  ofRadToDeg( atan2f(x[1], x[0]) );
        if (theta < 0) {
            theta *= -1;
        }
        float Cd_val = _coePtr->getInterpolation( theta, Cd);
        float Cl_val = _coePtr->getInterpolation( theta, Cl);
    
    
//    cout << theta << ":" << Cd_val<< endl; //めちゃ重くなる
    
//        return -( rho*(x[0]*x[0] + x[1]*x[1])*area*(Cd_val*sin(theta) - Cl_val*cos(theta)) )/(2*m) + m*g; //vya
    //    return -( rho*(x[0]*x[0] + x[1]*x[1])*area*(Cd_val*sin(theta) - Cl_val*cos(theta)) )/(2*m) + m*g;//vya
    return m*g;
}

// dx/dt =
double Simulation::f3(double t, double *x, CoefficientSpline *_coePtr)
{
    return x[0]; //vx
}

// dy/dt =
double Simulation::f4(double t, double *x, CoefficientSpline *_coePtr)
{
    return x[1]; //vy
}



void Simulation::update()
{
    
    double t=0, tn=10000; //1s
    double x[4];
    
    // 初期値
    x[0] = v0.x;
    x[1] = v0.y;
    x[2] = pos0.x;
    x[3] = pos0.y;
 
    double h;
    double (*f[4])(double , double*, CoefficientSpline*);
    
    f[0] = f1;
    f[1] = f2;
    f[2] = f3;
    f[3] = f4;
    
    h = (tn - t)/LOOP;
    points.clear();
    for(int i=0; i<LOOP; i++){
        //Runge_Kutta(double (*f[])(double t, double *x), double t0, double *x, double tn, int div, int num)
        Runge_Kutta(f, t, x, t+h, 1, 4);
        ofVec3f p = ofVec3f(x[2], x[3], 0);
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
        //        ofVertex(points[i].x, points[i].y);
        ofVertex(points[i].x, points[i].y);
    }
    ofEndShape();
    ofPopStyle();
    
}

vector <ofVec3f> Simulation::getPos()
{
    return points;
}

void Simulation::setCoeSpline(CoefficientSpline *_coePtr) {
    coePtr = _coePtr;
}