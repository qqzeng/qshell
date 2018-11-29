#ifndef _BUILTIN_COMMAND_H_
#define _BUILTIN_COMMAND_H_

int builtin_cmd_process(link_cmd_node *);
int check_builtin_cmd(char* , link_cmd_node *);

typedef int (*cmd_action_handler)(char*, link_cmd_node*);

typedef struct builtin_cmd {
	char *cmd_name;
	cmd_action_handler handler;

} builtin_cmd;

int do_exit(char*, link_cmd_node*);
int do_cd(char*, link_cmd_node*);
int do_jobs(char*, link_cmd_node*);
int do_kill(char*, link_cmd_node*);


#endif