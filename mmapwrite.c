#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
    struct stat fileInfo;
    char * filepath = "link.txt";
    char * update = "hello mmap!";

    int fd = open(filepath, O_RDWR, (mode_t)0600);

    if( fd == -1)
    {
        perror("Error opening file for writing!");
        exit(EXIT_FAILURE);
    }

    fstat(fd, &fileInfo);

    printf("%ld\n", fileInfo.st_size);
    char *map = mmap(0, fileInfo.st_size, PROT_READ| PROT_WRITE , MAP_SHARED, fd, 0);
    if(map == MAP_FAILED)
    {
        close(fd);
        perror("Error mapping the file");
        exit(EXIT_FAILURE);
    }

    printf("%ld", strlen(update));
    for(size_t i = 0; i< strlen(update); i++)
    {
        printf("Writing character %c at %zu\n", update[i], i);
        map[i] = update[i];
    }
    if(msync(map, fileInfo.st_size, MS_SYNC)== -1)
    {
        perror("Cound not sync with file to disk");
    }

    if(munmap(map, fileInfo.st_size) == -1)
    {
        close(fd);
        perror("Error un-mapping the file");
        exit(EXIT_FAILURE);
    }
    close(fd);
    return 0;
}