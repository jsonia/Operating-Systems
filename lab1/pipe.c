#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	if(argc<=1){
		exit(EINVAL);
	}
	
	int idx = 1;
	int fds[2];
	while(idx < argc){
		int tmp;
		if(idx > 1){
			tmp = fds[0];
		}
		if(pipe(fds) < 0){
			fprintf(stderr,"error while creating pipe. Error: %s\n",strerror(errno));
			exit(errno);
		}
		int rc = fork();
		if(rc < 0){
			fprintf(stderr, "fork failed. Error: %s\n",strerror(errno));
			exit(errno);
		}else if(rc == 0){
			close(fds[0]);
			if(argc > 2){
				if(idx == 1){
					dup2(fds[1],1);
					close(fds[1]);
				}else{
					dup2(tmp,0);
					close(tmp);
					if(idx < argc-1){
						dup2(fds[1],1);
					}
					close(fds[1]);
				}
			}

			int res = execlp(argv[idx],argv[idx],(char *)NULL);
			if(res == -1){
				exit(errno);
			}

		}else{
			close(fds[1]);
			if(idx > 1){
				close(tmp);
			}
			if(idx == argc-1){
				close(fds[0]);
			}
			int status;
			int rc_wait = wait(&status);
			if(WIFEXITED(status)){
				if(WEXITSTATUS(status)){
					fprintf(stderr,"Failed to run \"%s\", Error: %s\n",argv[idx],strerror(WEXITSTATUS(status)));
					close(fds[0]);
					break;
				}
			}
		}
		idx++;
	}
	return 0;
}

