void runPipedCommands(cmdLine* command, char* user_input) 
{
    int npipe = countPipes(user_input);
    int status;
    int i = 0;
    pid_t pid;
    int pipe_fds[2*npipe];
    int j = 0;

    for(i = 0; i < (npipe); i++){
        if(pipe(pipe_fds + i*2) < 0) {
            perror("pipe error");
            exit(EXIT_FAILURE);
        }
    }
    
    while(command) {
        pid = fork();
        if(pid == 0) {

            //if not last command, out
            if(command->next){
                if(dup2(pipe_fds[j + 1], 1) < 0){
                    perror("dup2 error");
                    exit(EXIT_FAILURE);
                }
            }

            //if not first command && j!= 2*npipe, in
            if(j != 0 ){
                if(dup2(pipe_fds[j - 2], 0) < 0){
                    perror("dup2 error");///j-2 0 j+1 1
                    exit(EXIT_FAILURE);
                }
            }

            for(i = 0; i < 2*npipe; i++){
                close(pipe_fds[i]);
            }

            if(execvp(*command->arguments, command->arguments) < 0){
                perror(*command->arguments);
                exit(EXIT_FAILURE);
            }

        } else if(pid < 0){
            perror("fork error");
            exit(EXIT_FAILURE);
        }
        command = command->next;
        j+=2;
    }

    /**Parent closes the pipes and wait for children*/
    for(i = 0; i < 2 * npipe; i++){
        close(pipe_fds[i]);
    }
    for(i = 0; i < npipe + 1; i++){
        wait(&status);
    }
}