#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableSmoothing();
    ofEnableDepthTest();
    ofSetFrameRate(75);
    ofSetBackgroundAuto(true);
    
    ofBackground(ofColor::black);
    
    cSpline.readData();
    simu.setCoeSpline(&cSpline);
    
    mass = .001*4.8;
    area = .001*33*33*PI;// Skirt Diameter = 66mm
    rho = 1.205;
    gravity = -9.807;
    
    velocity = ofVec3f(30, 20);
    racketPos[0] = racketPos[1] = ofVec3f(0);
    shuttlePos = ofVec3f(300, 200);
    
    //static double setValue(double m, double area ,double Cd, double Cl, double theta, double rho);
    simu.setValue( mass, area, .001*1.3, .01*1.7, ofDegToRad(180), velocity, rho, shuttlePos);
    simu.setGravity(gravity);
    simu.update();
    
    curTime = 0;
    preTime = 0;
    elapsedTime = 0;
    
    shotSwitch = false;
    
    curNum = 0;
    
    oscMocapReciever.setup( PORT_FROM_CORTEX );
    oscMocapReciever.isInitialized();
    
    sender.setup("localhost", PORT_TO_UNITY);
    
    //実空間とピクセルの単位を調整
    //z軸追加
}



//--------------------------------------------------------------
void ofApp::update(){
    
    //--> PCテスト用
    if (0 < mouse.x) {
        racketPos[0] = mouse;
    }else {
        racketPos[1] = mouse;
    }
    
    //--> Cortex
//    while(oscMocapReciever.hasWaitingMessages()){
//        float **unBuf = oscMocapReciever.getTemplateMarkerData(TEMPLATE_RACKET_NUM, "racket01");
//        for (int i = 0; i < TEMPLATE_RACKET_MARKER_NUM; i++) {
//            racketMarkerPos[0][i].x = unBuf[i][0];
//            racketMarkerPos[0][i].y = unBuf[i][1];
//            racketMarkerPos[0][i].z = unBuf[i][2];
//        }
//        
//        float **unBuf2 = oscMocapReciever.getTemplateMarkerData(TEMPLATE_RACKET_NUM, "racket02");
//        for (int i = 0; i < TEMPLATE_RACKET_MARKER_NUM; i++) {
//            racketMarkerPos[1][i].x = unBuf2[i][0];
//            racketMarkerPos[1][i].y = unBuf2[i][1];
//            racketMarkerPos[1][i].z = unBuf2[i][2];
//        }
//        
//        racketPos[0] = racketMarkerPos[0][0];
//        racketPos[1] = racketMarkerPos[1][0];
//    }
    
    //--> ラケット位置バッファの保存
    for (int i=0; i<2; i++) {
        bpos[i].push_back(racketPos[i]);
        if (bpos[i].size() > NUM_POS) {
            bpos[i].erase(bpos[i].begin());
        }
    }
    
    //--> 1spf
    curTime = ofGetElapsedTimef();
    elapsedTime = curTime - preTime;
    preTime = curTime;
    
    //--> シャトルにあたったとき
    if (0 < mouse.x) { //右エリア
        if (20 > ofDist(racketPos[0].x, racketPos[0].y, shuttlePos.x, shuttlePos.y)) {
            
            //--> 初速度
            vector<ofVec3f>::iterator fitr = bpos[0].end() - 5;
            vector<ofVec3f>::iterator eitr = bpos[0].end() - 1;
            initVel = *eitr - *fitr;
            initVel *= 0.1; //速度
            
            //--> 初角度
            initDeg =  ofRadToDeg( atan2( initVel.y, initVel.x) );
            if (initDeg < 0) {
                initDeg *= -1;
            }
            
            simu.setValue( mass, area, cSpline.getInterpolation( initDeg, Cd), cSpline.getInterpolation( initDeg, Cl), ofDegToRad(initDeg), initVel, rho, shuttlePos);
            simu.update();
            bspos.clear();
            bspos = simu.getPos();
            curNum = 0;
            shotSwitch = true;
        }else {
            shotSwitch = false;
        }
    } else { //左エリア
        if (20 > ofDist(racketPos[1].x, racketPos[1].y, shuttlePos.x, shuttlePos.y)) {
            
            //--> 初速度
            vector<ofVec3f>::iterator fitr = bpos[1].end() - 5;
            vector<ofVec3f>::iterator eitr = bpos[1].end() - 1;
            initVel = *eitr - *fitr;
            initVel *= 0.1;
            
            //--> 初角度
            initDeg =  ofRadToDeg( atan2( initVel.y, initVel.x) );
            if (initDeg < 0) {
                initDeg *= -1;
            }
            
            simu.setValue( mass, area, cSpline.getInterpolation( initDeg, Cd), cSpline.getInterpolation( initDeg, Cl), ofDegToRad(initDeg), initVel, rho, shuttlePos);
            simu.update();
            bspos.clear();
            bspos = simu.getPos();
            curNum = 0;
            shotSwitch = true;
        }else {
            shotSwitch = false;
        }
    }
    
    if(!bspos.empty()){
        shuttlePos = bspos[curNum];
        curNum ++;
    }
    
    //--> send to Unity
    // シャトル
    ofxOscMessage sx;
    sx.setAddress("/shuttlePos/x");
    sx.addFloatArg(shuttlePos.x);
    sender.sendMessage(sx);
    
    ofxOscMessage sy;
    sy.setAddress("/shuttlePos/y");
    sy.addFloatArg(shuttlePos.x);
    sender.sendMessage(sy);
    
    ofxOscMessage sz;
    sz.setAddress("/shuttlePos/z");
    sz.addFloatArg(shuttlePos.z);
    sender.sendMessage(sz);
    
    
    // ラケット1
    ofxOscMessage r1x;
    r1x.setAddress("/racket1Pos/x");
    r1x.addFloatArg(racketPos[0].x);
    sender.sendMessage(r1x);
    
    ofxOscMessage r1y;
    r1y.setAddress("/racket1Pos/y");
    r1y.addFloatArg(racketPos[0].y);
    sender.sendMessage(r1y);
    
    ofxOscMessage r1z;
    r1z.setAddress("/racket1Pos/z");
    r1z.addFloatArg(racketPos[0].z);
    sender.sendMessage(r1z);
    
    // ラケット2
    ofxOscMessage r2x;
    r2x.setAddress("/racket2Pos/x");
    r2x.addFloatArg(racketPos[1].x);
    sender.sendMessage(r2x);
    
    ofxOscMessage r2y;
    r2y.setAddress("/racket2Pos/y");
    r2y.addFloatArg(racketPos[1].y);
    sender.sendMessage(r2y);
    
    ofxOscMessage r2z;
    r2z.setAddress("/racket2Pos/z");
    r2z.addFloatArg(racketPos[1].z);
    sender.sendMessage(r2z);
    
    
}


