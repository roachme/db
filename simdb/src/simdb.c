#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "simdb.h"

#define CMD_DELIM 		" "
#define CMD_COUNT  		sizeof(commands) / sizeof(commands[0])


/*
create and maintain one table
	- make_table
	+ add_row
	+ chn_row
	+ del_row
	+ make_query
	+ show_table
*/


const char *usage = 
"Usage: simdb\n"
"1. Add new row\n"
"2. Change row\n"
"3. Delete row\n"
"4. Make query\n"
"5. Show table\n"
"6. Struct of table\n"
"7. Help\n"
"0. Exit\n";


const char *commands[] = 
{
	"table",
	"add",
	"change",
	"delete",
	"query",
	"show",
	"exit",

	"struct",
	"help"
};

typedef enum command 
{
	DB_MAKE_TBL,
	DB_ADD,
	DB_CHG,
	DB_DEL,
	DB_MAKE_QRY,
	DB_SHOW_TBL,
	DB_EXIT,

	DB_STRUCT,
	DB_HELP,

	DB_INV_CMD = -1
} command_t;


int db_interface_gui(void)
{
	return 0;
}

int db_usage(void)
{
	printf("%s\n", usage);
	return 0;
}

command_t get_command(void)
{
	int flag;
	char cmd[100], *p = cmd;

	printf("\nsimdb> ");
	if ((flag = in_getstr(p)) != DB_OK) {
		printf("INVALID COMMAND\n");
		return DB_INV_CMD;
	}

	unsigned int i;
	for (i = 0; i < CMD_COUNT; i++)
		if (strcmp(commands[i], p) == 0)
			break;

	if (i == CMD_COUNT)
		return DB_INV_CMD;

	return (command_t)i;
}


int db_interface_cli(void)
{
	db_init();
	int cmd, flag, idx;

	while (1) {
		cmd = (int)get_command();
		//printf("cmd: [%d]\n", cmd);

		switch(cmd) {
		case DB_MAKE_TBL:
			//printf("make table: [%d]\n", cmd);
			flag = db_make_table();
		break;


		case DB_ADD:
			flag = db_add_row();
		break;


		case DB_CHG:
			printf("idx: ");
			if ((flag = in_getnum(&idx)) != DB_OK)
				return flag;

			flag = db_chg_row(idx);
		break;


		case DB_DEL:
			printf("idx: ");
			if ((flag = in_getnum(&idx)) != DB_OK)
				return flag;

			flag = db_del_row(idx);
			break;
		break;


		case DB_MAKE_QRY:
			//printf("[&] Performing query\n");
			flag = db_query();
		break;


		case DB_SHOW_TBL:
			//printf("show: [%d]\n", cmd);
			flag = db_show_table();
		break;


		case DB_EXIT:
			return 0;
		break;


		case DB_STRUCT:
			flag = 0;
		break;


		case DB_HELP:
			db_usage();
		break;

		default:
			printf("[~] WARNING: Invalid command. Use help command\n");
			break;
		}
	}

	return 0;
}


char *db_func(void)
{
	char *s = (char *)malloc(20);
	strcpy(s, "hinata hyuga");
	printf("from C: [%s]\n", s);
	return s;
}

int func()
{
	char s[20];
	int num;

	printf("enter string: ");
	scanf("%s", s);
	printf("[%s]\n", s);


	printf("enter number: ");
	scanf("%d", &num);
	printf("[%d]\n", num);

	return 0;
}


int main(int argc, char const *argv[])
{


	db_interface_cli();
	return 0;
}


#ifdef __cplusplus
}
#endif
