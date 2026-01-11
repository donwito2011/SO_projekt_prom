#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "components/utils.h"

static int N = 3;  // liczba promów
static int P = 10; // pojemność promu
static int K = 3;  // pojemność trapezu

static int passenger_count = 5; // liczba pasażerów

static int pasazer_min_mp = 10; // minimalna waga bagażu pasażera
static int pasazer_max_mp = 40; // maksymalna waga bagażu pasażera
static int prom_min_mp = 20;    // minimalna akceptowana waga bagażu na prom
static int prom_max_mp = 50;    // maksymalna akceptowana waga bagażu na prom

int main(void)
{
    log_started("master");

    pid_t pid;

    pid = fork();
    if (pid == 0)
    {
        execl("./manager", "manager", NULL);
        perror("execl manager");
        exit(1);
    }
    else if (pid < 0)
    {
        perror("fork manager");
        return 1;
    }

    pid = fork();
    if (pid == 0)
    {
        char nbuf[16], pbuf[16], kbuf[16], pminbuf[16], pmaxbuf[16];
        snprintf(nbuf, sizeof(nbuf), "%d", N);
        snprintf(pbuf, sizeof(pbuf), "%d", P);
        snprintf(kbuf, sizeof(kbuf), "%d", K);
        snprintf(pminbuf, sizeof(pminbuf), "%d", prom_min_mp);
        snprintf(pmaxbuf, sizeof(pmaxbuf), "%d", prom_max_mp);
        execl("./ferry", "ferry", nbuf, pbuf, kbuf, pminbuf, pmaxbuf, NULL);
        perror("execl ferry");
        exit(1);
    }
    else if (pid < 0)
    {
        perror("fork ferry");
        return 1;
    }

    pid = fork();
    if (pid == 0)
    {
        char pminbuf[16], pmaxbuf[16], countbuf[16];
        snprintf(pminbuf, sizeof(pminbuf), "%d", pasazer_min_mp);
        snprintf(pmaxbuf, sizeof(pmaxbuf), "%d", pasazer_max_mp);
        snprintf(countbuf, sizeof(countbuf), "%d", passenger_count);
        execl("./passenger", "passenger", pminbuf, pmaxbuf, countbuf, NULL);
        perror("execl passenger");
        exit(1);
    }
    else if (pid < 0)
    {
        perror("fork passenger");
        return 1;
    }

    int status;
    while (wait(&status) > 0)
        ;

    return 0;
}
