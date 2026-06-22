/*
 libhp34401A Ver 1.1 2026-06-22
 (c)2026 squad
*/

#ifndef LIBHP34401A_H_INCLUDED
#define LIBHP34401A_H_INCLUDED

#include <stdio.h>
#include <libserialport.h>

int open_comport(char *portname, int baudrate, int bits, enum sp_parity parity, int stopbits, enum sp_flowcontrol flowcontrol, struct sp_port **port);
int hp34401ASendCommand(struct sp_port *port, char *command);
int hp34401ASendCommandWithRead(struct sp_port *port, char *command, size_t delay_read_data, char **readData);
int is34401A(struct sp_port *port);
int hp34401APrintText(struct sp_port *port, size_t scrollDelay, char *format, ...);


#endif // LIBHP34401A_H_INCLUDED
