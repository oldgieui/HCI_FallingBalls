#include "testApp.h"
#include <math.h>
//#define PI 3.14

//--------------------------------------------------------------
void testApp::setup() {
    
    ofSetLogLevel(OF_LOG_VERBOSE);

    openNIDevice.setup();
    openNIDevice.addImageGenerator();
    openNIDevice.addDepthGenerator();
    openNIDevice.setRegister(true);
    openNIDevice.setMirror(true);
    openNIDevice.setSkeletonProfile(XN_SKEL_PROFILE_ALL);

    openNIDevice.setLogLevel(OF_LOG_VERBOSE);
    openNIDevice.setResolution(WINDOW_WIDTH, WINDOW_HEIGHT, 30);
    openNIDevice.addUserGenerator();
    openNIDevice.setMaxNumUsers(1);

    mRadius = 15;
    srand((unsigned int)time(NULL));
    mColFieldL = createCircle(312, 700);
    mColFieldL->radius = 45;
    
    mColFieldR = createCircle(712, 700);
    mColFieldR->radius = 45;
    
    

    
    // setup the hand generator
//    openNIDevice.addHandsGenerator();
    
    // add all focus gestures (ie., wave, click, raise arm)
//    openNIDevice.addAllHandFocusGestures();
    
    // or you can add them one at a time
    //vector<string> gestureNames = openNIDevice.getAvailableGestures(); // you can use this to get a list of gestures
                                                                         // prints to console and/or you can use the returned vector
    //openNIDevice.addHandFocusGesture("Wave");
    
    openNIDevice.setMaxNumHands(4);
    
    for(int i = 0; i < openNIDevice.getMaxNumHands(); i++){
        ofxOpenNIDepthThreshold depthThreshold = ofxOpenNIDepthThreshold(0, 0, false, true, true, true, true); 
        // ofxOpenNIDepthThreshold is overloaded, has defaults and can take a lot of different parameters, eg:
        // (ofxOpenNIROI OR) int _nearThreshold, int _farThreshold, bool _bUsePointCloud = false, bool _bUseMaskPixels = true, 
        // bool _bUseMaskTexture = true, bool _bUseDepthPixels = false, bool _bUseDepthTexture = false, 
        // int _pointCloudDrawSize = 2, int _pointCloudResolution = 2
        openNIDevice.addDepthThreshold(depthThreshold);
    }
    
    openNIDevice.start();
    openNIDevice.setFrame(30);
    
    verdana.loadFont(ofToDataPath("verdana.ttf"), 18);
}

