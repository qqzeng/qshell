#include "qshell.h"
#include "init.h"

/* int to str */
int int_to_str(int num, char* str, int base)
{
	if (NULL == str){ return 0;}
	switch (base) {
		case 10:
			sprintf(str,"%d",num);
			break;
		case 16:
			sprintf(str,"%x",num);
			break;
		case 8:
			sprintf(str,"%o",num);
			break;
		default:
			perror("[qshell:error:int_to_str] unsupported base value!\n");
			return 0;
	}
	return 1;
}

// return:
// R  Running
// S  Sleeping in an interruptible wait
// D  Waiting in uninterruptible disk sleep
// Z  Zombie
// T  Stopped (on a signal) or trace stopped
// t  Tracing stop
// W  Paging
// X  Dead
// x  Dead
// K  Wakekill
// W  Waking
// P  Parked
// get a process status
char get_process_status(int pid)
{
    char file_path[PATH_MAX_VALUE] = "/proc/";
    // char pid_str[(int)(log10(PID_MAX_VALUE) + 1)];
    char pid_str[(int)(4 + 1)];
    int_to_str(pid, pid_str, 10);
    char* str2 = "/status";
    strcat(file_path, pid_str);
    strcat(file_path, str2);
    FILE *fp = fopen(file_path, "r");
    // /proc/pid/status doesn't exist
    if (!fp) { printf("[qshell:info:get_process_status] file open error because of process quiting!\n"); return 'N';}
    int i = 0;
    if (fp) {
    	char *buf = malloc(sizeof(char)*100);
    	buf = fgets(buf, 100, fp);
        while (NULL != buf) {
            char *delim = "\t";
            char *p;
            p = strtok(buf, delim);
            if (!strncmp(p, "State", 5)) {
            	p = strtok(NULL, delim);
            	// the file was closed when reading its State?
            	if(NULL != fp && NULL != p) {
            		fclose(fp);
            		return p[0];
            	}
                else {
                	printf("[qshell:error:get_process_status] failure ocurs when getting process State !\n");
                	return 'N';
                }
            }
            i++;
            buf = fgets(buf, 100, fp);
        }
    }
}