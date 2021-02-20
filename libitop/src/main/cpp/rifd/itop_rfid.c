//
// Created by sinochem on 2020/10/10.
//
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <android/log.h>

#include "itop_rfid.h"
#include "spidev.h"
#include "spidev_test.h"

#undef TCSAFLUSH
#define TCSAFLUSH TCSETSF
#ifndef _TERMIOS_H_
#define _TERMIOS_H_
#endif

int fd = 0;
int flag = -1;

unsigned char UID[5], temp[4];

int writeRawRc(int addr, int data) {
    int ret;
    // int fd = g_SPI_Fd;
    unsigned char txBuf[2];
    // bit7:MSB=0, bit6~1:addr,bit0:RFU=0
    txBuf[0] = ((unsigned char) addr << 1) & 0x7E;
    // TxBuf[0] &= 0x7E;
    txBuf[1] = (unsigned char) data;
    ret = write(fd, txBuf, 2);
    if (ret < 0) {
        __android_log_print(ANDROID_LOG_INFO, "rfid", "spi:SPI Write error\n");
    }
    usleep(10);
    return ret;
}

unsigned char readRawRc(int addr) {
    int ret;
    unsigned char reData;
    unsigned char address;
    address = (unsigned char) addr << 1;
    address |= (1 << 7);
    address &= ~(1 << 0);
    ret = write(fd, &address, 1);
    if (ret < 0) {
        __android_log_print(ANDROID_LOG_INFO, "rfid", "spi:SPI Write error\n");
    }
    usleep(100);
    ret = read(fd, &reData, 1);
    if (ret < 0) {
        __android_log_print(ANDROID_LOG_INFO, "rfid", "spi:SPI Read error\n");
    }
    return reData;
}

void setBitMask(unsigned char reg, unsigned char mask) {
    char tmp = 0x0;
    tmp = readRawRc(reg) | mask;
    writeRawRc(reg, tmp | mask);
}

void clearBitMask(unsigned char reg, unsigned char mask) {
    char tmp = 0x0;
    tmp = readRawRc(reg) & (~mask);
    writeRawRc(reg, tmp);
}

int rc522_init() {
    int ret;
    char version = 0;
    // reset
    writeRawRc(CommandReg, PCD_RESETPHASE);
    usleep(10);
    writeRawRc(ModeReg, 0x3D);
    writeRawRc(TReloadRegL, 30);
    writeRawRc(TReloadRegH, 0);
    writeRawRc(TModeReg, 0x8D);
    writeRawRc(TPrescalerReg, 0x3E);

    version = readRawRc(VersionReg);
    __android_log_print(ANDROID_LOG_INFO, "rfid", "Chip Version: 0x%x\n", version);
    usleep(50000);
    return 0;
}

void pcdAntennaOn() {
    unsigned char i;
    writeRawRc(TxASKReg, 0x40);
    usleep(20);
    i = readRawRc(TxControlReg);
    if (!(i & 0x03)) {
        setBitMask(TxControlReg, 0x03);
    }
    i = readRawRc(TxASKReg);
}

