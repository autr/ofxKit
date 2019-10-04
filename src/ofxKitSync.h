#pragma once

#include "ofMain.h"
#include "ofxKitRect.h"
//#include "ofxTxt.h"

namespace ofxKit {
    
    
    /*-- syncs an unknown texture into ofxKit::Rect --*/
    
    class SyncTxt {
    public:
        Rect * rect;
//        ofxTxt * txt;
//        ofTexture * texture;
//        ofEventListener onTextureSet;
//
//        SyncTxt( Rect * r , ofxTxt * t) {
//            rect = r;
//            txt = t;
//            onTextureSet = txt->textureSet.newListener([this]( ofTexture * newTexture ){
//                ofLogNotice("ofxKit") << "syncing texture to rect";
//                rect->texture.set( newTexture );
//            });
//
//        }
    };
    
    class Sync {
    private:
        static ofPtr<Sync> sync;
        static Sync *ptr;
        vector<SyncTxt *> conf;
        bool inited;
        Sync() {
            inited = true;
        }
        
    public:
        
        virtual ~Sync() {}
        
        static ofPtr<Sync> &get() {
            if(ptr==NULL) {
                ptr = new Sync();
                sync = ofPtr<Sync>(ptr);
            }
            return sync;
        }
//        static void add( Rect * r, ofxTxt * t ) {
//            SyncTxt * synctext = new SyncTxt(r, t);
//            get()->conf.push_back(synctext);
//        }
    };
}
