#include "ofxKitRect.h"
#include "ofxKitTypes.h"
#include "ofxKitInteract.h"

namespace ofxKit {
    
    Rect::Rect(int type, bool root_) {
        init();
        conf.type = type;;
        root = root_;
        set( conf.outer );
        scroller.init(this);
        if (root) global.push_back(this);
    }
    Rect::Rect( RectConf conf_, bool root_ ) {
        
        /*-- root --*/
        
        init();
        conf = conf_;
        root = root_;
        set( conf.outer );
        scroller.init(this);
        if (root) global.push_back(this);
//        ofLogNotice("ofxKit") << "creating rect" << conf_.type << conf.type;
    };
    
    void Rect::init() {
        
        scroller.init(this);
        parent = nullptr;
        offset.set(0,0);
        origin.set(0.5,0.5);
        transform.set(0,0,1,1);
        ofAddListener(scroller.event , this, &Rect::scrollEvent);
    }
    
    
    Rect & Rect::add( RectConf conf_, int idx) {
        
        Rect * ch = new Rect(conf_);
        ch->root = false;
        amendPtrs(ch, idx);
        amend();
//        ofLogNotice("ofxKit") << "adding rect" << conf_.type;
        return * ch;
        
    }
    
    Rect & Rect::add( int t, int idx) {
        
        Rect * ch = new Rect( t );
        ch->root = false;
        ch->conf.type = t;
//        ofLogNotice("ofxKit") << "adding rect" << t << ch->conf.type;
        amendPtrs(ch, idx);
        amend();
        return * ch;
        
    }
    
    
    
    bool Rect::move( Rect * u , int idx) {
        
        if ( isInvalid(u) ) return false;
        
        ofLogVerbose("[Rect]") << "Moving from: " << u->getLocationString();
        ofLogVerbose("[Rect]") << "Moving to  : " << getLocationString();
        
        if (u->parent) remove( u->parent->childr, u ); // remove from old parent
        u->conf.ghost = false;
//        amendPtrs( u, idx ); // add here
        return true;
    }
    
    void Rect::amendPtrs( Rect * ch, int idx) {
        
        Rect * r = getRoot();
        remove(r->global, ch);
        
        ch->parent = this;
        /*-- Insert new rect --*/
        
        if (idx > childr.size() || idx < 0) idx = childr.size(); // safe IDX
        
        int d = 0;
        childr.insert(childr.begin() + idx, ch); // insert @ IDX
        
        /*--- Generate position ---*/
        
        ofxKit::Interact::get()->add(ch);
        r->global.push_back(ch);
        amend();
        
        //        ofLogNotice("ofxKit::Rect") << "adding child @ position:" << idx << "size:" << ch->conf.outer;
        Event e("added", ch);
        ofLogNotice("ofxKit") << "added" << r->id << ch->conf.type << ch->id << r->global.size() << r->global.back()->conf.type;
        ofNotifyEvent(r->event, e);
        
        /*-- Return vector --*/
        
//        return ch;
    }
    
    
    
    int Rect::getScrollingGrids( vector<Rect *> & list ) {
        if (conf.scroll) list.push_back(this);
        for (auto & ch : childr ) ch->getScrollingGrids( list );
        return list.size();
    }
    
    void Rect::setFixed( bool b ) {
        conf.fixed = b;
        ( !root ) ? parent->amend() : amend();
        ofxKit::Event e("setfixed", this);
        ofNotifyEvent(getRoot()->event, e);
    }
    void Rect::setScroll( bool b ) {
        conf.scroll = b;
        ( !root ) ? parent->amend() : amend();
        ofxKit::Event e("setscroll", this);
        ofNotifyEvent(getRoot()->event, e);
    }
    
    void Rect::scrollEvent(string & e) {
        if (e == "scrolled") amend();
    }
    
