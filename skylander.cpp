//
// Created by thn on 27.12.16.
//

#include "skylander.h"
#include "portalio.h"


SkylanderIO::SkylanderIO() {
    buffer = new unsigned char[1025];
}

SkylanderIO::~SkylanderIO() {
    delete buffer;
}

void SkylanderIO::ReadSkylander() throw(int) {

    unsigned char data[0x10];
    unsigned char *ptr;

    PortalIO *port;
    port = new PortalIO();
    port->SetPortalColor(0x00, 0xff, 0x00);
    port->SetPortalColor(0x00, 0xff, 0x00);
    port->SetPortalColor(0x00, 0xff, 0x00);

    bool found = false;
    bool first = true;
    int count = 0;
    while (!found) {
        try {
            // must start with a read of block zero (only skylander 0)
            port->ReadBlock(0, data, 0);
            printf("\n");
            found = true;
        } catch (int e) {
            if (e != 11) {
               throw e;
            } else {
                if (first) {
                    printf("waiting for skylander ");
                    first = false;
                } else {
                    count++;
                    if (count == 10) {
                        count = 0;
                        printf(".");
                    }
                }
            }
            fflush(stdout);
        }
        if (found) {
            found = true;
        }
    }

    port->SetPortalColor(0xff, 0x00, 0x00);
    port->SetPortalColor(0xff, 0x00, 0x00);

    ptr = buffer;
    memcpy(ptr, data, sizeof(data));

    for (unsigned int block = 1; block < 0x40; ++block) {
        ptr += 0x10;
        port->ReadBlock(block, data, 0);
        memcpy(ptr, data, sizeof(data));
    }

    port->SetPortalColor(0x00, 0x00, 0xff);
    port->SetPortalColor(0x00, 0x00, 0xff);
    delete port;
}

void SkylanderIO::WriteSkylander() throw(int) {
    PortalIO *port;
    port = new PortalIO();
    bool selectAccessControlBlock;

    for (int i = 0; i < 2; i++) {
        selectAccessControlBlock = i == 0;
        for (unsigned int block = 0; block < 0x40; ++block) {
            int offset = block * 0x10;
            if (IsAccessControlBlock(block) == selectAccessControlBlock) {
                port->WriteBlock(block, buffer + offset, 0);
            }
        }
    }
}

void SkylanderIO::ResetSkylander() throw(int) {
    for (unsigned int block = 0; block < 0x40; ++block) {
        int offset = block * 0x10;
        if (!IsAccessControlBlock(block) && block > 0x4 && block != 0x22 && block != 0x3e) {
            for (int i = 0; i < 0x10; ++i) {
                int offset2 = offset + i;
                buffer[offset2] = 0;
            }
        }
    }

}

void SkylanderIO::ReadSkylanderFile(char *name) throw(int) {
    FILE *file;
    int count;
    unsigned long fileLen;

    //Open file
    file = fopen(name, "rb");
    if (!file) {
        throw 1;
    }

    //Get file length
    fseek(file, 0, SEEK_END);
    fileLen = (unsigned long)ftell(file);
    fseek(file, 0, SEEK_SET);

    if (fileLen != 1024) {
        throw 2;
    }
    //Read file contents into buffer
    count = (int) fread(buffer, fileLen, 1, file);
    if (count < 1) {
        fclose(file);
        throw 3;
    }
    fclose(file);
}

void SkylanderIO::FileExists(char *name) throw (int) {
    if (realpath(name, NULL)) {
        throw 4;
    }
}

void SkylanderIO::WriteSkylanderFile(char *name, unsigned char *filedata) throw(int) {
    FILE *file;
    int count;

    file = fopen(name, "wb");
    if (!file) {
        throw 1;
    }
    count = (int) fwrite(filedata, 1024, 1, file);
    if (count < 1) {
        fclose(file);
        throw 3;
    }

    fclose(file);
}


void SkylanderIO::dump(unsigned char *c, unsigned int n) {
    unsigned int h, i;
    unsigned char j;

    for (h = 0; h < n; h += 16) {

        printf("%04x: ", h);

        for (i = 0; i < 16; i++) {
            if (i + h < n)
                printf("%02x ", *(c + i + h) & 0xff);
            else
                printf("   ");
        }
        for (i = 0; i < 16; i++) {
            if (i + h < n) {
                j = *(c + i + h);
                if (j < 32) j = '.';
                if (j >= 127) j = '.';
                printf("%c", j);
            } else
                printf(" ");
        }
        printf("\n");
    }
}


