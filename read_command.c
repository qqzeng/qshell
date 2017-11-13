/**
@author: qqzeng
@email: qtozeng@gmail.com
@date: 17-11-13
@desc: 模拟接收 qshell 命令的输入
**/

#include <stdlib.h>
#include <stdio.h>

#define MAX_COMMAND_LEN 1024
#define MAXARG 20


int read_command(char **command, char **parameters)
{

    char *buffer = (char*)malloc(sizeof(char) * MAX_COMMAND_LEN);
    if (NULL == fgets(buffer, MAX_COMMAND_LEN, stdin)){
        // error input
        printf("\n");
        exit(0);
    }
    // no input
    if (buffer[0] == '\0'){
        return -1;
    }
    char *ptr_start, *ptr_end;
    int count = 0;
    int over = 0;
    ptr_start = ptr_end = buffer;
    while (!over)
    {
        // leading spaces or tab
        while ((*ptr_end == ' ' && *ptr_start == ' ') || (*ptr_end == '\t' && *ptr_start == '\t'))
        {
            ptr_start++;
            ptr_end++;
        }

        // end of a command
        if (*ptr_end == '\0' || *ptr_end == '\n')
        {
            // no input
            if (count == 0)
                return -1;
            break;
        }

        while (*ptr_end != ' ' && *ptr_end != '\0' && *ptr_end != '\n')
            ptr_end++;

        if (!count) {
            char *p = ptr_end;
            command = ptr_start;
            while (p != ptr_start && *p != '/')
                p--;
            if (*p == '/')
                p++;
            //else //p==ptr_start
            parameters[0] = p;
            count += 2;
// #ifdef DEBUG
            printf("\ncommand:%s\n",command);
// #endif
        } else if (count <= MAXARG) {
            parameters[count - 1] = ptr_start;
            count++;
        } else {
            break;
        }

        if (*ptr_end == '\0' || *ptr_end == '\n') {
            *ptr_end = '\0';
            over = 1;
        } else {
            *ptr_end = '\0';
            ptr_end++;
            ptr_start = ptr_end;
        }
    }
    parameters[count - 1] = NULL;
    printf("input analysis:\n");
    printf("pathname:[%s]\ncommand:[%s]\nparameters:\n",*command,parameters[0]);
    int i;
    for(i=0;i<count-1;i++)
        printf("[%s]\n",parameters[i]);
    return count;
}


int main()
{
    char ** parameters = malloc(sizeof(char *)*(MAXARG+2));
    char *command = NULL;
    while(1){
        int param_count = read_command(command,parameters);
        printf("%d\n", param_count);
        for (int i = 0; i < param_count - 1; i++){
            printf("%s %s\n", command[i], parameters[i]);
        }
    }
    return 0;
}

