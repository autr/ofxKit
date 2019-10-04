#include "ofxKitRect.h"
#include "ofxKitTypes.h"
#include "ofxKitInteract.h"

namespace ofxKit {

    Rect::Rect() {
        init();
    }

    Rect::~Rect() {

    }
    Rect::Rect( RectConf conf_ ) {
        init();
        conf = conf_;
    }
    Rect::Rect(int type) {
        init();
        conf.type = type;

    }
    Rect::Rect(float x, float y, float w, float h, int type ) {
        
        init();
        conf.type = type;
        set(x,y,w,h);
        
    };



    void Rect::init() {

        dpi = ((ofAppGLFWWindow*)ofGetWindowPtr())->getPixelScreenCoordScale();
        root = true;
        parent = nullptr;
        debug = false;
        inited = false;
        added = false;
        id = "0";
        conf.type = OFXKIT_COL;
        set(0,0,0,0);
        offset.set(0,0);
        origin.set(0.5,0.5);
        transform.set(0,0,1,1);

    }


    Rect & Rect::add( Rect & rect, int idx ) {

        if (conf.scroll && childr.size() >= 1) {
            ofLogError("ofxKit::Rect") << "scroll container already has an inner rect, add unset";
            return rect;
        }

        rect.root = false;
        rect.debug = debug;
        amendPtrs(&rect, idx);
        ofLogNotice("ofxKit") << "adding" << rect.id;
        rect.added = true;
        Event e("added", &rect);
        ofNotifyEvent(getRoot()->event, e);
        return rect;
    }


    Rect & Rect::add( RectConf conf, int idx) {

        Rect * rect = new Rect(conf);
        add(*rect);
        return *rect;

    }

    Rect & Rect::add( int type, int idx) {

        Rect * rect = new Rect(type);
        add(*rect);
        return *rect;

    }


    void Rect::set(float x, float y, float w, float h) {

        ofRectangle r(x, y, w, h);
        set(r);
    }
    void Rect::set(ofRectangle r) {
        
//        if (r.width < conf.minimum.width) r.width = conf.minimum.width;
//        if (r.height < conf.minimum.height) r.height = conf.minimum.height;
        
        conf.outer = r;
        conf.inner = ofxKit::Shrink(conf.outer, conf.margins);
    
        Event e("size", this);
        ofNotifyEvent(event, e);
        
    }
    void Rect::setWidth(float w) {
        set(conf.outer.x, conf.outer.y, w, conf.outer.height);
    }
    void Rect::setHeight(float h) {
        set(conf.outer.x, conf.outer.y, conf.outer.width, h);
    }
    
    void Rect::setFixed( bool b ) {
        conf.fixed = b;
//        ( !root ) ? parent->amend() : amend();
        ofxKit::Event e("setfixed", this);
        ofNotifyEvent(getRoot()->event, e);
    }
    void Rect::setScroll( bool b ) {

        if (childr.size() > 1) {
            ofLogError("ofxKit::Rect") << "cannot add more than one child to a scroll wrapper, scrolling unset";
            return;
        }
        conf.scroll = b;
        if (b && scroller == nullptr) {
            ofLogNotice("ofxKit::Rect") << "creating scroller for" << id;
            scroller = new ofxKit::Scroller();
            ofAddListener(scroller->event , this, &Rect::scrollEvent);
        }
        if (!b && scroller != nullptr) {
            ofRemoveListener(scroller->event , this, &Rect::scrollEvent);
            delete scroller;
            scroller = nullptr;
        }
        ofxKit::Event e("setscroll", this);
        ofNotifyEvent(getRoot()->event, e);
    }

//    Rect & Rect::( Rect * rect ) {
//        rect->root = false;
//    }


    
    
    bool Rect::move( Rect * u , int idx) {
        
        if ( isInvalid(u) ) return false;
        
        ofLogVerbose("[Rect]") << "Moving from: " << u->getLocationString();
        ofLogVerbose("[Rect]") << "Moving to  : " << getLocationString();
        
        if (u->parent) remove( u->parent->childr, u ); // remove from old parent
        u->conf.ghost = false;
//        amendPtrs( u, idx ); //  here
        return true;
    }
    
