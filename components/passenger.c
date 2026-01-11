#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "utils.h"

int pasazer_min_mp = 0;
int pasazer_max_mp = 0;
int passenger_count = 0;

int status = 0;
int prom_id;

// Removed local to_int function, using utils.h version instead

int main(int argc, char **argv)
{
    if (argc >= 4)
    {
        pasazer_min_mp = to_int(argv[1], 0);
        pasazer_max_mp = to_int(argv[2], 0);
        passenger_count = to_int(argv[3], 0);
    }

    char msg[160];
    snprintf(msg, sizeof(msg), "Parameters: pasazer_min_mp=%d, pasazer_max_mp=%d, passenger_count=%d", pasazer_min_mp, pasazer_max_mp, passenger_count);
    log_started("passenger");
    log_message("passenger", msg);

    // Fork passenger_count-1 children (this process is also a passenger)
    for (int i = 1; i < passenger_count; ++i)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            // Child process: log and exit
            log_started("passenger");
            return 0;
        }
        else if (pid < 0)
        {
            perror("fork");
            break;
        }
    }

    // Parent waits for all children
    int status;
    while (wait(&status) > 0)
        ;

    return 0;
}
