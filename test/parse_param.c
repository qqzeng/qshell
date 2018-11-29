/**
@author: qqzeng
@email: qtozeng@gmail.com
@date: 17-12-09
@desc: 主函数
**/
#include "qshell.h"
#define DEBUG

void param_parse_info_init(struct param_parse_info *info)
{
    info->flag = 0;
    info->in_file = NULL;
    info->out_file = NULL;
    info->com_second = NULL;
    info->param_second = NULL;
    return;
}

int param_parse(char **parameters, int ncommand, struct param_parse_info *info)
{
    int i;
    param_parse_info_init(info);
    // background job
    if(0 == strcmp(parameters[ncommand - 1],"&"))
    {
        info->flag |= BACKGROUND;
        parameters[ncommand-1] = NULL;
        ncommand--;
    }
    // ls -ll | grep a | wc | wc -w >> tmp &
    for(i = 0; i < ncommand; )
    {
    	// input redirect
        if(0 == strcmp(parameters[i],"<<") || 0 == strcmp(parameters[i],"<"))
        {
            info->flag |= IN_REDIRECT;
            info->in_file = parameters[i+1];
            parameters[i] = NULL;
            i+=2;
        }
        // output redirect
        else if(0 == strcmp(parameters[i],">"))
        {
            info->flag |= OUT_REDIRECT;
            info->out_file = parameters[i+1];
            parameters[i] = NULL;
            i+=2;
        }
        else if(0 == strcmp(parameters[i],">>"))
        {
            info->flag |= OUT_REDIRECT_APPEND;
            info->out_file = parameters[i+1];
            parameters[i] = NULL;
            i+=2;
        }
        // pipeline
        else if(0 == strcmp(parameters[i],"|"))
        {
            char* pCh;
            info->flag |= IS_PIPED;
            parameters[i] = NULL;
            info->com_second = parameters[i+1];
            info->param_second = &parameters[i+1];
            for(pCh = info->param_second[0]+strlen(info->param_second[0]);
                    pCh != &(info->param_second[0][0]) && *pCh != '/'; pCh--)
                ;
            if(*pCh == '/')
                pCh++;
            info->param_second[0] = pCh;
            break;
        }
        else
            i++;
    }
#ifdef DEBUG
	printf("\nbackground:%s\n",info->flag & BACKGROUND?"yes":"no");
	printf("in redirect:");
	info->flag & IN_REDIRECT ? printf("yes,file:%s\n",info->in_file) : printf("no\n");
	printf("out redirect:");
	info->flag & OUT_REDIRECT ? printf("yes,file:%s\n",info->out_file) : printf("no\n");
	printf("pipeline:");
	info->flag & IS_PIPED ? printf("yes,command:%s %s %s\n",info->com_second,info->param_second[0],info->param_second[1]):printf("no\n");
#endif
    return 1;
}