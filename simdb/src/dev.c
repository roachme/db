#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif

#include "dev.h"



static FILE *image = NULL;
static char fname[20];



static int dev_read_line(void *buffer)
{
	char *p = (char*)buffer;
	unsigned int size = ROW_SIZE;
	if (p == NULL) {
		printf("dev_read: Cannot read with null buffer string\n");
		return -1;
	}

	if (fread(p, 1, size, image) != size) {
		printf("dev_read: [!] Error while reading row\n");
		return -1;
	}

	return 0;
}

static int dev_write_line(const void *buffer)
{
	unsigned int size = ROW_SIZE;
	if (buffer == NULL) {
		printf("dev_write: Cannot write from null buffer string\n");
		return -1;
	}

	if (fwrite(buffer, 1, size, image) != size) {
		printf("[!] Error while writing %u bytes into row\n", size);
		return -1;
	}

	return 0;
}


int dev_row_count()
{
	char row[ROW_SIZE+1], *p = row;
	int  count = 0;

	while (fread(p, 1, ROW_SIZE, image) == ROW_SIZE)
		count++;

	if (count > MAX_ROW_COUNT) {
		fprintf(stderr, "[~] WARNING: Table contains more row than `db' can load\n");
		count = MAX_ROW_COUNT;
	}

	dev.seek(0);
	return count;
}


int dev_del_row(int idx)
{
	const char *fname_copy = "table.cp";
	FILE *f2 = fopen(fname_copy, "w");

	if (!f2) {
		printf("ERROR: open file in write mode: %s\n", fname_copy);
		return -1;
	}
	int cutter = 0, c;


	if (idx == 0) {
		while ((c = getc(image)) != EOF) {
			if (c == '\n') {
				cutter++;
				break;
			}
		}
	}

	while ((c = getc(image)) != EOF) {
		if (c == '\n')
			cutter++;

		if (cutter != idx) {
			putc(c, f2);
		}
	}
	fclose(f2);

	remove(fname);
	rename(fname_copy, fname);

	/* update file position */
	image = freopen(fname, "r+", image);
	return 0;
}


int dev_get_row(int idx, struct auditorium *row)
{
	char line[ROW_SIZE+1];

	dev.seek(idx * ROW_SIZE);

	dev_read_line(line);

	sscanf(line, "%s %d %d %d %d %d",
		(row->name),
		&(row->number),
		&(row->size),
		&(row->capacity),
		&(row->audit_type),
		&(row->board_type));

	return 0;
}


int dev_put_row(int idx, struct auditorium *row)
{
	const int size = ROW_SIZE;
	char str_row[size+1], line[size+1];

	/* row with data */
	sprintf(str_row, "%s %d %d %d %d %d",
		row->name,
		row->number,
		row->size,
		row->capacity,
		row->audit_type,
		row->board_type);


	/* padding */
	int width = ROW_SIZE;
	sprintf(line, "%-*s", width, str_row);
	line[size-1] = '\n';
	line[size-0] = '\0';


	dev.seek(idx * ROW_SIZE);
	dev_write_line(line);
	return 0;
}

int dev_seek(uint32_t offset)
{
	return fseek(image, offset, SEEK_SET);
}


int dev_open(const char *_fname, const char *mode)
{
	if (_fname == NULL) {
		printf("dev_open: Cannot open image with null fname\n");
		return -1;
	}

	if ((image = fopen(_fname, mode)) == NULL) {
		if ((image = fopen(_fname, "w+")) == NULL)
			printf("dev_open: Could not open file\n");

		return -1;
	}

	strcpy(fname, _fname);
	return 0;
}

int dev_close()
{
	return fclose(image);
}


int dev_func(void)
{
	printf("ROW_SIZE %d\n", ROW_SIZE);
	return 0;
}



struct storage_dev_t dev = {
	dev_get_row,
	dev_put_row,
	dev_del_row,
	dev_seek
};


#ifdef __cplusplus
}
#endif