#pragma once

#include "ofMain.h"
#include "Simulation.h"
#include "CoefficientSpline.h"

#include "ofxOsc.h"
#include "natOscMocapDataReceiver.h"

#define NUM_POS 20

#define PORT_FROM_CORTEX 12345
#define PORT_TO_UNITY 8810
#define NUM_MSG_STRINGS 20

#define TEMPLATE_RACKET_NUM 2
#define TEMPLATE_RACKET_MARKER_NUM 4


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
    float unit;
    
    float mass;
    float area;
    double rho;
    double gravity;
    
    ofVec3f velocity;
    
    Simulation simu;
    CoefficientSpline cSpline;
    
    ofVec3f racketPos[2];
    vector <ofVec3f> bpos[2];
    
    ofVec3f mouse;
    
    // receive from Cortex
    ofTrueTypeFont font;
    ofxOscReceiver receiver;
    int current_msg_string;
    string msg_strings[NUM_MSG_STRINGS];
    float timers[NUM_MSG_STRINGS];
    nat::OscMocapDataReceiver oscMocapReciever;
    ofVec3f racketMarkerPos[TEMPLATE_RACKET_NUM][TEMPLATE_RACKET_MARKER_NUM];
    
    // send to Unity
    ofxOscSender sender;
    
    ofVec3f shuttlePos;
    vector <ofVec3f> bspos;
    int curNum;
    
    ofVec3f initVel;
    float initDeg;
    float initRad;
	float initRadZ;
    
    float curTime;
    float preTime;
    float elapsedTime;
    
    bool shotSwitch;
    bool area_1, area_2;

	bool landingFlag;

	ofEasyCam camera;
		
};
