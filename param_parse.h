#ifndef _PARAM_PARSE_H_
#define _PARAM_PARSE_H_

#include "cmd_linklist.h"


link_cmd_node* cmd_parse(char **, link_cmd_node*, int*);
int in_redirect_info_parse(struct cmd_info *);
int out_redirect_info_parse(struct cmd_info *);

#endif

