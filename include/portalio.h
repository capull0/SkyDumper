#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <string.h>

// unix io functions
#include <fcntl.h>


#include "hidapi.h"
#include "skylander.h"
#include "crypt.h"

#define rw_buf_size  0x21
#define TIMEOUT 30000
#define SKYLANDER_SIZE 1024
#define MAX_STR 255

#define DEBUG 0

typedef struct {
    unsigned char buf[rw_buf_size];
    int dwBytesTransferred;
} RWBlock;

class PortalIO {

    hid_device *hPortalHandle;

public:
    PortalIO();

    ~PortalIO();

    bool ReadBlock(unsigned int, unsigned char [0x10], int);

    void SetPortalColor(unsigned char, unsigned char, unsigned char);

    bool WriteBlock(unsigned int, unsigned char [0x10], int);

    void flash(void);

    unsigned char PortalStatus();

private:
    void OpenPortalHandle();

    void Write(RWBlock *);

    void RestartPortal(void);

    void ActivatePortal(int);

    bool CheckResponse(RWBlock *, char);
};
