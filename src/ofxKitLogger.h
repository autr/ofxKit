#pragma once

#include "ofMain.h"
#include "ofxKitRect.h"

namespace ofxKit {
    
    static int GetStringLineBreaks(string & str, ofRectangle rect) {
        
        int charWidth = 8; // char width
        
        vector<int> breaks;
        int iw = 0;
        int ix = 0;
        
        for (int i = 0; i < str.size(); i++) {
            char c = str[i];
            if (c == ' ')  iw = i;
            if ( c == '\n' ) {
                ix = 0;
            } else if ( (ix + charWidth) > rect.width ) {
                ix = (i - iw) * charWidth;
                breaks.push_back( iw );
            } else {
                ix += charWidth;
            }
        }
        
        for (int i = 0; i < breaks.size(); i++) {
            int index = breaks[i];
            str.replace(index, 1, "\n");
        }
        return ofSplitString(str, "\n").size();
    }
    static ofRectangle GetStringBounds(string & str, ofRectangle rect, int padd = 0) {
        ofRectangle inner(rect);
        inner.x += padd;
        inner.y += padd;
        inner.width -= padd*2;
        inner.height -= padd*2;
        rect.height = GetStringLineBreaks( str, inner ) * 8 * 1.7 - 1;
        rect.height += padd * 2;
        rect.width += padd * 2;
        return rect;
    }
    static ofRectangle DrawStringBox(string str, ofRectangle rect, int padd = 0) {
        rect = GetStringBounds( str, rect , padd);
        ofDrawBitmapString( str, rect.x + padd, rect.y + 11 + padd );
        return rect;
    }
    

    
    class Logger : public ofConsoleLoggerChannel {
    private:
        
        ofConsoleLoggerChannel consoleLogger;
        ofFileLoggerChannel fileLogger;
        ofFbo fbo;
        int max;
        string lastConsole;
        string console;
        ofRectangle bounds;
        static ofPtr<Logger> logger;
        static Logger *logPtr;
        Logger() {
            console = "";
            max = 240;
        }
        
    public:

        virtual ~Logger() {}
        
        static ofPtr<Logger> &getLogger() {
            if(logPtr==NULL) {
                logPtr = new Logger();
                logger = ofPtr<Logger>(logPtr);
            }
            return logger;
        }
        
        static ofRectangle & getBounds() {
            return logPtr->bounds;
        }
        
        //
        
        
        
        //--------------------------------------------------
        void log(ofLogLevel level, const string & module, const string & message){
            string l = "";
            if (level == OF_LOG_NOTICE) l = "[i] ";
            if (level == OF_LOG_WARNING) l = "[!] ";
            if (level == OF_LOG_ERROR) l = "[X] ";
            l += message;
            console += l + "\n";
            
            // print to cerr for OF_LOG_ERROR and OF_LOG_FATAL_ERROR, everything else to cout
            ostream& out = level < OF_LOG_ERROR ? cout : cerr;
            out << "[" << ofGetLogLevelName(level, true)  << "] ";
            // only print the module name if it's not ""
            if(module != ""){
                out << module << ": ";
            }
            out << message << endl;
        }
        
        void log(ofLogLevel level, const string & module, const char* format, ...){
            va_list args;
            va_start(args, format);
            log(level, module, format, args);
            va_end(args);
        }
        
        void log(ofLogLevel level, const string & module, const char* format, va_list args){
            FILE* out = level < OF_LOG_ERROR ? stdout : stderr;
            fprintf(out, "[%s] ", ofGetLogLevelName(level, true).c_str());
            if(module != ""){
                fprintf(out, "%s: ", module.c_str());
            }
            vfprintf(out, format, args);
            fprintf(out, "\n");
        }

        
        void draw(Rect * rect) {
            
            if (rect->conf.inner.width <= 0) return;
            
            
            if (console != lastConsole) {

                rect->texture.align.x = 0;
                rect->texture.align.y = 1;
                int lines = GetStringLineBreaks(console, rect->conf.inner);
                while ( lines > max) {
                    console.erase(0, console.find("\n") + 1);
                    lines -= 1;
                }
                bounds = GetStringBounds(console, rect->conf.inner, 20);
                
                if (bounds.width != fbo.getWidth() || bounds.height != fbo.getHeight() ) {
//                    ofLogNotice("ofxKit") << "resetting console logger texture";
                    fbo.allocate((int)bounds.width + 20, (int)bounds.height + 20, GL_RGBA, 1);
                }
                fbo.begin();
                ofClear(255,0);
                ofSetColor(255);
                ofDrawBitmapString(console, 10,  10 );
                fbo.end();
                rect->texture.set( &fbo.getTexture() );
                lastConsole = console;
            }
        
            ofFill();
            ofSetColor(255);
//            fbo.draw((int)rect->conf.inner.x, (int)rect->conf.inner.y - ((int)bounds.getBottom() - (int)rect->conf.inner.getBottom()));
        }
        
    };

}
