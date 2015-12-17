//
//  Simulation.h
//  traceSumilation
//
//  Created by Yumiko Kokubu on 12/9/15.
//
//

#pragma once
#include "ofMain.h"

#define LOOP 10000

class Simulation {
    
public:
    Simulation();
    
    void Runge_Kutta(double (*f[])(double t, double *x), double t0, double *x, double tn, int div, int num);
    
    static double f1(double t, double *x);
    static double f2(double t, double *x);
    static double f3(double t, double *x);
    static double f4(double t, double *x);
    
    static double setValue(double d, double m, double area ,double Cd, double Cl, double theta, double rho);
    static double setGravity(double g);
    void update();
    void draw();
    
    static double d, velocity, area, Cd, Cl, theta, m, rho;
    static double g;
    
    vector <ofVec2f> points;
    ofVec2f point;
    
    int j;
    
    
};
