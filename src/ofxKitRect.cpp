#include "ofxKitRect.h"
#include "ofxKitTypes.h"

namespace ofxKit {
    
    Rect *& Rect::operator[](size_t i) {
        return childr[i];
    }
    
    Rect::Rect() {
        
        /*-- root --*/
        
        ofLogNotice("ofxKit::Rect") << "setting root";
        Rect(OFXKIT_COL); // reset child
        root = true;
        global.push_back(this);
    };
    Rect::Rect(float x, float y, float w, float h) {
        
        /*-- root --*/
        
        ofLogNotice("ofxKit::Rect") << "setting root" << x << y << w << h;
        Rect(OFXKIT_COL); // reset child
        root = true;
        global.push_back(this);
        set(x,y,w,h);
        amend();
        
    };
    Rect::Rect(int t) {
        
        /*-- child --*/
        
        type = t;
        conf.ratio.height = 1;
        conf.ratio.width = 1;
        fixed = false;
        ghost = false;
        root = false;
        scroll = false;
        sideOffset = 0;
        parent = nullptr;
        ofAddListener(scrollBox.event , this, &Rect::scrollEvent);
    };
    
    int Rect::getScrollingGrids( vector<Rect *> & list ) {
        if (scroll) list.push_back(this);
        for (auto & ch : childr ) ch->getScrollingGrids( list );
        return list.size();
    }
    
    void Rect::setFixed( bool b ) {
        fixed = b;
        ( !root ) ? parent->amend() : amend();
        ofxKit::Event e("setfixed", this);
        ofNotifyEvent(getRoot()->event, e);
    }
    void Rect::setScroll( bool b ) {
        scroll = b;
        ( !root ) ? parent->amend() : amend();
        ofxKit::Event e("setscroll", this);
        ofNotifyEvent(getRoot()->event, e);
    }
    
