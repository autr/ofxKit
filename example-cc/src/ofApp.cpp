#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSoundStreamSettings settings;
    settings.numOutputChannels = 2;
    settings.sampleRate = 44100;
    settings.bufferSize = 512;
    settings.numBuffers = 4;
    settings.setOutListener(this);
    soundStream.setup(settings);
    currSample = 0;
    
    
    
    ofLog::setAutoSpace(true);
    ofSetFrameRate(60);
    
    layout = new ofxKit::Rect(0,0,ofGetWidth(),ofGetHeight());
    
    layout->add( OFXKIT_ROW );
    layout->add( OFXKIT_ROW );
    layout->add( OFXKIT_ROW );
    
    for (int i = 0; i < 1000; i++) {
        int r = ofRandom(0, layout->global.size());
        int t = (ofRandom(0,1) > 0.5) ? OFXKIT_COL : OFXKIT_ROW;
        ofxKit::Rect * rect = &layout->global[r]->add(t);
//        for (int i = 0; i < rect->depth() - 1; i++) {
//            layout->global.push_back(rect);
//        }
        sines.push_back(0);
    }
    gui.setup("panel");
    gui.add( volume.set("volume", 0.3, 0, 1) );
    gui.add( minSpeed.set("minSpeed", 0, 0, 2) );
    gui.add( maxSpeed.set("maxSpeed", 2, 0, 2) );
    gui.add( minSize.set("minSize", 0, 0, 20) );
    gui.add( maxSize.set("maxSize", 2, 0.1, 20) );
}

//--------------------------------------------------------------
void ofApp::update(){
    for (int i = 0; i < layout->global.size(); i++) {
        
        float v = (layout->global[i]->conf.outer.width + layout->global[i]->conf.outer.height);
        float sp = ofMap( v, 0, ofGetWidth()+ofGetHeight(), 0.1, maxSpeed);
        
        layout->global[i]->conf.ratio.width = ofMap(sin(ofGetElapsedTimef() * sp), -1, 1, minSize, maxSize);
        layout->global[i]->conf.ratio.height = ofMap(sin(ofGetElapsedTimef() * sp), -1, 1, minSize, maxSize);
    }
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(20);
    layout->set(0,0,720,576);
    
//    layout->global[r]->amend();
    float s2 = ofMap(sin(ofGetElapsedTimef() * 2), -1.0,1.0,0,1.0 );
    float s4 = ofMap(sin(ofGetElapsedTimef() * 4), -1.0,1.0,0,1.0 );
    float s8 = ofMap(sin(ofGetElapsedTimef() * 8), -1.0,1.0,0,1.0 );
    float s12 = ofMap(sin(ofGetElapsedTimef() * (s4*80) ), -1.0,1.0,0,1.0 );
    layout->origin.set(s2, s4);
//    layout->transform.set( 0.5 , 0.5, s2 + 1, s2 + 1);
    layout->amend();
    ofSetColor(255);
    ofPushMatrix();
    ofSetLineWidth(2);
//    ofScale( ofGetWidth()/720, ofGetHeight()/576  );
    layout->drawWireframes();
    ofPopMatrix();
    gui.draw();
    
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



void ofApp::audioOut(ofSoundBuffer &outBuffer) {
    
    // base frequency of the lowest sine wave in cycles per second (hertz)
    float frequency = 172.5;
    
    // mapping frequencies from Hz into full oscillations of sin() (two pi)
    float wavePhaseStep = (frequency / outBuffer.getSampleRate()) * TWO_PI;
    float pulsePhaseStep = (0.5 / outBuffer.getSampleRate()) * TWO_PI;
    
    // this loop builds a buffer of audio containing 3 sine waves at different
    // frequencies, and pulses the volume of each sine wave individually. In
    // other words, 3 oscillators and 3 LFOs.
    
    int w = ofGetWidth();
    int size = layout->global.size();
    for(size_t i = 0; i < outBuffer.getNumFrames(); i++) {
//
//        while (currSample >= size) currSample -= size;
//        float s = sin( i + ofMap( layout->global[currSample]->conf.outer.width, 0, w, 0, 1)   );
        
        float s = 0;
        for (int ii = 0; ii < sines.size(); ii += 4) {
            sines[ii] += 0.1;
            s += sin(sines[ii]);
        }
        s /= sines.size();
        
        s *= volume;
        outBuffer.getSample(i, 0) = s;
        outBuffer.getSample(i, 1) = s;
        
        currSample += 1;
    }
    
    unique_lock<mutex> lock(audioMutex);
    lastBuffer = outBuffer;
}
