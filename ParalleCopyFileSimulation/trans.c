#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <errno.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

void display(char *prog, char *bytes, int n);

int main(int argc, char *argv[])
{

    int SequencePipe[2];
    int BytePipe[2];
    int ConfirmationPipe[2];
    char write_msg[BUFFER_SIZE];
    char read_msg[BUFFER_SIZE];

    if (pipe(SequencePipe) == -1)
    {
        fprintf(stderr, "Pipe failed");
        return 1;
    }
    if (pipe(BytePipe) == -1)
    {
        fprintf(stderr, "Pipe failed");
        return 1;
    }
    if (pipe(ConfirmationPipe) == -1)
    {
        fprintf(stderr, "Pipe failed");
        return 1;
    }

    int cpid;
    cpid = fork();
    if (cpid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0)
    { /* parent reads through pipe */

        close(SequencePipe[WRITE_END]);
        close(BytePipe[WRITE_END]);
        close(ConfirmationPipe[READ_END]);

        const char *name = "/hg19b"; // file name
        const int SIZE = 4096;       // file size

        int shm_fd;     // file descriptor, from shm_open()
        char *shm_base; // base address, from mmap()

        /* open the shared memory segment as if it was a file */
        shm_fd = shm_open(name, O_RDONLY, 0666);
        if (shm_fd == -1)
        {
            printf("cons: Shared memory failed: %s\n", strerror(errno));
            exit(1);
        }

        /* map the shared memory segment to the address space of the process */
        shm_base = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
        if (shm_base == MAP_FAILED)
        {
            printf("cons: Map failed: %s\n", strerror(errno));
            // close and unlink?
            exit(1);
        }

        printf("In parent \n");


        int sequenceNum, numBytes;
        do {
            read(SequencePipe[READ_END], &sequenceNum, sizeof(sequenceNum));
            write(ConfirmationPipe[WRITE_END], &sequenceNum, sizeof(sequenceNum));
            printf("just read the block numer: %d \n", sequenceNum);
            if (sequenceNum != 0) {
                
                read(SequencePipe[READ_END], &sequenceNum, sizeof(sequenceNum));
                write(ConfirmationPipe[WRITE_END], &sequenceNum, sizeof(sequenceNum));
                printf("just read the block numer: %d \n", sequenceNum);

            }
        } while (sequenceNum != 0);
        exit(1);
            
            
            //printf("here %s \n\n\n", (char *)shm_base);

            /*
            while (strcmp(read_msg, "0") != 0){

                char prev[BUFFER_SIZE];
                strncpy(prev, read_msg, BUFFER_SIZE);

                read(SequencePipe[READ_END], read_msg, BUFFER_SIZE);
                if (strcmp(read_msg, prev) != 0)
                    {
                        int seq = atoi(read_msg);
                        printf("here %s \n\n\n", (char *)shm_base);
                        sprintf(write_msg, "%d", seq);
                        write(ConfirmationPipe[WRITE_END], write_msg, strlen(write_msg)+1);
                    }
                

            }
            */

        /* read from the mapped shared memory segment */
        // display("cons", shm_base, 64);	// first as bytes, then as a string
        // printf("%s", shm_base);

        /* remove the mapped shared memory segment from the address space of the process */
        if (munmap(shm_base, SIZE) == -1)
        {
            printf("cons: Unmap failed: %s\n", strerror(errno));
            exit(1);
        }

        /* close the shared memory segment as if it was a file */
        if (close(shm_fd) == -1)
        {
            printf("cons: Close failed: %s\n", strerror(errno));
            exit(1);
        }

        /* remove the shared memory segment from the file system */
        if (shm_unlink(name) == -1)
        {
            printf("cons: Error removing %s: %s\n", name, strerror(errno));
            exit(1);
        }
    }
    else
    { /* child sends through pipe */

        FILE *fp;
        fp = fopen(argv[1], "r");
        if (NULL == fp)
        {
            printf("file open failed");
            exit(1);
        }
       
        unsigned char buffer[4000]; // array of bytes, not pointers-to-bytes
        size_t bytesRead = 0;

        close(SequencePipe[READ_END]);
        close(BytePipe[READ_END]);
        close(ConfirmationPipe[WRITE_END]);


        const char *name = "/hg19b"; // file name
        const int SIZE = 4096;       // file size

        int shm_fd;     // file descriptor, from shm_open()
        char *shm_base; // base address, from mmap()
        char *ptr;      // shm_base is fixed, ptr is movable

        /* create the shared memory segment as if it was a file */
        shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
        if (shm_fd == -1)
        {
            printf("prod: Shared memory failed: %s\n", strerror(errno));
            exit(1);
        }

        /* configure the size of the shared memory segment */
        ftruncate(shm_fd, SIZE);

        /* map the shared memory segment to the address space of the process */
        shm_base = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

        if (shm_base == MAP_FAILED)
        {
            printf("prod: Map failed: %s\n", strerror(errno));
            // close and shm_unlink?
            exit(1);
        }

        

        int block = 1;


        while ((bytesRead = fread(buffer, 1, sizeof(buffer), fp)) > 0)
        {
            int seqNum;

            //ptr = shm_base;
            //ptr += sprintf(ptr, "%s", buffer);
            //memset(buffer, 0, 4000);

            printf("block number: %d \n", block);
            write(SequencePipe[WRITE_END], &block, sizeof(block));
            printf("just wrote the block number \n");

            do 
            {
                read(ConfirmationPipe[READ_END], &seqNum, sizeof(seqNum));
                printf("read confirmation pipe \n");

            } while(seqNum != 0);

            block++;

            // read(ConfirmationPipe[READ_END], read_msg, BUFFER_SIZE);
            // while (strcmp(read_msg, blockNum) != 0){
            //     read(ConfirmationPipe[READ_END], read_msg, BUFFER_SIZE);
            // }
        }

        block = 0;
        printf("block number: %d \n", block);
        write(SequencePipe[WRITE_END], &block, sizeof(block));
   

        /* remove the mapped memory segment from the address space of the process */
        if (munmap(shm_base, SIZE) == -1)
        {
            printf("prod: Unmap failed: %s\n", strerror(errno));
            exit(1);
        }

        /* close the shared memory segment as if it was a file */
        if (close(shm_fd) == -1)
        {
            printf("prod: Close failed: %s\n", strerror(errno));
            exit(1);
        }
    }

    return 0;
}

void display(char *prog, char *bytes, int n)
{
    printf("display: %s\n", prog);
    for (int i = 0; i < n; i++)
    {
        printf("%02x%c", bytes[i], ((i + 1) % 16) ? ' ' : '\n');
    }
    printf("\n");
}
