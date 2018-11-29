/**
@author: qqzeng
@email: qtozeng@gmail.com
@date: 17-12-10
@desc: 解析命令
**/
#include "qshell.h"
#include "param_parse.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define DEBUG

/**
cat < pipe_command_test.c | grep a
wc test.c > tmp.txt 2>&1
ls -ll | grep a | wc | wc -w >> tmp &
**/

link_cmd_node* cmd_parse(char **parameters, link_cmd_node* cmd_linklist, int *ncom)
{
	*ncom = 0;
	int tmp_ncom = 0;
	cmd_linklist = cmd_linklist_create(cmd_linklist, parameters, &tmp_ncom);
	*ncom = tmp_ncom;
#ifdef DEBUG
	cmd_linklist_print(cmd_linklist);
	printf("[qshell:info:cmd_parse] ncom=%d\n", *ncom);
#endif
	return cmd_linklist;
}

int in_redirect_info_parse(struct cmd_info *cmd)
{
	if (NULL == cmd) return 0;
	char** arg_p = cmd->arguments;
	while (NULL != arg_p) {
		// in_file is on the right of the "<" operator.
		if (0 == strcmp(*arg_p, "<")) {
			int fd_in_tmp = open(*(arg_p + 1), O_CREAT |O_RDONLY, 0666);
			if (fd_in_tmp < 0) {
				printf("[qshell:error:param_parse]open in file error for cmd: %s\n", cmd->cmd);
				return 0;
			}
			cmd->in_fd = fd_in_tmp;
			// remove the redundant argument info. 
			// i.e. cat < qshell.c | grep a ==> cat | grep a
			if (!((cmd->flag & OUT_REDIRECT) || (cmd->flag & OUT_REDIRECT_APPEND)))
				*arg_p = NULL;
			// while (NULL != arg_p) {
			// 	arg_p++;
			// }
            return 1;
		}
		arg_p++;
	}
	return 0;
}

int out_redirect_info_parse(struct cmd_info *cmd)
{
	if (NULL == cmd) return 0;
	char** arg_p = cmd->arguments;
	while (NULL != arg_p) {
		// out_file is on the right of the ">>" or ">" operator.
		if (0 == strcmp(*arg_p, ">")) {
		#ifdef DEBUG
			printf("-----out_redirect_info_parse----\n");
		#endif
			int fd_out_tmp = open(*(arg_p + 1), O_WRONLY|O_CREAT|O_TRUNC, 0666);
			if (fd_out_tmp < 0) {
				printf("[qshell:error:param_parse]open in file error for cmd: %s\n", cmd->cmd);
				return 0;
			}
			cmd->out_fd = fd_out_tmp;
			// handle cmd like "cat < a.txt >>|> b.txt"
			if (cmd->flag & IN_REDIRECT) {
				arg_p = cmd->arguments;
				arg_p++;
				*arg_p = NULL;
			} else {
				*arg_p = NULL;
			}
            return 1;
		} else if (0 == strcmp(*arg_p, ">>")) {
		#ifdef DEBUG
			printf("-----out_append_redirect_info_parse----\n");
		#endif
			int fd_out_append_tmp = open(*(arg_p + 1),  O_WRONLY | O_CREAT | O_APPEND, 0666);
			if (fd_out_append_tmp < 0) {
				printf("[qshell:error:param_parse]open in file error for cmd: %s\n", cmd->cmd);
				return 0;
			}
			cmd->out_fd = fd_out_append_tmp;
			// handle cmd like "cat < a.txt >>|> b.txt"
			if (cmd->flag & IN_REDIRECT) {
				arg_p = cmd->arguments;
				arg_p++;
				*arg_p = NULL;
			} else {
				*arg_p = NULL;
			}
			return 1;
		}
		arg_p++;
	}
	return 0;
}