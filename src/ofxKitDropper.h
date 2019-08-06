
#pragma once
#include "ofMain.h"
#include "ofxRect.h"

class Dropper {
public:
    ofxKitRow * row;
    ofxKitRow * source;
    Dropper(ofxKitRow * r) {
        row = r;
        source = new Dropper();
    }
    
    
};