//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(255, 255, 255);
    //    ofDrawBitmapString(ofGetTimestampString(), 50, 50);
    ofDrawBitmapString(ofToString(ofGetFrameRate()) + "fps", 50, 70);
    //    ofDrawBitmapString(ofToString(elapsedTime), 50, 80);
    ofDrawBitmapString("initVelocity: " + ofToString(initVel), 50, 100);
    ofDrawBitmapString("initDegree: " + ofToString(initDeg), 50, 120);
    ofDrawBitmapString("Cd: "+ofToString(cSpline.getInterpolation( initDeg, Cd)), 50, 150 );
    
    
    //--> 抵抗係数グラフ
    //    cSpline->drawSpline();
    
    //--> 原点を下辺中央に
    ofTranslate(ofGetWidth()/2, ofGetHeight());
    ofScale( 1, -1, 1);
    
    //--> シュミレーション軌跡描画
    ofPushMatrix();
    //    ofTranslate(shuttlePos.x, shuttlePos.y);
    simu.draw();
    ofPopMatrix();
    
    //--> ラケット描画
    ofSetColor(ofColor::yellow);
    ofCircle(racketPos[0].x, racketPos[0].y, 10);
    ofSetColor(ofColor::pink);
    ofCircle(racketPos[1].x, racketPos[1].y, 10);
    
    //    glBegin( GL_LINE_STRIP );
    //    ofSetColor(ofColor::green);
    //    for (int i = 0; i<2; i++) {
    //        for (vector<ofVec3f>::iterator itr = bpos[i].begin(); itr != bpos[i].end(); ++itr) {
    //            //        ofCircle(itr->.x, itr->y, itr->z, 4);
    //            glVertex3d(itr->x, itr->y, itr->z);
    //        }
    //    }
    //    glEnd();
    
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
    
    switch (key) {
        case 'f':
            ofSetFullscreen(true);
            break;
        case 's':
            bspos.clear();
            shuttlePos = ofVec3f(300, 200);
            break;
            
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
    mouse = ofVec3f(x - ofGetWidth()/2, -y + ofGetHeight(), 0);
    
}
