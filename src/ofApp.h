#pragma once

#include "ofMain.h"
#include "Simulation.h"
#include "CoefficientSpline.h"

#define LOOP 10000
#define NUM_POS 20

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
//		void mouseDragged(int x, int y, int button);
//		void mousePressed(int x, int y, int button);
//		void mouseReleased(int x, int y, int button);
//		void windowResized(int w, int h);
//		void dragEvent(ofDragInfo dragInfo);
//		void gotMessage(ofMessage msg);
    
    
    float unit;
    
    float mass;
    float area;
    double rho;
    double gravity;
    
    ofVec3f velocity;
    
    Simulation simu;
    CoefficientSpline cSpline;
    
    ofVec3f racketPos;
    vector <ofVec3f> bpos;
    
    ofVec3f mouse;
    
    ofVec3f shuttlePos;
    
    ofVec3f initVel;
    float initDeg;
    
    float curTime;
    float preTime;
    float elapsedTime;
    
    bool shotSwitch;
		
};
