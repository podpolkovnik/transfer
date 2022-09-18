#include "run_mode.h"
#include "const.h"

typedef struct s_packet {
    char message[MSG_LEN];
    struct timespec time_val;
} packet;

typedef struct s_handler_args {
    int client_sock_fd;
    char message[MSG_LEN];
    size_t time;
} handler_args;

typedef struct s_run_params {
    char message[MSG_LEN];
    size_t time;
    int run_mode;
} run_params;