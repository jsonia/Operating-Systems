#include <iostream>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
using namespace std;
#define MSGSIZE 15
int main(int argc, char** argv)
{
    cout << "You have entered " << argc
         << " arguments:" << "\n";
  
    for (int i = 0; i < argc; ++i)
        cout << argv[i] << "\n";
    
    
    char inbuf[MSGSIZE];
        int p[2], pid, nbytes;
      
        if (pipe(p) < 0)
            exit(1);
      
        /* continued */
        if ((pid = fork()) > 0) {
            
            for(int i = 1; i<argc; i++){
                write(p[1], argv[i],  strlen(argv[i]) + 1);
            }
            
           
            // Adding this line will
            // not hang the program
            close(p[1]);
            wait(NULL);
        }
        else {
                // Adding this line will
                // not hang the program
                close(p[1]);
                while ((nbytes = read(p[0], inbuf, MSGSIZE)) > 0)
                    printf("% s\n", inbuf);
                if (nbytes != 0)
                    exit(2);
                printf("Finished reading\n");
            }
  
    return 0;
}