    void Rect::amendPtrs( Rect * rect, int idx) {
        
        Rect * r = getRoot();
        
        rect->parent = this;
        /*-- Insert new rect --*/
        
        if (idx > childr.size() || idx < 0) idx = childr.size(); // safe IDX
        
        int d = 0;
        childr.insert(childr.begin() + idx, rect); // insert @ IDX
        
        /*--- Generate position ---*/
        Event e("amended", rect);
        ofNotifyEvent(r->event, e);

    }
    
    string Rect::whatAreYou() {
        string str = "UNKNOWN";
        if (conf.type == OFXKIT_COL) str = "OFXKIT_COL";
        if (conf.type == OFXKIT_ROW) str = "OFXKIT_ROW";
        if (conf.type == OFXKIT_OBJ) str = "OFXKIT_OBJ";
        return str;
    }
    string Rect::whoAreYou() {
        return conf.name;
    }
    string Rect::whereAreYou() {
        return id;
    }
    
    int Rect::getScrollingGrids( vector<Rect *> & list ) {
        if (conf.scroll) list.push_back(this);
        for (auto & rect : childr ) rect->getScrollingGrids( list );
        return list.size();
    }
    
    void Rect::scrollEvent(string & e) {
        if (e == "scrolled") {
            if (childr.size() > 0) childr[0]->offset.y = -scroller->scrollY;
//            amend();
        }
    }

    
    
    Rect * Rect::get(vector<int> idx) {
        Rect * u = getRoot();
        for (auto & i : idx) {
            int ii = i;
            if (ii < 0) ii = 0;
            if (ii >= u->childr.size()) ii = u->childr.size() - 1;
            if (u->childr.size() > 0) u = u->childr[ii];
        }
        return u;
    }
    
    
    
    vector<float> Rect::getWidths() {
        vector<float> w;
        if ( root ) return w;
        for (int i = 0; i < parent->childr.size(); i++) {
            Rect * rect = parent->childr[i];
            if (!rect->conf.ghost) w.push_back(rect->conf.ratio.width);
        }
        return w;
    }
    vector<float> Rect::getHeights() {
        vector<float> h;
        if ( root ) return h;
        for (int i = 0; i < parent->childr.size(); i++) {
            Rect * rect = parent->childr[i];
            if (!rect->conf.ghost) h.push_back(rect->conf.ratio.height);
        }
        return h;
    }

    void Rect::drawScrollers() {

        if (conf.scroll) scroller->draw();
        for (auto & rect : childr) rect->drawScrollers();
    }



    vector<Rect *> Rect::collect() {
        vector<Rect *> a;
        a.insert( a.end(), childr.begin(), childr.end() );
        for (auto & u : childr) {
            vector<Rect *> b = u->collect();
            a.insert( a.end(), b.begin(), b.end() );
        }
        return a;
    }

    void Rect::drawWireframes(bool drawOuter, bool drawInner, bool drawObj) {
        
        ofPushMatrix();
        float x = ofMap( origin.x, 0, 1, conf.outer.x, conf.outer.getRight());
        float y = ofMap( origin.y, 0, 1, conf.outer.y, conf.outer.getBottom());
        ofTranslate( x,y);
        ofScale( transform.width, transform.height, 1 );
        ofTranslate( -x , -y);
        ofVec2f o;
        getOffsetXY(o);
        ofTranslate(o.x, o.y);

        ofNoFill();
        if (drawOuter) ofDrawRectangle( conf.outer );
        if (drawInner) ofDrawRectangle( conf.inner );
        if (drawObj && conf.type == OFXKIT_OBJ) {
            ofDrawLine( conf.inner.getTopLeft(), conf.inner.getBottomRight() );
            ofDrawLine( conf.inner.getTopRight(), conf.inner.getBottomLeft() );
        }
        for (auto & rect : childr) rect->drawWireframes(drawOuter, drawInner, drawObj);
        ofPopMatrix();
    }

    void Rect::draw() {
        for (auto & rect : childr) rect->draw();
    }
    
