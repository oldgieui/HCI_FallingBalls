#include "ofApp.h"
#include <time.h>

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(50,50,50);
    mRadius = 20;
    mFrameCount = 0;
    srand((unsigned int)time(NULL));
    mColField = createCircle(512, 700);
    mColField->radius = 75;
}

//--------------------------------------------------------------
void ofApp::update(){
    circles.push_back(createCircle(ofRandom(1025), ofRandom(192)));
    circles.push_back(createCircle(ofRandom(1025), ofRandom(192)));
    circles.push_back(createCircle(ofRandom(1025), ofRandom(192)));
    
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
            circles[i]->vecX *= -1;
        }
        if (CheckCollisionBetweenCircles(*circles[i], *mColField)) {
            if (circles[i]->circlePosition.y <= mColField->circlePosition.y) {
                if (circles[i]->circlePosition.x - circles[i]->radius >= mColField->circlePosition.x + (mColField->radius/2)) {
                    if (circles[i]->vecX < 0) {
                        circles[i]->vecX *= -0.75;
                    }
                }
                else if(circles[i]->circlePosition.x - circles[i]->radius <= mColField->circlePosition.x + (mColField->radius/2)) {
                    if (circles[i]->circlePosition.x + circles[i]->radius >= mColField->circlePosition.x) {
                        if (circles[i]->vecX < 0) {
                            circles[i]->vecX *= -0.75;
                        }
                    }
                    else if(circles[i]->circlePosition.x + circles[i]->radius < mColField->circlePosition.x) {
                        if (circles[i]->vecX > 0) {
                            circles[i]->vecX *= -0.75;
                        }
                    }
                    if (circles[i]->vecY > 0) {
                        circles[i]->vecY *= -0.75;
                    }
                }
                else if (circles[i]->circlePosition.x + circles[i]->radius <= mColField->circlePosition.x - (mColField->radius/2)){
                    if (circles[i]->vecX > 0) {
                        circles[i]->vecX *= -0.75;
                    }
                }
            }
            else if (circles[i]->circlePosition.y >= mColField->circlePosition.y){
                if (circles[i]->circlePosition.x > mColField->circlePosition.x) {
                    if (circles[i]->vecX < 0) {
                        circles[i]->vecX *= -0.75;
                    }
                    if (circles[i]->vecY < 0) {
                        circles[i]->vecY *= -0.75;
                    }
                }
                else if (circles[i]->circlePosition.x < mColField->circlePosition.x){
                    if (circles[i]->vecX > 0) {
                        circles[i]->vecX *= -0.75;
                    }
                    if (circles[i]->vecY < 0) {
                        circles[i]->vecY *= -0.75;
                    }
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
//    ofSetColor(200,0,0,80);
    for (int i = 0; i < circles.size(); i++)
    {
        Circle* c = circles[i];
        ofSetColor(c->red, c->green, c->blue, 180);
        ofCircle(c->circlePosition, c->radius);
    }
    ofSetColor(mColField->red, mColField->green, mColField->blue, 180);
    ofCircle(mColField->circlePosition.x, mColField->circlePosition.y, mColField->radius);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    mColField->circlePosition.x = x;
    mColField->circlePosition.y = y;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if (button == 0)
    {
        circles.push_back(createCircle(x, y));
    }
    else if (button == 2 && circles.empty() == false)
    {
        int closestCircleNum;
        double closestDistance;
        for (int i = 0; i < circles.size(); i++)
        {
            Circle* c = circles[i];
            double xLength = c->circlePosition.x - x;
            double yLength = c->circlePosition.y - y;
            double distance = sqrt((xLength * xLength) + (yLength * yLength));
            if (i == 0)
            {
                closestCircleNum = 0;
                closestDistance = distance;
            }
            else if (closestDistance > distance)
            {
                closestCircleNum = i;
                closestDistance = distance;
            }
        }
        circles.erase(circles.begin()+closestCircleNum);
    }
    else if (button == 1)
    {
        mRadius = (ofRandom(5) + 1) * 10;
    }
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

Circle* ofApp::createCircle(int x, int y){
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

int ofApp::getRandom1(){
    int r = (rand()%2);
    if (r == 0) {
        return -1;
    } else if (r == 1){
        return 1;
    }
}

bool ofApp::CheckCollisionBetweenCircles(Circle c1, Circle c2){
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

bool ofApp::CheckCollisionBetweenCircleAndRectangle(Circle c, Rectangle r){
    
}
