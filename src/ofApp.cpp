#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableSmoothing();
    ofEnableDepthTest();
    ofSetFrameRate(60);
    ofSetBackgroundAuto(true);
    
    ofBackground(ofColor::black);
    
    cSpline.readData();
    
    mass = .001*4.8;
    area = .001*66*PI/4;
    rho = 1.205;
    gravity = -9.8;
    
    velocity = ofVec3f(30, 20);
    
    //static double setValue(double d, double m, double area ,double Cd, double Cl, double theta, double rho);
    simu.setValue( mass, area, .001*1.3, .01*1.7, ofDegToRad(180), velocity, rho);
    simu.setGravity(gravity);
    
    racketPos = ofVec3f(0);
    shuttlePos = ofVec3f(0, ofGetHeight()/3);
    
    curTime = 0;
    preTime = 0;
    elapsedTime = 0;
    
    shotSwitch = false;

    //実空間とピクセルの単位を調整
    //打点、初速度の取得
    //z軸追加
}



//--------------------------------------------------------------
void ofApp::update(){
    simu.update();
    
    //--> PCテスト用
    racketPos = mouse;
    
    //--> ラケット位置バッファの保存
    bpos.push_back(racketPos);
    if (bpos.size() > NUM_POS) {
        bpos.erase(bpos.begin());
    }
    
    //--> 初速度
    vector<ofVec3f>::iterator fitr = bpos.end() - 4;
    vector<ofVec3f>::iterator eitr = bpos.end() - 1;
    initVel = *eitr - *fitr;
    
    //--> 初角度
    initDeg = ofRadToDeg( atan2( initVel.x, initVel.y) );
    
    //--> 1spf
    curTime = ofGetElapsedTimef();
    elapsedTime = curTime - preTime;
    preTime = curTime;
    
    //
    if (20 > ofDist(racketPos.x, racketPos.y, shuttlePos.x, shuttlePos.y)) {
        simu.setValue( mass, area, .001*1.3, .01*1.7, ofDegToRad(initDeg), initVel, rho);
        shotSwitch = true;
    }else {
        shotSwitch = false;
    }
    
}


//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(255, 255, 255);
//    ofDrawBitmapString(ofGetTimestampString(), 50, 50);
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 50, 70);
//    ofDrawBitmapString(ofToString(elapsedTime), 50, 80);
//    ofDrawBitmapString(ofToString(initVel), 50, 100);
//    ofDrawBitmapString(ofToString(initDeg), 50, 120);
    
    //--> 抵抗係数グラフ
//    ofPushStyle();
//    ofNoFill();
//    ofSetLineWidth(2);
//    ofSetColor(255);
//    ofBeginShape();
//    for(float i=0; i<180 ; i+=0.1) {
//        ofVertex(ofGetWidth()/180. * i, ofGetHeight()/2 - 300 * cSpline.interpolate(i, Cl));
//    }
//    ofEndShape();
//    ofBeginShape();
//    for(float i=0; i<180 ; i+=0.1) {
//        ofVertex(ofGetWidth()/180. * i, ofGetHeight() - 300 * cSpline.interpolate(i, Cd));
//    }
//    ofEndShape();
//    ofPopStyle();
    
    //--> 原点を下辺中央に
    ofTranslate(ofGetWidth()/2, ofGetHeight());
    ofScale( 1, -1, 1);

    //--> シュミレーション軌跡描画
    simu.draw();
    
    //--> ラケット描画
    ofSetColor(ofColor::yellow);
    ofCircle(racketPos.x, racketPos.y, 10);
    
    glBegin( GL_LINE_STRIP );
    ofSetColor(ofColor::green);
    for (vector<ofVec3f>::iterator itr = bpos.begin(); itr != bpos.end(); ++itr) {
//        ofCircle(itr->.x, itr->y, itr->z, 4);
        glVertex3d(itr->x, itr->y, itr->z);
    }
    glEnd();

    //--> シャトル描画
    if (shotSwitch) {
        ofSetColor(ofColor::red);
    }else {
        ofSetColor(ofColor::green);
    }
    
    ofCircle(shuttlePos.x, shuttlePos.y, 10);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
    mouse = ofVec3f(x - ofGetWidth()/2, -y + ofGetHeight(), 0);
    
}
