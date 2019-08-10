#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60.0);
    txt.setup();
    ofLog::setAutoSpace(true);
    r = new ofxKit::Rect( ofxKit::RectConf(20,20,ofGetWidth()-40,ofGetHeight()-40)  );
    r->add();
    ofxKit::Rect * scroll = &r->add();
    txtRect = &scroll->add( OFXKIT_OBJ );
    
    ofxKit::Sync::add( txtRect , &txt ); // synchronise texture 
    
    r->add();
}

//--------------------------------------------------------------
void ofApp::update(){
    r->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    for (auto & rr : r->global) {
//        ofLog() << rr->id << rr->conf.outer << rr->conf.ratio;
    }
    ofBackground(20);
    ofSetColor(255);
<<<<<<< HEAD
    r->drawWireframes();
    ofxTxtConf settings;
    txt.draw( ofxTxtConf("hello world", r->conf.inner) );
    (*r->texture)->draw(120, 120, 200, 200);
=======
    txt.drawTexture( ofxTxtConf("Curabitur ex mauris, tristique in dolor in, ullamcorper consectetur turpis. Nunc blandit libero eu posuere convallis. Nullam dapibus, velit non cursus ultrices, orci velit blandit est, sit amet pharetra tellus nisl luctus ante. Praesent pulvinar tellus elit, at pretium urna efficitur quis. Donec luctus, arcu vitae molestie lobortis, dui nisl placerat lacus, ut pharetra lectus tellus vitae tortor. Proin a eros orci. Morbi id erat id risus viverra commodo fermentum quis libero. Donec consectetur placerat lacinia. Donec quis iaculis arcu. Maecenas et diam ac sem placerat volutpat. Donec varius diam sit amet elementum rhoncus. Nullam turpis magna, tincidunt quis orci at, vestibulum molestie est. Mauris non nunc congue, porta quam sed, pharetra est. Etiam convallis nunc ut tristique ultricies. Cras malesuada varius felis, et sollicitudin ante ultrices eu. Praesent ornare tellus felis, in efficitur augue facilisis quis. Curabitur ex mauris, tristique in dolor in, ullamcorper consectetur turpis. Nunc blandit libero eu posuere convallis. Nullam dapibus, velit non cursus ultrices, orci velit blandit est, sit amet pharetra tellus nisl luctus ante. Praesent pulvinar tellus elit, at pretium urna efficitur quis. Donec luctus, arcu vitae molestie lobortis, dui nisl placerat lacus, ut pharetra lectus tellus vitae tortor. Proin a eros orci. Morbi id erat id risus viverra commodo fermentum quis libero. Donec consectetur placerat lacinia. Donec quis iaculis arcu. Maecenas et diam ac sem placerat volutpat. Donec varius diam sit amet elementum rhoncus. Nullam turpis magna, tincidunt quis orci at, vestibulum molestie est. Mauris non nunc congue, porta quam sed, pharetra est. Etiam convallis nunc ut tristique ultricies. Cras malesuada varius felis, et sollicitudin ante ultrices eu. Praesent ornare tellus felis, in efficitur augue facilisis quis. Curabitur ex mauris, tristique in dolor in, ullamcorper consectetur turpis. Nunc blandit libero eu posuere convallis. Nullam dapibus, velit non cursus ultrices, orci velit blandit est, sit amet pharetra tellus nisl luctus ante. Praesent pulvinar tellus elit, at pretium urna efficitur quis. Donec luctus, arcu vitae molestie lobortis, dui nisl placerat lacus, ut pharetra lectus tellus vitae tortor. Proin a eros orci. Morbi id erat id risus viverra commodo fermentum quis libero. Donec consectetur placerat lacinia. Donec quis iaculis arcu. Maecenas et diam ac sem placerat volutpat. Donec varius diam sit amet elementum rhoncus. Nullam turpis magna, tincidunt quis orci at, vestibulum molestie est. Mauris non nunc congue, porta quam sed, pharetra est. Etiam convallis nunc ut tristique ultricies. Cras malesuada varius felis, et sollicitudin ante ultrices eu. Praesent ornare tellus felis, in efficitur augue facilisis quis. ", txtRect->conf.inner) );
//    r->drawWireframes();
    r->drawTextures();
//    (*r->texture)->draw(120, 120, 200, 200);
>>>>>>> 06ffb23f00aba44151b6cbd1f19238bfd3634362
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
