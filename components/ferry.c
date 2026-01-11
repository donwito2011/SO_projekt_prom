#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include "utils.h"

int N = 0; // liczba promów
int P = 0; // pojemność promu
int K = 0; // pojemność trapezu

int prom_min_mp = 0; // minimalna akceptowana waga bagażu na prom
int prom_max_mp = 0; // maksymalna akceptowana waga bagażu na prom

// System V message queue example
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <unistd.h>

struct ferry_msg
{
    long mtype;
    char mtext[128];
};

int walkway_deployed = 0;

int main(int argc, char **argv)
{
    if (argc >= 6)
    {
        N = to_int(argv[1], 0);
        P = to_int(argv[2], 0);
        K = to_int(argv[3], 0);
        prom_min_mp = to_int(argv[4], 0);
        prom_max_mp = to_int(argv[5], 0);
    }

    char msg[256];
    snprintf(msg, sizeof(msg), "Parameters: N=%d, P=%d, K=%d, prom_min_mp=%d, prom_max_mp=%d", N, P, K, prom_min_mp, prom_max_mp);
    log_started("ferry");
    log_message("ferry", msg);

    // Setup message queue (ftok with a fixed path and id)
    key_t key = ftok("/tmp", 65);
    if (key == -1)
    {
        perror("ftok");
        return 1;
    }
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        perror("msgget");
        return 1;
    }

    // Main loop: wait for messages and act
    while (1)
    {
        struct ferry_msg m;
        ssize_t r = msgrcv(msgid, &m, sizeof(m.mtext), 0, 0);
        if (r < 0)
        {
            if (errno == EINTR)
                continue;
            perror("msgrcv");
            break;
        }
        // Actions: log, terminate, dock, depart
        if (strncmp(m.mtext, "log:", 4) == 0)
        {
            log_message("ferry", m.mtext + 4);
        }
        else if (strncmp(m.mtext, "terminate", 9) == 0)
        {
            log_message("ferry", "Received terminate. Exiting.");
            break;
        }
        else if (strncmp(m.mtext, "dock", 4) == 0)
        {
            log_message("ferry", "Received dock signal. Ferry is docking.");
            sleep(1);
            walkway_deployed = 1;
            log_message("ferry", "Walkway deployed.");
        }
        else if (strncmp(m.mtext, "depart", 6) == 0)
        {
            log_message("ferry", "Received depart signal. Ferry is departing.");
            sleep(1);
            walkway_deployed = 0;
            log_message("ferry", "Walkway retracted.");
        }
        else
        {
            log_message("ferry", "Unknown message received");
        }
    }

    // Cleanup
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}
