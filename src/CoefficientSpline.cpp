//
//  coefficientSpline.cpp
//  traceSumilation
//
//  Created by Yumiko Kokubu on 12/18/15.
//
//

#include "coefficientSpline.h"

CoefficientSpline::CoefficientSpline() {
    
}

void CoefficientSpline::readData() {
    string filePath = "coefficient.csv";
    ofFile file(filePath);
    
    //読み込み失敗した場合
    if(!file.exists()){
        ofLogError("The file " + filePath + " is missing");
    }
    
    ofBuffer buffer(file);
    while (!buffer.isLastLine()) {
        string line = buffer.getNextLine();
        vector<string> elements = ofSplitString(line, ",");

        CoefficientValue values;
        values.degree = ofToDouble(elements[0]);
        values.Cl = ofToDouble(elements[1]);
        values.Cd = ofToDouble(elements[2]);
        
        coValues.push_back(values); //coValuesに値入ってない...
        
    }
    
//     cout << coefficientValues[2].degree << endl;
    
}

void CoefficientSpline::drawSpline() {
    
}