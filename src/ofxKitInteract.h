#pragma once

#include "ofMain.h"
#include "ofxKitRect.h"

namespace ofxKit {
    
    struct InteractConf {
        Rect * rect;
        InteractConf(Rect * r) {
            rect = r;
        }
    };
    
    class Interact {
    private:
        vector<InteractConf> rects;
        static ofPtr<Interact> intrctr;
        static Interact *ptr;
        Interact() {
            ofRegisterMouseEvents(this);
        }
        
        Rect * findFromXY(int x, int y) {
            
            ofxKit::Rect * selected = nullptr;
            for (auto & r : rects) {
                bool isDeeper = (selected != nullptr) ? selected->depth() : true;
                if (r.rect->conf.outer.inside(x,y) && isDeeper) selected = r.rect;
            }
            return selected;
        }
        
    public:
        
        virtual ~Interact() {};
        
        static ofPtr<Interact> &get() {
            if(ptr==NULL) {
                ptr = new Interact();
                intrctr = ofPtr<Interact>(ptr);
            }
            return intrctr;
        }
        void add(ofxKit::Rect * rect) {
            InteractConf conf(rect);
            rects.push_back(conf);
        }
        
        void mouseMoved(ofMouseEventArgs & e) {
            
        }
        void mouseDragged(ofMouseEventArgs & e) {
            
        }
        void mousePressed(ofMouseEventArgs & e) {
            Rect * selected = findFromXY( e.x, e.y );
            if (selected) selected->pressed(e.x, e.y);
        }
        void mouseScrolled(ofMouseEventArgs & e) {
            
            ofxKit::Rect * selected = nullptr;
            for (auto & r : rects) {
                bool isDeeper = (selected != nullptr) ? selected->depth() : true;
                if (r.rect->conf.outer.inside(e.x,e.y) && isDeeper && r.rect->scroll) selected = r.rect;
            }
            if (selected) selected->scrolled(e);
        }
        void mouseReleased(ofMouseEventArgs & e) {
            
        }
        void mouseEntered(ofMouseEventArgs & e) {
            
        }
        void mouseExited(ofMouseEventArgs & e) {
            
        }
    };
}
