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

void SkylanderIO::ReadSkylander() {

    unsigned char data[0x10];
    unsigned char *ptr;
    int status;

    PortalIO *port;
    port = new PortalIO();
    port->SetPortalColor(0x00, 0xff, 0x00);
    port->SetPortalColor(0x00, 0xff, 0x00);
    port->SetPortalColor(0x00, 0xff, 0x00);

    bool found = false;
    bool first = true;
    int count = 0;
    while (!found) {
        status = port->PortalStatus();
        if (status > 0) {
            break;
        } else {
            if (first) {
                printf("waiting for skylander ");
                first = false;
            } else {
                count++;
                if (count == 30) {
                    count = 0;
                    printf(".");
                }
            }
            fflush(stdout);
        }
    }

    printf("\n");
    port->SetPortalColor(0xff, 0x00, 0x00);
    port->SetPortalColor(0xff, 0x00, 0x00);

    ptr = buffer;

    for (unsigned int block = 0; block < 0x40; ++block) {
        port->ReadBlock(block, data, 0);
        memcpy(ptr, data, sizeof(data));
        ptr += 0x10;
    }

    port->SetPortalColor(0x00, 0x00, 0xff);
    port->SetPortalColor(0x00, 0x00, 0xff);
    delete port;
}

void SkylanderIO::WriteSkylander() {
    PortalIO *port;
    bool selectAccessControlBlock;
    int status;

    port = new PortalIO();
    port->SetPortalColor(0x00, 0xff, 0x00);
    port->SetPortalColor(0x00, 0xff, 0x00);
    port->SetPortalColor(0x00, 0xff, 0x00);

    bool found = false;
    bool first = true;
    int count = 0;
    while (!found) {
        status = port->PortalStatus();
        if (status > 0) {
            break;
        } else {
            if (first) {
                printf("waiting for skylander ");
                first = false;
            } else {
                count++;
                if (count == 30) {
                    count = 0;
                    printf(".");
                }
            }
            fflush(stdout);
        }
    }

    printf("\n");
    port->SetPortalColor(0xff, 0x00, 0x00);
    port->SetPortalColor(0xff, 0x00, 0x00);

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

void SkylanderIO::ResetSkylander() {
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

void SkylanderIO::ReadSkylanderFile(char *name) {
    FILE *file;
    int count;
    unsigned long fileLen;

    //Open file
    file = fopen(name, "rb");
    if (!file) {
        printf("Cannot open File.\n");
        exit(1);
    }

    //Get file length
    fseek(file, 0, SEEK_END);
    fileLen = (unsigned long)ftell(file);
    fseek(file, 0, SEEK_SET);

    if (fileLen != 1024) {
        printf("Invalid Skylander File.\n");
        exit(1);
    }
    //Read file contents into buffer
    count = (int) fread(buffer, fileLen, 1, file);
    if (count < 1) {
        fclose(file);
        printf("Cannot read/write to File.\n");
        exit(1);
    }
    fclose(file);
}

void SkylanderIO::FileExists(char *name)  {
    if (realpath(name, NULL)) {
        printf("Skylander File already exists.\n");
        exit(1);
    }
}

void SkylanderIO::WriteSkylanderFile(char *name, unsigned char *filedata) {
    FILE *file;
    int count;

    file = fopen(name, "wb");
    if (!file) {
        printf("Cannot open File.\n");
        exit(1);
    }
    count = (int) fwrite(filedata, 1024, 1, file);
    if (count < 1) {
        fclose(file);
        printf("Cannot read/write to File.\n");
        exit(1);
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
