//
//  coefficientSpline.h
//  traceSumilation
//
//  Created by Yumiko Kokubu on 12/18/15.
//
//


#pragma once
#include "ofMain.h"

struct CoefficientValue {
    double degree;
    double Cl;
    double Cd;
};


class CoefficientSpline {
    
public:
    CoefficientSpline();
    
    void readData();
    void drawSpline();
    
    vector<CoefficientValue> coValues;
};