    void Rect::drawInfo( ) {
        
        
        ofColor c = style.defaultColor;
        
        ofSetColor(c);
        ofNoFill();
        ofSetLineWidth(2);
        
        ofPushMatrix();
//        if (iso) {
//            ofTranslate(0,0,depth() * 80);
//        }
        
        
        ofRectangle r = conf.outer;
        ofDrawRectangle(r);
        string s = "x";
        s += ofToString(conf.outer.x) + " y";
        s += ofToString(conf.outer.y) + " w";
        s += ofToString(conf.outer.width) + " h";
        s += ofToString(conf.outer.height) + " ";
        
        
        if (conf.type == OFXKIT_OBJ) {
            ofDrawLine( conf.outer.getTopLeft(), conf.outer.getBottomRight() );
            ofDrawLine( conf.outer.getTopRight(), conf.outer.getBottomLeft() );
        }
        
        ofSetColor(255);
        if (childr.size() > 0) ofDrawBitmapString( ofToString( childr.size() ) , conf.outer.getCenter() );
        
//        if (parent && iso) {
//            ofSetColor(255,255,255,100);
//            ofVec3f p = parent->conf.outer.getCenter();
//            ofVec3f c = conf.outer.getCenter();
//            p.z -= 80;
//            ofDrawLine(c, p);
//        }
        
        
        ofFill();
        ofSetColor(0);
        
        ofPopMatrix();
        
        if (conf.scroll) scroller->draw();
        
        int i = 0;
        for (auto & rect : childr) rect->draw();
        for (auto & rect : ghosts) rect->draw();
        
    }
    
    void Rect::drawIsomorphic() {
        
    }
    ofVec2f & Rect::getOffsetXY( ofVec2f & o ) {
        
        o.x += offset.x;
        o.y += offset.y;
        if (root) return o;
        return parent->getOffsetXY(o);
    }
    void Rect::drawTextures() {
        
        if (texture.active) {
            
            /*-- set ofxKit::Rect to texture size --*/

            ofRectangle parent_(  parent->conf.outer );
            ofRectangle inner_( conf.inner );
            ofRectangle outer_( conf.outer );
            ofRectangle texture_(parent_);
            texture_.width = texture.ptr->getWidth();
            texture_.height = texture.ptr->getHeight();
            
            bool isWider =  texture_.width > (int)outer_.width;
            bool isTaller = texture_.height > (int)outer_.height;
            
            if (isWider || isTaller) {
                conf.fixed = true;
                if (isWider) setWidth((int)texture_.width);
                if (isTaller) setHeight((int)texture_.height);
                parent->amend();
            }
            
            /*-- crop visible area --*/
            
            ofRectangle crop_(  parent_.x - inner_.x, parent_.y - inner_.y, parent_.width, parent_.height );
            
            /*-- align from ofxKit::Texture 0-1 align variable --*/
            
            ofRectangle draw_(parent_);
            crop_.x += ofMap( texture.align.x , 0, 1, 0, -(parent_.width - texture_.width) );
            crop_.y += ofMap( texture.align.y , 0, 1, 0, -(parent_.height - texture_.height) );
            
            /*-- draw cropped --*/
            
            ConvertInt(draw_);
            ConvertInt(crop_);
            
            texture.ptr->drawSubsection(draw_, crop_);
        }
        
//        if (texture != nullptr) {
//            try {
//                if ((*texture) != nullptr) {
//                    try {
//
//                    } catch (const std::exception& e) {
//                        ofLogError("ofxKit") << "invalid texture";
//                    }
//                }
//            } catch (const std::exception& e) {
//                ofLogError("ofxKit") << "invalid texture";
//            }
//        }
        for (auto & c : childr) c->drawTextures();
    }
    
    void Rect::scrolled( ofMouseEventArgs & e ) {
        if (conf.scroll && scroller != nullptr) {
            scroller->scrolled(e);
            Event e("scrolled", this);
            ofNotifyEvent(event, e);
        }
    }
    
    void Rect::pressed( int x, int y, int id ) {

        if (conf.scroll) scroller->pressed(x,y);
        Event e("pressed", this);
        ofNotifyEvent(event, e);
    }
    void Rect::dragged( int x, int y, int id ) {

        if (conf.scroll) scroller->dragged(x,y);
        Event e("dragged", this);
        ofNotifyEvent(event, e);
    }
    void Rect::released( int x, int y, int id ) {
        if (conf.scroll) scroller->released(x,y);
        Event e("released", this);
        ofNotifyEvent(event, e);
    }

    void Rect::moved( int x, int y, int id) {

    };
    
    
    vector<int> & Rect::position(vector<int> & pos) {
        
        if (parent == this) {
            ofLogError("position(): Parent is also this");
            return pos;
        }
        
        if ( root ) {
            //ofLogError("position(): No Parent");
            return pos;
        }
        int i = getIndex();
        pos.insert(pos.begin(), i);
        return parent->position(pos);
    }
    
