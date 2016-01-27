#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofEnableSmoothing();
	ofEnableDepthTest();
	ofSetFrameRate(75);
	ofSetBackgroundAuto(true);

	ofBackground(ofColor::black);

	//実空間と画面上の位置をあわせる
	gui.setup();
	gui.add(magValue.setup("magValue", 5.f, 1, 1000));
	ofVec3f val = ofVec3f(100);
	gui.add( offset.setup("offset", ofVec3f(0,0,-10), -val, val) );
	gui.add(magScreen.setup("magValueforTest", 50, 1, 1000));

	cSpline.readData();
	simu.setCoeSpline(&cSpline);

	mass = .001*4.8 *10;
	area = .001*33*33*PI;// Skirt Diameter = 66mm
	rho = 1.205;
	gravity = -9.807 *500;

	velocity = ofVec3f(0);
	racketPos[0] = racketPos[1] = ofVec3f(0);
	/*initPos = ofVec3f(-3, 3, -12);*/
	initPos = ofVec3f(-3, 3, 0); //for mouse check
	//initPos.x /= (float)maValue ;
	//initPos.y /=  (float)magValue ;//なぜかココダケout of range..
	//initPos.z /= (float)magValue ;
	//initPos += offset;
	shuttlePos = initPos;

	initRadZ = 0;

	curTime = 0;
	preTime = 0;
	elapsedTime = 0;

	shotSwitch = false;
	landingFlag = false;

	curNum = 0;

	oscMocapReciever.setup( PORT_FROM_CORTEX );
	oscMocapReciever.isInitialized();

	sender.setup("localhost", PORT_TO_UNITY);


	//static double setValue(double m, double area ,double Cd, double Cl, double theta, double rho);
	simu.setValue( mass, area, .001*1.3, .01*1.7, ofDegToRad(180), velocity, rho, shuttlePos, 0);
	simu.setGravity(gravity);
	simu.setMagValue(magValue, offset);
	simu.update();

}



//--------------------------------------------------------------
void ofApp::update(){

	//--> PCテスト用
	if (0 < mouse.x) {
		racketPos[0] = mouse* magValue;
	}else {
		racketPos[1] = mouse* magValue;
	}

	//--> Get from Cortex
	while(oscMocapReciever.hasWaitingMessages()){
		float **unBuf = oscMocapReciever.getTemplateMarkerData(0, "/racket01");
		for (int i = 0; i < TEMPLATE_RACKET_MARKER_NUM; i++) {
			racketMarkerPos[0][i].x = unBuf[i][0]/1000 * magValue + offset->x; // 1mmから1m単位に
			racketMarkerPos[0][i].y = unBuf[i][1]/1000 * magValue + offset->y;
			racketMarkerPos[0][i].z = unBuf[i][2]/1000 * magValue + offset->z;
		}

		float **unBuf2 = oscMocapReciever.getTemplateMarkerData(1, "/racket02");
		for (int i = 0; i < TEMPLATE_RACKET_MARKER_NUM; i++) {
			racketMarkerPos[1][i].x = unBuf2[i][0]/1000 * magValue + offset->x;
			racketMarkerPos[1][i].y = unBuf2[i][1]/1000 * magValue + offset->y;
			racketMarkerPos[1][i].z = unBuf2[i][2]/1000 * magValue + offset->z;
		}

		/*	racketPos[0] = racketMarkerPos[0][0];
		racketPos[1] = racketMarkerPos[1][0];*/
	}

	//--> ラケット位置バッファの保存
	for (int i=0; i<2; i++) {
		bpos[i].push_back(racketPos[i]);
		if (bpos[i].size() > NUM_POS) {
			bpos[i].erase(bpos[i].begin());
		}
	}

	//--> シャトルにあたったとき
	for( int i = 0; i < TEMPLATE_RACKET_NUM; i++) {
		//if ( 1 > pow( (racketPos[i].x-shuttlePos.x)*(racketPos[i].x-shuttlePos.x) + (racketPos[i].y- shuttlePos.y)*(racketPos[i].y- shuttlePos.y) + (racketPos[i].z- shuttlePos.z)*(racketPos[i].z- shuttlePos.z), 0.5f ) ) 
		if (10 > ofDist(racketPos[i].x, racketPos[i].y, shuttlePos.x, shuttlePos.y) )
		{

			//--> 初速度
			vector<ofVec3f>::iterator fitr = bpos[i].end() - 5;
			vector<ofVec3f>::iterator eitr = bpos[i].end() - 1;
			initVel = *eitr - *fitr;
			initVel *= 3; //速度

			//--> 初角度
			initDeg =  ofRadToDeg( atan2( initVel.y, initVel.x) );
			if (initDeg < 0) {
				initDeg *= -1;
			}

			initRadZ =  atan2( initVel.z, initVel.x) ;
			if (initRadZ >= PI/2)
			{
				initRadZ =  atan2( initVel.z, -initVel.x) ;
			}

			simu.setValue( mass, area, cSpline.getInterpolation( initDeg, Cd), cSpline.getInterpolation( initDeg, Cl), ofDegToRad(initDeg), initVel, rho, shuttlePos,  initRadZ);
			simu.setMagValue(magValue, offset);
			simu.update();

			bspos.clear();
			bspos = simu.getPos();
			curNum = 0;

			shotSwitch = true;
		}else {
			shotSwitch = false;
		}
	}

	// 1sごとに軌跡を更新
	if (curNum >= LOOP-2)
	{
		//--> 初速度
		vector<ofVec3f>::iterator fitr = bspos.end() - 5;
		vector<ofVec3f>::iterator eitr = bspos.end() - 1;
		initVel = *eitr - *fitr;
		initVel *= 12; //速度

		//--> 初角度
		initDeg =  ofRadToDeg( atan2( initVel.y, initVel.x) );
		if (initDeg < 0) {
			initDeg *= -1;
		}

		initRadZ =  atan2( initVel.z, initVel.x) ;
		if (initRadZ >= PI/2)
		{
			initRadZ =  atan2( initVel.z, -initVel.x) ;
		}

		simu.setValue( mass, area, cSpline.getInterpolation( initDeg, Cd), cSpline.getInterpolation( initDeg, Cl), ofDegToRad(initDeg), initVel, rho, shuttlePos,  initRadZ);
		simu.setMagValue(magValue, offset);
		simu.update();

		bspos.clear();
		bspos = simu.getPos();
		curNum = 0;
	}

	if(!bspos.empty()){
		shuttlePos = bspos[curNum];
		curNum ++;
	}


	if(shuttlePos.y < 0 ) {
		//float tempX = 0;
		//if(!landingFlag) { tempX = shuttlePos.x; }
		landingFlag = true;

		bspos.clear();
		shuttlePos = initPos;
	}else {
		landingFlag = false;
	}




	//--> send to Unity

	ofxOscMessage m;
	std::string sendName;

	// シャトル
	sendName = "/shuttlePos/";
	m.setAddress(sendName);
	m.addFloatArg(shuttlePos.x);
	m.addFloatArg(shuttlePos.y);
	m.addFloatArg(shuttlePos.z);
	sender.sendMessage(m);
	m.clear();

	// ラケット
	for (int i=0; i<TEMPLATE_RACKET_NUM; i++) {
		for (int j = 0; j < 3; j++) {
			sendName = "/racket0" + ofToString(i+1) + ofToString(j);
			m.setAddress(sendName);
			m.addFloatArg(racketMarkerPos[i][j].x);
			m.addFloatArg(racketMarkerPos[i][j].y);
			m.addFloatArg(racketMarkerPos[i][j].z);
			sender.sendMessage(m);
			m.clear();
		}
	}



}


