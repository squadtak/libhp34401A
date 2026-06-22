/*
 libhp34401A Ver 1.1 2026-06-22
 (c)2026 squad
*/

#include "libhp34401A.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

int is34401A(struct sp_port *port)
{
	char *buf;

	if(hp34401ASendCommandWithRead(port, "*IDN?", 300000, &buf) != 0) return -1;

	char *model_pos = NULL;
	if((model_pos = strchr((char *)buf, ',')) == NULL){
		free(buf);
		return -2;
	}

	model_pos += 1; // ,

	if(strtok(model_pos, ",") == NULL){
		free(buf);
		return -3;
	}

	int ret = 0;

	if(strstr(model_pos, "34401A") - model_pos == 0 && strlen(model_pos) == strlen("34401A")) ret = 1;

	free(buf);
	return ret;
}

int hp34401ASendCommandWithRead(struct sp_port *port, char *command, size_t delay_read_data, char **readData)
{
	int ret = 0;

	if(hp34401ASendCommand(port, command)) return -1;

	if(delay_read_data) usleep(delay_read_data);

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
	va_list vargs;
	va_start(vargs, format);

	size_t strLen = 0;
	if((strLen = vsnprintf(NULL, 0, format, vargs)) < 0) return -1;

	char buf[strLen + 1];
	vsprintf(buf, format, vargs);

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

	va_end(vargs);
	return 0;
}
