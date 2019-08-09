#pragma once

#include "ofxKitRect.h"

namespace ofxKit {

    struct Event {
        Rect * rect;
        string type;
        Event( string t, Rect * r ) {
            type = t;
            rect = r;
        }
    };


    struct Action {
        ofRectangle oBounds;
        int type;
        ofPoint oPress;
        vector<float> mHeights;
        vector<float> mWidths;
        ofxKit::Rect * unit;
        
        Action(int t,  Rect * u, ofPoint p, vector<float> mW = {}, vector<float> mH = {}) {
            type = t;
            unit = u;
            oPress = p;
            mWidths = mW;
            mHeights = mH;
            oBounds = u->conf.outer;
        }
    };


    struct Areas {
        ofRectangle t, r, b, l;
        bool isLast, isFirst, isInCol, isInRow, noInner;
        
        
        Areas() {
            
        }
        Areas(ofxKit::Rect * u) {
            
            int padd = 5;
            
            ofRectangle bo(u->conf.outer);
            
            isLast = false;
            isFirst = (u->getIndex() == 0);
            isInCol = false;
            isInRow = false;
            
            if (u->parent) {
                isLast = (u->getIndex() == u->parent->childr.size() - 1);
                isInCol = (u->parent->conf.type == OFXKIT_COL);
                isInRow = (u->parent->conf.type == OFXKIT_ROW);
            }
            noInner = (u->childr.size() <= 1);
            
            ofPoint tl = bo.getTopLeft();
            ofPoint tr = bo.getTopRight();
            ofPoint br = bo.getBottomRight();
            ofPoint bl = bo.getBottomLeft();
            
            float w = bo.width;
            float h = bo.height;
            
            /*-- offset by number of overlapping areas --*/
            
            if (isLast && isInCol) {
                int i = u->getLastAndInColCount(0);
                bl.y -= (padd * 2) * i;
                br.y -= (padd * 2) * i;
            }
            if (isFirst && isInCol) {
                int i = u->getFirstAndInColCount(0);
                tl.y += (padd * 2) * i;
                tr.y += (padd * 2) * i;
            }
            
            b.set( bl.x, bl.y - padd, w, padd * 2 );
            t.set( tl.x, tl.y - padd, w, padd * 2 );
            
            l.set( tl.x - padd, tl.y, padd * 2, h );
            r.set( tr.x - padd, tr.y, padd * 2, h );
            
            /*-- offset corners --*/
            
            b.x -= padd;
            b.width += padd * 2;
            t.x -= padd;
            t.width += padd * 2;
            l.y -= padd;
            l.height += padd * 2;
            r.y -= padd;
            r.height += padd * 2;
            
            
        }
    };


    struct Error {
        string msg;
        string name;
        Error(string m, string n) {
            msg = m;
            name = n;
        }
    };

}
