#include "operations.h"
#include "common.h"
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

/* Session Ids */
static session_entry session_table[MAX_CLIENTS];
static char freesession_entries[MAX_CLIENTS];


int main(int argc, char **argv) {
    int fd1, op_code;
    size_t bytes_read=0;
    char *client_pipe;
    char buffer[MAX_PIPENAME_SIZE];

    for(int i = 0; i<MAX_CLIENTS; i++){
        session_table[i].id = i;
        freesession_entries[i] = FREE;
    }

    if (argc < 2) {
        printf("Please specify the pathname of the server's pipe.\n");
        return 1;
    }

    char *pipename = argv[1];
    printf("Starting TecnicoFS server with pipe called %s\n", pipename);
    unlink(pipename);
    mkfifo(pipename, 0777);
    if(fd1 = open(pipename, O_RDONLY) < 0)
        return -1;
    while(1){
        int bytes_read;
        size_t current;
        while(bytes_read < sizeof(char)){
            if(current = read(fd1, buffer, (sizeof(char) - bytes_read)) == -1){
                return -1;
            }
            bytes_read += current;
        }
        op_code = atoi(buffer);
        memset(buffer, "\0", sizeof(char));

        switch(op_code){
            case(TFS_OP_CODE_MOUNT):
                if(tfs_mount(fd1, client_pipe) ==-1){
                    return -1;
                }
                break;

            case(TFS_OP_CODE_UNMOUNT):
                if(tfs_unmount(fd1, client_pipe) == -1){
                    return -1;
                }
                break;

            case(TFS_OP_CODE_OPEN):
                break;
            
            case(TFS_OP_CODE_CLOSE):
                break;

            case(TFS_OP_CODE_WRITE):
                break;
            
            case(TFS_OP_CODE_READ):
                break;

            case(TFS_OP_CODE_SHUTDOWN_AFTER_ALL_CLOSED):
                break;
            default :
                return -1;
        }
    }
    return 0;
}

int tfs_mount(int fds, char *client_pipe){
    size_t bytes_read=0;
    size_t current;
    int fdc;
    char buffer[MAX_PIPENAME_SIZE];
    while(bytes_read < MAX_PIPENAME_SIZE){
        if(current = read(fds, buffer, ((sizeof(char)*MAX_PIPENAME_SIZE) - bytes_read)) == -1){
            return -1;
        }
        bytes_read += current;
    }

    memcpy(client_pipe,buffer,MAX_PIPENAME_SIZE);
    
    if(fdc = open(client_pipe, O_WRONLY) < 0){
        return -1;
    }
    int valid_session = 0;
    for(int i=0; i<MAX_CLIENTS; i++){
        if(freesession_entries[i]==FREE){
            freesession_entries[i]=TAKEN;
            if(current = write(fdc, i, sizeof(int)) == -1){
                close(fdc);
                return -1;
            }
            valid_session=1;
            break;
        }
    }
    if(valid_session == 0){
        if(current = write(fdc, -1, sizeof(int)) == -1){
            close(fdc);
            return -1;
        }
    }
    close(fdc);
    return 0;
}
    
int tfs_unmount(int fds, char *client_pipe){
    return 0;
}