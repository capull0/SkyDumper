#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <iostream>
#include <skylander.h>
#include "skylander.h"

using namespace std;

void usage() {
    printf("\n"
            "Usage:\n"
            "skylauncher [-o <filename>] [-i <filename> [-r]]\n"
            "-i <filename>\t read Skylander Image from file (default: read from portal)\n"
            "-o <filename>\t write Skylander Image to file (default: write to portal)\n"
            "-r\t\t reset Skylander before writing (e.g. reset crystal)\n"
    );
}

int main(int argc, char *argv[]) {
    unsigned char *buffer;

    bool reset, verbose;

    char *inFile, *outFile;

    const static char *legal_flags = "hri:o:v";

    inFile = NULL;
    outFile = NULL;
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
                break;
            default:
                usage();
                exit(0);
        }
    }

    try {
        // validate command line options
        if (!inFile && !outFile) {
            printf("missing arguments -i <file> or -o <file>\n");
            usage();
            exit(0);
        }

        skio = new SkylanderIO();

        if (inFile) {
            printf("Read Skylander Image from %s\n\n", inFile);
            skio->ReadSkylanderFile(inFile);
        } else {
            printf("Read/Dump Skylander from portal\n\n");
            skio->ReadSkylander();
        }

        if (reset) {
            printf("Reset Skylander\n\n");
            skio->ResetSkylander();
        }

        buffer = skio->getSkylander();
        if (verbose)
            skio->dump(buffer, 1024);

        if (outFile) {
            printf("Write Skylander Image to %s\n\n", outFile);
            skio->WriteSkylanderFile(outFile, buffer);
        } else {
            printf("Write Skylander to portal\n\n");
            skio->WriteSkylander();
        }

        delete skio;

        printf("\nSuccess!\n\n");
        return 0;

    } catch (int e) {

        switch (e) {
            case 1:
                printf("Cannot open File.\n");
                break;
            case 2:
                printf("Invalid Skylander File.\n");
                break;
            case 3:
                printf("Cannot write to File.\n");
                break;
            case 4:
                printf("Unable to get USB Device List.\n");
                break;
            case 5:
                printf("Cannot Find Portal USB.\n");
                break;
            case 6:
                printf("Unable to write to Portal.\n");
                break;
            case 7:
                printf("Invalid Skylander Block.\n");
                break;
            case 8:
                printf("Unable to read Skylander from Portal.\n");
                break;
            case 9:
                printf("Wireless portal not connected.\n");
                break;
            case 10:
                printf("Skylander Write Verify Error.\n");
                break;
            case 11:
                printf("No Skylander detected on portal.\n");
                break;
            default:
                printf("Unknown exception: %d.\n", e);
                break;
        }
    }
}