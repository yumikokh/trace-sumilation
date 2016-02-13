#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofEnableSmoothing();
	ofEnableDepthTest();
	ofSetFrameRate(60);
	ofSetBackgroundAuto(true);

	ofBackground(ofColor::black);

	//実空間と画面上の位置をあわせる
	gui.setup();
	ofVec3f val = ofVec3f(100);
	gui.add( offset.setup("offset", ofVec3f(0,0,0), -val, val) );

	cSpline.readData();
	simu.setCoeSpline(&cSpline);

	//1m is 1000 in oF
	unit = 1000;
	//acculate to Unity unit
	u_unit = 10;

	mass = 4.8 / unit; // 4.8g
	area = 33*33*PI / (unit*unit);// Skirt Diameter = 66mm
	rho = 1.205;
	gravity = -9.807;

	velocity = ofVec3f(0);
	racketPos[0] = racketPos[1] = ofVec3f(0);
	/*initPos = ofVec3f(-3, 3, -12);*/
	initPos = ofVec3f(100, 100, 0); //for mouse check
	shuttlePos = initPos;

	initRadZ = 0;

	shotSwitch = false;
	landingFlag = false;

	curNum = 0;

	oscMocapReciever.setup( PORT_FROM_CORTEX );
	oscMocapReciever.isInitialized();

	sender.setup("localhost", PORT_TO_UNITY);
	sender2.setup("10.1.1.38", PORT_TO_UNITY);


	//static double setValue(double m, double area ,double Cd, double Cl, double theta, double rho);
	simu.setValue( mass, area, .001*1.3, .01*1.7, ofDegToRad(180), velocity, rho, shuttlePos, 0);
	simu.setGravity(gravity);
	//simu.setMagValue(magValue, offset);
	simu.update();

}



