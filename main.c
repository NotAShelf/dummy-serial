#include <errno.h>
#include <pty.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define LOG_LEVEL 3
#define LOG_INFO 1
#define LOG_ERROR 2
#define LOG_DEBUG 3

void log_message(int level, const char *format, ...) {
  time_t rawtime;
  struct tm *timeinfo;
  char buffer[80];

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
  printf("%s ", buffer);

  va_list args;
  va_start(args, format);
  if (level <= LOG_LEVEL) {
    vprintf(format, args);
  }
  va_end(args);
}

void close_pseudoterminal(int master, int slave) {
  close(master);
  close(slave);
  log_message(LOG_INFO, "Pseudoterminal closed\n");
}

int main(int argc, char *argv[]) {
  if (argc > 1 && strcmp(argv[1], "--quiet") == 0) {
    int master, slave;
    char slave_path[100];

    if (openpty(&master, &slave, slave_path, NULL, NULL) == -1) {
      log_message(LOG_ERROR, "Failed to open pseudoterminal: %s\n",
                  strerror(errno));
      exit(EXIT_FAILURE);
    }

    printf("%s\n", slave_path);
    exit(EXIT_SUCCESS);
  }

  int master, slave;
  char slave_path[100];

  if (openpty(&master, &slave, slave_path, NULL, NULL) == -1) {
    log_message(LOG_ERROR, "Failed to open pseudoterminal: %s\n",
                strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (chmod(slave_path, 0777) == -1) {
    log_message(LOG_ERROR,
                "Failed to change mode of the slave pseudoterminal: %s\n",
                strerror(errno));
    exit(EXIT_FAILURE);
  }

  log_message(LOG_INFO, "Pseudoterminal opened: %s\n", slave_path);

  while (1) {
    unsigned char dummy_data[6];
    for (int i = 0; i < 6; i++) {
      if (i == 2 || i == 4) {
        dummy_data[i] = rand() % 256; // Generate random 2-byte integer
      } else {
        dummy_data[i] = 0x00; // Keep other bytes as 0x00
      }
    }

    if (write(master, dummy_data, sizeof(dummy_data)) != sizeof(dummy_data)) {
      log_message(LOG_ERROR,
                  "Failed to write to master end of pseudoterminal: %s\n",
                  strerror(errno));
    } else {
      log_message(LOG_DEBUG, "Wrote data to master end of pseudoterminal\n");
    }

    usleep(500000); // 500ms
  }

  close_pseudoterminal(master, slave);
  return 0;
}
