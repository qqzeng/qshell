objects = error.o init.o qutil.o type_prompt.o read_command.o signal_handler_set.o builtin_command.o \
cmd_linklist.o param_parse.o command_execute.o qshell.o
a : $(objects)
		cc -o a $(objects)
init.o : init.c qshell.h init.h
		cc -c init.c qshell.h init.h
error.o : error.c qshell.h
		cc -c error.c
qutil.o : qutil.c error.c qshell.h
		cc -c qutil.c
type_prompt.o : error.c qutil.c type_prompt.c qshell.h
		cc -c error.c qutil.c type_prompt.c
read_command.o : error.c qutil.c read_command.c qshell.h
		cc -c error.c qutil.c read_command.c
signal_handler_set.o : error.c qutil.c signal_handler_set.c qshell.h init.h cmd_linklist.h builtin_command.h
		cc -c -lm error.c qutil.c signal_handler_set.c
builtin_command.o : error.c qutil.c builtin_command.c qshell.h builtin_command.h init.h cmd_linklist.h
		cc -c error.c qutil.c builtin_command.c 
cmd_linklist.o : error.c qutil.c cmd_linklist.c qshell.h cmd_linklist.h
		cc -c error.c qutil.c cmd_linklist.c 
param_parse.o : error.c qutil.c param_parse.c qshell.h cmd_linklist.h param_parse.h
		cc -c error.c qutil.c param_parse.c 
command_execute.o : error.c qutil.c command_execute.c qshell.h cmd_linklist.h
		cc -c error.c qutil.c command_execute.c
qshell.o : error.c qutil.c qshell.c qshell.h cmd_linklist.h
		cc -c error.c qutil.c qshell.c
.PHONY : clean
clean :
		rm a $(objects)