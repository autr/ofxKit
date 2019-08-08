
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
    
//    class Texture {
//    public:
//        ofTexture * texture;
//        float lastUpdated;
//    }
    
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
        string name;
        int type;
        bool root;
        bool fixed;
        bool ghost;
        bool scroll;
        ofRectangle ratio;
        ofRectangle outer;
        ofRectangle inner;
        ofRectangle minimum;
        ofVec4f margins;
        RectConf() {
            name = "";
            type = OFXKIT_COL;
            root = false;
            fixed = false;
            ghost = false;
            root = false;
            scroll = false;
            ratio.set(0,0,1,1);
            minimum.set(0,0,2,2);
            margins.set(0,0,0,0);
        }
    };
    
    struct TextureConf {
        ofTexture ** texture;
        ofRectangle bounds;
    };
    
    
    static ofRectangle Shrink(ofRectangle r, ofVec4f v) {
            r.x += v.z;
            r.y += v.w;
            r.width -= v.x;
            r.height -= v.y;
            r.width -= v.z;
            r.height -= v.w;
            return r;
    }
    static ofRectangle Shrink(ofRectangle r, float x, float y, float w, float h) {
            return Shrink(r, ofVec4f(x,y,w,h));
    }

    class Rect {
    public:
        
        RectConf conf;
        RectStyle style;
        ofTexture ** texture;
        
        bool root;
        bool scroll;
        string id;
        
        float opacity;
        ofRectangle offset;
        ofRectangle transform;
        ofVec3f origin;
        
        void init();
        
        ofxKit::Scroller scroller;
        
        vector<int> location;
        Rect * parent;
        vector<Rect *> childr;
        vector<Rect *> global;
        vector<Rect *> ghosts;
        vector<ofxKit::Error> errors;
        ofEvent<ofxKit::Event> event;
        
        ofEvent<ofxKit::Event> added;
        ofEvent<ofxKit::Event> amended;
        
        
        void detectOverflow(bool a);
        
        Rect *& operator[] (size_t i);
        
        Rect();
        Rect(float x, float y, float w, float h);
        Rect(int t);
        
        int getScrollingGrids( vector<Rect *> & list );
        
        void setFixed( bool b );
        void setScroll( bool b );
        
        void scrollEvent(string & e);
        void set(float x, float y, float w, float h, bool a = true);
        void set(ofRectangle & r, bool a = true);
        void setWidth(float w, bool a = true);
        void setHeight(float h, bool a = true);
        
        Rect * get(vector<int> idx);
        
        
        
        vector<float> getWidths();
        vector<float> getHeights();
        
        void drawWireframes();
        void drawScrollers();
        
        void draw(bool iso = false);
        
        void scrolled( ofMouseEventArgs & e );
        
        void pressed( int x, int y );
        void dragged( int x, int y );
        void released( int x, int y );
        
        vector<int> & position(vector<int> & pos);
        
        void tag();
        void amend();
        void update();
        
        int depth(int d = -1);
        
        bool hasError();
        Rect * getRoot();
        
        void remove(vector<Rect *> & v, Rect * c);
        
        void clear();
        
        
        void remove();
        
        vector<Rect *> collect();
        
        bool isInvalid( Rect * g );
        
        bool move( Rect * u , int idx = -1);
        
        Rect & add(Rect * ch, int idx = -1);
        
        Rect & add(int t, int idx = -1);
        
        string getLocationString(string l = "");
        
        int getIndex();
        
        
        int getFirstAndInColCount(int i);
        int getLastAndInColCount(int i);
        int getLastAndInRowCount(int i);
        
        
        bool inside( Rect * u );
        
        
    };



}
