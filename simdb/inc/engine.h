#ifndef __DB_ENGINE_H
#define __DB_ENGINE_H

#include <stdio.h>
#include "input.h"

struct auditorium
{
	char name[MAX_NAME_LENGTH];
	int number;
	int size;
	int capacity;
	int audit_type;
	int board_type;
};



enum DB_ERROR_TYPE
{
	DB_OK,
	DB_ROW_OVERFLOW,
	DB_ROW_UNDERFLOW,
	DB_INVALID_CMD = -1
};


/*
	Открыть файл (таблицу) в режиме для чтения и записи
	подсчитать код-во строк в ней
*/
void db_init(void);



/*
	Добавить новую строку в файл
*/
int  db_add_row();



/*
	Изменить строку в файле (по индексу)
*/
int  db_chg_row(int row);



/*
	Удалить строку в файле (по индексу)
*/
int  db_del_row(int row);



/*
	Выволнить запрос в БД
	Ограничения: 
		- только SELECT
		- нет поддержки для `or' подвыражения
*/
int  db_query(void);

/*
	Закрыть файл
	Освободить память
*/
int  db_exit(void);


/* 
	Показать содержимое таблицы из файла
	Все или определенный ряд
 */
int db_show_table();
int show_row(struct auditorium *row);
int db_show_table(void);
int db_make_table(void);



extern struct storage_dev_t dev;
extern const int ROW_SIZE;

#endif