#ifndef COMPONENTS_UTILS_H
#define COMPONENTS_UTILS_H

#include <time.h>

void log_started(const char *who);
void log_message(const char *who, const char *msg);

void log_repeat(const char *who, const char *msg, int count);
int to_int(const char *s, int fallback);

#endif // COMPONENTS_UTILS_H
