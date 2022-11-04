#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "query.h"
#include "dev.h"
#include "input.h"
#include "output.h"
#include "engine.h"


const  int TEST_MODE = 0;
static int row_count;




static int check_row(int idx)
{
	if (idx < 0)
		return DB_ROW_UNDERFLOW;

	if (idx > row_count) {
		printf("[+] %d : %d\n", idx, row_count);
		return DB_ROW_OVERFLOW;
	}

	return DB_OK;
}


void db_init(void)
{
	dev_open("data/table.db", "r+");

	/* init row_count */
	row_count = dev_row_count();
	//printf("[+] row_count: %d\n", row_count);
}


int db_add_row(void)
{
	int num, flag;
	struct auditorium row;
	char buffer[MAX_NAME_LENGTH], *p = buffer;


	if (TEST_MODE) printf("\n");
label_name:
	printf("    corpus name       : ");
	if ((flag = in_getword(p) != INPUT_OK))
		goto label_name;
	strcpy(row.name, p);
	
	if (TEST_MODE) printf("\n");


label_number:
	printf("    auditorium number : ");
	if ( (flag = in_getnum(&row.number) ) != INPUT_OK)
		goto label_number;
	if (TEST_MODE) printf("\n");


label_size:
	printf("    auditorium size   : ");
	if ( (flag = in_getnum(&row.size) ) != INPUT_OK)
		goto label_size;
	if (TEST_MODE) printf("\n");


label_capac:
	printf("    auditorium capac  : ");
	if ( (flag = in_getnum(&row.capacity) ) != INPUT_OK)
		goto label_capac;
	if (TEST_MODE) printf("\n");


/* (1 - multimedia, 0 - casual) */
label_audit:
	printf("    auditorium type   : ");
	if ( (flag = in_getnum(&num) ) != INPUT_OK)
		goto label_audit;

	if (num > 1) {
		fprintf(stderr, "[~] WARNING: audit_types: 1 or 0\n");
		goto label_audit;
	}
	row.audit_type = num;
	if (TEST_MODE) printf("\n");


/* 1 - oxcid, 0 - marker */
label_board:
	printf("    board type        : ");
	if ( (flag = in_getnum(&num) ) != INPUT_OK)
		return flag;
	if (num > 1) {
		fprintf(stderr, "[~] WARNING: board_types: 1 or 0\n");
		goto label_board;
	}
	row.board_type = num;
	if (TEST_MODE) printf("\n");


	row_count++;
	dev.put_row(row_count - 1, &row);
	return 0;
}

int db_chg_row(int idx)
{

	int  num, flag, column;
	char buffer[MAX_NAME_LENGTH], *p = buffer;
	struct auditorium row;


	/* check out if we have any row */
	if (row_count == 0)
		return DB_OK;

	/* Prevent attemt to access non-existing row */
	if (idx > row_count) {
		printf("[!] ERROR: Invalid row index [%d]\n", idx);
		return DB_INVALID_CMD;
	}

	dev.get_row(idx, &row);


	out_table_options();
in_getnum_label:
	printf(">_ ");
	if ((flag = in_getnum(&column)) != DB_OK
		|| !(column >= 1 && column <= 6)) {
		fprintf(stderr, "[~] WARNING: Invalid row index [%d]\n", column);
		goto in_getnum_label;
	}

	switch(column) {
		case 1:
			printf("name  : ");
			if ((flag = in_getword(p)) != DB_OK)
				return flag;
			memcpy(row.name, p, MAX_NAME_LENGTH);
			break;

		case 2:
			printf("number: ");
			if ((flag = in_getnum(&row.number)) != DB_OK)
				return flag;
			break;

		case 3:
			printf("size  : ");
			if ((flag = in_getnum(&row.size)) != DB_OK)
				return flag;
			break;

		case 4:
			printf("capacity: ");
			if ((flag = in_getnum(&row.capacity)) != DB_OK)
				return flag;
			break;

		case 5:
			printf("audit_type: ");
			if ((flag = in_getnum(&num)) != DB_OK)
				return flag;
			if (num > 2) {
				fprintf(stderr, "WARNING: audit types: 1 or 0\n");
				return -1;
			}
			row.audit_type = num;
			break;

		case 6:
			printf("board_type: ");
			if ((flag = in_getnum(&num)) != DB_OK)
				return flag;
			if (num > 2) {
				fprintf(stderr, "WARNING: board types: 1 or 0\n");
				return -1;
			}
			row.board_type = num;
			break;

		default:
			printf("WARNING: invalid column\n");
			return 0;
	}


	dev.put_row(idx, &row);
	return 0;
}


int db_del_row(int idx)
{
	int flag;
	if ((flag = check_row(idx)) != DB_OK) {
		printf("[~] WARNING No such row: %d\n", idx);
		return flag;
	}
	dev.del_row(idx);

	row_count--;
	return DB_OK;
}


int db_query(void)
{
	char query[200], *p = query;

	/* get query string */
	printf("[*] SELECT * FROM TABLE ");
	in_getquery(p);

	/* send query string for parsing */
	struct param_t param[6];
	memset(param, 0, sizeof(param));
	if (qry_parse_query(p, param) == -1) {
		fprintf(stderr, "[-] ERROR: Invalid query\n");
		return -1;
	}

	/* perform query */
	return qry_run_query(param, row_count);
}


int db_show_table(void)
{
	struct auditorium row;

	out_table_header();
	for (int i = 0; i < row_count; i++) {
		dev.get_row(i, &row);
		out_table_row(i, &row);
	}

	return dev.seek(0);
}

int db_make_table(void)
{
	return 0;
}


int db_exit(void)
{
	dev_close();
	return 0;
}



#ifdef __cplusplus
}
#endif
