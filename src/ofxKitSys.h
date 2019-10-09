#pragma once
#include "ofMain.h"
#include <sys/stat.h>
#define OFXKIT_BYTE 0
#define OFXKIT_MB 1
#define OFXKIT_GB 2
#define OFXKIT_TB 3

namespace ofxKit {
    
    struct ByteSize {
        double bytes;
        float kb;
        float mb;
        float gb;
        float tb;
        string human;
        ByteSize( double b ) {
            
            bytes = b;
            kb = (int)((bytes/1024/1024)*1000);
            mb = (int)(bytes/1024/1024);
            gb = roundf(mb / 1000 * 1)/1;
            tb = roundf(mb / 1000 / 1000 * 10)/10;
            human = ofToString(kb) + "KB";
            if (mb != 0) human = ofToString(mb) + "MB";
            if (mb > 500) human = ofToString(gb) + "GB";
            if (gb > 1000) human = ofToString(tb) + "TB";
        }
    };
    
    class File : public ofFile {
    public:
        ByteSize * size;
        std::tm * date;
        string path;
        File(string p) {
            path = p;
            open(p);
            size = new ByteSize(getSize());
            struct stat t_stat;
            stat(p.c_str(), &t_stat);
            date = localtime(&t_stat.st_ctime);
        }
        string getDate(string format = "%a %b %d %y %H:%M:%S ") {
            std::stringstream ss;
            ss << std::put_time(date, format.c_str());
            return ss.str();
        }
    };
    
    static string HumanTime( float timeInSeconds ) {
        return ofToString( (int)timeInSeconds/60 ) + "m" + ofToString( (int)fmod(timeInSeconds, 60) ) + "s";
    }


    static string ExpandTilda(string location) {
        
        if (location.substr(0,1) == "~") {
            ofLogNotice("ofxKit") << "expanding tilda";
            location = ofSystem("eval echo " + location + "");
            location = location.substr(0, location.size() - 1);
            ofLogNotice("ofxKit") << "converting home path" << location;
        }
        
        return location;
    }
    static string ExpandFwdSlash(string path) {
        
        if (path.substr(path.size() - 1,1) != "/") {
            ofLog() << "ExpandFwdSlash" << path << path.substr(path.size() - 1,1);
            path += "/";
        }
        
        return path;
    }


    class Directory : public ofDirectory {
    public:
        vector<ofFile> files;
        Directory() {
            
        }
        Directory(string location, vector<string> exts = {}) {
            init(location, exts);
        }
        void init(string location, vector<string> exts = {}) {

            location = ExpandTilda(location);
            close();
            try {

            open(location);
//            if(!exists()){
//                create(true);
//            }
            if (isDirectory()) {
                for(auto & ext : exts) allowExt(ext);
                listDir();
                sortByDate();
                for(auto & f : getFiles() ) {
                        File file(f.path());
                        files.push_back(file);
                }
                ofLogNotice("ofxKit") << "listed" << files.size() << "files in" << location;
            } else {
                ofLogError("ofxKit") << location << "is not a directory";
            }
            } catch  (exception & e) {
                ofLogError("ofxKit") << "could not open" << location;
            }
        }
    };

    struct Path {
        string name;
        string path;
        bool success;
        Path(string fn, string fp, bool s) {
            name = fn;
            path = fp;
            success = s;
        }
    };
    class DiskSpace {
    public:
        ByteSize * capacity;
        ByteSize * available;
        ByteSize * free;
        ofDirectory dir;
        string path;
        DiskSpace( ) {

        }
        DiskSpace( string location ) {
            set (location);
        }
        bool set( string location ) {
            path = location;
            if (!exists()) {
                ofLogError("ofxKit") << "directory does not exist" << location;
                return false;
            }
            dir.open(location);
            location = ExpandTilda(location);
            std::filesystem::space_info devi = std::filesystem::space(location);
            capacity = new ByteSize(devi.capacity);
            free = new ByteSize(devi.free);
            available = new ByteSize(devi.available);
            return true;
        }
        bool exists() {
            dir.open(path);
            return dir.exists();
        }
    };
    class Sys {
    public:
        static float convertSize(double bytes, int t) {
            try {
                
                bytes = bytes /1024/1024;
                if (t == OFXKIT_MB) return (int)bytes;
                if (t == OFXKIT_GB) return roundf(bytes / 1000 * 10)/10;
                if (t == OFXKIT_TB) return roundf(bytes / 1000 / 1000 * 10)/10;
                return bytes;
                
            } catch (exception & e) {
                ofLogError("could not convert byte size");
                return 0;
            }
        }
        
        static vector<string> volumes() {
            vector<string> vols;
            string str;
#ifdef TARGET_OSX
            ofLogNotice("ofxKit") << "listing vols";
            string location = "/Volumes";
            str = ofSystem("ls " + location);
#endif
            vols = ofSplitString(str, "\n");
            return vols;
        }
        
    };
    
}
