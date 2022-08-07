#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    printf("hello world (pid:%d)\n", (int) getpid());
    int rc = fork();
    int rc2 = fork();

    printf("I am %d, with rc: %d\n", (int) getpid(), rc);
    printf("I am %d, with rc2: %d\n", (int) getpid(), rc2);

    if(rc<0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if(rc==0){
        printf("hello, i am child (pid:%d)\n", (int) getpid());
    }
    else{
        printf("hello, I am parent of %d (pid:%d)\n", rc, (int) getpid());
    }

    return 0;
}