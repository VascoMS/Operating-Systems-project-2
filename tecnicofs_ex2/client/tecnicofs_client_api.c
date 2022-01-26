#include "tecnicofs_client_api.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char client_pipe_name[MAX_PIPENAME_SIZE];
int session_id, client_pipe_fd, server_pipe_fd;


int request_handler(char const *client_pipe_path, char const *server_pipe_path, char *write_buffer, int *read_buffer, int to_read, size_t to_write){
    size_t bytes_written, bytes_read;

    if(server_pipe_fd = open(server_pipe_path, O_WRONLY) == -1)
        return -1;

    while(bytes_written < to_write){
        size_t written;
        if(written = write(server_pipe_fd, write_buffer + bytes_written, to_write - bytes_written) == -1)
            return -1;
        bytes_written += written;
    }
    if(close(server_pipe_fd) == -1){
        return -1;
    }

    if(client_pipe_fd = open(client_pipe_path, O_RDONLY) == -1)
        return -1;

    while(bytes_read < to_read){
        size_t current_read;
        if(current_read = read(client_pipe_fd, read_buffer + bytes_read, to_read-bytes_read) == -1)
            return -1;
        bytes_read += current_read;
    }

    if(close(client_pipe_fd) == -1){
        return -1;
    }
    //perguntar sobre leitura de inteiro vindo do servidor
    return *read_buffer;
}

char *opcode_convert(int op_code_num){
    char op_code_char = (char)(op_code_num + '0'), opcode_message[OPCODE_MESSAGE_SIZE];
    opcode_message[0] = op_code_char;
    opcode_message[1] = '\0';
    return opcode_message;
}

int tfs_mount(char const *client_pipe_path, char const *server_pipe_path) {
    size_t bytes_written = 0, bytes_read = 0;
    int message_size = MAX_PIPENAME_SIZE + OPCODE_MESSAGE_SIZE, read_buffer; 
    char *op_code = opcode_convert(TFS_OP_CODE_MOUNT), write_buffer[message_size];
    memset(write_buffer, 0, message_size);
    strcpy(write_buffer, op_code);
    strcpy(write_buffer + OPCODE_MESSAGE_SIZE, client_pipe_path);
    //TO D0: FORMATAR BEM A MENSAGEM DO WRITE BUFFER. 
    unlink(client_pipe_path);
    if((server_pipe_fd = open(server_pipe_path, O_WRONLY)) == -1)
        return -1;

    if(mkfifo(client_pipe_path, 0777) < 0)
        return -1;

    strcpy(client_pipe_name, client_pipe_path);
    
    if(session_id = request_handler(client_pipe_path, server_pipe_path, write_buffer, &read_buffer, sizeof(int), message_size) == -1){
        return -1;
    }
    
/*
    if(client_pipe_fd = open(client_pipe_path, O_RDONLY) == -1)
        return -1;

    while(bytes_written < message_size){
        size_t written;
        if(written = write(server_pipe_fd, write_buffer + bytes_written, message_size - bytes_written) == -1)
            return -1;
        bytes_written += written;
    }

    if(close(server_pipe_fd) == -1){
        return -1;
    }

    while(bytes_read < sizeof(int)){
        size_t current_read;
        if(current_read = read(client_pipe_fd, read_buffer + bytes_read, sizeof(int)-bytes_read) == -1)
            return -1;
        bytes_read += current_read;
    }

    if(close(client_pipe_fd) == -1){
        return -1;
    }

    if(session_id = atoi(read_buffer) == -1){
        return -1;
    }
*/

    return 0;
}

int tfs_unmount() {
    /* TODO: Implement this */
    return -1;
}

int tfs_open(char const *name, int flags) {
    /* TODO: Implement this */
    return -1;
}

int tfs_close(int fhandle) {
    /* TODO: Implement this */
    return -1;
}

ssize_t tfs_write(int fhandle, void const *buffer, size_t len) {
    /* TODO: Implement this */
    return -1;
}

ssize_t tfs_read(int fhandle, void *buffer, size_t len) {
    /* TODO: Implement this */
    return -1;
}

int tfs_shutdown_after_all_closed() {
    /* TODO: Implement this */
    return -1;
}
