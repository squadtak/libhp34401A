/*
 libhp34401A Ver 1.5 2026-06-29
 (c)2026 squad
*/

#ifndef LIBHP34401A_H_INCLUDED
#define LIBHP34401A_H_INCLUDED

#include <stdio.h>
#include <stdarg.h>
#include <libserialport.h>

extern unsigned short hp34401ACalChksumTableEarly[9];
/*
{
	0x004,
	0x00A,
	0x014,
	0x04A,
	0x07C,
	0x0A2,
	0x0D8,
	0x112,
	0x158,
}
*/

extern unsigned short hp34401ACalChksumTableLater[9];
/*
{
	0x004,
	0x00A,
	0x060,
	0x096,
	0x0C8,
	0x0EE,
	0x124,
	0x15E,
	0x1A4,
}
*/

int hp34401AGetIDN(struct sp_port *port, char **brand, char **model, unsigned int *firmVer, unsigned int *ioVer, unsigned int *frontVer);
int is34401A(struct sp_port *port);

int hp34401ASendCommand(struct sp_port *port, char *command);
int hp34401ASendCommandWithRead(struct sp_port *port, char *command, size_t delayReadData, char **readData);

int hp34401APrintText(struct sp_port *port, size_t scrollDelay, char *format, ...);
int hp34401APrintTextV(struct sp_port *port, size_t scrollDelay, char *format, va_list vList);

#endif // LIBHP34401A_H_INCLUDED
