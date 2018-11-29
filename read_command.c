/**
@author: qqzeng
@email: qtozeng@gmail.com
@date: 17-11-13
@desc: 模拟接收 qshell 命令的输入
**/
#include "qshell.h"
// #define DEBUG

int read_command(char **command, char **parameters)
{

    char *buffer = malloc(sizeof(char*)*(MAX_COMMAND_LEN));
    buffer = fgets(buffer, MAX_COMMAND_LEN, stdin);
    // error input
    if (NULL == buffer)
    {
        exit(0);
    }
    // no input
    if (buffer[0] == '\0')
    {
        return -1;
    }
#ifdef DEBUG
    printf("[qshell:info:read_command] buffer:%s", buffer);
#endif

    char *ptr_start, *ptr_end;
    int count = 0;
    int over = 0;
    ptr_start = ptr_end = buffer;
    while (!over)
    {
        // ignore leading spaces or tab
        while ((*ptr_end == ' ' && *ptr_start == ' ') || 
            (*ptr_end == '\t' && *ptr_start == '\t')){
            ptr_start++;
            ptr_end++;
        }

        // end of a command
        if (*ptr_end == '\0' || *ptr_end == '\n'){
            // no input
            if (count == 0)
                return -1;
            break;
        }

        while (*ptr_end != ' ' && *ptr_end != '\0' && *ptr_end != '\n')
            ptr_end++;

        // extract the first command, may be some options behind left.
        if (0 == count){
            char *p = ptr_end;
            command[0] = ptr_start;
            int i = 0;
            while (p != ptr_start && *p != '/') {
                p--;
            }
            if (*p == '/')
                p++;
            //else //p==ptr_start
            parameters[0] = p;
            // count += 2;
            count++;
#ifdef DEBUG
            printf("[qshell:info:read_command] command:%s", command[0]);
#endif
        } else if (count <= MAXARG) {
            // extract next one
            parameters[count] = ptr_start;
            count++;
        } else {
#ifdef DEBUG
            printf("[qshell:info] count=%d\n", count);
#endif
            break;
        }
        // end
        if (*ptr_end == '\0' || *ptr_end == '\n') {
            *ptr_end = '\0';
            over = 1;
        } else {
            *ptr_end = '\0';
            ptr_end++;
            ptr_start = ptr_end;
        }
    }
    parameters[count] = NULL;
#ifdef DEBUG
    printf("[qshell:info:read_command] count:%d\tpathname:[%s]\n",count, *command);
    int i;
    for(i = 0; i< count; i++)
        printf("[qshell:info:read_command] parameters[%d]:%s\n", i, parameters[i]);
#endif
    return count;
}


// int main()
// {
//     char **parameters = malloc(sizeof(char*)*(MAXARG + 2));
//     char **command = malloc(sizeof(char*)*(MAXARG));
//     while(1){
//         int param_count = read_command(command, parameters);
//         printf("%d\n", param_count);
//         printf("%s\n", command[0]);
//         for (int i = 0; i < param_count; i++){
//             printf("%s\t", parameters[i]);
//         }
//         printf("\n");
//         break;
//     }
//     return 0;
// }

