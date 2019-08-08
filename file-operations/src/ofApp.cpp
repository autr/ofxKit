#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    txt.setup();
    ofLog::setAutoSpace(true);
    r = new ofxKit::Rect(20,20,200,200);
    r->texture = &txt.texture;
}

//--------------------------------------------------------------
void ofApp::update(){
    r->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(20);
    ofSetColor(255);
    r->drawWireframes();
    ofxTxtConf settings;
    txt.draw( ofxTxtConf("hello world", r->conf.inner) );
//    (*r->texture)->draw(120, 120, 200, 200);
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