    void Rect::tag() {
        
        /*-- amend naming and hash to position --*/
        
        vector<int> pos;
        location = position(pos);
        string ss = "0";
        for (auto & i : location) {
            ss += "-"+ofToString(i);
        }
        id = ss;
    }
    
    void Rect::setRatios( vector<float> ratios ) {
        if (ratios.size() == 0) return;
        int i = 0;
        bool isCol = (conf.type == OFXKIT_COL || conf.type == OFXKIT_OBJ);
        bool isRow = (conf.type == OFXKIT_ROW);
        for (auto & rect : childr) {
            if (i < ratios.size()) {
                if (isCol) rect->conf.ratio.height = ratios[i];
                if (isRow) rect->conf.ratio.width = ratios[i];
            } else {
                if (isCol) rect->conf.ratio.height = ratios.back();
                if (isRow) rect->conf.ratio.width = ratios.back();
            }
            i += 1;
        }
    }
    
    void Rect::amend(float inSeconds) {
        
        if (inSeconds > 0) {
            
            if (!needsAmend) {
                amendDelay = inSeconds;
                amendTimestamp = ofGetElapsedTimef();
                needsAmend = true;
            }
            return;
        }
        
        if (!conf.ghost) tag();
        
        Event e("amended", this);
        ofNotifyEvent(getRoot()->event, e);
        
        /*-- iterate width, height and position into pixels --*/
        
        ofRectangle wrap_( conf.inner );
        ofRectangle inner_( conf.inner );
        ofRectangle ratio_;
        
        bool isCol = (conf.type == OFXKIT_COL || conf.type == OFXKIT_OBJ);
        bool isRow = (conf.type == OFXKIT_ROW);
        bool isScroll = (conf.scroll);
        
        /*-- get totals --*/
        
        bool hadFixed;
        
        for (auto & c : childr) {
            
            bool isFixed = c->conf.fixed;
            bool isGhost = c->conf.ghost;
            if (isFixed) hadFixed = true;
            
            /*-- get total without fixed widths and heights --*/
            
            if (!isGhost) {
                
                if (isCol && isFixed) {
                    ratio_.width += c->conf.ratio.width;
                    wrap_.height -= c->conf.outer.height;
                    
                } else if (isRow && isFixed) {
                    
                    ratio_.height += c->conf.ratio.height;
                    wrap_.width -= c->conf.outer.width;
                } else {
                    
                    ratio_.width += c->conf.ratio.width;
                    ratio_.height += c->conf.ratio.height;
                }
            }
        }
        
//        if (hadFixed) {
//            ofLog() << "B " << id << conf.inner.height << wrap_.height << ratio_.height;
//        }
        
        
//        if (id == "0-0") ofLogNotice("ofxKit") << "A-1. totals" << id << ratio_.width << ratio_.height;
//        if (id == "0-0-0") ofLogNotice("ofxKit") << "B-1. totals" << id << ratio_.width << ratio_.height;
        
        
        ofRectangle move_( wrap_ );
        
        /*-- amend childr --*/
        
        for (auto & c : childr) {
            
            c->tag();
            
            bool isFixed = (c->conf.fixed);
            bool isGhost = c->conf.ghost;
            
            if (!isGhost) {
                
                float ww = ( wrap_.width / ratio_.width ) * c->conf.ratio.width;
                float hh = ( wrap_.height / ratio_.height ) * c->conf.ratio.height;
                
                ofRectangle outer_( c->conf.outer );
                ofRectangle iter_( wrap_ );
                
//                if (id == "0-0") ofLogNotice("ofxKit") << "A-2. proportion" << id << ww << hh;
//                if (id == "0-0-0") ofLogNotice("ofxKit") << "B-2. proportion" << id << ww << hh;
                
                if (isCol) {
                    iter_.y = move_.y;
                    iter_.height = hh;
                    iter_.width = inner_.width;
                }
                if (isRow) {
//                    ofLog() << "is row";
                    iter_.x = move_.x;
                    iter_.width = ww;
                    iter_.height = inner_.height;
                }
                if (isFixed && isCol) {
//                    ofLog() << "fixed";
                    iter_.height = outer_.height;
                }
                if (isFixed && isRow) {
//                    ofLog() << "fixed";
                    iter_.width = outer_.width;
                }
                
                
                
                /*-- set outer --*/
                
//                if (id == "0-0") ofLogNotice("ofxKit") << "A-3. final" << id << iter_.width << iter_.height;
//                ofLogNotice("ofxKit") << "setting" << id << iter_.width << iter_.height;
                c->set(iter_);
                
                
                move_.x += c->conf.outer.width;
                move_.y += c->conf.outer.height;
                
                
                
                c->amend();
            }
        }

        if (conf.scroll) {
            scroller->outer = conf.inner;
            if (childr.size() > 0) {
                scroller->inner = childr[0]->conf.outer;
            }
        }
        
    }
    
    
    int Rect::depth(int d) {
        //        if (d == -1) d = 0;
        //        if (!parent) return d;
        //        d += 1;
        return location.size();//parent->depth(d);
    }
    
    
    Rect * Rect::getRoot() {
        if ( root ) {
            return this;
        }
        return parent->getRoot();
    }
    
