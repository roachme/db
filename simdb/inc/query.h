#ifndef __QUERY_H
#define __QUERY_H

#include "engine.h"

#define QRY_MAX_LEN 	100
#define USED 			1
#define UNUSED 			0

#define Q_DELIM 	" ;\n"
#define Q_SIZE		sizeof(query_keywords)   / sizeof(query_keywords[0])
#define T_SIZE 		sizeof(table_columns)    / sizeof(table_columns[0])
#define OP_SIZE 	sizeof(query_operators)  / sizeof(query_operators[0])


extern const char *table_columns[6]; 
extern const char *query_keywords[5]; 
extern const char *query_operators[3];


enum query_operator
{
	QRY_EQ,
	QRY_LT,
	QRY_GT
};

struct param_t 
{
	char column;				  	/* 1 used, 0 unused */
	enum query_operator op;   		/* which operator is used <, >, = */
	char value[MAX_NAME_LENGTH];
	char bound;						/* -1, 0, 1 */
};

struct pair_t {
	char flag;
	char bop;
};


int qry_getcolumn(char *fact, struct param_t *dst, int *idx);
int qry_getop(char *term, struct param_t *dst, int idx);
int qry_getvalue(char *term, struct param_t *dst, int idx);
int qry_getbop(char *query, struct param_t *dst, int idx);
int qry_getexpr(char *query, struct param_t *param);

int qry_parse_query(char *query_string, struct param_t *param);
int qry_run_query(struct param_t *param, int count);
int qry_show(struct param_t *param, int count);





#endif
