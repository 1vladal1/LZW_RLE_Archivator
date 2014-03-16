#include <stdlib.h>

#define BITS 12
#define HASHING_SHIFT BITS-8
#define MAX_VALUE (1 << BITS) - 1
#define MAX_CODE MAX_VALUE - 1

#if BITS == 14
  #define TABLE_SIZE 18041
#endif
#if BITS == 13
  #define TABLE_SIZE 9029
#endif
#if BITS <= 12
  #define TABLE_SIZE 5021
#endif

void *malloc();
int *code_value;


unsigned int *prefix_code;
unsigned char *append_character;
unsigned char decode_stack[4000];

void compress(FILE*,FILE*);
void expand(FILE*,FILE*);
int  find_match(int,unsigned int);
char *decode_string(unsigned char*,unsigned int);
unsigned int input_code(FILE*);
void output_code(FILE*,unsigned int);
void mainmenu(int);
void cdlzw(int);

void cdlzw(int cd)
{
FILE *input_file;
FILE *output_file;
FILE *lzw_file;
char input_file_name[81];
struct time t;

 code_value=(int*) malloc(TABLE_SIZE*sizeof(unsigned int));
 prefix_code=(unsigned int*) malloc(TABLE_SIZE*sizeof(unsigned int));
 append_character=(unsigned char*) malloc(TABLE_SIZE*sizeof(unsigned char));
 if (code_value==NULL || prefix_code==NULL || append_character==NULL)
 {
     printf("Fatal error allocating table space!\n");
     mainmenu(-2);
  }



  if (cd==1) {
   printf("Input file to compress: ");
   scanf("%s",input_file_name);
   input_file=fopen(input_file_name,"rb");
   lzw_file=fopen("out.lzw","wb");
    if (input_file==NULL || lzw_file==NULL)
     {
	printf("Fatal error opening files.\n");
	mainmenu(-2);
     }
      t.ti_min=0;
      t.ti_sec=0;
      t.ti_hund=0;
    settime(&t);
    compress(input_file,lzw_file);
    gettime(&t);
    printf("Time of compress: %d:%d:%d\n",t.ti_min, t.ti_sec, t.ti_hund);
    fclose(input_file);
    fclose(lzw_file);
    free(code_value);
    getch();
    mainmenu(-2);
   }
  else
   {
   printf("Input file to Extrac:  ");
   scanf("%s",input_file_name);
   input_file=fopen(input_file_name,"rb");
   lzw_file=fopen("lzw.ext","wb");
   if (input_file==NULL || lzw_file==NULL)
    {
	printf("Fatal error opening files.\n");
	mainmenu(-2);
    };
      t.ti_min=0;
      t.ti_sec=0;
      t.ti_hund=0;
    settime(&t);
    expand(input_file,lzw_file);
    gettime(&t);
    printf("Time of compressed: %d:%d:%d\n",t.ti_min, t.ti_sec, t.ti_hund);
    fclose(lzw_file);
    fclose(output_file);
    free(prefix_code);
    free(append_character);
    getch();
    mainmenu(-2);
   }
}


void compress(FILE *input,FILE *output)
{
unsigned int next_code;
unsigned int character;
unsigned int string_code;
unsigned int index;
int i;
    next_code=256;
    for (i=0;i<TABLE_SIZE;i++)
    code_value[i]=-1;
    i=0;
    printf("Compressing...\n");
    string_code=getc(input);


    while ((character=getc(input)) != (unsigned)EOF)
    {
	index=find_match(string_code,character);
	if (code_value[index] != -1)
	    string_code=code_value[index];
	else
	{
	    if (next_code <= MAX_CODE)
	    {
		code_value[index]=next_code++;
		prefix_code[index]=string_code;
		append_character[index]=character;
	    }
	    output_code(output,string_code);
	    string_code=character;
	}
    }

    output_code(output,string_code);
    output_code(output,MAX_VALUE);
    output_code(output,0);
    printf("\n");
}

int find_match(int hash_prefix,unsigned int hash_character)
{
int index;
int offset;

    index = (hash_character << HASHING_SHIFT) ^ hash_prefix;
    if (index == 0)
	offset = 1;
    else
	offset = TABLE_SIZE - index;
    while (1)
    {
if (code_value[index] == -1)
      return(index);
if (prefix_code[index]==hash_prefix
&&append_character[index]==hash_character)
      return(index);
  index -= offset;
  if (index < 0)
      index += TABLE_SIZE;
    }
}


void expand(FILE *input,FILE *output)
{
unsigned int next_code;
unsigned int new_code;
unsigned int old_code;
int character;
unsigned char *string;
char *decode_string(unsigned char *buffer,unsigned int code);
    next_code=256;
    printf("Expanding...\n");

  old_code=input_code(input);
  character=old_code;
  putc(old_code,output);

    while ((new_code=input_code(input)) != (MAX_VALUE))
    {


	if (new_code>=next_code)
	{
	    *decode_stack=character;
	    string=decode_string(decode_stack+1,old_code);
	}


	else
	    string=decode_string(decode_stack,new_code);

	character=*string;
	while (string >= decode_stack)
	    putc(*string--,output);


	if (next_code <= MAX_CODE)
	{
	    prefix_code[next_code]=old_code;
	    append_character[next_code]=character;
	    next_code++;
	}
	old_code=new_code;
    }
    printf("\n");
}



char *decode_string(unsigned char *buffer,unsigned int code)
{
int i;

    i=0;
    while (code > 255)
    {
	*buffer++ = append_character[code];
	code=prefix_code[code];
	if (i++>=4094)
	{
	    printf("Fatal error during code expansion.\n");
	    getch();
	    mainmenu(-2);

	}
    }
    *buffer=code;
    return(buffer);
}

unsigned int input_code(FILE *input)
{
unsigned int return_value;
static int input_bit_count=0;
static unsigned long input_bit_buffer=0L;
    while (input_bit_count <= 24)
    {
input_bit_buffer|=(unsigned long)getc(input)<<(24-input_bit_count);
input_bit_count += 8;
    }
    return_value=input_bit_buffer >> (32-BITS);
    input_bit_buffer <<= BITS;
    input_bit_count -= BITS;
    return(return_value);
}
void output_code(FILE *output,unsigned int code)
{
static int output_bit_count=0;
static unsigned long output_bit_buffer=0L;
output_bit_buffer|=(unsigned long)code<<(32-BITS-output_bit_count);
    output_bit_count += BITS;
    while (output_bit_count >= 8)
    {
	putc(output_bit_buffer >> 24,output);
	output_bit_buffer <<= 8;
	output_bit_count -= 8;
    }
}