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

    const char *name = "/hg19b"; // file name
    const int SIZE = 4096;       // file size

    int shm_fd;     // file descriptor, from shm_open()
    char *shm_base; // base address, from mmap()

    /* open the shared memory segment as if it was a file */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        printf("cons: Shared memory failed: %s\n", strerror(errno));
        exit(1);
    }

    ftruncate(shm_fd, 4096);

    /* map the shared memory segment to the address space of the process */
    shm_base = mmap(0, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_base == MAP_FAILED)
    {
        printf("cons: Map failed: %s\n", strerror(errno));
        // close and unlink?
        exit(1);
    }

    if (cpid == 0)
    { /* parent reads through pipe */

        close(SequencePipe[WRITE_END]);
        close(BytePipe[WRITE_END]);
        close(ConfirmationPipe[READ_END]);
        FILE *outFP = fopen(argv[2], "w");

        int prevBlock = -1;
        int sequenceNum, numBytes;
        do {
            read(SequencePipe[READ_END], &sequenceNum, sizeof(sequenceNum));
            read(BytePipe[READ_END], &numBytes, sizeof(numBytes));

            printf("            Just read the block number %d Previous: %d \n",sequenceNum, prevBlock );
           
            if (prevBlock != sequenceNum && sequenceNum != 0)
            {
                prevBlock = sequenceNum;
                printf("            in here for the print, number of bytes: %d \n\n\n", numBytes);
                printf("             %s \n\n\n", (char *)shm_base);
                
                
                fwrite((char *)shm_base, 1, numBytes, outFP);
                
                
                write(ConfirmationPipe[WRITE_END], &sequenceNum, sizeof(sequenceNum));
            }
            else if (prevBlock == sequenceNum)
            {
                write(ConfirmationPipe[WRITE_END], &sequenceNum, sizeof(sequenceNum));
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


        for(int i =0; i < 4000; i++)
        {
            buffer[i] = '\0';
        }
        size_t bytesRead = 0;

        close(SequencePipe[READ_END]);
        close(BytePipe[READ_END]);
        close(ConfirmationPipe[WRITE_END]);

        printf("In child  \n");

        char *ptr;      // shm_base is fixed, ptr is movable


        int block = 1;
        int byteSize = 4000;

        while ((bytesRead = fread(buffer, 1, sizeof(buffer), fp)) > 0)
        {
            int seqNum;

            ptr = shm_base;
            ptr += sprintf(ptr, "%s", buffer);
            
            for(int i =0; i < 4000; i++)
            {
                if (buffer[i] != '\0')
                {
                    byteSize = i;
                }
                buffer[i] = '\0';
            }
            
            printf("block number: %d \n", block);
            byteSize++;

            write(SequencePipe[WRITE_END], &block, sizeof(block));
            write(BytePipe[WRITE_END], &byteSize, sizeof(byteSize));

            printf("just wrote the block number \n");

            do 
            {
                read(ConfirmationPipe[READ_END], &seqNum, sizeof(seqNum));
                printf("read confirmation pipe \n");

            } while(seqNum != block);

            block++;

        }

        block = 0;
        printf("block number: 0 \n");
        write(SequencePipe[WRITE_END], &block, sizeof(block));

    }




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
