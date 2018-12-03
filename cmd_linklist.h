#ifndef _CMD_LINKLIST_H_
#define _CMD_LINKLIST_H_

#ifndef STRUCT_CMD_INFO
#define STRUCT_CMD_INFO
#define BACKGROUND 			1
#define IN_REDIRECT 		2
#define OUT_REDIRECT        4
#define OUT_REDIRECT_APPEND	8
struct cmd_info
{
    int flag;
	int in_fd;
	int out_fd;
    char** arguments;
    char* cmd;
};
typedef struct link_cmd_node{
	struct cmd_info info_data;
	struct link_cmd_node *next;
} link_cmd_node;
#endif

link_cmd_node* cmd_linklist_create(link_cmd_node *, char**, int*);
void cmd_info_init(link_cmd_node *);
void cmd_linklist_print(link_cmd_node *);
int find_specific_cmd(char*, link_cmd_node *);
int remove_process_by_id(int);

#endif