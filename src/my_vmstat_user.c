#include <stdio.h>
#include <unistd.h>
#include "util.h"
#include <stdbool.h>

long time_out = -1;
long count = -1;
char* file_path = DEBUG_PATH DIR_NAME "/" FILE_NAME;

int main(int argc, char *argv[]) {
    
  set_settings(argc, argv);
  read_from_file(0);



    if (time_out > 0) {
      if (count >0) {
        for (int i = 0; i < count - 1; ++i) {
          sleep(time_out);
          read_from_file(1);
        }
      }
      else {
        while (true)
        {
          sleep(time_out);
          read_from_file(1);
        }
        
      }
    }
    
    return 0;
}

void read_from_file(int output) {
    FILE *vmstat_file = fopen(file_path, "r+");
    if (!vmstat_file) {
        printf("File doesn't exist\n");
        return -1;
    }
    fprintf(vmstat_file, "-o:%d", output);
    print_output(vmstat_file);
    fclose(vmstat_file);
}

void print_output(FILE *vmstat_file) {
    
    char *buffer[BUFFER_SIZE];
    while (!feof(vmstat_file)) {
      char *result = fgets(buffer, BUFFER_SIZE, vmstat_file);
      printf(result);
    }
}

void set_settings(int argc, char *argv[]) {
  for (int i = 1; i < argc; ++i) { 
    char * settings = argv[i];
    char *endptr;
    long num = strtol(settings, &endptr, 10);
    if (*endptr != '\0') {
        continue;
    }
    if (time_out < 0) {
        time_out = num;
    }
    else {
          count = num;
      }
    }
}
