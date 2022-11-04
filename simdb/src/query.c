#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "output.h"
#include "query.h"
#include "dev.h"




const char *table_columns[] = 
{
	"name",
	"number",
	"size",
	"capacity",
	"audit_type",
	"board_type"
};

const char *query_keywords[] = 
{
	"select",
	"from",
	"table",
	"where",
	"and",
};

const char *query_operators[] =
{
	"=",
	"<",
	">"
};


int qry_getcolumn(char *fact, struct param_t *dst, int *idx)
{
	char *tok;

	if ((tok = strtok(fact, Q_DELIM)) == NULL) {
		printf("[-] Syntax error: excpected column label\n");
		return -1;
	}

	unsigned i;
	for (i = 0; i < T_SIZE; i++)
		if (strcmp(tok, table_columns[i]) == 0)
			break;

	if (i == T_SIZE) {
		printf("[-] Syntax error: Column not found %d: %s\n", i, tok);
		return -1;
	}

	*idx = i;
	dst[i].column = USED;
	return 0;
}


int qry_getop(char *term, struct param_t *dst, int idx)
{
	char *tok;

	if ((tok = strtok(term, Q_DELIM)) == NULL) {
		printf("Syntax error: excpected operator\n");
		return -1;
	}

	unsigned i;
	for (i = 0; i < OP_SIZE; i++)
		if (strcmp(tok, query_operators[i]) == 0) {
			break;
		}

	if (i == OP_SIZE) {
		printf("Syntax error: Invalid compare operator %d\n", i);
		return -1;
	}

	dst[idx].op = (enum query_operator)i;
	return 0;
}

int qry_getvalue(char *term, struct param_t *dst, int idx)
{
	char *tok;

	if ((tok = strtok(term, Q_DELIM)) == NULL) {
		printf("Syntax error: excpected value\n");
		return -1;
	}

	strcpy(dst[idx].value, tok);
	return 0;
}

int qry_getbop(char *query, struct param_t *dst, int idx)
{
	char *tok;

	if ((tok = strtok(query, Q_DELIM)) == NULL) {
		dst[idx].bound = -1;
		return 1;
	}

	if (strcmp(tok, "and") == 0)
		dst[idx].bound = 1;

	else if (strcmp(tok, "or") == 0)
		dst[idx].bound = 0;
	
	else {
		printf("Syntax error: Invalid bound operator %s\n", tok);
		return -1;
	}

	return 0;
}

int qry_getexpr(char *query, struct param_t *param)
{
	int i, flag;
	for (i = 0; i < 6; i++)
		param[i].column = UNUSED;


	/* start */
	strtok(query, Q_DELIM);
	for (i = 0; i < 6; i++) {
		if (qry_getcolumn(NULL, param, &i) != 0)
			return -1;

		if (qry_getop(NULL, param, i) != 0)
			return -1;

		if (qry_getvalue(NULL, param, i) != 0)
			return -1;

		flag = qry_getbop(NULL, param, i);


		if (flag == -1){
			return -1;
		}
		else if (flag == 1){
			i++;
			break;
		}

	}

	return 0;
}


/*
- name = jack and size < 10 and board_type = 1
-  number > 1000
*/

int qry_parse_query(char *query_string, struct param_t *param)
{
	return qry_getexpr(query_string, param);
}



int show_param(struct param_t *param)
{
	printf("\ncolumn\t operat\t value\t bound\t\n");
	for (int i = 0; i < 6; i++) {
		printf("[*] %d\t %d\t %s\t %d\n",
			param[i].column,
			param[i].op,
			param[i].value,
			param[i].bound);
	}
	printf("\n");
	return 0;
}

/* struct -> array */
int row_to_param(struct param_t *dst, struct auditorium *src)
{
	int i = 0;
	sprintf(dst[i++].value, "%s", src->name);
	sprintf(dst[i++].value, "%d", src->number);
	sprintf(dst[i++].value, "%d", src->size);
	sprintf(dst[i++].value, "%d", src->capacity);
	sprintf(dst[i++].value, "%d", src->audit_type);
	sprintf(dst[i++].value, "%d", src->board_type);
	return 0;
}

int qry_run_query(struct param_t *param, int count)
{
	int i, k;
	int flag, num1, num2, op, line = 0;
	struct param_t comp[6];
	struct auditorium row;

	memset(comp, 0, sizeof(struct param_t));

	for (i = 0; i < count; i++) {
		flag = 1;
		dev.get_row(i, &row);


		/* convert into param_t */
		row_to_param(comp, &row);


		/*	compare string values*/
		if (param[0].column) {
			if (strcmp(comp[0].value, param[0].value) != 0) {
				continue;
			}
		}

		/*	compare numeric values */
		for (k = 1; k < 6; k++) {
			if (param[k].column) {
				num1 = atoi(comp[k].value);
				num2 = atoi(param[k].value);
				op   = param[k].op;

				if (op == QRY_EQ) {
					if (num1 != num2) {
						flag = 0;
						continue;
					}
				}

				if (op == QRY_LT) {
					if (num1 == num2 || num1 > num2) {
						flag = 0;
						continue;
					}
				}

				if (op == QRY_GT) {
					if (num1 == num2 || num1 < num2) {
						flag = 0;
						continue;
					}
				}

			}
		}

		if (flag) {
			out_table_row(i, &row);
			line++;
		}
	}
	return 0;
}


int qry_show(struct param_t *param, int count)
{
	printf("count: %d\n", count);
	for (int i = 0; i < count; i++) {
		printf("[*] used: %d\n", param[i].column);
		printf("[*] oper: %d\n", param[i].op);
		printf("[*] val : %s\n", param[i].value);
		printf("[*] bop : %d\n\n", param[i].bound);
	}
	printf("\n\n");
	return 0;
}


#ifdef __cplusplus
}
#endif