//--------------------------------------------------------------
void ofApp::update(){

	//--> PCテスト用
	//if (0 < mouse.x) {
	//	racketPos[0] = mouse;
	//}else {
	//	racketPos[1] = mouse;
	//}

	//--> Get from Cortex
	while(oscMocapReciever.hasWaitingMessages()){
		float **unBuf = oscMocapReciever.getTemplateMarkerData(0, "/racket01");
		for (int i = 0; i < TEMPLATE_RACKET_MARKER_NUM; i++) {
			racketMarkerPos[0][i].x = unBuf[i][0]; 
			racketMarkerPos[0][i].y = unBuf[i][1];
			racketMarkerPos[0][i].z = unBuf[i][2];	
			(float)racketMarkerPos[0][i].x /= unit;
			(float)racketMarkerPos[0][i].y /= unit;
			(float)racketMarkerPos[0][i].z /= unit;
		}
		racketMarkerPos[0][0] = (racketMarkerPos[0][0]+racketMarkerPos[0][2])/2;

		float **unBuf2 = oscMocapReciever.getTemplateMarkerData(1, "/racket02");
		for (int i = 0; i < TEMPLATE_RACKET_MARKER_NUM; i++) {
			racketMarkerPos[1][i].x = unBuf2[i][0];
			racketMarkerPos[1][i].y = unBuf2[i][1];
			racketMarkerPos[1][i].z = unBuf2[i][2];
			(float)racketMarkerPos[1][i].x /= unit;
			(float)racketMarkerPos[1][i].y /= unit;
			(float)racketMarkerPos[1][i].z /= unit;
		}
		racketMarkerPos[1][0] = (racketMarkerPos[1][0]+racketMarkerPos[1][2])/2;

		racketPos[0] = racketMarkerPos[0][0];
		racketPos[1] = racketMarkerPos[1][0];
		

			float **unBuf3 = oscMocapReciever.getTemplateMarkerData(2, "/player01");
		for (int i = 0; i < 2; i++) {
			playerMarkerPos[0][i].x = unBuf3[i][0];
			playerMarkerPos[0][i].y = unBuf3[i][1];
			playerMarkerPos[0][i].z = unBuf3[i][2];
			(float)playerMarkerPos[0][i].x /= unit;
			(float)playerMarkerPos[0][i].y /= unit;
			(float)playerMarkerPos[0][i].z /= unit;
		}

		float **unBuf4 = oscMocapReciever.getTemplateMarkerData(3, "/player02");
		for (int i = 0; i < 2; i++) {
			playerMarkerPos[1][i].x = unBuf4[i][0];
			playerMarkerPos[1][i].y = unBuf4[i][1];
			playerMarkerPos[1][i].z = unBuf4[i][2];
			(float)playerMarkerPos[1][i].x /= unit;
			(float)playerMarkerPos[1][i].y /= unit;
			(float)playerMarkerPos[1][i].z /= unit;
		}

		playerPos[0] = (playerMarkerPos[0][0]+playerMarkerPos[0][1])/2;
		playerPos[1] = (playerMarkerPos[1][0]+playerMarkerPos[1][1])/2;
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
		if ( 50 > pow( (racketPos[i].x-shuttlePos.x)*(racketPos[i].x-shuttlePos.x) + (racketPos[i].y- shuttlePos.y)*(racketPos[i].y- shuttlePos.y) + (racketPos[i].z- shuttlePos.z)*(racketPos[i].z- shuttlePos.z), 0.5f ) ) 
		//if (20 > ofDist(racketPos[i].x, racketPos[i].y, shuttlePos.x, shuttlePos.y) )
		{

			//--> 初速度
			vector<ofVec3f>::iterator fitr = bpos[i].end() - 5;
			vector<ofVec3f>::iterator eitr = bpos[i].end() - 1;
			initVel = *eitr - *fitr;
			initVel *= 6; //速度

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
	m.addFloatArg(shuttlePos.x*u_unit);
	m.addFloatArg(shuttlePos.y*u_unit);
	m.addFloatArg(-shuttlePos.z*u_unit);//左手系
	sender.sendMessage(m);
	sender2.sendMessage(m);
	m.clear();

	// ラケット
	for (int i=0; i<TEMPLATE_RACKET_NUM; i++) {
		for (int j = 0; j < 3; j++) {
			sendName = "/racket0" + ofToString(i+1) + ofToString(j);
			m.setAddress(sendName);
			m.addFloatArg(racketMarkerPos[i][j].x*u_unit);
			m.addFloatArg(racketMarkerPos[i][j].y*u_unit);
			m.addFloatArg(-racketMarkerPos[i][j].z*u_unit);
			sender.sendMessage(m);
			sender2.sendMessage(m);
			m.clear();
		}
	}

	//プレイヤー（視点）
	for (int i=0; i<TEMPLATE_RACKET_NUM; i++) {
			sendName = "/player0" + ofToString(i+1);
			m.setAddress(sendName);
			m.addFloatArg(playerPos[i].x*u_unit);
			m.addFloatArg(playerPos[i].y*u_unit);
			m.addFloatArg(-playerPos[i].z*u_unit);
			sender.sendMessage(m);
			sender2.sendMessage(m);
			m.clear();
	}


}


//--------------------------------------------------------------
void ofApp::draw(){


	ofSetColor(255, 255, 255);
	glDisable(GL_DEPTH_TEST);
	//gui.draw();
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
	//camera.begin();
	//camera.setPosition(racketPos[0]);
	//camera.setDistance(100);

	//--> ラケットのマウス、判定マーカー描画
	ofSetColor(ofColor::red);
	ofDrawSphere(racketPos[0]*unit, 20);
	ofSetColor(ofColor::blue);
	ofDrawSphere(racketPos[1]*unit, 20);

	//--> ラケットマーカー5つ
	for (int j = 0; j < TEMPLATE_RACKET_MARKER_NUM; j++) {
		ofSetColor(ofColor::pink);
		ofDrawSphere(racketMarkerPos[0][j]*unit, 10);

		ofSetColor(ofColor::skyBlue);
		ofDrawSphere(racketMarkerPos[1][j]*unit, 10);
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
	//	ofDrawSphere(shuttlePos.x*magScreen, shuttlePos.y*magScreen, shuttlePos.z*magScreen, 10);
	ofDrawSphere(shuttlePos.x*unit, shuttlePos.y*unit, shuttlePos.z*unit, 10);


	//--> 軸描画
	ofDrawAxis(1000);


	//camera.end();
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

	mouse = ofVec3f(x - ofGetWidth()/2, -y + ofGetHeight(), 0)/unit;

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