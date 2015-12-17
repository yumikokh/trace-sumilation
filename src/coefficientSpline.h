//
//  CoefficientSpline.h
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
    
    vector<CoefficientValue> coValues;
    
public:
    CoefficientSpline();
    
    void readData();
    void drawSpline();
};


