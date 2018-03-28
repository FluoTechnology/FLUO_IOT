#include "FS.h"
#include "SPIFFS.h"
#include <Fluotube.h>

void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
void createDir(fs::FS &fs, const char * path);
void removeDir(fs::FS &fs, const char * path);
void readFile(fs::FS &fs, const char * path);
void writeFile(fs::FS &fs, const char * path, const char * message);
void appendFile(fs::FS &fs, const char * path, const char * message);
void renameFile(fs::FS &fs, const char * path1, const char * path2);
void deleteFile(fs::FS &fs, const char * path);

void setup(){
    FluoTube.setup();

    if(!SPIFFS.begin()){
        FluoTube.debugln("SPIFFS Mount Failed");
        return;
    }
    
    listDir(SPIFFS, "/", 0);
    writeFile(SPIFFS, "/hello.txt", "Hello ");
    appendFile(SPIFFS, "/hello.txt", "World!\n");
    readFile(SPIFFS, "/hello.txt");
    deleteFile(SPIFFS, "/foo.txt");
    renameFile(SPIFFS, "/hello.txt", "/foo.txt");
    readFile(SPIFFS, "/foo.txt");
    testFileIO(SPIFFS, "/test.txt");
}

void loop(){

}




void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    FluoTube.debugln("Listing directory: " + String(dirname) );

    File root = fs.open(dirname);
    if(!root){
        FluoTube.debugln("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        FluoTube.debugln("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            FluoTube.debug("  DIR : ");
            FluoTube.debugln( String(file.name()) );
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            FluoTube.debug("  FILE: ");
            FluoTube.debug( String(file.name()) );
            FluoTube.debug("  SIZE: ");
            FluoTube.debugln( String(file.size()) );
        }
        file = root.openNextFile();
    }
}

void createDir(fs::FS &fs, const char * path){
    FluoTube.debugln("Creating Dir: " + String(path) );
    if(fs.mkdir(path)){
        FluoTube.debugln("Dir created");
    } else {
        FluoTube.debugln("mkdir failed");
    }
}

void removeDir(fs::FS &fs, const char * path){
    FluoTube.debugln("Removing Dir: %s\n", path);
    if(fs.rmdir(path)){
        FluoTube.debugln("Dir removed");
    } else {
        FluoTube.debugln("rmdir failed");
    }
}

void readFile(fs::FS &fs, const char * path){
    FluoTube.debugln("Reading file: " + String(path) );

    File file = fs.open(path);
    if(!file){
        FluoTube.debugln("Failed to open file for reading");
        return;
    }

    FluoTube.debug("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    FluoTube.debugln("Writing file: " + String(path) );

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        FluoTube.debugln("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        FluoTube.debugln("File written");
    } else {
        FluoTube.debugln("Write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    FluoTube.debugln("Appending to file: " + String(path) );

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        FluoTube.debugln("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        FluoTube.debugln("Message appended");
    } else {
        FluoTube.debugln("Append failed");
    }
    file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
    FluoTube.debugln("Renaming file from " + String(path1) + " to " + String(path2));
    if (fs.rename(path1, path2)) {
        FluoTube.debugln("File renamed");
    } else {
        FluoTube.debugln("Rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * path){
    FluoTube.debugln("Deleting file: " + String(path) );
    if(fs.remove(path)){
        FluoTube.debugln("File deleted");
    } else {
        FluoTube.debugln("Delete failed");
    }
}

void testFileIO(fs::FS &fs, const char * path){
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if(file){
        len = file.size();
        size_t flen = len;
        start = millis();
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        FluoTube.debugln( String(flen) + " bytes read for " + String(end) + " ms");
        file.close();
    } else {
        FluoTube.debugln("Failed to open file for reading");
    }


    file = fs.open(path, FILE_WRITE);
    if(!file){
        FluoTube.debugln("Failed to open file for writing");
        return;
    }

    size_t i;
    start = millis();
    for(i=0; i<2048; i++){
        file.write(buf, 512);
    }
    end = millis() - start;
    FluoTube.debugln( String(2048 * 512) + " bytes written for " + end + " ms");
    file.close();
}