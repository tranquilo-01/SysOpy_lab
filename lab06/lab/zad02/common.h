#ifndef __COMMON_H__
#define __COMMON_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>

/* message buffer for msgsnd and msgrcv calls */
struct msgbuf {
    long mtype;     /* typ komunikatu   */
    char mtext[64]; /* tresc komunikatu */
}msgbuf;

const int MSG_SIZE = sizeof(msgbuf);

#endif