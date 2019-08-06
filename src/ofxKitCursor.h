#pragma once

#include "ofMain.h"

namespace ofxKit {

//    enum Cursor {
//        openHand, closedHand, arrowCursor, resizeLeftRight
//    };
    //#include "ofxSVG.h"


    class Cursor {
        
    private:
        static ofPtr<Cursor> instance;
        static Cursor *instancePtr;
    public:
        ofDirectory d;
        vector<string> names;
        map<string, ofTexture> t;
        string curr;
        Cursor();
        
        void random();
        
    //    void draw(float x, float y);
        static void set(string s);
        
        virtual ~Cursor();
        static ofPtr<Cursor> &getInstance();
        
        
    };
}
