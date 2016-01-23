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
float Simulation::thetaZ0;

CoefficientSpline Simulation::*coePtr;


Simulation::Simulation() {
}

void Simulation::Runge_Kutta(double (*f[])(double t, double *x, CoefficientSpline *_coePtr), double t0, double *x, double tn, int div, int num)
{
    
	const int nnum = 4;
    double k1[nnum], k2[nnum], k3[nnum], k4[nnum], temp[nnum];
    
    double h = (tn - t0);
    //    if(div) h /= div;
    
    double t = t0;
    
    //    for(int i=0; i<div; i++){
    for(int j=0; j<nnum; j++){
        k1[j] = (*f[j])(t, x, coePtr);
        temp[j] = x[j] + h*k1[j]/2;
    }
    for(int j=0; j<nnum; j++){
        k2[j] = (*f[j])(t+h/2, temp, coePtr);
    }
    for(int j=0; j<nnum; j++){
        temp[j] = x[j] + h*k2[j]/2;
    }
    for(int j=0; j<nnum; j++){
        k3[j] = (*f[j])(t+h/2, temp, coePtr);
    }
    for(int j=0; j<nnum; j++){
        temp[j] = x[j] + h*k3[j];
    }
    for(int j=0; j<nnum; j++){
        k4[j] = (*f[j])(t+h, temp, coePtr);
        x[j] += (k1[j] + 2*k2[j] + 2*k3[j] + k4[j])*h/6;
    }
    //        t += h;
    //    }
}

void Simulation::setValue(double _m, double _area ,double _cd, double _cl, double _theta, ofVec3f _velocity, double _rho, ofVec3f _initPos,  float _thetaZ)
{
    m = _m; //kg
    area = _area; //m2
//        Cd = _cd; 
//        Cl = _cl;
    theta0 = _theta;
    rho = _rho; //
    
    v0 = _velocity;
    pos0 = _initPos;
	thetaZ0 = _thetaZ;
    
}

void Simulation::setGravity(double _g)
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
    /*x[2] = pos0.x;
	x[3] = pos0.y;*/
	x[2] = x[3] = 0;
 
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
        ofVec3f pre = ofVec3f(x[2], x[3], 0);

		ofMatrix4x4 m;
		m.set ( cos(thetaZ0), 0, -sin(thetaZ0), pos0.x,
				0, 1, 0, pos0.y,
				sin(thetaZ0), 0, cos(thetaZ0), 0,
				0, 0, 0, 1);

		ofVec3f post = m * pre;

        points.push_back(post);
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
        ofVertex(points[i].x, points[i].y, points[i].z);
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