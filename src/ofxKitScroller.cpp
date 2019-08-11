#pragma once
#include "ofxKitRect.h"

namespace ofxKit {

    float Scroller::easeOut(float v, float minIn, float maxIn, float minOut, float maxOut) {
        float t = v - minIn;
        float c = maxOut - minOut;
        float d = maxIn - minIn;
        float b = minOut;
        return -c *(t/=d)*(t-2) + b;
    }

    float Scroller::easeInOut(float v, float minIn, float maxIn, float minOut, float maxOut) {
        float t = v - minIn;
        float c = maxOut - minOut;
        float d = maxIn - minIn;
        float b = minOut;
        if ((t/=d/2) < 1) return c/2*t*t + b;
        return -c/2 * ((--t)*(t-2) - 1) + b;
    }

    void Scroller::setScrollBar() {
        
        float oY = rect->conf.outer.y;
        float oH = rect->conf.outer.height;
        float iY = rect->conf.inner.y;
        float iH = rect->conf.inner.height;
        
        
        scrollBarOuter.x = rect->conf.outer.getTopRight().x - s.barOuterWidth;
        scrollBarOuter.y = rect->conf.outer.y;
        scrollBarOuter.width = s.barOuterWidth;
        scrollBarOuter.height = rect->conf.outer.height;
        
        
        scrollBar.set( scrollBarOuter );
        scrollBar.height = ofMap( oH, 0, iH, 0, oH);
        
        
        float fromA = oY;
        float fromB = (oH - iH) + oY;
        float toA = oY;
        float toB = oY + oH - scrollBar.height;
        
        scrollBar.y = ofMap( iY, fromB, fromA, toB, toA );
        
        if (scrollBar.y < oY) {
            scrollBar.height -= oY - scrollBar.y;
            scrollBar.y = oY;
        }
        float ibr = scrollBar.getBottomRight().y;
        float obr = rect->conf.outer.getBottomRight().y;
        if (ibr > obr) {
            scrollBar.height -= ibr - obr;
        }
        
        float boop = s.barOuterWidth - ( s.barPadding * 2 );
        scrollBar.width = ( boop * hoverScale ) + ( s.barWidth * (1-hoverScale) );
        scrollBar.x = rect->conf.outer.getTopRight().x - scrollBar.width - s.barPadding;
        scrollBar.y += s.barPadding;
        scrollBar.height -= s.barPadding * 2;
        
        if (!barsInited) barsInited = true;
        
    }

    void Scroller::set() {
        
        if (rect->conf.inner.y != scrollY) {
            rect->conf.inner.y = scrollY;
            setScrollBar();
            string a = "scrolled";
            ofNotifyEvent(event, a);
        }
    }
    Scroller::Scroller() {
        
    }
    Scroller::Scroller(ofxKit::Rect * r) {
        init(r);
    }
    
    void Scroller::init(ofxKit::Rect * r) {
        
        lastDirection = 9;
        scrollY = 999999;
        velocity = 0;
        timestamp = 0;
        activeTimestamp = 0;
        scrollState = SCROLL_PASSED;
        lastInside = false;
        hoverOpacity = 0;
        hoverBarTimestamp = 0;
        hoverScale = 0;
        barsInited = false;
        rect = r;
    }
    
    void Scroller::update() {
        if (rect == nullptr) return;
        if (rect->conf.inner.height < rect->conf.outer.height) return;
        
        float t = ofGetElapsedTimef();
        
        if (scrollY == 999999) {
            scrollY = rect->conf.inner.y;
            set();
        }
        
        if (scrollState == SCROLL_CHECK) {
            if (t - timestamp > 0.1) {
                scrollState = SCROLL_PASSED;
//                ofLogVerbose("[Scroller]") << "Native timeout PASSED";
            }
        }
        
        if (a.active) {
            
            float l = a.length;
            float split = s.elasticBalance;
            
            if (a.timeline < l * split) {
                scrollY = easeOut(a.timeline, 0, l * split, a.from, a.to);
            } else {
                scrollY = easeInOut(a.timeline, l * split, l, a.to, a.from);
            }
            
            float fr = 1.0/ofGetFrameRate();
            a.timeline += fr;
            
            if (a.timeline >= a.length) {
//                ofLogVerbose("[Scroller]") << "Animate END";
                a.active = false;
                a.timeline = 0;
            }
            set();
        }
        
        bool innerDiff = lastInnerHeight != rect->conf.inner.height || lastInnerWidth != rect->conf.inner.width;
        bool outerDiff = lastOuterHeight != rect->conf.outer.height || lastOuterWidth != rect->conf.outer.width;
        
        if (innerDiff || outerDiff) {
            
            if (innerDiff) {
                lastInnerHeight = rect->conf.inner.height;
                lastInnerWidth = rect->conf.inner.width;
                string a = "innerBoxResized";
                ofNotifyEvent(event, a);
            }
            if (outerDiff) {
                lastOuterHeight = rect->conf.outer.height;
                lastOuterWidth = rect->conf.outer.width;
                string a = "outerBoxResized";
                ofNotifyEvent(event, a);
            }
            setScrollBar();
        }
        
    }

