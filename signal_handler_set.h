#ifndef _SIGNAL_HANDLER_SET_H_
#define _SIGNAL_HANDLER_SET_H_

/* signal handlers */

/* handle the signal of bg process exit */
void sig_handle_bgp_exit(int);
int handle_sigchild(int);

#endif
