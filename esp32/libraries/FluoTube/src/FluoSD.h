#ifndef FluoSD_h
#define FluoSD_h

#include <FS.h>
#include <SD.h>

// var publish
extern uint64_t cardSize_I;
extern uint8_t cardType_I; 

extern void listDir_I(fs::FS &fs, const char * dirname, uint8_t levels);
extern void createDir_I(fs::FS &fs, const char * path);
extern void removeDir_I(fs::FS &fs, const char * path);
extern void readFile_I(fs::FS &fs, const char * path);
extern void writeFile_I(fs::FS &fs, const char * path, const char * message);

extern void appendFile_I(fs::FS &fs, const char * path, const char * message);
extern void renameFile_I(fs::FS &fs, const char * path1, const char * path2);
extern void deleteFile_I(fs::FS &fs, const char * path);

extern File WrapOpen(fs::FS &fs, const char *path);

void SDCardInit();

#endif