    void Rect::remove(vector<Rect *> & v, Rect * c) {
        auto idx = std::find(v.begin(), v.end(), c);
        if (idx != v.end()) v.erase(idx);
    }
    
    
    void Rect::clear() {
        
        Rect * r = getRoot();
        Event e("clear", this);
        ofNotifyEvent(r->event, e);
        for (auto & u : collect() ) {
            // remove( r->global, u);
            delete u;
        }
        childr.clear();
        // global.push_back(this);
    }
    
    
    void Rect::remove() {
        Rect * r = getRoot();
        Event e("removed", this);
        ofNotifyEvent(r->event, e);
        for (auto & u : collect() ) {
            // remove( r->global, u);
            delete u;
        }
        if ( !root ) remove(parent->childr, this);
        // remove(r->global, this);
        delete this;
    }

    
    bool Rect::isInvalid( Rect * g ) {
        
        if ( g == this ) return true;
        
        bool invalid = true;
        
        for ( int i = 0; i < location.size(); i++ ) {
            int p = location[i]; // to
            if (i < g->location.size()) {
                int pp = g->location[i]; // from
                
                //                ofLogVerbose("[Rect]") << "Checking if invalid:" << p << pp;
                
                if (p != pp) invalid = false;
            }
        }
        //        if (invalid) ofLogVerbose("[Rect]") << "Invalid move!";
        return invalid;
    }
    
    void Rect::update() {

        if (conf.scroll && scroller != nullptr) scroller->update();
        for (auto & rect : childr) rect->update();

        if (!inited) {
            amend();
            inited = true;
            ofxKit::Interact::get()->add(this);
        }
        if (needsAmend && ofGetElapsedTimef() > amendTimestamp + amendDelay) {
            amend();
            needsAmend = false;
        }
    }
    
    
    
    string Rect::getLocationString(string l) {
        if (  root  ) return l;
        l = " > " +  ofToString( getIndex() ) + l ;
        return parent->getLocationString( l );
    }
    
    int Rect::getIndex() {
        if ( root ) return 0;

//        if (debug) ofLogNotice("ofxKit::Rect") << "getting index for" << id;
        Rect * t = this;
        auto it = find(parent->childr.begin(), parent->childr.end(), t);
        if (it != parent->childr.end()) {
            return distance(parent->childr.begin(), it);
            
        } else {
            
            ofLogError("Could not find index");
            return 0;
        }
    }
    
    
    int Rect::getFirstAndInColCount(int i) {
        if ( root ) return i;
        
        bool isFirst = (getIndex() == 0);
        bool isInCol = (parent->conf.type == OFXKIT_COL);
        
        if (isFirst && isInCol && parent->parent) i += 1;
        return parent->getFirstAndInColCount(i);
    }
    int Rect::getLastAndInColCount(int i) {
        if ( root ) return i;
        
        bool isLast = (getIndex() == parent->childr.size() - 1);
        bool isInCol = (parent->conf.type == OFXKIT_COL);
        
        if (isLast && isInCol && parent->parent) i += 1;
        return parent->getLastAndInColCount(i);
    }
    int Rect::getLastAndInRowCount(int i) {
        if ( root ) return i;
        
        bool isLast = (getIndex() == parent->childr.size() - 1);
        bool isInRow = (parent->conf.type == OFXKIT_ROW);
        
        if (isLast && isInRow) i += 1;
        return parent->getLastAndInRowCount(i);
    }
    
    
    bool Rect::inside( Rect * u ) {
        if ( u == this ) return true;
        bool b = false;
        for (auto & el : childr) if (el->inside( u )) b = true;
        return b;
    }
    
}
