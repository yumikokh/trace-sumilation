#pragma once

#include "ofMain.h"
#include "Simulation.h"
#include "CoefficientSpline.h"

#include "ofxOsc.h"
#include "natOscMocapDataReceiver.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"

#define NUM_POS 20

#define PORT_FROM_CORTEX 19781
#define PORT_TO_UNITY 8810
#define NUM_MSG_STRINGS 20

#define TEMPLATE_RACKET_NUM 2
#define TEMPLATE_RACKET_MARKER_NUM 5


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
    
    //1m is 1000 in oF
    float unit;
	//acculate to Unity unit
	float u_unit;
    
    float mass;
    float area;
    double rho;
    double gravity;
    
    ofVec3f velocity;
    
    Simulation simu;
    CoefficientSpline cSpline;
    
    ofVec3f racketPos[TEMPLATE_RACKET_NUM];
    vector <ofVec3f> bpos[TEMPLATE_RACKET_NUM];

	ofVec3f playerPos[TEMPLATE_RACKET_NUM];
    
    ofVec3f mouse;
    
    // receive from Cortex
    ofTrueTypeFont font;
    ofxOscReceiver receiver;
    int current_msg_string;
    string msg_strings[NUM_MSG_STRINGS];
    float timers[NUM_MSG_STRINGS];
    nat::OscMocapDataReceiver oscMocapReciever;
    ofVec3f racketMarkerPos[TEMPLATE_RACKET_NUM][TEMPLATE_RACKET_MARKER_NUM];
	ofVec3f playerMarkerPos[TEMPLATE_RACKET_NUM][2];
    
    // send to Unity
    ofxOscSender sender;
	ofxOscSender sender2;
    
    ofVec3f shuttlePos;
    vector <ofVec3f> bspos;
    int curNum;
    
	ofVec3f initPos;
    ofVec3f initVel;
    float initDeg;
    float initRad;
	float initRadZ;
    
    bool shotSwitch;
    bool area_1, area_2;

	bool landingFlag;

	ofEasyCam camera;


	//í≤êÆópGUI
	ofxPanel gui;
	ofxFloatSlider magValue;
	ofxVec3Slider offset;
		
};