    void Scroller::draw() {
        
        if (rect->conf.inner.height < rect->conf.outer.height) return;
        
        float t = ofGetElapsedTimef();
        
        bool isInsideBar = scrollBarOuter.inside( ofGetMouseX(), ofGetMouseY() );
        if (isInsideBar != lastInsideBar) hoverBarTimestamp = t;
        lastInsideBar = isInsideBar;
        
        bool isInside = rect->conf.outer.inside( ofGetMouseX(), ofGetMouseY() );
        if (isInside != lastInside) hoverTimestamp = t;
        lastInside = isInside;
        
        bool drawOuter = (isInsideBar || (t < hoverBarTimestamp + s.barHoverSpeed) || isDragged);
        bool drawBar = (isInside || (t < hoverTimestamp + s.barHoverSpeed) || isDragged);
        
        if (drawOuter) {
            
            if (isInsideBar) hoverScale = easeInOut(t, hoverBarTimestamp, hoverBarTimestamp + s.barHoverSpeed, hoverScale, 1);
            if (!isInsideBar && !isDragged) hoverScale = easeInOut(t, hoverBarTimestamp, hoverBarTimestamp + s.barHoverSpeed, hoverScale, 0);
            
            setScrollBar();
        }
        
        if (drawBar) {
            
            if (isInside) hoverOpacity = easeInOut(t, hoverTimestamp, hoverTimestamp + s.barHoverSpeed, hoverOpacity, 255);
            if (!isInside && !isDragged) hoverOpacity = easeInOut(t, hoverTimestamp, hoverTimestamp + s.barHoverSpeed, hoverOpacity, 0);
            
        }
        
        if ((drawOuter || drawBar)&&barsInited) {

            ofEnableAlphaBlending();
            ofFill();
            ofSetColor(s.barOuterColor, hoverScale * s.barOuterColor[3]);
            ofDrawRectangle( scrollBarOuter );
            ofFill();
            ofSetColor(s.barInnerColor, hoverOpacity);
            ofDrawRectRounded( scrollBar , s.barRounded * scrollBar.width);
        }
        
        
    }
    void Scroller::scrolled( ofMouseEventArgs & e ) {
        if ((int)rect->conf.inner.height <= (int)rect->conf.outer.height) {
//            ofLogError("ofxKit::scroller") << "inside is smaller or same than outside.";
            return;
        }
        if (!rect->conf.outer.inside( ofGetMouseX(), ofGetMouseY() )) return;
        
        float t = ofGetElapsedTimef();
        float d = ( e.scrollY * -3 );
        
        bool directionUp = (e.scrollY > 0);
        bool directionDown = (e.scrollY < 0);
        
        
        bool isOpposite = (directionDown && lastDirection == -1)||(directionUp && lastDirection == 1);
        if (isOpposite) scrollState = SCROLL_PASSED;
        
        lastDirection = 0;
        if (directionUp) lastDirection = -1;
        if (directionDown) lastDirection = 1;
        
        if (!a.active) {
            
            bool isStateful = (scrollState == SCROLL_PASSED);
            
            if (isStateful) {
                scrollY = scrollY - d;
                set();
            }
            
            float outerY = rect->conf.outer.y;
            float outerH = rect->conf.outer.height;
            float innerH = rect->conf.inner.height;
            
            float bottomInner = scrollY + rect->conf.inner.height;
            float bottomOuter = rect->conf.outer.y + rect->conf.outer.height;
            
            bool isLess = bottomInner < bottomOuter;
            bool isLessThan = bottomOuter - bottomInner > 0;
            
            bool isMore = scrollY > outerY;
            bool isMoreThan = scrollY - outerY > 0;
            
            float length = ( t - timestamp ) * s.elasticSpeed;
            if (length > 0.6) length = 0.6;
            float distance = d * s.elasticDistance;
            
            if (isLess && isLessThan && isStateful) {
                
                scrollState = SCROLL_CHECK;
//                ofLogVerbose("[Scroller]") << "Animate UP" << length << a.from << a.to;
                a.direction = "UP";
                a.timeline = 0;
                a.active = true;
                a.length = length;
                a.from = rect->conf.outer.y - (innerH - outerH);
                a.to = rect->conf.outer.y - (innerH - outerH) - (distance);
            }
            
            if (isMore && isMoreThan && isStateful) {
                
                scrollState = SCROLL_CHECK;
//                ofLogVerbose("[Scroller]") << "Animate DOWN" << length << a.from << a.to;
                a.direction = "DOWN";
                a.timeline = 0;
                a.active = true;
                a.length = length;
                a.from = rect->conf.outer.y;
                a.to = rect->conf.outer.y - (distance);
            }
            
        }
        
        timestamp = t;
    }

    void Scroller::pressed( int x, int y ) {
        
        if (scrollBarOuter.inside(x, y)) {
            
            dragOrigin.set(x, y);
            dragOriginBar = scrollBar;
            isDragged = true;
        }
    }
    void Scroller::dragged( int x, int y ) {
        if (isDragged) {
            
            float start = ( dragOrigin.y - dragOriginBar.y ) + rect->conf.outer.y;
            float end = ( rect->conf.outer.y + rect->conf.outer.height ) - ( (dragOriginBar.y + dragOriginBar.height) - dragOrigin.y);
            scrollY = ofMap( y, start, end, rect->conf.outer.y, ( rect->conf.outer.height - rect->conf.inner.height ) + rect->conf.outer.y, true);
            hoverTimestamp = ofGetElapsedTimef();
            hoverBarTimestamp = hoverTimestamp;
            set();
        }
        
    }
    void Scroller::released( int x, int y ) {
        isDragged = false;
        
    }
} 
