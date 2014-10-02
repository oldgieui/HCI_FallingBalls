#pragma once
#ifndef _TEST_APP
#define _TEST_APP

#include "ofxOpenNI.h"
#include "ofMain.h"

#define MAX_DEVICES 2

const unsigned short WINDOW_WIDTH = 1024;
const unsigned short WINDOW_HEIGHT = 768;

struct Circle
{
    ofPoint circlePosition;
    int radius;
    int red;
    int green;
    int blue;
    
    float vecX;
    float vecY;
    float accX;
    float accY;
};

class testApp : public ofBaseApp{

public:
    
	void setup();
	void update();
	void draw();
    void exit();
    
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
    
    void RenderSkeleton();
    float getAngleBetweenPoints(float x1, float y1, float x2, float y2);
    float getAngleBetweenJoints(ofxOpenNIJoint j1, ofxOpenNIJoint j2);

    
    Circle* createCircle(int x, int y);
    int getRandom1();
    
    int mRadius;
    vector<Circle*> circles;
    
    int mFrameCount;
    Circle* mColFieldL;
    Circle* mColFieldR;
    
    bool CheckCollisionBetweenCircles(Circle c1, Circle c2);

private:
    
    void handEvent(ofxOpenNIHandEvent & event);
    
	ofxOpenNI openNIDevice;
    ofTrueTypeFont verdana;
    
};

#endif
