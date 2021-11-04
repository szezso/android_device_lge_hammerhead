/*
 * Copyright (C) 2020-2021 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#define LOG_TAG "bluetooth_loader"
#include <cutils/log.h>
#include <cutils/properties.h>

#define BDADDR_BIN_LEN 6
#define BDADDR_STR_LEN 18

#define BDADDR_DATA_FILEPATH "/data/misc/bluetooth/.bdaddr"
#define BDADDR_PERSIST_FILEPATH "/persist/bluetooth/.bdaddr"
#define BDADDR_SERVICE_PROPERTY "persist.service.bdroid.bdaddr"

typedef struct {
    unsigned char bytes[BDADDR_BIN_LEN];
} BDADDR;

int checkAddr(BDADDR *bdAddr) {
    unsigned char addrNums = 0;
    for (int i = 0; i < BDADDR_BIN_LEN; i += 1) {
        addrNums |= bdAddr->bytes[i];
    }
    return (addrNums > 0 ? 0 : 1);
}

int generateAddr(BDADDR *bdAddr) {
    bdAddr->bytes[0] = 0x22;
    bdAddr->bytes[1] = 0x22;
    bdAddr->bytes[2] = (unsigned char)(rand() % 256);
    bdAddr->bytes[3] = (unsigned char)(rand() % 256);
    bdAddr->bytes[4] = (unsigned char)(rand() % 256);
    bdAddr->bytes[5] = (unsigned char)(rand() % 256);
    return 0;
}

int setAddr(BDADDR *bdAddr, const char *propertyName) {
    char str[BDADDR_STR_LEN];
    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
            bdAddr->bytes[0], bdAddr->bytes[1], bdAddr->bytes[2],
            bdAddr->bytes[3], bdAddr->bytes[4], bdAddr->bytes[5]);

    int res = property_set(propertyName, str);
    return (res == 0 ? 0 : 1);
}

int readAddr(BDADDR *bdAddr, const char *filePath) {
    int fd = open(filePath, O_RDONLY);
    if (fd < 0) {
        return 1;
    }

    int count = read(fd, bdAddr->bytes, BDADDR_BIN_LEN);
    close(fd);

    return (count == BDADDR_BIN_LEN ? 0 : 2);
}

int writeAddr(BDADDR *bdAddr, const char *filePath) {
    int fd = open(filePath, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    if (fd < 0) {
        return 1;
    }

    int count = write(fd, bdAddr->bytes, BDADDR_BIN_LEN);
    close(fd);

    return (count == BDADDR_BIN_LEN ? 0 : 2);
}

int main() {
    BDADDR addr;
    srand(time(NULL));

    // Use .bdaddr from /data if exists
    if (readAddr(&addr, BDADDR_DATA_FILEPATH) == 0) {
        if (checkAddr(&addr) == 0) {
            setAddr(&addr, BDADDR_SERVICE_PROPERTY);
            return 0;
        }
    }

    // Use .bdaddr from /persist if exists and store it on /data
    if (readAddr(&addr, BDADDR_PERSIST_FILEPATH) == 0) {
        if (checkAddr(&addr) == 0) {
            writeAddr(&addr, BDADDR_DATA_FILEPATH);
            setAddr(&addr, BDADDR_SERVICE_PROPERTY);
            return 0;
        }
    }

    // Generate new .bdaddr and store it on /data
    generateAddr(&addr);
    writeAddr(&addr, BDADDR_DATA_FILEPATH);
    setAddr(&addr, BDADDR_SERVICE_PROPERTY);
    return 0;
}
