#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(ofColor::black);
    ofSetFrameRate(60);
    ofSetBackgroundAuto(true);
    
    //static double setValue(double d, double m, double area ,double Cd, double Cl, double theta, double rho);
    simu.setValue(.001*66, 4.8, .001*66*PI/4, 10.3, 1.7, PI, 1.205);
    simu.setGravity(-100.8);

    cSpline.readData();

}



//--------------------------------------------------------------
void ofApp::update(){
    
    simu.update();
    
}


//--------------------------------------------------------------
void ofApp::draw(){

//    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    //simu.update();
    simu.draw();
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
