
//
// Created by thn on 27.12.16.
//

#ifndef SKYLAUNCHER_SKYLANDER_H
#define SKYLAUNCHER_SKYLANDER_H

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

class SkylanderIO {
    unsigned char *buffer;

public:
    SkylanderIO();

    ~SkylanderIO();

    void ReadSkylander();

    void WriteSkylander();

    void ResetSkylander();

    void ReadSkylanderFile(char *name);

    void WriteSkylanderFile(char *name, unsigned char *filedata);

    void FileExists(char *name);

    unsigned char *getSkylander() { return buffer; }

    bool IsAccessControlBlock(unsigned int blockIndex) { return blockIndex % 4 == 3; }

    void dump(unsigned char *, unsigned int);
};

#endif //SKYLAUNCHER_SKYLANDER_H