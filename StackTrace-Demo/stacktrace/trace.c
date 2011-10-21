#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "symbols.h"
#include "stack.h"


char fileName[10][50];
int noOfFiles;

int main( int argc, char *argv[] )
{
  FILE *tracef;
  char type;
  unsigned int address,i;
  int flag=0;
  
  if(argc > 2)
  {
  noOfFiles=argc-2;
	for (i=0;i<noOfFiles;i++)
  	{	
		printf("Fname in trace.c is %s",argv[i+2]);
  		strcpy(fileName[i],argv[i+2]);
                printf("copy of Fname in trace.c is %s",fileName[i]);

  	}
  }
  
  initSymbol( argv[1] );
  stackInit();

  tracef = fopen("trace.txt", "r");

  if (tracef == NULL) {
    printf("Can't open trace.txt\n");
    exit(-1);
  }

  while (!feof(tracef)) {

    fscanf( tracef, "%c0x%x\n", &type, &address );

    if        (type == 'E') {

      /* Function Entry */

      addSymbol( address );

      addCallTrace( address );

      stackPush( address );

    } else if (type == 'X') {

      /* Function Exit */

      (void) stackPop();

    }

  }
  if(argc > 2)
  {     
if((strcmp(argv[2],"stack")!=0))  	
inject();
else 
emitSymbols();       
  }
 
  
  fclose( tracef );
  
  return 0;
}

