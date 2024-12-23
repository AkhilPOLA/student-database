#include "Parser.h"
#include "Client.h"
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void signal_handler(int signum);

int main(int argc, char *argv[])
{
    signal(SIGPIPE, signal_handler);
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <Input_File> <Server_IP> <Port>\n", argv[0]);
        return 1;
    }

    const char *server_ip = argv[2];
    const char *server_port = argv[3];

    connectSocket(server_ip, server_port);  
    
    FILE *input_file = fopen(argv[1], "r");
    if (!input_file)
    {
        fprintf(stderr,"Error opening input file %s",argv[1]);       
        return 1;
    }
    parsingFile(input_file);  
    fclose(input_file);
    closeFd();

    return 0;
}

void signal_handler(int signum)
{
    if (signum == SIGPIPE)
    {
        printf("Received SIGPIPE signal\n");
        exit(1);
    }
}
