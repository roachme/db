#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif

#include "output.h"
const char *table_headers[] = 
{
	"N",
	"NAME",
	"NUMBER",
	"SIZE",
	"CAPAC",
	"ATYPE",
	"BTYPE"
};

const int table_header_limits[] = {
	3, 11, 12, 12, 12, 7, 2
};


#define TBL_HEADERS_COUNT  sizeof(table_headers) / sizeof(table_headers[0])


int out_table_header(void)
{
	unsigned int i;
	for (i = 0; i < TBL_HEADERS_COUNT; i++)
		printf("%-*s", table_header_limits[i], table_headers[i]);
	printf("\n");

	return 0;
}

int out_table_row(unsigned int i, struct auditorium *row)
{
	printf("%-*d%-*s%-*d%-*d%-*d%-*d%-*d\n",
		table_header_limits[0], i,
		table_header_limits[1], row->name,
		table_header_limits[2], row->number,
		table_header_limits[3], row->size,
		table_header_limits[4], row->capacity,
		table_header_limits[5], row->audit_type,
		table_header_limits[6], row->board_type);

	return 0;
}

int out_table_options(void)
{

	printf("1. name\t\t2. number\t\t3. size\n");
	printf("4. capacity\t5. audit_type\t\t6. board_type\n");
	return 0;
}



#ifdef __cplusplus
}
#endif
