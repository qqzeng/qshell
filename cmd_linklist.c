#include "qshell.h"
#include "cmd_linklist.h"
// #define DEBUG

void cmd_info_init(link_cmd_node *cmd_node)
{
    cmd_node->info_data.flag = 0;
    cmd_node->info_data.in_fd = STDIN_FILENO;
    cmd_node->info_data.out_fd = STDOUT_FILENO;
    cmd_node->info_data.cmd = NULL;
    cmd_node->info_data.arguments = NULL;
}

// create command linklist
// the head node contains no info. i.e. isn't data node.
link_cmd_node* cmd_linklist_create(link_cmd_node *L, char** parameters, int *ncommand)
{
	if (parameters == NULL){
		return NULL;
	}
	char** pdata = parameters;
	link_cmd_node *p, *q;
	L = (link_cmd_node *)malloc(sizeof(link_cmd_node));
	L->next = NULL;
	q = p = (link_cmd_node *)malloc(sizeof(link_cmd_node));
	cmd_info_init(p);
	int i = 0;
	while ((*pdata) != NULL) {
		char** arg = malloc(sizeof(char*)*MAXARG);
		int j = 0;
		while((*pdata) != NULL && 0 != strcmp((*pdata), "|")) {
			char* str_tmp = malloc(sizeof(char)*MAX_SINGLE_CMD);
			strcpy(str_tmp, *pdata);
			arg[j] = str_tmp;
			if (0 == strcmp(*pdata, "<")) {
				p->info_data.flag |= IN_REDIRECT;
			}
			//TODO valid command.(cat tmp > res | wc -l)(cat tmp | ls -l)
			if (0 == strcmp(*pdata, ">")) {
				p->info_data.flag |= OUT_REDIRECT;
			}
			if (0 == strcmp(*pdata, ">>")) {
				p->info_data.flag |= OUT_REDIRECT_APPEND;
			}
			//except command like (ls -l >> tmp 2>&1)
			if (0 == strcmp(*pdata, "&")) {
				p->info_data.flag |= BACKGROUND;
			}
			if (0 == j) p->info_data.cmd = (*pdata);
#ifdef DEBUG
	printf("[qshell:info:cmd_linklist] %d\t%s\n", j, arg[j]);
#endif
			pdata++;
			j++;
		}
		arg[j] = NULL;
		p->info_data.arguments = arg;
		if (0 == i) {
			p->next = L->next;
			L->next = p;
		} else {
			p->next = q->next;
			q->next = p;
			q = p;
		}
		p = (link_cmd_node *)malloc(sizeof(link_cmd_node));
		cmd_info_init(p);
		if ((*pdata) != NULL) pdata++;
		i++;
	}
	// free last unused memory
	free(p);
	*ncommand = i;
#ifdef DEBUG
	printf("%d\t%s\n", L->next->info_data.flag, L->next->info_data.cmd);
#endif
	return L;
}

/* print the cmd linklist */
void cmd_linklist_print(link_cmd_node *L){
	if (NULL == L || NULL == L->next) {
		perror("[qshell:info] empty command linklist.");
		return ;
	}
	link_cmd_node *p;
	p = L -> next;
	while (p){
		printf("BACKGROUND: %d\tIN_REDIRECT: %d\tOUT_REDIRECT: %d\tOUT_REDIRECT_APPEND: %d\n", 
			p->info_data.flag & BACKGROUND, p->info_data.flag & IN_REDIRECT, p->info_data.flag & OUT_REDIRECT, 
			p->info_data.flag & OUT_REDIRECT_APPEND);
		printf("in_fd: %d\tout_fd: %d\n", p->info_data.in_fd, p->info_data.out_fd);
		printf("command: %s\t", p->info_data.cmd);
		char** ptr = p->info_data.arguments;
		printf("arguments: ");
		while (*ptr != NULL) {
			printf("%s ", *ptr++);
		}
		printf("\n");
		p = p->next;
	}
}

/* check whether specific cmd exits */
int find_specific_cmd(char* cmd_name, link_cmd_node *L)
{
	if (NULL == L || NULL == L->next) {
		perror("[qshell:info] empty command linklist.");
		return 0;
	}
	link_cmd_node *p;
	p = L -> next;
	while (p){
#ifdef DEBUG
		printf("command: %s\t", p->info_data.cmd);
		printf("arguments: ");
#endif
		char** ptr = p->info_data.arguments;
		if (0 == strcmp(p->info_data.cmd, cmd_name))
			return 1;
		p = p->next;
	}
	return 0;
}