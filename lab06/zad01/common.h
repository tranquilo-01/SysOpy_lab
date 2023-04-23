#ifndef __COMMON_H__
#define __COMMON_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>

#define HOME_PATH getenv("HOME")
#define MAX_CLIENT_NUMBER 10
#define MESSAGE_LENGTH 64
#define PROJECT_ID 'A'

/* message buffer for msgsnd and msgrcv calls */
typedef struct msgbuf {
    long type;       /* typ komunikatu   */
    char text[64];   /* tresc komunikatu */
    int clientID;    /*id klienta przyznane przez serwer*/
    key_t clientKey; /*klucz dostepu do kolejki klienta*/
} msgbuf;

const int MSG_BUFF_SIZE = sizeof(msgbuf) - sizeof(long);

enum job {
    INIT = 1,
    STOP = 2,
    LIST = 3,
    TALL = 4,
    TONE = 5
} job;

#endif