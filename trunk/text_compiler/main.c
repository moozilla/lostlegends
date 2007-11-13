/******************************************************************************
 * Lost Legends I text compiler
 *
 * This is meant to compile ASCII text with special characters into an array
 * that is parserable by the LLI text displayer.
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define CNULL 0x7F
#define MAX_TAGSIZE 24
#define NUM_TAGS 50

static const char tags[][MAX_TAGSIZE] = {
	"elip","right_quotation","left_quotation","left_quotation_period",
	"li","il","ll","right_arrow",
	"left_arrow","up_arrow","up_double1","up_double2",
	"heart","sword","staff","bow_and_arrow",
	"book","armor","glove","ring",
	"potion","key","poison","seperator",
	"corner_pane_ul","upper_pane","corner_pane_ur","side_pane_l",
	"side_pane_r","corner_pane_bl","lower_pane","corner_pane_br",
	"status1","status2","status3","status4",
	"equip1","equip2","equip3","item1",
	"item2","item3","command1","command2","command3",
	"command4","left_arrow_blend","right_arrow_blend"
};

static const char lltags_map[] = {
	40,44,45,46,
	52,53,54,55,
	56,57,58,59,
	60,61,62,63,
	64,65,66,67,
	68,69,70,71,
	72,73,74,75,
	76,77,78,79,
	80,81,82,83,
	84,85,86,87,
	88,89,90,91,
	92,93,94,95
};

static const char llchar_map[] = {
	CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,
	CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,
	CNULL,CNULL,0x26,0x0A,CNULL,0x32,CNULL,CNULL,CNULL,0x2A,CNULL,CNULL,0x31,0x33,0x2B,0x2F,
	0x27,0x30,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x29,CNULL,CNULL,
	CNULL,CNULL,0x0B,CNULL,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
	0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,CNULL,CNULL,
	CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,
	CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,CNULL,
	CNULL,CNULL,CNULL,CNULL,CNULL,CNULL
};

void get_tag(FILE* f, char* buf); /* Function assumes that the position is after the '[' */
int do_tag(char* tag);

int main(int argc, char* argv[])
{
	FILE* infile;
	char c_char = CNULL, buffer[24];
	unsigned int byte_count = 0, tag_count = 0;
	int tag_number;
	
	if (argc != 2) {
		printf("Usage: %s <file>", argv[0]);
		exit(0);
	}
	
	if (!(infile = fopen(argv[1], "r"))) {
		puts("ERROR: Could not open the file, check the extension and/or name.");
		exit(0);
	}
	
	c_char = toupper(fgetc(infile));
	while (c_char != EOF) {
		
		if (c_char == '[') {
			tag_count++; byte_count++;
			get_tag(infile, buffer);
			c_char = CNULL;
			tag_number = do_tag(buffer);
			
			if (tag_number == -1) {
				printf("ERROR: \"%s\" not recognized\n", buffer);
			} else
				printf("Tag (id): %s (%d)\n", buffer, lltags_map[tag_number]);
		}
		
		if (llchar_map[c_char] != CNULL) {
			byte_count++;
			printf("Char (id): %c (%d)\n", c_char, llchar_map[c_char]);
		} else if (c_char != '\n' && c_char != '\t' && c_char != '\r' && c_char != CNULL)
			printf("ERROR: '%c' not recognized\n", c_char);
			
		c_char = toupper(fgetc(infile));
	}
	
	printf("\nNumber of bytes: %d\n", byte_count);
	printf("Number of tags: %d\n", tag_count);
	fclose(infile);
	exit(0); /* Some shells may not support 'return 0;' */
}

void get_tag(FILE* f, char* buf)
{
	char c = CNULL, len = 0;
	
	while ((c = fgetc(f)) != ']' && len < 24)
		*buf++ = c;
		
	*buf = '\0';
}

int do_tag(char* tag)
{
	int i = 0;
	
	while (i < NUM_TAGS) {
		if (!strcmp(tag, tags[i]))
			return i;
		i++;
	}
	
	return -1;
}
