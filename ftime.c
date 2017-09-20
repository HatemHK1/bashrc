#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <inttypes.h>
#include <math.h>

#define NANOSECOND 1LL
#define MICROSECOND 1000LL
#define MILLISECOND 1000000LL
#define SECOND 1000000000LL
#define MINUTE 60000000000LL
#define HOUR 3600000000000LL


#define COLOR_OFF     "\x1B[0m"
#define COLOR_RED     "\x1B[31m"
#define COLOR_GREEN   "\x1B[32m"
#define COLOR_YELLOW  "\x1B[33m"
#define COLOR_BLUE    "\x1B[34m"
#define COLOR_MAGENTA "\x1B[35m"
#define COLOR_CYAN    "\x1B[36m"
#define COLOR_WHITE   "\x1B[37m"

int64_t getNanoTimestamp(void) {
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);

    return 1000000000LL * spec.tv_sec + spec.tv_nsec;
}

struct tm_ext {
  struct tm *dateTime;
  int32_t nanos;
};

void getUtcDateTime(struct tm_ext * timeInfo) {
  int64_t nanoTs;
  int32_t nanos;
  time_t timestamp;
  struct tm *dateTime;

  nanoTs = getNanoTimestamp();
  timestamp = nanoTs / 1000000000;
  dateTime = gmtime(&timestamp);

  *timeInfo = (struct tm_ext) {
    .dateTime = dateTime,
    .nanos = nanoTs
  };
}

struct tm *getDateTime(void) {
  time_t timestamp;
  struct tm *dateTime;
  
  timestamp = time(NULL);
  dateTime = localtime(&timestamp);
  
  return dateTime;
}

void printNanoTime(int64_t nanoTime) {
  int64_t high = 0;
  int64_t low = 0;
  int negative = nanoTime < 0LL ? 1 : 0;
  nanoTime = negative ? (-1LL * nanoTime) : nanoTime;

  if (nanoTime >= HOUR) {
    high = nanoTime / HOUR;
    low = nanoTime % HOUR / MINUTE;
    printf("%ld h, %ld m\n", high, low);
  } else if (nanoTime >= MINUTE) {
    high = nanoTime / MINUTE;
    low = nanoTime % MINUTE / SECOND;
    printf("%s%ld m, %ld s%s\n", high, low);
  } else if (nanoTime >= SECOND) {
    high = nanoTime / SECOND;
    low = nanoTime % SECOND / MILLISECOND;
    printf("%ld.%03ld s\n", high, low);
  } else if (nanoTime >= MILLISECOND) {
    high = nanoTime / MILLISECOND;
    low = nanoTime % MILLISECOND / MICROSECOND;
    printf("%ld.%03ld ms\n", high, low);
  } else {
    high = nanoTime / MICROSECOND;
    low = nanoTime % MICROSECOND;
    printf("%ld.%03ld us\n", high, low);
  }
}

void usage(void) {
  printf("Usage: time format <nano_timestamp>\n");
  printf("       time ns|us|ms|s|m|h\n");
  printf("       time iso|iso-bash\n");
  printf("       time bash\n");
}

int main(int argc, char **argv) {
  char *dateColor = COLOR_GREEN;
  char *timeColor = COLOR_YELLOW;
  char *noColor = COLOR_OFF;
  int64_t nanoTime;
  struct tm_ext timeInfo;
  struct tm *dt;

  if (argc == 2 && strcmp(argv[1], "ns") == 0) {
    printf("%lld\n", getNanoTimestamp());
  } else if (argc == 2 && strcmp(argv[1], "us") == 0) {
    printf("%lld\n", getNanoTimestamp() / MICROSECOND);
  } else if (argc == 2 && strcmp(argv[1], "ms") == 0) {
    printf("%lld\n", getNanoTimestamp() / MILLISECOND);
  } else if (argc == 2 && strcmp(argv[1], "s") == 0) {
    printf("%lld\n", getNanoTimestamp() / SECOND);
  } else if (argc == 2 && strcmp(argv[1], "m") == 0) {
    printf("%lld\n", getNanoTimestamp() / MINUTE);
  } else if (argc == 2 && strcmp(argv[1], "h") == 0) {
    printf("%lld\n", getNanoTimestamp() / HOUR);
  } else if (argc == 2 && strcmp(argv[1], "iso") == 0) {
    getUtcDateTime(&timeInfo);
    dt = timeInfo.dateTime; 
    printf("%04d-%02d-%02dT%02d:%02d:%02d.%03dZ\n",
        dt->tm_year + 1900,
        dt->tm_mon + 1,
        dt->tm_mday,
        dt->tm_hour,
        dt->tm_min,
        dt->tm_sec,
        timeInfo.nanos / 1000000
    );
  } else if (argc == 2 && strcmp(argv[1], "iso-bash") == 0) {
    getUtcDateTime(&timeInfo);
    dt = timeInfo.dateTime; 
    printf("%s%04d%s-%s%02d%s-%s%02d%sT%s%02d%s:%s%02d%s:%s%02d%s.%s%03d%sZ\n",
        dateColor, dt->tm_year + 1900, noColor,
        dateColor, dt->tm_mon + 1, noColor,
        dateColor, dt->tm_mday, noColor,
        timeColor, dt->tm_hour, noColor,
        timeColor, dt->tm_min, noColor,
        timeColor, dt->tm_sec, noColor,
        timeColor, timeInfo.nanos / 1000000, noColor
    );
  } else if (argc == 2 && strcmp(argv[1], "bash") == 0) {
    dt = getDateTime();
    printf("%s%04d%s-%s%02d%s-%s%02d%s %s%02d%s:%s%02d%s:%s%02d%s\n",
        dateColor, dt->tm_year + 1900, noColor,
        dateColor, dt->tm_mon + 1, noColor,
        dateColor, dt->tm_mday, noColor,
        timeColor, dt->tm_hour, noColor,
        timeColor, dt->tm_min, noColor,
        timeColor, dt->tm_sec, noColor
    );
  } else if (argc == 3 && strcmp(argv[1], "format") == 0) {
    nanoTime = atoll(argv[2]);
    printNanoTime(nanoTime);
  } else {
    usage();
    return 1;
  }

  return 0;
}
