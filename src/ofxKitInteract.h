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
            debug = false;
            ofRegisterMouseEvents(this);
        }
        
        Rect * findFromXY(int x, int y, bool onlyScroll) {

            ofxKit::Rect * selected = nullptr;
            for (auto & r : rects) {
                bool isNull = (selected == nullptr);
                bool isInside = r.rect->conf.outer.inside(x,y);
                bool isScroll = (onlyScroll) ? r.rect->conf.scroll : true;
                bool isDeeper = (isNull) ? true : ( r.rect->depth() > selected->depth() );
                if (isInside && isScroll && isDeeper) selected = r.rect;
            }
            return selected;
        }
        
    public:
        
        bool debug;
        virtual ~Interact() {};
        
        static ofPtr<Interact> &get() {
            if(ptr==NULL) {
                ptr = new Interact();
                intrctr = ofPtr<Interact>(ptr);
            }
            return intrctr;
        }
        void add(ofxKit::Rect * rect) {
            ofLogNotice("ofxKit::Interact") << "adding" << rects.size();
            InteractConf conf(rect);
            rects.push_back(conf);
        }
        
        void mouseMoved(ofMouseEventArgs & e) {

        }
        void mouseDragged(ofMouseEventArgs & e) {

            Rect * selected = findFromXY( e.x, e.y , false);
            if (selected) {
                if (debug) ofLogNotice("ofxKit::Interact") << "pressed on" << selected->id;
                selected->dragged(e.x, e.y);
            }
        }
        void mousePressed(ofMouseEventArgs & e) {
            Rect * selected = findFromXY( e.x, e.y , false );
            if (selected) {
                if (debug) ofLogNotice("ofxKit::Interact") << "pressed on" << selected->id;
                selected->pressed(e.x, e.y);
            }
        }
        void mouseScrolled(ofMouseEventArgs & e) {

            Rect * selected = findFromXY( e.x, e.y , true );
            if (selected) {

                if (debug) ofLogNotice("ofxKit::Interact") << "scrolled on" << selected->whereAreYou();
                selected->scrolled(e);
            }
        }
        void mouseReleased(ofMouseEventArgs & e) {
            for(auto & rect : rects) rect.rect->released(e.x,e.y);
        }
        void mouseEntered(ofMouseEventArgs & e) {
            
        }
        void mouseExited(ofMouseEventArgs & e) {
            
        }
    };
}
