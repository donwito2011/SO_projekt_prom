#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static void write_log(const char *who, const char *line)
{
    // FILE *f = fopen("program.log", "a");
    // if (!f)
    //     return;
    time_t t = time(NULL);
    char ts[64];
    struct tm *tm = localtime(&t);
    if (tm)
        strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", tm);
    printf("%s [%d]: %s %s\n", ts, (int)getpid(), who, line);
    // fprintf(f, "%s [%d]: %s %s\n", ts, (int)getpid(), who, line);
    // fclose(f);
}

void log_started(const char *who)
{
    write_log(who, "started");
    printf("%s started (pid=%d)\n", who, (int)getpid());
}

void log_message(const char *who, const char *msg)
{
    write_log(who, msg);
}

int to_int(const char *s, int fallback)
{
    if (!s)
        return fallback;
    char *end;
    long v = strtol(s, &end, 10);
    return (end == s) ? fallback : (int)v;
}