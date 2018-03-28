#ifndef FluoSD_h
#define FluoSD_h

#include <FS.h>
#include <SD.h>

// var publish
extern uint64_t cardSize;
extern uint8_t cardType; 

extern void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
extern void createDir(fs::FS &fs, const char * path);
extern void removeDir(fs::FS &fs, const char * path);
extern void readFile(fs::FS &fs, const char * path);
extern void writeFile(fs::FS &fs, const char * path, const char * message);

extern void appendFile(fs::FS &fs, const char * path, const char * message);
extern void renameFile(fs::FS &fs, const char * path1, const char * path2);
extern void deleteFile(fs::FS &fs, const char * path);

extern File WrapOpen(fs::FS &fs, const char *path);

void SDCardInit();

#endif