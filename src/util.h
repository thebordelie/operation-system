#define DIR_NAME "my_vmstat_dir"
#define FILE_NAME "my_vmstat_file"
#define BUFFER_SIZE 1024
#define DEBUG_PATH "/sys/kernel/debug/"

enum settings_output {
    OUTPUT_DEFAULT = 0,
    OUTPUT_WITHOUT_COLUMN
};

typedef enum settings_output output_settings;