//--------------------------------------------------------------
void testApp::update(){
    openNIDevice.update();
    
    // reset all depthThresholds to 0,0,0
    for(int i = 0; i < openNIDevice.getMaxNumHands(); i++){
        ofxOpenNIDepthThreshold & depthThreshold = openNIDevice.getDepthThreshold(i);
        ofPoint leftBottomNearWorld = ofPoint(0,0,0);
        ofPoint rightTopFarWorld = ofPoint(0,0,0);
        ofxOpenNIROI roi = ofxOpenNIROI(leftBottomNearWorld, rightTopFarWorld);
        depthThreshold.setROI(roi);
    }
    
    // iterate through users
    for (int i = 0; i < openNIDevice.getNumTrackedHands(); i++){
        
        // get a reference to this user
        ofxOpenNIHand & hand = openNIDevice.getTrackedHand(i);
        
        // get hand position
        ofPoint & handWorldPosition = hand.getWorldPosition(); // remember to use world position for setting ROIs!!!
        
        // set depthThresholds based on handPosition
        ofxOpenNIDepthThreshold & depthThreshold = openNIDevice.getDepthThreshold(i); // we just use hand index for the depth threshold index
        ofPoint leftBottomNearWorld = handWorldPosition - 100; // ofPoint has operator overloading so it'll subtract/add 50 to x, y, z
        ofPoint rightTopFarWorld = handWorldPosition + 100;
        
        ofxOpenNIROI roi = ofxOpenNIROI(leftBottomNearWorld, rightTopFarWorld);
        depthThreshold.setROI(roi);
        
    }
    
    circles.push_back(createCircle(ofRandom(1025), ofRandom(90)));
    circles.push_back(createCircle(ofRandom(1025), ofRandom(90)));
//    circles.push_back(createCircle(ofRandom(1025), ofRandom(192)));
//    circles.push_back(createCircle(ofRandom(1025), ofRandom(192)));
    
    for (int i = 0; i < circles.size(); i++) {
        circles[i]->circlePosition.x += circles[i]->vecX;
        circles[i]->circlePosition.y += circles[i]->vecY;
        circles[i]->vecX += circles[i]->accX;
        circles[i]->vecY += circles[i]->accY;
        if (circles[i]->circlePosition.y > 768 + circles[i]->radius) {
            circles.erase(circles.begin() + i);
        }
        if (circles[i]->circlePosition.x <= 0 ||
            circles[i]->circlePosition.x >= 1024) {
//            circles[i]->vecX *= -1;
            circles.erase(circles.begin() + i);
        }
        
        if (CheckCollisionBetweenCircles(*circles[i], *mColFieldL)) {
            if (circles[i]->circlePosition.y <= mColFieldL->circlePosition.y) {
                if (circles[i]->circlePosition.x - circles[i]->radius >= mColFieldL->circlePosition.x + (mColFieldL->radius/2)) {
                    if (circles[i]->vecX < 0) {
                        circles[i]->vecX *= -0.75;
                    }
                }
                else if(circles[i]->circlePosition.x - circles[i]->radius <= mColFieldL->circlePosition.x + (mColFieldL->radius/2)) {
                    if (circles[i]->circlePosition.x + circles[i]->radius >= mColFieldL->circlePosition.x) {
                        if (circles[i]->vecX < 0) {
                            circles[i]->vecX *= -0.75;
                        }
                    }
                    else if(circles[i]->circlePosition.x + circles[i]->radius < mColFieldL->circlePosition.x) {
                        if (circles[i]->vecX > 0) {
                            circles[i]->vecX *= -0.75;
                        }
                    }
                    if (circles[i]->vecY > 0) {
                        circles[i]->vecY *= -0.75;
                    }
                }
                else if (circles[i]->circlePosition.x + circles[i]->radius <= mColFieldL->circlePosition.x - (mColFieldL->radius/2)){
                    if (circles[i]->vecX > 0) {
                        circles[i]->vecX *= -0.75;
                    }
                }
            }
            else if (circles[i]->circlePosition.y >= mColFieldL->circlePosition.y){
                if (circles[i]->circlePosition.x > mColFieldL->circlePosition.x) {
                    if (circles[i]->vecX < 0) {
                        circles[i]->vecX *= -0.75;
                    }
                    if (circles[i]->vecY < 0) {
                        circles[i]->vecY *= -0.75;
                    }
                }
                else if (circles[i]->circlePosition.x < mColFieldL->circlePosition.x){
                    if (circles[i]->vecX > 0) {
                        circles[i]->vecX *= -0.75;
                    }
                    if (circles[i]->vecY < 0) {
                        circles[i]->vecY *= -0.75;
                    }
                }
            }
        }
        if (CheckCollisionBetweenCircles(*circles[i], *mColFieldR)) {
            if (circles[i]->circlePosition.y <= mColFieldR->circlePosition.y) {
                if (circles[i]->circlePosition.x - circles[i]->radius >= mColFieldR->circlePosition.x + (mColFieldR->radius/2)) {
                    if (circles[i]->vecX < 0) {
                        circles[i]->vecX *= -0.75;
                    }
                }
                else if(circles[i]->circlePosition.x - circles[i]->radius <= mColFieldR->circlePosition.x + (mColFieldR->radius/2)) {
                    if (circles[i]->circlePosition.x + circles[i]->radius >= mColFieldR->circlePosition.x) {
                        if (circles[i]->vecX < 0) {
                            circles[i]->vecX *= -0.75;
                        }
                    }
                    else if(circles[i]->circlePosition.x + circles[i]->radius < mColFieldR->circlePosition.x) {
                        if (circles[i]->vecX > 0) {
                            circles[i]->vecX *= -0.75;
                        }
                    }
                    if (circles[i]->vecY > 0) {
                        circles[i]->vecY *= -0.75;
                    }
                }
                else if (circles[i]->circlePosition.x + circles[i]->radius <= mColFieldR->circlePosition.x - (mColFieldR->radius/2)){
                    if (circles[i]->vecX > 0) {
                        circles[i]->vecX *= -0.75;
                    }
                }
            }
            else if (circles[i]->circlePosition.y >= mColFieldR->circlePosition.y){
                if (circles[i]->circlePosition.x > mColFieldR->circlePosition.x) {
                    if (circles[i]->vecX < 0) {
                        circles[i]->vecX *= -0.75;
                    }
                    if (circles[i]->vecY < 0) {
                        circles[i]->vecY *= -0.75;
                    }
                }
                else if (circles[i]->circlePosition.x < mColFieldR->circlePosition.x){
                    if (circles[i]->vecX > 0) {
                        circles[i]->vecX *= -0.75;
                    }
                    if (circles[i]->vecY < 0) {
                        circles[i]->vecY *= -0.75;
                    }
                }
            }
        }

        if (CheckCollisionBetweenCircles(*mColFieldL, *mColFieldR)){
            mRadius = ofRandom(30);
        }
    }

}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(255, 255, 255);
    
