
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

    void ReadSkylander() throw(int);

    void WriteSkylander() throw(int);

    void ResetSkylander() throw(int);

    void ReadSkylanderFile(char *name) throw(int);

    void WriteSkylanderFile(char *name, unsigned char *filedata) throw(int);

    void FileExists(char *name) throw (int);

    unsigned char *getSkylander() { return buffer; }

    bool IsAccessControlBlock(unsigned int blockIndex) { return blockIndex % 4 == 3; }

    void dump(unsigned char *, unsigned int);

};


#endif //SKYLAUNCHER_SKYLANDER_H