//--------------------------------------------------------------
void ofApp::draw(){


	ofSetColor(255, 255, 255);
	glDisable(GL_DEPTH_TEST);
	gui.draw();
	//    ofDrawBitmapString(ofGetTimestampString(), 50, 50);
	ofDrawBitmapString(ofToString(ofGetFrameRate()) + "fps", 50, 40);
	ofDrawBitmapString("racket01:" + ofToString(racketPos[0]), 50, 60);
	ofDrawBitmapString("racket02:" + ofToString(racketPos[1]), 50, 70);
	ofDrawBitmapString("shuttle:" + ofToString(shuttlePos), 50, 80);
	ofDrawBitmapString("initVelocity: " + ofToString(initVel), 50, 100);
	ofDrawBitmapString("initDegree: " + ofToString(initDeg), 50, 120);
	ofDrawBitmapString("Cd: "+ofToString(cSpline.getInterpolation( initDeg, Cd)), 50, 150 );
	ofDrawBitmapString("initDegreeZ: "+ofToString(ofRadToDeg(initRadZ)), 50, 170 );

	glEnable(GL_DEPTH_TEST);
	//--> 抵抗係数グラフ
	//    cSpline->drawSpline();

	//--> 原点を下辺中央に
	ofTranslate(ofGetWidth()/2, ofGetHeight());
	ofScale( 1, -1, 1);

	//--> カメラ設定
	camera.begin();
	//camera.setPosition(racketPos[0]);
	//camera.setDistance(100);

	//--> ラケットのマウス、判定マーカー描画
	/*ofSetColor(ofColor::red);
	ofDrawSphere(racketPos[0].x*magScreen, racketPos[0].y*magScreen, 20);
	ofSetColor(ofColor::blue);
	ofDrawSphere(racketPos[1].x*magScreen, racketPos[1].y*magScreen, 20);*/
	ofSetColor(ofColor::red);
	ofDrawSphere(racketPos[0].x, racketPos[0].y, 20);
	ofSetColor(ofColor::blue);
	ofDrawSphere(racketPos[1].x, racketPos[1].y, 20);

	//--> ラケットマーカー
	for (int j = 0; j < TEMPLATE_RACKET_MARKER_NUM; j++) {
		ofSetColor(ofColor::red);
		ofDrawSphere(racketMarkerPos[0][j]*magScreen, 10);

		ofSetColor(ofColor::blue);
		ofDrawSphere(racketMarkerPos[1][j]*magScreen, 10);
	}

	//    glBegin( GL_LINE_STRIP );
	//    ofSetColor(ofColor::green);
	//    for (int i = 0; i<2; i++) {
	//        for (vector<ofVec3f>::iterator itr = bpos[i].begin(); itr != bpos[i].end(); ++itr) {
	//            //        ofCircle(itr->.x, itr->y, itr->z, 4);
	//            glVertex3d(itr->x, itr->y, itr->z);
	//        }
	//    }
	//    glEnd();


	//--> シュミレーション軌跡描画
	//simu.draw();

	//--> シャトル描画
	if (shotSwitch) {
		ofSetColor(ofColor::red);
	}else {
		ofSetColor(ofColor::green);
	}
	//ofDrawSphere(shuttlePos.x*magScreen, shuttlePos.y*magScreen, shuttlePos.z*magScreen, 10);
	ofDrawSphere(shuttlePos.x*magScreen, shuttlePos.y*magScreen, shuttlePos.z, 10);


	//--> 軸描画
	ofDrawAxis(1000);


	camera.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	switch (key) {
	case 'f':
		ofToggleFullscreen();
		break;
	case 'd':
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