    void Rect::detectOverflow() {
        
        
        ofRectangle inner_(conf.inner);
        for ( auto & ch : childr) {
            
            ofRectangle child_(ch->conf.outer);
            bool overW = child_.getRight() > inner_.getRight();
            bool overH = child_.getBottom() > inner_.getBottom();
            
            if (overW) conf.inner.width =  child_.getRight() - inner_.getX() ;
            if (overH) conf.inner.height =  child_.getBottom() - inner_.getY() ;
            if (overW||overH) {
                ofLog() << "detected overflow" << id << conf.inner.height;
                scroll = true;
            }
        }
        
    }
    
    
    void Rect::set(float x, float y, float w, float h, bool reload) {
        ofRectangle r(x, y, w, h);
        set(r, reload);
    }
    void Rect::set(ofRectangle & r, bool reload) {
        
        if (r.width < conf.minimum.width) r.width = conf.minimum.width;
        if (r.height < conf.minimum.height) r.height = conf.minimum.height;
        
        conf.outer = r;
        
        conf.inner = ofxKit::Shrink(conf.outer, conf.margins);
    
        if (!root) detectOverflow();
        if (reload) amend();
        
    }
    void Rect::setWidth(float w, bool reload) {
        set(conf.outer.x, conf.outer.y, w, conf.outer.height, reload);
    }
    void Rect::setHeight(float h, bool reload) {
        set(conf.outer.x, conf.outer.y, conf.outer.width, h, reload);
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
            if (!ch->conf.ghost) w.push_back(ch->conf.ratio.width);
        }
        return w;
    }
    vector<float> Rect::getHeights() {
        vector<float> h;
        if ( root ) return h;
        for (int i = 0; i < parent->childr.size(); i++) {
            Rect * ch = parent->childr[i];
            if (!ch->conf.ghost) h.push_back(ch->conf.ratio.height);
        }
        return h;
    }
    
    void Rect::drawWireframes() {
        
        ofPushMatrix();
        float x = ofMap( origin.x, 0, 1, conf.outer.x, conf.outer.getRight());
        float y = ofMap( origin.y, 0, 1, conf.outer.y, conf.outer.getBottom());
        ofTranslate( x,y);
        ofScale( transform.width, transform.height, 1 );
        ofTranslate( -x , -y);
        ofVec2f o;
        getOffsetXY(o);
        ofTranslate(o.x, o.y);
        
        int i = 0;
        for (auto & ch : global) {
            ofNoFill();
//            ofLog() << ch->conf.type << ch->id << i;
//            ofLog() << ch->id << ch->conf.inner.x << ch->conf.inner.y;
            ofDrawRectangle( ch->conf.outer );
            ofDrawRectangle( ch->conf.inner );
            if (ch->conf.type == OFXKIT_OBJ) {
                ofDrawLine( ch->conf.inner.getTopLeft(), ch->conf.inner.getBottomRight() );
                ofDrawLine( ch->conf.inner.getTopRight(), ch->conf.inner.getBottomLeft() );
            }
            i += 1;
        }
        ofPopMatrix();
    }
    
    void Rect::draw( ) {
        
        
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
        
        if (conf.scroll) scroller.draw();
        
        int i = 0;
        for (auto & ch : childr) ch->draw();
        for (auto & ch : ghosts) ch->draw();
        
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
//                ofLog() << "texture" << id;
            ofRectangle r(0,0,texture.ptr->getWidth(), texture.ptr->getHeight());
            bool isWider =  r.width > (int)conf.outer.width;
            bool isTaller = r.height > (int)conf.outer.height;
            if (isWider || isTaller) {
//                r = Shrink( r , conf.margins );
                if (isWider) setWidth((int)r.width);
                if (isTaller) setHeight((int)r.height);
                parent->amend();
            }
            ofRectangle parent_outer_(parent->conf.outer);
            ofRectangle draw_(  parent_outer_ );
            ofRectangle crop_(  parent_outer_.x - conf.inner.x, parent_outer_.y - conf.inner.y, parent_outer_.width, parent_outer_.height );
//            draw_.x =
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
        if (scroll) {
            scroller.scrolled(e);
        }
    }
    
    void Rect::pressed( int x, int y ) {
        
    }
    void Rect::dragged( int x, int y ) {
    }
    void Rect::released( int x, int y ) {
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
        id = ss;
    }
    
    void Rect::amend() {
        
        
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
        
        for (auto & c : childr) {
            
            bool isFixed = c->conf.fixed;
            bool isGhost = c->conf.ghost;
            
            /*-- get total without fixed widths and heights --*/
            
            if (!isGhost) {
                
                if (isCol && isFixed) {
                    
                    ratio_.width += c->conf.ratio.width;
                    wrap_.height -= inner_.height;
                    
                } else if (isRow && isFixed) {
                    
                    ratio_.height += c->conf.ratio.height;
                    wrap_.width -= inner_.width;
                } else {
                    
                    ratio_.width += c->conf.ratio.width;
                    ratio_.height += c->conf.ratio.height;
                }
            }
        }
        
        
        
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
                
                
                
                /*-- set outer --*/
                
                c->set(iter_, false);
                
                
                move_.x += c->conf.outer.width;
                move_.y += c->conf.outer.height;
                
                
                
                c->amend();
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
    
    void Rect::update() {
        
        if (scroll) scroller.update();
        for (auto & ch : childr) ch->update();
    }
    void Rect::drawScrollers() {
        
        if (scroll) scroller.draw();
        for (auto & ch : childr) ch->drawScrollers();
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
