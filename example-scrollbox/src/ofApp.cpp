#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofLog::setAutoSpace(true);
    ofSetFrameRate( 60.0f );
    ofBackground(0);
    rect.debug = true;
    ofxKit::Interact::get()->debug = true;
    rect.conf.margins.set(10,10,10,10);
    rect.setScroll(true);
    scroll.conf.type = OFXKIT_OBJ;
    rect.add(scroll);
    ofLog() << "added?" << scroll.added;
    scroll.setFixed(true);
    scroll.setHeight(1200);

//    ofxKit::Rect r;
//    rect.add(r);
}

//--------------------------------------------------------------
void ofApp::update(){
    rect.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    /*-- always do this: --*/

    ofSetColor(255);
    rect.set(10,10,ofGetWidth()-20, ofGetHeight()-20);
    rect.amend();
    rect.drawWireframes(true, true, true);
    rect.drawScrollers();
    
}


/*-- always do this: pass events --*/
/*-- dragged, pressed, released, scrolled --*/


//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

void ofApp::mouseScrolled( ofMouseEventArgs & e ) {

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == ' ') {
        
        /*-- extra: generate some nonsense --*/
        
//        rows.clear();
//        vector<string> ladida = {"li", "la", "di", "da", "di"};
//        for (int i = 0; i < 25; i++) {
//            string scrolli = "Scrol";
//            for (int ii = 0; ii < ofRandom( 0, 7); ii++ ) {
//                int rand = (ii == 0) ? ofRandom( 0, 2) : ofRandom(0, ladida.size());
//                scrolli += ladida[ rand ];
//            }
//            rows.push_back(scrolli);
//        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

//    scrollBox.inner.width = ofGetWidth() - 200;
//    scrollBox.outer.width = ofGetWidth() - 200;
//    scrollBox.outer.height = ofGetHeight() - 200;
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