    void Rect::scrollEvent(string & e) {
        if (e == "scrolled") amend();
    }
    void Rect::set(float x, float y, float w, float h) {
        ofRectangle r(x, y, w, h);
        set(r);
    }
    void Rect::set(ofRectangle & r) {
        
        if (r.width < conf.minimum.width) r.width = conf.minimum.width;
        if (r.height < conf.minimum.height) r.height = conf.minimum.height;
        
        conf.outer = r;
        conf.inner = ofxKit::Shrink(conf.outer, 10);
        
        if (scrollBox.scrollY == 999999) scrollBox.scrollY = conf.outer.y;
        
        scrollBox.outer = conf.outer;
        scrollBox.inner = conf.outer;
        scrollBox.inner.y = scrollBox.scrollY;
        
        
    }
    void Rect::setWidth(float w) {
        set(conf.outer.x, conf.outer.y, w, conf.outer.height);
    }
    void Rect::setHeight(float h) {
        set(conf.outer.x, conf.outer.y, conf.outer.width, h);
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
            Rect * ch = parent->childr[i];
            if (!ch->ghost) w.push_back(ch->conf.ratio.width);
        }
        return w;
    }
    vector<float> Rect::getHeights() {
        vector<float> h;
        if ( root ) return h;
        for (int i = 0; i < parent->childr.size(); i++) {
            Rect * ch = parent->childr[i];
            if (!ch->ghost) h.push_back(ch->conf.ratio.height);
        }
        return h;
    }
    
    void Rect::drawWireframes() {
        for (auto & ch : global) {
            ofNoFill();
            ofDrawRectangle( ch->conf.outer );
        }
    }
    
    void Rect::draw(bool iso ) {
        
        
        ofColor c = style.defaultColor;
        
        ofSetColor(c);
        ofNoFill();
        ofSetLineWidth(2);
        
        ofPushMatrix();
        if (iso) {
            ofTranslate(0,0,depth() * 80);
        }
        
        
        ofRectangle r = conf.outer;
        ofDrawRectangle(r);
        string s = "x";
        s += ofToString(conf.outer.x) + " y";
        s += ofToString(conf.outer.y) + " w";
        s += ofToString(conf.outer.width) + " h";
        s += ofToString(conf.outer.height) + " ";
        
        
        if (type == OFXKIT_OBJ) {
            ofDrawLine( conf.outer.getTopLeft(), conf.outer.getBottomRight() );
            ofDrawLine( conf.outer.getTopRight(), conf.outer.getBottomLeft() );
        }
        
        ofSetColor(255);
        if (childr.size() > 0) ofDrawBitmapString( ofToString( childr.size() ) , conf.outer.getCenter() );
        
        if (parent && iso) {
            ofSetColor(255,255,255,100);
            ofVec3f p = parent->conf.outer.getCenter();
            ofVec3f c = conf.outer.getCenter();
            p.z -= 80;
            ofDrawLine(c, p);
        }
        
        
        ofFill();
        ofSetColor(0);
        
        ofPopMatrix();
        
        if (scroll) scrollBox.draw();
        if (scroll) {
            ofNoFill();
            ofSetColor(255,0,0);
            ofDrawRectangle( scrollBox.inner );
        }
        
        int i = 0;
        for (auto & ch : childr) ch->draw(iso);
        for (auto & ch : ghosts) ch->draw(iso);
        
    }
    
    void Rect::scrolled( ofMouseEventArgs & e ) {
        
        for(auto & ch : global) {
            if (ch->scroll) ch->scrollBox.scrolled(e);
        }
    }
    
    void Rect::pressed( int x, int y ) {
        for(auto & ch : global) {
            if (ch->scroll) ch->scrollBox.pressed(x,y);
        }
        
    }
    void Rect::dragged( int x, int y ) {
        for(auto & ch : global) {
            if (ch->scroll) ch->scrollBox.dragged(x,y);
        }
    }
    void Rect::released( int x, int y ) {
        for(auto & ch : global) {
            if (ch->scroll) ch->scrollBox.released(x,y);
        }
    }
    
    
    
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
        string ss = "|";
        for (auto & i : location) {
            ss += ">"+ofToString(i + 1);
        }
        name = ss;
    }
    
    void Rect::amend() {
        
        
        if (!ghost) tag();
        
        Event e("amended", this);
        ofNotifyEvent(getRoot()->event, e);
        
        /*-- iterate width, height and position into pixels --*/

        ofRectangle parent_( conf.outer );
        ofRectangle ratio_;
        
        bool isCol = (type == OFXKIT_COL || type == OFXKIT_OBJ);
        bool isRow = (type == OFXKIT_ROW);
        bool isScroll = (scroll);
        
        /*-- get totals --*/
        
        for (auto & c : childr) {
            
            bool isFixed = c->fixed;
            bool isGhost = c->ghost;
            
            /*-- get total without fixed widths and heights --*/
            
            if (!isGhost) {
                
                if (isCol && isFixed) {
                    
                    ratio_.width += c->conf.ratio.width;
                    parent_.height -= c->conf.outer.height;
                    
                } else if (isRow && isFixed) {
                    
                    ratio_.height += c->conf.ratio.height;
                    parent_.width -= c->conf.outer.width;
                } else {
                    
                    ratio_.width += c->conf.ratio.width;
                    ratio_.height += c->conf.ratio.height;
                }
            }
        }
        
        
        /*-- set scrollbox fixed --*/
        
        
        if (scroll) for (auto & ch : childr) if (!ch->fixed) ch->fixed = true;
        
        
        
        ofRectangle move_( parent_ );
        
        /*-- amend childr --*/
        
        for (auto & c : childr) {
            
            c->tag();
            
            bool isFixed = (c->fixed);
            bool isGhost = c->ghost;
            
            if (!isGhost) {
                
                float ww = ( parent_.width / ratio_.width ) * c->conf.ratio.width;
                float hh = ( parent_.height / ratio_.height ) * c->conf.ratio.height;
                
                ofRectangle outer_( c->conf.outer );
                ofRectangle iter_( parent_ );
                
                if (isCol) {
                    iter_.y = move_.y;
                    iter_.height = hh;
                }
                if (isRow) {
                    iter_.x = move_.x;
                    iter_.width = ww;
                }
                if (isFixed && isCol) {
                    iter_.height = outer_.height;
                }
                if (isFixed && isRow) {
                    iter_.width = outer_.width;
                }
                
                /*-- set scroll offset --*/
                
                
                if (scroll) {
                    float s = scrollBox.getScrollOffset();
                    iter_.y -= s;
                    
                }
                
                if (sideOffset != 0) {
                    iter_.x += sideOffset;
                }
                
                
                /*-- set outer --*/
                
                c->set(iter_);
                
                move_.x += c->conf.outer.width;
                move_.y += c->conf.outer.height;
                
                c->amend();
            }
        }
        
        /*-- amend scrollbox --*/
        
        
        if (scroll) {
            float h = 0;
            for (auto & ch : childr) h += ch->conf.outer.height;
            scrollBox.inner.height = h;
        }
        
    }
    
    
    int Rect::depth(int d) {
        //        if (d == -1) d = 0;
        //        if (!parent) return d;
        //        d += 1;
        return location.size();//parent->depth(d);
    }
    
    bool Rect::hasError() {
        return errors.size() > 0;
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
    
    void Rect::load(ofJson & j) {
        
        
        clear();
        
        
        if (this == getRoot()) {
            
            float x = j["x"].get<float>();
            float y = j["y"].get<float>();
            float w = j["width"].get<float>();
            float h = j["height"].get<float>();
            set( x, y, w, h );
            
        }
        
        for (int i = 0; i < j["childr"].size(); i++) {
            add( j["childr"][i], i);
        }
        Event e("loaded", this);
        ofNotifyEvent(getRoot()->event, e);
        
    }
    
    void Rect::clear() {
        
        Rect * r = getRoot();
        Event e("clear", this);
        ofNotifyEvent(r->event, e);
        for (auto & u : collect() ) {
            remove( r->global, u);
            delete u;
        }
        childr.clear();
        global.push_back(this);
    }
    
    
    void Rect::remove() {
        Rect * r = getRoot();
        Event e("removed", this);
        ofNotifyEvent(r->event, e);
        for (auto & u : collect() ) {
            remove( r->global, u);
            delete u;
        }
        if ( !root ) remove(parent->childr, this);
        remove(r->global, this);
        delete this;
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
    
    bool Rect::move( Rect * u , int idx) {
        
        if ( isInvalid(u) ) return false;
        
        ofLogVerbose("[Rect]") << "Moving from: " << u->getLocationString();
        ofLogVerbose("[Rect]") << "Moving to  : " << getLocationString();
        
        if (u->parent) remove( u->parent->childr, u ); // remove from old parent
        u->ghost = false;
        add( u, idx ); // add here
        return true;
    }
    
    Rect & Rect::add(ofJson & j, int idx) {
        
        Rect * ch = new Rect( j["type"].get<int>() );
        //        ch->conf.ratio.height = j["height"].get<float>(); // default multiH
        //        ch->conf.ratio.width = j["width"].get<float>(); // default multiWidth
        //
        //    //        if (!j["fX"].is_null()) ch->fX = j["fX"].get<float>();
        //    //        if (!j["fY"].is_null()) ch->fY = j["fY"].get<float>();
        //    //        if (!j["fW"].is_null()) ch->fW = j["fW"].get<float>();
        //    //        if (!j["fH"].is_null()) ch->fH = j["fH"].get<float>();
        //
        //        ch->type = j["type"].get<int>();
        //        ch->parent = nullptr;
        //        add(ch, idx);
        //
        //        for (int i = 0; i < j["childr"].size(); i++) ch->add( j["childr"][i], i);
        
        return * ch;
    }
    
    /*-- Main Add Func --*/
    
    Rect & Rect::add(Rect * ch, int idx) {
        
        
        Rect * root = getRoot();
        remove(root->global, ch);
        
        ch->parent = this;
        /*-- Insert new rect --*/
        
        if (idx > childr.size() || idx < 0) idx = childr.size(); // safe IDX
        
        int d = 0;
        childr.insert(childr.begin() + idx, ch); // insert @ IDX
        
        /*--- Generate position ---*/
        
        root->global.push_back(ch);
        amend();
        
        ofLogNotice("ofxKit::Rect") << "adding child @ position:" << idx << "size:" << ch->conf.outer;
        Event e("added", ch);
        ofNotifyEvent(root->event, e);
        
        /*-- Return vector --*/
        
        return * ch;
    }
    
    /*-- Abstract Add Func --*/
    
    Rect & Rect::add(int t, int idx) {
        
        Rect * ch = new Rect(t);
        add(ch, idx);
        
        return * ch;
    }
    
    
    
    
    
    
    
    
    
    string Rect::getLocationString(string l) {
        if (  root  ) return l;
        l = " > " +  ofToString( getIndex() ) + l ;
        return parent->getLocationString( l );
    }
    
    int Rect::getIndex() {
        if ( root ) return 0;
        //        if (!ghost) return -1;
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
        bool isInCol = (parent->type == OFXKIT_COL);
        
        if (isFirst && isInCol && parent->parent) i += 1;
        return parent->getFirstAndInColCount(i);
    }
    int Rect::getLastAndInColCount(int i) {
        if ( root ) return i;
        
        bool isLast = (getIndex() == parent->childr.size() - 1);
        bool isInCol = (parent->type == OFXKIT_COL);
        
        if (isLast && isInCol && parent->parent) i += 1;
        return parent->getLastAndInColCount(i);
    }
    int Rect::getLastAndInRowCount(int i) {
        if ( root ) return i;
        
        bool isLast = (getIndex() == parent->childr.size() - 1);
        bool isInRow = (parent->type == OFXKIT_ROW);
        
        if (isLast && isInRow) i += 1;
        return parent->getLastAndInRowCount(i);
    }
    
    ofJson Rect::json() {
        
        ofJson j;
        
        if ( root ) {
            j["fX"] = conf.outer.x;
            j["fY"] = conf.outer.y;
            j["fW"] = conf.outer.width;
            j["fH"] = conf.outer.height;
        } else {
            j["type"] = type;
            j["width"] = conf.ratio.width;
            j["height"] = conf.ratio.height;
        }
        
        for (int i = 0; i < childr.size(); i++) {
            j["childr"][i] = childr[i]->json();
        }
        
        return j;
    }
    
    bool Rect::inside( Rect * u ) {
        if ( u == this ) return true;
        bool b = false;
        for (auto & el : childr) if (el->inside( u )) b = true;
        return b;
    }
    
}
