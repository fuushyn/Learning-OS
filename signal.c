#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    printf("hello world (pid:%d)\n", (int) getpid());
    int x = 5;

    int rc = fork();

    x++;
    printf("%d\n", x);


    return 0;
}