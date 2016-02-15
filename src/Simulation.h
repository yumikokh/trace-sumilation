//
//  Simulation.h
//  traceSumilation
//
//  Created by Yumiko Kokubu on 12/9/15.
//
//
#pragma once
#include "ofMain.h"
#include "CoefficientSpline.h"

#define LOOP 600

class Simulation {
    
public:
    Simulation();
    
    void Runge_Kutta(double (*f[])(double t, double *x, CoefficientSpline *_coePtr), double t0, double *x, double tn, int div, int num);
    
    static double f1(double t, double *x, CoefficientSpline *_coePtr);
    static double f2(double t, double *x, CoefficientSpline *_coePtr);
    static double f3(double t, double *x, CoefficientSpline *_coePtr);
    static double f4(double t, double *x, CoefficientSpline *_coePtr);
    
    static void setValue(double _m, double _area ,double _cd, double _cl, double _theta, ofVec3f _velocity, double _rho, ofVec3f _pos, float _thetaZ);
    static void setGravity(double g);
    void update();
    void draw();
    vector <ofVec3f> getPos();
    void setCoeSpline(CoefficientSpline *_coePtr);
    
    static ofVec3f v0, v;
    static ofVec3f pos0;
    static double area, m, rho;
    static double theta, theta0;
    static float g;
	static float thetaZ0;
    
     vector <ofVec3f> points;
//    ofVec2f point;
    CoefficientSpline *coePtr;
};
