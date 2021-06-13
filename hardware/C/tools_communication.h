#ifndef _TOOLS_COMM_H_
#define _TOOLS_COMM_H_

int send_data1M(int fd_in, int fd_out, struct Motor* m1);

int launch_start();

int send_data2M(int fd_in, int fd_out, struct Motor* m1, struct Motor* m2);

#endif
