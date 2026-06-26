/*
 libhp34401A Ver 1.3 2026-06-26
 (c)2026 squad
*/

#ifndef LIBHP34401A_H_INCLUDED
#define LIBHP34401A_H_INCLUDED

#include <stdio.h>
#include <stdarg.h>
#include <libserialport.h>

int hp34401AGetIDN(struct sp_port *port, char **brand, char **model, unsigned int *firmVer, unsigned int *ioVer, unsigned int *frontVer);
int is34401A(struct sp_port *port);

int hp34401ASendCommand(struct sp_port *port, char *command);
int hp34401ASendCommandWithRead(struct sp_port *port, char *command, size_t delayReadData, char **readData);

int hp34401APrintText(struct sp_port *port, size_t scrollDelay, char *format, ...);
int hp34401APrintTextV(struct sp_port *port, size_t scrollDelay, char *format, va_list vList);

#endif // LIBHP34401A_H_INCLUDED
