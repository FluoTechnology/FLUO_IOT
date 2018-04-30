#include <Fluotube.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"


void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
void createDir(fs::FS &fs, const char * path);
void removeDir(fs::FS &fs, const char * path);
void readFile(fs::FS &fs, const char * path);
void writeFile(fs::FS &fs, const char * path, const char * message);
void appendFile(fs::FS &fs, const char * path, const char * message);
void renameFile(fs::FS &fs, const char * path1, const char * path2);
void deleteFile(fs::FS &fs, const char * path);

SPIClass SPISD(VSPI);

void setup(){
   
    FluoTube.setup();

    if(! SD.begin(SD_CS, SPISD, 4000000, "/sd") ){
        FluoTube.debugln("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        FluoTube.debugln("No SD card attached");
        return;
    }

    FluoTube.debug("SD Card Type: ");
    if(cardType == CARD_MMC){
        FluoTube.debugln("MMC");
    } else if(cardType == CARD_SD){
        FluoTube.debugln("SDSC");
    } else if(cardType == CARD_SDHC){
        FluoTube.debugln("SDHC");
    } else {
        FluoTube.debugln("UNKNOWN");
    }

    uint32_t cardSize = SD.cardSize() / (1024 * 1024);
    FluoTube.debugln("SD Card Size: " + String(cardSize) + " MB");

    listDir(SD, "/", 0);
    createDir(SD, "/mydir");
    listDir(SD, "/", 0);
    removeDir(SD, "/mydir");
    listDir(SD, "/", 2);
    writeFile(SD, "/hello.txt", "Hello ");
    appendFile(SD, "/hello.txt", "World!\n");
    readFile(SD, "/hello.txt");
    deleteFile(SD, "/foo.txt");
    renameFile(SD, "/hello.txt", "/foo.txt");
    readFile(SD, "/foo.txt");
    testFileIO(SD, "/test.txt");

    uint32_t totalBytes = SD.totalBytes() / (1024 * 1024);
    uint32_t usedBytes = SD.usedBytes() / (1024 * 1024);
    
    FluoTube.debugln("Total space: " + String(totalBytes) + " MB");
    FluoTube.debugln("Used space: " + String(usedBytes) + " MB");
}

void loop(){

}

// Filesystem Function

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
    FluoTube.debugln("Removing Dir: " + String(path));
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