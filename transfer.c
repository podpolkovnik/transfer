#include <stdlib.h>
#include <unistd.h>

#include "client_server.h"

int main(int argc, char **argv) {
    int opt, run_mode = RUN_MODE_UNDEF;
    char message[MSG_LEN] = "";
    size_t time = -1;
    run_params params;

    while ((opt = getopt (argc, argv, "scm:t:")) != -1) {
        switch (opt) {
            case 's':
                run_mode = RUN_MODE_SERVER;
                break;
            case 'c':
                run_mode = RUN_MODE_CLIENT;
                break;
            case 'm':
                strncpy(message, optarg, sizeof(message));
                break;
            case 't':
                time = atoi(optarg);
                break;
        }
    }
    
    params.run_mode = run_mode;
    strncpy(params.message, message, sizeof(params.message));
    params.time = time;

    run(&params);

    return 0;
}