char pcdComMF522(unsigned char command, unsigned char *pInData, unsigned char inLenByte,
                 unsigned char *pOutData, unsigned int *pOutLenBit) {
    char status = MI_ERR;
    unsigned char irqEn = 0x00;
    unsigned char waitFor = 0x00;
    unsigned char lastBits;
    unsigned char n;
    unsigned int i;

    switch (command) {
        case PCD_AUTHENT:
            irqEn = 0x12;
            waitFor = 0x10;
            break;
        case PCD_TRANSCEIVE:
            irqEn = 0x77;
            waitFor = 0x30;
            break;
        default:
            break;
    }
    writeRawRc(ComIEnReg, irqEn | 0x80);
    clearBitMask(ComIrqReg, 0x80);
    writeRawRc(CommandReg, PCD_IDLE);
    setBitMask(FIFOLevelReg, 0x80);
    for (i = 0; i < inLenByte; i++) {
        writeRawRc(FIFODataReg, pInData[i]);
    }
    writeRawRc(CommandReg, command);
    if (command == PCD_TRANSCEIVE) {
        setBitMask(BitFramingReg, 0x80);
    }
    i = 6000;
    do {
        n = readRawRc(ComIrqReg);
        i--;
    } while (i != 0 && !(n & 0x01) && !(n & waitFor));

    clearBitMask(BitFramingReg, 0x80);

    if (i != 0) {
        if (!(readRawRc(ErrorReg) & 0x1B)) {
            status = MI_OK;
            if (n & irqEn & 0x01) {
                status = MI_NOTAGERR;
            }
            if (command == PCD_TRANSCEIVE) {
                n = readRawRc(FIFOLevelReg);
                lastBits = readRawRc(ControlReg) & 0x07;
                if (lastBits) {
                    *pOutLenBit = (n - 1) * 8 + lastBits;
                } else {
                    *pOutLenBit = n * 8;
                }
                if (n == 0) {
                    n = 1;
                }
                if (n > MAXRLEN) {
                    n = MAXRLEN;
                }
                for (i = 0; i < n; i++) {
                    pOutData[i] = readRawRc(FIFODataReg);
                }
            }
        } else {
            status = MI_ERR;
        }
    }

    setBitMask(ControlReg, 0x80);
    writeRawRc(CommandReg, PCD_IDLE);

    return status;
}

char pcdRequest(unsigned char req_code, unsigned char *pTagType) {
    char status;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN];

    clearBitMask(Status2Reg, 0x08);
    writeRawRc(BitFramingReg, 0x07);
    setBitMask(TxControlReg, 0x03);
    ucComMF522Buf[0] = req_code;

    status = pcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 1, ucComMF522Buf, &unLen);

    if (status == MI_OK && unLen == 0x10) {
        *pTagType = ucComMF522Buf[0];
        *(pTagType + 1) = ucComMF522Buf[1];
    } else {
        status = MI_ERR;
    }

    return status;
}

char pcdAnticoll(unsigned char *pSnr) {
    char status;
    unsigned char i, snr_check = 0;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN];

    clearBitMask(Status2Reg, 0x08);
    writeRawRc(BitFramingReg, 0x00);
    clearBitMask(CollReg, 0x80);

    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = pcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 2, ucComMF522Buf, &unLen);

    if (status == MI_OK) {
        for (i = 0; i < 4; i++) {
            *(pSnr + i) = ucComMF522Buf[i];
            snr_check ^= ucComMF522Buf[i];
        }
        if (snr_check != ucComMF522Buf[i]) {
            status = MI_ERR;
        }
    }

    setBitMask(CollReg, 0x80);

    return status;
}

int autoReader(void) {
    int i = 0;
    unsigned long num = 0;

    memset(UID, 0, 5);
    // while(1)
    {
        if (pcdRequest(0x52, temp) == MI_OK) {
            if (temp[0] == 0x04 && temp[1] == 0x00) {
                __android_log_print(ANDROID_LOG_INFO, "rfid", "MFOne-S50\n");
            } else if (temp[0] == 0x02 && temp[1] == 0x00) {
                __android_log_print(ANDROID_LOG_INFO, "rfid", "MFOne-S70\n");
            } else if (temp[0] == 0x44 && temp[1] == 0x00) {
                __android_log_print(ANDROID_LOG_INFO, "rfid", "MF-UltraLight\n");
            } else if (temp[0] == 0x08 && temp[1] == 0x00) {
                __android_log_print(ANDROID_LOG_INFO, "rfid", "MF-Pro\n");
            } else if (temp[0] == 0x44 && temp[1] == 0x03) {
                __android_log_print(ANDROID_LOG_INFO, "rfid", "MF Desire\n");
            } else {
                __android_log_print(ANDROID_LOG_INFO, "rfid", "Unknown\n");
            }

            if (pcdAnticoll(UID) == MI_OK) {
                __android_log_print(ANDROID_LOG_INFO, "rfid", "Card Id is (%d):", num++);
#if 1
                // for(i = 0; i<4; i++)
                __android_log_print(ANDROID_LOG_INFO, "rfid", "id: %x%x%x%x", UID[0], UID[1],
                                    UID[2], UID[3]);
#else
                tochar(UID[0]);
                tochar(UID[1]);
                tochar(UID[2]);
                tochar(UID[3]);
#endif
                __android_log_print(ANDROID_LOG_INFO, "rfid", "\n");

                pcdRequest(0x52, temp);
                return 4;
            } else {
                __android_log_print(ANDROID_LOG_INFO, "rfid", "no serial num read\n");
            }
        } else {
            __android_log_print(ANDROID_LOG_INFO, "rfid", "No Card!\n");
        }
        // usleep(300000);
        return 0;
    }
}

