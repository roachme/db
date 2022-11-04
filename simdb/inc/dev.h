#ifndef __DEV_H
#define __DEV_H

#include <stdint.h>
#include "engine.h"

#define ROW_SIZE 			87
#define MAX_ROW_COUNT 		20


/* 
	+49 name length
	+10 digits for each number (x3)
	+1  digit  for each type   (x2)

	+5 space padding symbols 
	+1 newline
	--------------

	ROW_SIZE = 87;
 */



/* 
	открыть и закрыть файл
 */
int dev_open(const char *fname, const char *mode);
int dev_close();

/* 
	чтение и запись в файл определенной строки
 */
int dev_row_count();
int dev_del_row(int idx);
int dev_get_row(int idx, struct auditorium *row);
int dev_put_row(int idx, struct auditorium *row);


struct storage_dev_t
{
	int (*get_row)(int idx, struct auditorium *row);
	int (*put_row)(int idx, struct auditorium *row);
	int (*del_row)(int idx);
	int (*seek)(uint32_t offset);
};





#endif