/**
 * \file		libhp34401A.h
 * \brief		library for HP/Agilent/Keysight(HPAK) 34401A
 * \author		squad
 * \version		Ver 1.6
 * \date		2026-06-30
 * \copyright	Copyright 2026 squad
 * \license		This library is released under the MIT license.
 */

 /**
 * @mainpage libhp34401A
 * library for HP/Agilent/Keysight(HPAK) 34401A 6-1/2 Multimeter
 *
 * \copyright	Copyright 2026 squad
 * \license		This library is released under the MIT license.
 *
 *
 * Headers
 * =======
 * including:
 * @code
 * #include "libhp34401A.h"
 * @endcode
 *
 *
 * Constant  variables
 * ===================
 * \sa hp34401ACalChksumTableEarly[9] calibration checksum address tables for early 34401A (before 07-xx-xx)
 * \sa hp34401ACalChksumTableLater[9] calibration checksum address tables for later 34401A (from 07-xx-xx)
 *
 *
 * Functions
 * =========
 * \sa hp34401AGetIDN() get Identificaton(*IDN?)string from 34401A
 * \sa is34401A() is it 34401A?
 * \sa hp34401ASendCommand() send command to 34401A
 * \sa hp34401ASendCommandWithRead() send command to 34401A and read return strings
 * \sa hp34401APrintText() send display print command(DISP:TEXT) to 34401A
 * \sa hp34401APrintTextV() send display print command(DISP:TEXT) to 34401A (va_list)
 */

#ifndef LIBHP34401A_H_INCLUDED
#define LIBHP34401A_H_INCLUDED

#include <stdio.h>
#include <stdarg.h>
#include <libserialport.h>

/**
 * \brief calibration checksum address tables for early 34401A (before 07-xx-xx)
*/
extern const unsigned short hp34401ACalChksumTableEarly[9];

/**
 * \brief calibration checksum address tables for later 34401A (from 07-xx-xx)
*/
extern const unsigned short hp34401ACalChksumTableLater[9];

/**
 * \brief	get Identificaton(*IDN?)string from 34401A
 *
 * \param 	[in]	*port		sp_port port handle from sp_open
 * \param 	[out]	*brand		brand strings (allocated with malloc)
 * \param 	[out]	**model		model strings (allocated with malloc)
 * \param 	[out]	*firmVer	firmware version number
 * \param 	[out]	*ioVer		IO chips version number
 * \param 	[out]	*frontVer	front panel version number
 *
 * \return	0					success
 * \return	-1 to -9			error
 */
int hp34401AGetIDN(struct sp_port *port, char **brand, char **model, unsigned int *firmVer, unsigned int *ioVer, unsigned int *frontVer);

/**
 * \brief	is it 34401A?
 *
 * \param 	[in]	*port	sp_port port handle from sp_open
 *
 * \return	1				it's 34401A
 * \return	0				it's not 34401A
 * \return	-1				error
 */
int is34401A(struct sp_port *port);

/**
 * \brief	send command to 34401A
 *
 * \param 	[in]	*port		sp_port port handle from sp_open
 * \param 	[in]	*command	command strings to send
 *
 * \return	0					success
 * \return	-1					error
 */
int hp34401ASendCommand(struct sp_port *port, char *command);

/**
 * \brief	send command to 34401A and read return strings
 *
 * \param 	[in]	*port			sp_port port handle from sp_open
 * \param 	[in]	*command		command strings to send
 * \param 	[in]	*delayReadData	delay between sending command and read received strings
 * \param 	[out]	**readData		returned strings ftom 34401A (allocated with malloc)
 *
 * \return	0						success
 * \return	-1 to -3				error
 */
int hp34401ASendCommandWithRead(struct sp_port *port, char *command, size_t delayReadData, char **readData);

/**
 * \brief	send display print command(DISP:TEXT) to 34401A
 *
 * \param 	[in]	*port		sp_port port handle from sp_open
 * \param 	[in]	scrollDelay	step delay for scrolling text
 * \param 	[in]	*format		format strings (like sprintf)
 * \param 	[in]	...			arguments
 *
 * \return	0					success
 * \return	-1 to -3			error
 */
int hp34401APrintText(struct sp_port *port, size_t scrollDelay, char *format, ...);

/**
 * \brief	send display print command(DISP:TEXT) to 34401A (va_list)
 *
 * \param 	[in]	*port		sp_port port handle from sp_open
 * \param 	[in]	scrollDelay	step delay for scrolling text
 * \param 	[in]	*format		format strings (like sprintf)
 * \param 	[in]	vList		variable arguments list
 *
 * \return	0						success
 * \return	-1 to -3				error
 */
int hp34401APrintTextV(struct sp_port *port, size_t scrollDelay, char *format, va_list vList);

#endif // LIBHP34401A_H_INCLUDED
