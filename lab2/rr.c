#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

typedef uint32_t u32;
typedef int32_t i32;

struct process {
  u32 pid;
  u32 arrival_time;
  u32 burst_time;

  TAILQ_ENTRY(process) pointers;

  /* Additional fields here */
    u32 lastExecutionTime;
    bool isFirstexec;
    u32 response_time;
    u32 waiting_time;
    bool inQueue;
    bool isComplete;
    u32 burstTimeRemaining;
  /* End of "Additional fields here" */
};

TAILQ_HEAD(process_list, process);

u32 next_int(const char **data, const char *data_end) {
  u32 current = 0;
  bool started = false;
  while (*data != data_end) {
    char c = **data;

    if (c < 0x30 || c > 0x39) {
      if (started) {
	return current;
      }
    }
    else {
      if (!started) {
	current = (c - 0x30);
	started = true;
      }
      else {
	current *= 10;
	current += (c - 0x30);
      }
    }

    ++(*data);
  }

  printf("Reached end of file while looking for another integer\n");
  exit(EINVAL);
}

u32 next_int_from_c_str(const char *data) {
  char c;
  u32 i = 0;
  u32 current = 0;
  bool started = false;
  while ((c = data[i++])) {
    if (c < 0x30 || c > 0x39) {
      exit(EINVAL);
    }
    if (!started) {
      current = (c - 0x30);
      started = true;
    }
    else {
      current *= 10;
      current += (c - 0x30);
    }
  }
  return current;
}

void init_processes(const char *path,
                    struct process **process_data,
                    u32 *process_size)
{
  int fd = open(path, O_RDONLY);
  if (fd == -1) {
    int err = errno;
    perror("open");
    exit(err);
  }

  struct stat st;
  if (fstat(fd, &st) == -1) {
    int err = errno;
    perror("stat");
    exit(err);
  }

  u32 size = st.st_size;
  const char *data_start = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (data_start == MAP_FAILED) {
    int err = errno;
    perror("mmap");
    exit(err);
  }

  const char *data_end = data_start + size;
  const char *data = data_start;
  

  *process_size = next_int(&data, data_end);

  *process_data = calloc(sizeof(struct process), *process_size);
  if (*process_data == NULL) {
    int err = errno;
    perror("calloc");
    exit(err);
  }

  for (u32 i = 0; i < *process_size; ++i) {
    (*process_data)[i].pid = next_int(&data, data_end);
    (*process_data)[i].arrival_time = next_int(&data, data_end);
    (*process_data)[i].burst_time = next_int(&data, data_end);
      //initialization --
     // printf("Average waiting time: %.2u\n", (*process_data)[i].burst_time);
     // (*process_data)[i].burstTimeRemaining = (*process_data)[i].burst_time;
      
  }
  
  munmap((void *)data, size);
  close(fd);
}

int main(int argc, char *argv[])
{
  if (argc != 3) {
    return EINVAL;
  }
  struct process *data;
  u32 size;
  init_processes(argv[1], &data, &size);

  u32 quantum_length = next_int_from_c_str(argv[2]);

  struct process_list list;
  TAILQ_INIT(&list);
    u32 current_time = 0;
  
  u32 total_waiting_time = 0;
  u32 total_response_time = 0;

  /* Your code here */
    struct process *p;
      for (u32 i = 0; i < size; ++i) {
        p = &data[i];
      //  p->remaining_time = p->burst_time;
          p->lastExecutionTime = 0;
          p->isFirstexec = true;
          p->response_time = 0;
          p->waiting_time = 0;
          p->inQueue = false;
          p->isComplete = false;
          p->burstTimeRemaining = p->burst_time;
         // printf("Average waiting time: %.2f\n", p->burstTimeRemaining);
      }
   /* for (u32 i = 0; i < size; ++i) {
      p = &data[i];
        printf("Average waiting time: %.2u\n", p->lastExecutionTime);}*/
    
    
    u32 t = 0;
    for(u32 i = 0; i< size; i++){
        p = &data[i];
              if (p->arrival_time == t) {
        TAILQ_INSERT_TAIL(&list, p, pointers);
                  p->inQueue = true;
              }
    }
    
    
    
    u32 p_size = size;
    
    while((!TAILQ_EMPTY(&list) || p_size != 0)&&quantum_length != 0){
        
        //printf("%s\n","hello");
        if(TAILQ_EMPTY(&list)){
            t++;
            //update queue
            for(u32 i = 0; i< size; i++){
                p = &data[i];
                      if (p->arrival_time <= t && p->inQueue == false && p->isComplete ==false) {
                TAILQ_INSERT_TAIL(&list, p, pointers);
                          p->inQueue = true;
                      }
            }
            
        }
        
        else{
            p = TAILQ_FIRST(&list);
            TAILQ_REMOVE(&list, p, pointers);
            
            if(p->burstTimeRemaining >= quantum_length ){
                p->burstTimeRemaining = p->burstTimeRemaining - quantum_length;
                if(p->isFirstexec == true){
                   // printf("%s", "its first exec");
                    p->response_time = t - p->arrival_time;
                    p->waiting_time = t - p->arrival_time;
                    p->isFirstexec = false;
                }
                else{
                    p->waiting_time = p->waiting_time + (t - p->lastExecutionTime);
                }
                t = t + quantum_length;
                //update queue
                struct process *q;
                for(u32 i = 0; i< size; i++){
                    q = &data[i];
                          if (q->arrival_time <= t && q->inQueue == false && q->isComplete ==false) {
                    TAILQ_INSERT_TAIL(&list, q, pointers);
                              q->inQueue = true;
                          }
                }
                
                struct process *temp;
                TAILQ_FOREACH(temp, &list, pointers) {
                   // printf(" %d\n", temp->pid);
                }
                
                //if process complete mark 0
                if(p->burstTimeRemaining == 0){
                    p->isComplete = true;
                    p_size--;
                }
                //else add in queue again
                else {
                    p->lastExecutionTime = t;
                    TAILQ_INSERT_TAIL(&list, p, pointers);
                }
               // printf("%d", p->pid);
               // printf("%2u", p->response_time);
            }
            
            else if(p->burstTimeRemaining < quantum_length){
                if(p->isFirstexec == true){
                    p->response_time = t - p->arrival_time;
                    p->waiting_time = t - p->arrival_time;
                    p->isFirstexec = false;
                }
                
                else{
                    p->waiting_time = p->waiting_time + (t - p->lastExecutionTime);
                }
                
                t = t + (p->burstTimeRemaining);
                p->burstTimeRemaining = 0;
                //update queue
                struct process *q;
                for(u32 i = 0; i< size; i++){
                    q = &data[i];
                          if (q->arrival_time <= t && q->inQueue == false && q->isComplete ==false) {
                    TAILQ_INSERT_TAIL(&list, q, pointers);
                              q->inQueue = true;
                          }
                }
                //if process complete mark 0
                if(p->burstTimeRemaining == 0){
                    p->isComplete = true;
                    p_size--;
                }
                
                
            }
        }
        
    }
    
    for(u32 i = 0; i< size; i++){
        p = &data[i];
       // printf("Average res time: %.2u\n", p->response_time);
        total_waiting_time = total_waiting_time + p->waiting_time;
        total_response_time = total_response_time + p->response_time;
    }
    
    
  /* End of "Your code here" */

  printf("Average waiting time: %.2f\n", (float) total_waiting_time / (float) size);
  printf("Average response time: %.2f\n", (float) total_response_time / (float) size);

  free(data);
  return 0;
}
