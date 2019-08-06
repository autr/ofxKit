
#pragma once
#include "ofMain.h"
#include "ofxKitScroller.h"

#define OFXKIT_COL 0
#define OFXKIT_ROW 1
#define OFXKIT_OBJ 2

#define OFXKIT_RR 0 // resize r
#define OFXKIT_RB 1 // resize b
#define OFXKIT_RL 2 // resize l
#define OFXKIT_RT 3 // resize t

#define OFXKIT_IN 5 // inside

#define OFXKIT_PL 6 // padding L
#define OFXKIT_PT 7 // padding T
#define OFXKIT_PR 8 // padding R
#define OFXKIT_PB 9 // padding B

#define OFXKIT_DT 10 // drop T
#define OFXKIT_DR 11 // drop R
#define OFXKIT_DB 12 // drop B
#define OFXKIT_DL 13 // drop L

#define OFXKIT_DH 14 // drop H
#define OFXKIT_DV 15 // drop V


//class Rect;


namespace ofxKit {
    
    struct Error;
    struct Event;
    
    struct RectStyle {
        ofColor defaultColor;
        ofColor hoverColor;
        ofColor dropColor;
        float minWidth;
        float minHeight;
        RectStyle() {
            defaultColor = ofColor(255);
            hoverColor = ofColor(255, 255, 255, 255);
            dropColor = ofColor(150, 255, 255, 100);
            minWidth = 10;
            minHeight = 10;
        }
    };
    
    struct RectConf {
        
        int type;
        float mH, mW;
        bool root;
        bool fixed;
        bool ghost;
        bool scroll;
        ofRectangle ratio;
        ofRectangle outer;
        ofRectangle inner;
        ofRectangle minimum;
        string name;
        RectConf() {
            type = OFXKIT_COL;
            mH = 1;
            mW = 1;
            root = false;
            fixed = false;
            ghost = false;
            root = false;
            scroll = false;
            ratio.set(0,0,1,1);
            minimum.set(0,0,20,20);
        }
    };
    
    
    static ofRectangle Shrink(ofRectangle r, float v) {
            r.x += v;
            r.y += v;
            r.width -= v*2;
            r.height -= v*2;
            return r;
    }
    static ofRectangle Shrink(float x, float y, float w, float h, float v) {
            ofRectangle r(x + v, y + v, w - (v*2), h - (v*2));
            return r;
    }

    class Rect {
    public:
        
        RectConf conf;
        RectStyle style;
        ofRectangle inner;
        
        int type;
        float mH, mW;
        bool fixed;
        bool ghost;
        bool scroll;
        bool root;
        string name;
        
        float sideOffset;
        ofVec2f offset; // USE THIS
        
        ofxKit::Scroller scrollBox;
        
        vector<int> location;
        Rect * parent;
        vector<Rect *> childr;
        vector<Rect *> global;
        vector<Rect *> ghosts;
        vector<ofxKit::Error> errors;
        ofEvent<ofxKit::Event> event;
        
        ofEvent<ofxKit::Event> added;
        ofEvent<ofxKit::Event> amended;
        
        
        Rect *& operator[] (size_t i);
        
        Rect();
        Rect(float x, float y, float w, float h);
        Rect(int t);
        
        int getScrollingGrids( vector<Rect *> & list );
        
        void setFixed( bool b );
        void setScroll( bool b );
        
        void scrollEvent(string & e);
        void set(float x, float y, float w, float h);
        void set(ofRectangle & r);
        void setWidth(float w);
        void setHeight(float h);
        
        Rect * get(vector<int> idx);
        
        
        
        vector<float> getWidths();
        vector<float> getHeights();
        
        void drawWireframes();
        
        void draw(bool iso = false);
        
        void scrolled( ofMouseEventArgs & e );
        
        void pressed( int x, int y );
        void dragged( int x, int y );
        void released( int x, int y );
        
        
        
        vector<int> & position(vector<int> & pos);
        
        void tag();
        void amend();
        
        
        int depth(int d = -1);
        
        bool hasError();
        Rect * getRoot();
        
        void remove(vector<Rect *> & v, Rect * c);
        
        void load(ofJson & j);
        
        void clear();
        
        
        void remove();
        
        vector<Rect *> collect();
        
        bool isInvalid( Rect * g );
        
        bool move( Rect * u , int idx = -1);
        
        Rect & add(ofJson & j, int idx = -1);
        
        Rect & add(Rect * ch, int idx = -1);
        
        Rect & add(int t, int idx = -1);
        
        string getLocationString(string l = "");
        
        int getIndex();
        
        
        int getFirstAndInColCount(int i);
        int getLastAndInColCount(int i);
        int getLastAndInRowCount(int i);
        
        ofJson json();
        
        bool inside( Rect * u );
        
        
    };



}
