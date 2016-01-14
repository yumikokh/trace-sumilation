//
//  CoefficientSpline.h
//  traceSumilation
//
//  Created by Yumiko Kokubu on 12/18/15.
//
//

#pragma once
#include "ofMain.h"

#define TYPE_NUM 2

struct CoefficientValue {
    float degree;
    float C[2];
};

enum CoefficientType { Cl, Cd };
    
class CoefficientSpline {
public:
    
    CoefficientSpline();
    
    void readData();
    float getInterpolation( float _t, CoefficientType _type );
    void drawSpline();
    vector <CoefficientValue> coValues;
    
    float * z[TYPE_NUM];
    
};

