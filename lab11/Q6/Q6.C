Q6. [9 marks] 
Consider a program similar to assignment 3 where a process will fork child
processes to carry out parts of the work.  In this case, each time a child 
completes a chunk of work, it will write one integer indicating its result to 
a pipe connected to the parent process. The parent will add this result to
"sum". It is not known how many times each child will write to the parent.

Your task is to implement the part of the program where the parent process
reads all the results from the pipes connected to the child processes.
Use select in the loop below so that the parent process will not block on
any read call. 
If the child closes their end of the pipe, the parent should handle that 
case appropriately.  (Error checking is not required.)

The function "is_empty(fd_set *fds)" will return true if the set is empty.  
You do not need to write this function.  

int num_procs;  // the number of child processes
fds[num_procs][2];  // assume these descriptors are open correctly
fd_set allfds;
FD_ZERO(&allfds);
int maxfd = 0;
int sum = 0;

// TODO initialization if needed
for(int i = 0; i < num_procs; i++){
    FD_SET(fds[i][1], &allfds);
    if(FD_SET(fds[i][1] > max_fd){
        max_fd = fds[i][1];
    }
}
// Read from the pipes connected to the child processes
while(!is_empty(&allfds)) {
    // TODO 
    fd_set rfds = allfds;
    select(max_fd + 1, &rfds, NULL, NULL, NULL);
    for(int i = 0; i < num_procs; i ++){
        if(FD_ISSET(fds[i][1], &rfds)){
            char buf[128];
            if(read(fds[i][1], buf, BUF_SIZE) == -1){
                FD_CLR(fds[i][1], &rfds);
            } else {
                int number = buf - '0';
                sum += number;
            }
        }
    }
}

printf("Sum is %d\n", sum);