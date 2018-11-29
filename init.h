#ifndef INIT_H
#define INIT_H

#include "apue.h"

#define MAX_INT_VALUE ((1 << sizeof(int) * 8 - 1) - 1)
#define PATH_MAX_VALUE 4096
// limit.h
// #define NGROUPS_MAX 65536    
// #define ARG_MAX 131072    
// #define CHILD_MAX 999    
// #define OPEN_MAX 256    
// #define LINK_MAX 127    
// #define MAX_CANON 255    
// #define MAX_INPUT 255    
// #define NAME_MAX 255        //文件名最大长度255个字符  
// #define PATH_MAX 4096       //路径最大长度4096个字符  
// #define PIPE_BUF 4096    
// #define XATTR_NAME_MAX 255    
// #define XATTR_SIZE_MAX 65536    
// #define XATTR_LIST_MAX 65536
#ifndef PROCESS_NODE_INFO
#define PROCESS_NODE_INFO

typedef struct process_node
{
   pid_t npid;
   char *cmd;
   char **arg;
   struct process_node *next;
}process_node;
#endif

process_node* process_head_node;

void init();
void other_init();
void init_bgp_process_linklist();
void singal_init();
void sigint_handler(int sig);

char* PID_VALUE_PATH;
int PID_MAX_VALUE;

#endif