//    pics[picIndex].draw(0, 0);
    ofPushMatrix();
    // draw debug (ie., image, depth, skeleton)
//    openNIDevice.drawDebug();
    openNIDevice.drawImage(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    ofPopMatrix();
    
    ofPushMatrix();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    ofDisableBlendMode();
    ofPopMatrix();
    
    RenderSkeleton();
    
    // draw some info regarding frame counts etc
//	ofSetColor(0, 255, 0);
//	string msg = " MILLIS: " + ofToString(ofGetElapsedTimeMillis()) + " FPS: " + ofToString(ofGetFrameRate()) + " Device FPS: " + ofToString(openNIDevice.getFrameRate());
//	verdana.drawString(msg, 20, 480 - 20);
    
    for (int i = 0; i < circles.size(); i++)
    {
        Circle* c = circles[i];
        ofSetColor(c->red, c->green, c->blue, 180);
        ofCircle(c->circlePosition, c->radius);
    }
    ofSetColor(mColFieldL->red, mColFieldL->green, mColFieldL->blue, 180);
    ofCircle(mColFieldL->circlePosition.x, mColFieldL->circlePosition.y, mColFieldL->radius);
    
    ofSetColor(mColFieldR->red, mColFieldR->green, mColFieldR->blue, 180);
    ofCircle(mColFieldR->circlePosition.x, mColFieldR->circlePosition.y, mColFieldR->radius);
}

//--------------------------------------------------------------
void testApp::handEvent(ofxOpenNIHandEvent & event){
    // show hand event messages in the console
    ofLogNotice() << getHandStatusAsString(event.handStatus) << "for hand" << event.id << "from device" << event.deviceID;
}

//--------------------------------------------------------------
void testApp::exit(){
    openNIDevice.stop();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

void testApp::RenderSkeleton(){
    int numUsers = openNIDevice.getNumTrackedUsers();
    for (int i = 0; i<numUsers; i++) {
        openNIDevice.drawSkeleton(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, i);
        ofxOpenNIUser &user = openNIDevice.getTrackedUser(i);
        
        if (!user.isSkeleton())
        {
            break;
        }
        
        ofxOpenNIJoint leftHand = user.getJoint(JOINT_LEFT_HAND);
        ofxOpenNIJoint rightHand = user.getJoint(JOINT_RIGHT_HAND);
        
//        float wRatio = WINDOW_WIDTH / 640;
//        float hRatio = WINDOW_HEIGHT / 480;
        
        mColFieldL->circlePosition.x = leftHand.getProjectivePosition().x *  WINDOW_WIDTH / 640;
        mColFieldL->circlePosition.y = leftHand.getProjectivePosition().y * WINDOW_HEIGHT / 480;
        mColFieldR->circlePosition.x = rightHand.getProjectivePosition().x *  WINDOW_WIDTH / 640;
        mColFieldR->circlePosition.y = rightHand.getProjectivePosition().y * WINDOW_HEIGHT / 480;
    }
}




float testApp::getAngleBetweenJoints(ofxOpenNIJoint j1, ofxOpenNIJoint j2){
    ofPoint& j1P = j1.getProjectivePosition();
    ofPoint& j2P = j2.getProjectivePosition();
    
    return getAngleBetweenPoints(j1P.x, j1P.y, j2P.x, j2P.y);
}


float testApp::getAngleBetweenPoints(float x1, float y1, float x2, float y2){

    float a = x2 - x1;
    if(a < 0 ){
        a *= -1;
    }
    float b = y2 - y1;
    if(b < 0){
        b *= -1;
    }
    float c = sqrt(pow(a, 2) + pow(b, 2));
    
    float angle = acosf(a / c);
    
    if(y1 > y2) {
        angle = (2*PI) - angle;
    }
    return angle;
}


Circle* testApp::createCircle(int x, int y){
    Circle* c = new Circle;
    c->circlePosition = ofPoint(x,y);
    c->radius = mRadius;
    c->red = ofRandom(256);
    c->green = ofRandom(256);
    c->blue = ofRandom(256);
    
    c->vecX = ofRandom(5) * getRandom1();
    c->vecY = ofRandom(5) * getRandom1();
    c->accX = 0;
    c->accY = 0.05;
    
    return c;
}

int testApp::getRandom1(){
    int r = (rand()%2);
    if (r == 0) {
        return -1;
    } else if (r == 1){
        return 1;
    }
}

bool testApp::CheckCollisionBetweenCircles(Circle c1, Circle c2){
    float dX = c1.circlePosition.x - c2.circlePosition.x;
    float dY = c1.circlePosition.y - c2.circlePosition.y;
    float totalRadius = c1.radius + c2.radius;
    if ((dX * dX + dY * dY) < (totalRadius * totalRadius)) {
        return true;
    }
    else{
        return false;
    }
}