/*
 * Class:     com_jiangyt_simple_itop_relay_ItopRfid
 * Method:    open
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_jiangyt_library_libitop_ItopRfid_open
        (JNIEnv *env, jobject obj) {
    if (fd <= 0) fd = open("dev/rc522", O_RDWR | O_NDELAY | O_NOCTTY);
    if (fd <= 0) {
        __android_log_print(ANDROID_LOG_INFO, "rfid", "open /dev/rc522 Error");
        flag = -1;
    } else {
        __android_log_print(ANDROID_LOG_INFO, "rfid", "open /dev/rc522 Success fd = %d", fd);
        flag = rc522_init();
        pcdAntennaOn();
    }
    return flag;
}

/*
 * Class:     com_jiangyt_simple_itop_relay_ItopRfid
 * Method:    close
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_jiangyt_library_libitop_ItopRfid_close
        (JNIEnv *env, jobject obj) {
    if (fd > 0) close(fd);
    return 0;
}

/*
 * Class:     com_jiangyt_simple_itop_relay_ItopRfid
 * Method:    ioCtl
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_com_jiangyt_library_libitop_ItopRfid_ioCtl
        (JNIEnv *env, jobject obj, jint num, jint en) {
    return ioctl(fd, en, num);
}

/*
 * Class:     com_jiangyt_simple_itop_relay_ItopRfid
 * Method:    read
 * Signature: ()[I
 */
JNIEXPORT jintArray JNICALL Java_com_jiangyt_library_libitop_ItopRfid_read
        (JNIEnv *env, jobject obj) {
    unsigned char buffer[512];
    int bufToJava[512];
    int len = 0, i = 0;

    memset(buffer, 0, sizeof(buffer));
    memset(bufToJava, 0, sizeof(bufToJava));

    len = read(fd, buffer, 512);

    if (len <= 0) return NULL;
    for (i = 0; i < len; i++) {
        __android_log_print(ANDROID_LOG_INFO, "rfid", "read %x", buffer[i]);
        bufToJava[i] = buffer[i];
    }
    jintArray array = (*env)->NewIntArray(env, len);
    (*env)->SetIntArrayRegion(env, array, 0, len, bufToJava);
    return array;
}

/*
 * Class:     com_jiangyt_simple_itop_relay_ItopRfid
 * Method:    readCardNum
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL Java_com_jiangyt_library_libitop_ItopRfid_readCardNum
        (JNIEnv *env, jobject obj) {
    unsigned char buffer[512];
    int bufToJava[512];
    int len = 0, i = 0;
#if 0
    memset(buffer, 0, sizeof(buffer));
    memset(bufToJava, 0, sizeof(bufToJava));

    len = read(fd, buffer, 512);

    if (len <= 0) return NULL;
    for (i = 0; i < len; i++) {
        __android_log_print(ANDROID_LOG_INFO, "rfid", "read %x", buffer[i]);
        bufToJava[i] = buffer[i];
    }
#else
    len = autoReader();
    if (len <= 0) return NULL;

    __android_log_print(ANDROID_LOG_INFO, "rfid", "id: %x%x%x%x", UID[0], UID[1],
                        UID[2], UID[3]);

    for (i = 0; i < len; i++) {
        bufToJava[i] = UID[i];
    }
#endif
    jbyteArray array = (*env)->NewByteArray(env, len);
    (*env)->SetByteArrayRegion(env, array, 0, len, UID);
    __android_log_print(ANDROID_LOG_INFO, "rfid", "num: %x%x%x%x", UID[0], UID[1],
                        UID[2], UID[3]);
    return array;
}


