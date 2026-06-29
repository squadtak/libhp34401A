/*
 libhp34401A Ver 1.4 2026-06-29
 (c)2026 squad
*/

#include "libhp34401A.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

unsigned short hp34401ACalChksumTableEarly[] =
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
};

unsigned short hp34401ACalChksumTableLater[] =
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
};

int gotIDNFlag = 0;
char *gotBrand = NULL;
char *gotModel = NULL;
unsigned int gotFirmVer = 0;
unsigned int gotIoVer = 0;
unsigned int gotFrontVer = 0;

int hp34401AGetIDN(struct sp_port *port, char **brand, char **model, unsigned int *firmVer, unsigned int *ioVer, unsigned int *frontVer)
{
	int ret = 0;

	if(!gotIDNFlag){

		char *buf;
		if(hp34401ASendCommandWithRead(port, "*IDN?", 300000, &buf) != 0) return -1;

		char *brandPos = NULL;
		if(!(brandPos = strtok(buf, ","))) return -2;
		if(!(gotBrand = calloc(strlen(brandPos) + 1, 1))) return -3;
		memcpy(gotBrand, brandPos, strlen(brandPos));

		char *modelPos = NULL;
		if(!(modelPos = strtok(NULL, ","))) { ret = -4; goto err0; }
		if(!(gotModel = calloc(strlen(modelPos) + 1, 1))) { ret = -5; goto err0; }
		memcpy(gotModel, modelPos, strlen(modelPos));

		if(!strtok(NULL, ",")) { ret = -6; goto err1; } //0

		char *firmVerPos = NULL;
		if(!(firmVerPos = strtok(NULL, "-"))) { ret = -7; goto err1; }
		gotFirmVer = strtoul(firmVerPos, NULL, 10);

		char *ioVerPos = NULL;
		if(!(ioVerPos = strtok(NULL, "-"))) { ret = -8; goto err1; }
		gotIoVer = strtoul(ioVerPos, NULL, 10);

		char *frontVerPos = NULL;
		if(!(frontVerPos = strtok(NULL, "\0"))) { ret = -9; goto err1; }
		gotFrontVer = strtoul(frontVerPos, NULL, 10);

		free(buf);
		gotIDNFlag = 1;
	}

	if(brand) *brand = gotBrand;
	if(model) *model = gotModel;
	if(firmVer) *firmVer = gotFirmVer;
	if(ioVer) *ioVer = gotIoVer;
	if(frontVer) *frontVer = gotFrontVer;

	return ret;

err1:
	free(gotModel);
err0:
	free(gotBrand);
	return ret;
}

int is34401A(struct sp_port *port)
{
	if(!gotIDNFlag) if(hp34401AGetIDN(port, NULL, NULL, NULL, NULL, NULL)) return -1;

	if(!strcmp(gotModel, "34401A")) return 1;

	return 0;
}

int hp34401ASendCommandWithRead(struct sp_port *port, char *command, size_t delayReadData, char **readData)
{
	int ret = 0;

	if(hp34401ASendCommand(port, command)) return -1;

	if(delayReadData) usleep(delayReadData);

	int retryFlag = 10;

	*readData = NULL;
	size_t readDataLen = 0;
	do{

		char *read_buf_tmp;

		if((read_buf_tmp = realloc(*readData, readDataLen + 1)) == NULL){
			if(*readData){
				free(*readData);
				*readData = NULL;
			}
			return -2;
		}
		*readData = read_buf_tmp;

retry:
		int rett = 0;
		if((rett = sp_blocking_read(port, *readData + readDataLen, 1, 10)) < 1){

			if(rett == 0){

				if(!retryFlag){
					ret = 1;
					break;
				}

				retryFlag--;
				goto retry;

			}else{

				free(*readData);
				*readData = NULL;
				return -3;

			}
		}

		if(*(*readData + readDataLen) == '\r'){
			char buf[1];
			sp_blocking_read(port, buf, 1, 1000);
			break;
		}

		readDataLen++;
	}while(1);

	*(*readData + readDataLen) = '\0';

	return ret;
}

int hp34401ASendCommand(struct sp_port *port, char *command)
{
	char sendBuf[strlen(command)];
	memcpy(sendBuf, command, strlen(command));
	sendBuf[strlen(command)] = '\n';

	if(sp_blocking_write(port, sendBuf, strlen(command) + 1, 1000) < strlen(command) + 1) return -1;
	usleep(20000);

	return 0;
}

int hp34401APrintText(struct sp_port *port, size_t scrollDelay, char *format, ...)
{
	va_list vList;
	va_start(vList, format);

	int ret = hp34401APrintTextV(port, scrollDelay, format, vList);

	va_end(vList);
	return ret;
}

int hp34401APrintTextV(struct sp_port *port, size_t scrollDelay, char *format, va_list vList)
{
	size_t strLen = 0;
	if((strLen = vsnprintf(NULL, 0, format, vList)) < 0) return -1;

	char buf[strLen + 1];
	vsprintf(buf, format, vList);

	char commandBuf[strlen("DISP:TEXT \"\"\r") + 12 + 1];
	memcpy(commandBuf, "DISP:TEXT \"", strlen("DISP:TEXT \""));

	if(strLen > 12){
		memcpy(commandBuf + strlen("DISP:TEXT \"") + 12, "\"\r\0", strlen("\"\r") + 1);

		for(size_t diffCnt = 0; diffCnt < (strLen - 12 + 1); diffCnt++){
			memcpy(commandBuf + strlen("DISP:TEXT \""), buf + diffCnt, 12);
			if(hp34401ASendCommand(port, commandBuf)) return -2;
			usleep(scrollDelay * 1000);
		}

	}else{
		memcpy(commandBuf + strlen("DISP:TEXT \""), buf, strLen);
		memcpy(commandBuf + strlen("DISP:TEXT \"") + strLen, "\"\r\0", strlen("\"\r") + 1);
		if(hp34401ASendCommand(port, commandBuf)) return -3;
	}

	return 0;
}
