#include "qshell.h"
#include "init.h"
#include "cmd_linklist.h"
#include "bg_process_handle.h"

/* remove a process node by pid */
int remove_process_by_id(int pid)
{
    process_node *p = process_head_node->next;
    process_node *pre = process_head_node;
    if (NULL == p) {printf("[qshell:info] empty bg linklist!\n");return 0;}
    while(p != NULL)
    {
        if (p->npid == pid) {
            pre->next = p->next;
            free(p);
            return 1;
        }
        pre = p;
        p = p->next;
    }
    return 0;
}

/* add a process node */
void add_process(int pid, struct cmd_info *command) 
{
    process_node *p=(process_node*)malloc(sizeof(process_node));
    p->npid = pid;
    p->cmd = command->cmd;
    p->arg = command->arguments;
    p->next = process_head_node->next;
    process_head_node->next=p;
}