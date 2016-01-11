//
//  CoefficientSpline.cpp
//  traceSumilation
//
//  Created by Yumiko Kokubu on 12/18/15.
//
//

#include "CoefficientSpline.h"

CoefficientSpline::CoefficientSpline() {
    
}

void CoefficientSpline::readData() {
    string filePath = "coefficient.csv";
    ofFile file(filePath);
    
    if(!file.exists()){
        ofLogError("The file " + filePath + " is missing");
    }
    
    ofBuffer buffer(file);
    CoefficientValue value;
    while (!buffer.isLastLine()) {
        string line = buffer.getNextLine();
        vector<string> elements = ofSplitString(line, ",");
        
        value.degree = ofToDouble(elements[0]);
        value.C[0] = ofToDouble(elements[1]);
        value.C[1] = ofToDouble(elements[2]);
        
        coValues.push_back(value);
    }
    
    int n = coValues.size();
    
    
    for (int typeIndex = 0; typeIndex < TYPE_NUM; ++typeIndex) {
        
        z[typeIndex] = new float[n]();
        
        
        float * h = new float[n]();
        float * d = new float[n]();
        
  
        z[typeIndex][0] = z[typeIndex][n-1] = 0;
        
        for (int i = 0; i < n-1; i++) {
            h[i]= coValues[i+1].degree - coValues[i].degree;
            d[i+1] = (coValues[i+1].C[typeIndex]-coValues[i].C[typeIndex])/h[i];
        }
        
        z[typeIndex][1]= d[2]-d[1]-h[0]*z[typeIndex][0];
        d[1] = 2*(coValues[2].degree - coValues[0].degree);
        
        for (int i = 1; i < n-2 ; i++) {
            float t = h[i]/d[i];
            z[typeIndex][i+1]= d[i+2] - d[i+1] - z[typeIndex][i]* t; // here
            d[i+1] = 2*(coValues[i+2].degree - coValues[i].degree) - h[i]*t;
        }
        
        z[typeIndex][n-2]-= h[n-2]*z[typeIndex][n-1];
        
        for ( int i = n-2; i > 0; i--) {
            z[typeIndex][i]= (z[typeIndex][i]-h[i]*z[typeIndex][i+1])/d[i];
        }
        
        delete h;
        delete d;
        
    }
    
}

float CoefficientSpline::interpolate(float t, CoefficientType type) {
    int i = 0;
    int j = coValues.size() - 1;
    
    int typeIndex;
    
    switch (type) {
        case Cl:
            typeIndex = 0;
            break;
            
        case Cd:
            typeIndex = 1;
            break;
            
        default:
            break;
    }
    
    while (i < j) {
        float k = (i+j)/2;
        if (coValues[k].degree < t) {
            i = k+1;
        }else {
            j = k;
        }
    }
    
    if (i > 0) {
        i -= 1;
    }
    
    float h = coValues[i+1].degree - coValues[i].degree;
    float d = t - coValues[i].degree;
    
    return ( ((z[typeIndex][i+1]- z[typeIndex][i]) * d / h + z[typeIndex][i]* 3) * d + ((coValues[i+1].C[typeIndex] - coValues[i].C[typeIndex]) / h - (z[typeIndex][i]* 2 + z[typeIndex][i+1]) * h) ) * d + coValues[i].C[typeIndex];
    
}

void CoefficientSpline::drawSpline() {
    
    
}