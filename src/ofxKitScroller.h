#pragma once

#define SCROLL_NOWT 1
#define SCROLL_CHECK 2
#define SCROLL_PASSED 3


namespace ofxKit {
    
    class Rect;

    struct ofxScrollAnim {
        float length;
        float from;
        float to;
        float timeline;
        string direction;
        bool active;
    };

    struct ScrollerSettings {
        float elasticDistance;
        float elasticSpeed;
        float elasticBalance;
        float barRounded;
        float barOuterWidth;
        float barWidth;
        float barPadding;
        float barHoverSpeed;
        ofColor barInnerColor;
        ofColor barOuterColor;
        
        ScrollerSettings() {
            
            elasticDistance = 3;
            elasticSpeed = 40;
            elasticBalance = 0.2;
            
            barRounded = 1;
            barWidth = 6;
            barPadding = 4;
            barOuterWidth = 16;
            barHoverSpeed = 0.4;
            barInnerColor.set(255);
            barOuterColor.set(255,255,255,80);
        }
    };

    class Scroller {
        
    private:
        
        ScrollerSettings s;
        float velocity;
        float activeTimestamp;
        float timestamp;
        float timeline;
        ofxScrollAnim a;
        int scrollState; // NOWT, CHECK, PASSED
        int lastDirection;
        ofRectangle scrollBar;
        ofRectangle scrollBarOuter;
        float lastInnerHeight;
        float lastOuterHeight;
        float lastInnerWidth;
        float lastOuterWidth;
        float hoverTimestamp;
        float hoverOpacity;
        float hoverBarTimestamp;
        float hoverScale;
        bool lastInside;
        bool lastInsideBar;
        bool barsInited;
        
        
        float easeOut(float v, float minIn, float maxIn, float minOut, float maxOut);
        
        float easeInOut(float v, float minIn, float maxIn, float minOut, float maxOut);
        
        void setScrollBar();
        
        void set();
        
    public:
        
        float updateTimestamp;
        ofEvent<string &> event; // innerBoxResized, outerBoxResized, drawing, scrolled
        ofxKit::Rect * rect;
        float scrollY;
        Scroller();
        Scroller(ofxKit::Rect * r);
        void init(ofxKit::Rect * r);
        void update();
        void draw();
        void scrolled( ofMouseEventArgs & e );
        
        bool isDragged;
        ofPoint dragOrigin;
        ofRectangle dragOriginBar;
        
        void pressed( int x, int y );
        void dragged( int x, int y );
        void released( int x, int y );
        
    };
}
