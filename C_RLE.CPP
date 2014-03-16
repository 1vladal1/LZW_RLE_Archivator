#include <memory.h>
#include <stdlib.h>

/* Error codes sent to the caller */
#define NO_ERROR      0
#define BAD_FILE_NAME 1
#define BAD_ARGUMENT  2

/* Useful constants */
#define FALSE 0
#define TRUE  1

/* Global variables */
FILE *source_file,*dest_file;



/* Pseudo procedures */
#define beginning_of_data()  (byte_stored_status=FALSE,(void)rewind(source_file))
#define end_of_data()  (byte_stored_status?FALSE:!(byte_stored_status=((val_byte_stored=fgetc(source_file))!=EOF)))
#define read_byte()  (byte_stored_status?byte_stored_status=FALSE,(unsigned char)val_byte_stored:(unsigned char)fgetc(source_file))
#define write_byte(byte)  ((void)fputc((byte),dest_file))

void rle2write_rep(unsigned char,unsigned char,unsigned int);
void rle2write_non_rep(unsigned char,unsigned char);
void rle2encoding(void);
void mainmenu(int);
struct time t;
int byte_stored_status=FALSE;
int val_byte_stored;

void rle2write_rep(header_byte,repeated_byte,repetition_number)

unsigned char header_byte,repeated_byte;
unsigned int repetition_number;
{ if (repetition_number<4)
     if (repeated_byte==header_byte)
	{ write_byte(header_byte);
	  write_byte(repetition_number-1);
	  if (repetition_number==3)
	     write_byte(header_byte);
	}
     else { register unsigned int i;

	    for (i=1;i<=repetition_number;i++)
		write_byte(repeated_byte);
	  }
  else { write_byte(header_byte);
	 write_byte(repetition_number-1);
	 write_byte(repeated_byte);
       }
}

void rle2write_non_rep(header_byte,non_repeated_byte)

unsigned char header_byte,non_repeated_byte;
{ if (non_repeated_byte==header_byte)
     { write_byte(header_byte);
       write_byte(0);
     }
  else write_byte(non_repeated_byte);
}

void rle2encoding()
{int byte_stored_status=FALSE;
int val_byte_stored;
 unsigned char byte1,byte2,header_byte;
  unsigned int frame_size;
  register unsigned int i;
  unsigned long int lookup_table[256];

  if (!end_of_data())        /* Is there at least a byte to analyze? */
     {                       /* Sets up the occurrence numbers of all bytes to 0 */
       (void)memset((char *)lookup_table,0,sizeof(lookup_table));
			     /* This is the same to fill 'lookup_table' to 0.
				It's fastest than to loop 256 times */
       while (!end_of_data())/* Valids the occurrences in 'lookup_table' in regard to the data to compress */
	     { byte1=read_byte();
	       lookup_table[byte1]++;
	     }
       header_byte=0;
       for (i=1;i<=255;i++)
	   if (lookup_table[i]<lookup_table[header_byte])
	      header_byte=i;
       write_byte(header_byte);
       beginning_of_data();  /* New data analysis */
       byte1=read_byte();
       frame_size=1;
       if (!end_of_data())
			     /* Are there at least two bytes? */
	  { byte2=read_byte();
	    frame_size=2;
	    do {             /* Real beginning of the compression */
		 if (byte1==byte2)
			     /* Do we meet only a sequence of identical bytes? */
		    { while ((!end_of_data())&&(byte1==byte2)&&(frame_size<256))
			    { byte2=read_byte();
			      frame_size++;
			    }
		      if (byte1==byte2)
			 { rle2write_rep(header_byte,byte1,frame_size);
			   if (!end_of_data())
			      { byte1=read_byte();
				frame_size=1;
			      }
			   else frame_size=0;
			 }
		      else { rle2write_rep(header_byte,byte1,frame_size-1);
			     byte1=byte2;
			     frame_size=1;
			   }
		    }
		 else {      /* No, then don't handle the last byte */
			rle2write_non_rep(header_byte,byte1);
			byte1=byte2;
			frame_size=1;
		      }
		 if (!end_of_data())
		    { byte2=read_byte();
		      frame_size=2;
		    }
	       }
	    while ((!end_of_data())||(frame_size>=2));
	  }
       if (frame_size==1)  /* Was there a last byte to analyze? */
	  rle2write_non_rep(header_byte,byte1);
     }
}


void Crle(char* in,char* out)
{
  if ((source_file=fopen(in,"rb"))==NULL)
	  { printf("File not found...");
	    getch();
	    mainmenu(-2);
		  }
       else if ((dest_file=fopen(out,"wb"))==NULL)
	       { printf("File not found fot output...");
		 getch();
		 mainmenu(-2);
	       }
	    else { rle2encoding();
		   fclose(source_file);
		   fclose(dest_file);
		 }
}
void C_rle()
{

char file[81];
int n,i;
 t.ti_min=0;
 t.ti_sec=0;
 t.ti_hund=0;
printf("Input file name to compress:");
scanf("%s",file);
printf("         Compressed\n\n       Please wait...\n");
settime(&t);
Crle(file,"out.rle");
gettime(&t);
printf("Time of compressed: %d:%d:%d\n",t.ti_min, t.ti_sec, t.ti_hund);
getch();
mainmenu(-2);
}
void rle2decoding()

{ unsigned char header_byte,byte_read,byte_to_repeat;
  register unsigned int i;

  if (!end_of_data())
     { header_byte=read_byte();
       do {                  /* Being that header byte is present, then there are bytes to decompress */
	    if ((byte_read=read_byte())==header_byte)
	       { byte_read=read_byte();
		 if (byte_read<3)
		    for (i=0;i<=byte_read;i++)
			write_byte(header_byte);
		 else { byte_to_repeat=read_byte();
			for (i=0;i<=byte_read;i++)
			    write_byte(byte_to_repeat);
		      }
	       }
	    else write_byte(byte_read);
	  }
       while (!end_of_data());
     }
}


void D_rle(void)
{
  char file[81],outf[81];


printf("Input file name to Extract:");
scanf("%s",file);
printf("         Extracting\n\n       Please wait...\n");
 t.ti_min=0;
 t.ti_sec=0;
 t.ti_hund=0;
settime(&t);

   if ((source_file=fopen(file,"rb"))==NULL)
	  { printf("File not found...");
	    getch();
	    mainmenu(-2);
	   }
       else if ((dest_file=fopen("rle.ext","wb"))==NULL)
	       { printf("Error to create output file...");
		 getch();
		 mainmenu(-2);
	       }
	    else { rle2decoding();
		   fclose(source_file);
		   fclose(dest_file);
		 }
		 gettime(&t);
printf("Time of Extracting: %d:%d:%d\n",t.ti_min, t.ti_sec, t.ti_hund);
getch();
mainmenu(-2);
}

