#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofLog::setAutoSpace(true);
    ofSetFrameRate(60);
    
    layout = new ofxKit::Rect(0,0,ofGetWidth(),ofGetHeight());
    
    layout->add( OFXKIT_ROW );
    layout->add( OFXKIT_ROW );
    layout->add( OFXKIT_ROW );
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(20);
    
    layout->set(0,0,ofGetWidth(),ofGetHeight());
    ofSetColor(255);
    layout->drawWireframes();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
