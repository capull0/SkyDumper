#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <iostream>
#include "skylander.h"

using namespace std;

void usage() {
    printf("\n"
            "Usage:\n"
            "SkyDumper [-o <filename> | -d] [-i <filename> [-r]]\n"
            "-i <filename>\t read Skylander Image from file (default: read from portal)\n"
            "-o <filename>\t write Skylander Image to file (default: write to portal)\n"
            "-d\t\t write Skylander image to file <Skylander-ID>.bin\n"
            "-r\t\t reset Skylander before writing (e.g. reset crystal)\n"
    );
}

int main(int argc, char *argv[]) {
    unsigned char *buffer;

    bool dump, reset, verbose;

    char *inFile, *outFile;

    const static char *legal_flags = "hrdi:o:v";

    inFile = NULL;
    outFile = NULL;
    dump = false;
    reset = false;
    verbose = false;
    SkylanderIO *skio;

    int k;

    while ((k = getopt(argc, argv, legal_flags)) != -1) {
        switch (k) {
            case 'i':
                inFile = new char[strlen(optarg) + 1];
                strcpy(inFile, optarg);
                break;
            case 'd':
                dump = true;
                break;
            case 'o':
                outFile = new char[strlen(optarg) + 1];
                strcpy(outFile, optarg);
                break;
            case 'r':
                reset = true;
                break;
            case 'v':
                verbose = true;
                break;
            case 'h':
                usage();
                exit(0);
            default:
                usage();
                exit(0);
        }
    }

    // validate command line options
    if (!inFile && !outFile && !dump) {
        printf("missing arguments -i <file> or -o <file>\n");
        usage();
        exit(0);
    }

    skio = new SkylanderIO();

    if (inFile) {
        printf("Read Skylander Image from %s\n", inFile);
        skio->ReadSkylanderFile(inFile);
    } else {
        printf("Read/Dump Skylander from portal\n");
        skio->ReadSkylander();
    }

    if (reset) {
        printf("Reset Skylander\n");
        skio->ResetSkylander();
    }

    buffer = skio->getSkylander();
    if (verbose)
        skio->dump(buffer, 1024);

    if (dump) {
        char f[16];
        sprintf(f, "%02X%02X.bin", buffer[0x11], buffer[0x10]);

        outFile = new char[strlen(f) + 1];
        strcpy(outFile, f);
    }

    if (outFile) {
        skio->FileExists(outFile);
        printf("Write Skylander Image to %s\n", outFile);
        skio->WriteSkylanderFile(outFile, buffer);
    } else {
        printf("Write Skylander to portal\n");
        skio->WriteSkylander();
    }

    skio = NULL;
    delete skio;

    printf("\n... Success!\n");
    return 0;

}
