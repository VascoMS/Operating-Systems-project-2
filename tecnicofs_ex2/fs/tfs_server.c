#include "operations.h"
static session_ids[MAX_CLIENTS];
int main(int argc, char **argv) {
    int fd1;
    int fd2;
    char *pipe_client;
    char pipe1[PIPENAME_SIZE+1];
    for(int i = 0; i<MAX_CLIENTS; i++){
        session_ids[i] = FREE;
    }

    if (argc < 2) {
        printf("Please specify the pathname of the server's pipe.\n");
        return 1;
    }

    char *pipename = argv[1];
    printf("Starting TecnicoFS server with pipe called %s\n", pipename);
    unlink(pipename);
    mkfifo(pipename, 0777);
    if(fd1 = open(pipename, TFS_OP_CODE_READ) < 0)
        return -1;

    read(fd1, pipe1, PIPENAME_SIZE);

    if(fd2 = open(pipe_client, TFS_OP_CODE_WRITE) < 0){
        return -1;
    }

    for(int i=0; i<MAX_CLIENTS; i++){
        if(session_ids[i]==FREE){
            session_ids[i]=TAKEN;
            write(fd2, i, sizeof(int));
        }
    }

    /* TO DO */

    return 0;
}