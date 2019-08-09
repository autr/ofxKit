
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
    
    
    class Texture {
    public:
        bool active;
        ofTexture * ptr;
        bool isNewFrame;
        ofEvent<ofTexture *> updated;
        Texture() {
            active = false;
        }
        void set(ofTexture * ptr_) {
            active = true;
            ptr = ptr_;
            isNewFrame = true;
            ofNotifyEvent(updated, ptr);
        }
        
        ofTexture * get() {
            isNewFrame = false;
            return ptr;
        }
        
        void init() {
            isNewFrame = false;
        }
    };
    
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
    
    class RectConf {
    public:
        string name;
        int type;
        bool fixed;
        bool ghost;
        bool scroll;
        ofRectangle ratio;
        ofRectangle outer;
        ofRectangle inner;
        ofRectangle minimum;
        ofVec4f margins;
        void init() {
            name = "";
            type = OFXKIT_COL;
            fixed = false;
            ghost = false;
            scroll = false;
            ratio.set(0,0,1,1);
            minimum.set(0,0,2,2);
            margins.set(10,10,10,10);
        }
        RectConf() {
            init();
        }
        RectConf(float x, float y, float w, float h, bool fixed = false) {
            init();
            outer.set(x,y,w,h);
        }
        RectConf(ofRectangle r, bool fixed = false) {
            init();
            outer.set(r);
        }
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
//        ofTexture ** texture;
        Texture texture;
        
        bool root;
        bool scroll;
        string id;
        
        float opacity;
        ofVec2f offset;
        ofRectangle transform;
        ofVec3f origin;
        
        void init();
        
        ofxKit::Scroller scroller;
        
        vector<int> location;
        Rect * parent;
        vector<Rect *> childr;
        vector<Rect *> global;
        vector<Rect *> ghosts;
        ofEvent<ofxKit::Event> event;
        
        ofEvent<ofxKit::Event> added;
        ofEvent<ofxKit::Event> amended;
        
        
        /*-- main functions --*/
        
        Rect(int type = OFXKIT_COL, bool root_ = true);
        Rect(RectConf conf_, bool root_ = true);
        
        Rect & add(int t = OFXKIT_COL, int idx = -1);
        Rect & add(RectConf conf_, int idx = -1);
        
        void update();
        
        void amend();
        bool move( Rect * u , int idx = -1);
        
        void remove();
        vector<Rect *> collect();
        void remove(vector<Rect *> & v, Rect * c);
        void clear();
        
        /*-- internal utilities --*/
        
        bool isInvalid( Rect * g );
        void tag();
        string getLocationString(string l = "");
        void amendPtrs( Rect * ch, int idx);
        Rect * getRoot();
        void detectOverflow();
        void scrollEvent(string & e);
        int getScrollingGrids( vector<Rect *> & list );
        
        int getFirstAndInColCount(int i);
        int getLastAndInColCount(int i);
        int getLastAndInRowCount(int i);
        
        ofVec2f & getOffsetXY(ofVec2f & o);
        
        
        /*-- external utilities --*/
        
        Rect * get(vector<int> idx);
        vector<int> & position(vector<int> & pos);
        
        vector<float> getWidths();
        vector<float> getHeights();
        void setFixed( bool b );
        void setScroll( bool b );
        
        bool inside( Rect * u );
        int getIndex();
        int depth(int d = -1);
        
        
        /*-- dimensions --*/
        
        void set(float x, float y, float w, float h, bool reload = true);
        void set(ofRectangle & r, bool reload = true);
        
        void setWidth(float w, bool reload = true);
        void setHeight(float h, bool reload = true);
        
        /*-- drawing --*/
        
        
        void drawWireframes();
        void drawScrollers();
        void draw();
        void drawIsomorphic();
        void drawTextures();
        
        /*-- interactions --*/
        
        void scrolled( ofMouseEventArgs & e );
        void pressed( int x, int y );
        void dragged( int x, int y );
        void released( int x, int y );
        
        
        
        
        
    };



}
