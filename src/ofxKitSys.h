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
        File(string p) {
            open(p);
            size = new ByteSize(getSize());
            ofLog() << getFileName() << size->human;

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
    
    class Directory : public ofDirectory {
    public:
        vector<ofFile> files;
        Directory(string location, vector<string> exts = {}) {
            init(location, exts);
        }
        void init(string location, vector<string> exts = {}) {
            
            if (location.substr(0,1) == "~") {
                location = ofSystem("eval echo " + location + "");
                location = location.substr(0, location.size() - 1);
            }
            
            open(location);
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
    struct DiskSpace {
        ByteSize * capacity;
        ByteSize * available;
        ByteSize * free;
        DiskSpace( string location ) {
            
            std::filesystem::space_info devi = std::filesystem::space(location);
            capacity = new ByteSize(devi.capacity);
            free = new ByteSize(devi.free);
            available = new ByteSize(devi.available);
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
        static Path open(string text = "Open", bool folders = false, string root = "/") {
            
            ofFileDialogResult p = ofSystemLoadDialog(text, folders, root);
            
            if( p.bSuccess ) {
                return Path(p.fileName, p.filePath, p.bSuccess);
            } else {
                return Path("", "", p.bSuccess);
            }
        }
        static vector<string> list(string path) {
            
        }
        
        static vector<string> volumes() {
            vector<string> vols;
#ifdef TARGET_OSX
            string location = "/Volumes";
            string str = ofSystem("ls " + location);
#endif
            vols = ofSplitString(str, "\n");
            return vols;
        }
        
    };
    
}
