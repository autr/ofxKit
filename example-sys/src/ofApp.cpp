#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    ofLog::setAutoSpace(true);
    ofSetFrameRate(60);
    
    layout = new ofxKit::Rect(20,20,ofGetWidth() - 40,ofGetHeight()/2);
    
    layout->add( OFXKIT_ROW );
    layout->add( OFXKIT_ROW );
    ofxKit::Rect * r = &layout->add( OFXKIT_ROW );
    r->conf.margins.set(20,20,20,20);
    ofxKit::Rect * thing = &r->add( OFXKIT_COL );
    thing->conf.margins.set(10,10,10,10);
    thing->add( OFXKIT_COL );
    r->add( OFXKIT_COL );
    r->add( OFXKIT_COL );
    ofxKit::Rect * c = &r->add( OFXKIT_COL );
    ofxKit::Rect * cc = &c->add( OFXKIT_OBJ );
    cc->conf.margins.set(10,10,10,10);
    cc->conf.fixed = true;
    cc->setHeight(600, true);
    
    ofxKit::Sys::volumes();
    ofxKit::DiskSpace dev("/Volumes/Macintosh HD/");
    ofLog() << "Disk A" << dev.capacity->human;
    ofxKit::Directory("~/Downloads");
    
}

//--------------------------------------------------------------
void ofApp::update(){
    layout->update();
//    ofLog() << ofRandom(0,1);
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(20);
//    layout->set(20,20,ofGetWidth() - 40,ofGetHeight()/2);
    
    ofSetColor(255);
    ofSetLineWidth(2);
    layout->drawWireframes();
    layout->drawScrollers();
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

//    ofxKit::Sys::open("Open A Thing", true);
    
    ofSystem("sleep 4");
    
    //ofxKit::ByteSize size = ofxKit::Sys::space("/Macintosh HD");
    
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
