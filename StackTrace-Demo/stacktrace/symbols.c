#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "stack.h"
#include "symbols.h"

func_t       functions[MAX_FUNCTIONS];
unsigned int totals[MAX_FUNCTIONS];
unsigned int calls[MAX_FUNCTIONS][MAX_FUNCTIONS];

char imageName[50];


//initialise the fuction datastructure with values to zero.
void initSymbol( char *image )
{
  int from, to;

  strcpy( imageName, image );

  for ( from = 0 ; from < MAX_FUNCTIONS ; from++ ) {

    functions[from].address = 0;
    functions[from].funcName[0] = 0;
    totals[from] = 0;
    functions[from].stacksize=0;
    functions[from].closed=55;
    for ( to = 0 ; to < MAX_FUNCTIONS ; to++ ) {

      calls[from][to] = 0;

    }

  }

  return;
}


//returns the index of the function correspronding to address in the functions[] array.
int lookupSymbol( unsigned int address )
{
  int index;

  for (index = 0 ; index < MAX_FUNCTIONS ; index++) {

    if (functions[index].address == 0) break;

    if (functions[index].address == address) return index;

  }

  assert(0);

  return 0;
}

//get the name of the function corresponding to address.This is performed by using addr2line
int translateFunctionFromSymbol( unsigned int address, char *func )
{
  FILE *p;
  char line[100];
  int  len, i;
  //encode the buffer with the command to be executed. -e executable name, -f to give functionname. -s basename.
  sprintf( line, "addr2line -e %s -f -s 0x%x", imageName, address );

  //popen will execute the command mentioned in line and returns a readable end of pipe between the child process onto which the output stream is written to.
  p = popen( line, "r" );

  if (p == NULL) return 0;
  else {

    len = fread( line, 99, 1, p );

    i = 0;
    //read the function name character by character until new line or carriage return.
    while ( i < strlen(line) ) {
      
      if ((line[i] == 0x0d) || (line[i] == 0x0a)) {
        func[i] = 0;
        break;
      } else {
        func[i] = line[i];
      }

      i++;

    }

    pclose(p);

  }

  return 1;
}

//add a new address to the functions[] .update the name also.
void addSymbol( unsigned int address )
{
  int index;

  for (index = 0 ; index < MAX_FUNCTIONS ; index++) {

    if (functions[index].address == address) return;

    if (functions[index].address == 0) break;

  }

  if (index < MAX_FUNCTIONS) {

    functions[index].address = address;

    translateFunctionFromSymbol( address, functions[index].funcName );

  } else {

    assert( 0 );

  }

  return;
}

//function called when in trace.txt a new entry of function is found.
void addCallTrace( unsigned int address )
{

  if (stackNumElems()) {
    //updates the calls[][] matrix to denote the number of calls.
    calls[lookupSymbol(stackTop())][lookupSymbol(address)]++;
  }

  return;
}

// Function to create a Dot file with graphs, nodes and edges

void emitSymbols( void )
{
  int from, to,count=0,stacktemp=0;
  FILE *fp,*fp_tmp,*my_fp;
  char str[100],functemp[100];
  char *pch=(char*)malloc(100);
  fp_tmp=fopen("stacktrace_tmp.txt","r"); 
  fp = fopen("graph.dot", "w");
  my_fp=fopen("stacksize.txt","w");
  if (fp == NULL) {
    printf("Couldn't open graph.dot\n");
    exit(0);
  }

  fprintf(fp, "digraph %s {\n\n", imageName );

fgets(str,500, fp_tmp); 
while (!feof(fp_tmp))
  {
  pch = strtok (str,"-\n");
  count=0;
  while(pch!= NULL)
  {
   count++;
   if(count==1)
   {
   strcpy(functemp,pch);

   }
   if(count==2)
   {
   stacktemp=atoi(pch);
   }

pch = strtok (NULL, "-\n");
   }


for (from = 0 ; from < MAX_FUNCTIONS ; from++) 
{
   if (functions[from].address == 0) break;
   if((strcmp(functemp,functions[from].funcName) == 0))
	{
          
	  if(functions[from].stacksize < stacktemp)
          functions[from].stacksize=stacktemp;
	  	 	
	}

}
fgets(str,500, fp_tmp);     
}    

for (from = 0 ; from < MAX_FUNCTIONS ; from++) 
{
  if (functions[from].address == 0) break;
  sprintf(functions[from].funcName,"%s_%d",functions[from].funcName,functions[from].stacksize);  
 
}

  /* Identify node shapes */
  for (from = 0 ; from < MAX_FUNCTIONS ; from++) {

    if (functions[from].address == 0) break;
    if(strstr(functions[from].funcName,"stacktrace_foo")!=NULL) break;
    for (to = 0 ; to < MAX_FUNCTIONS ; to++) {

      if(strstr(functions[to].funcName,"stacktrace_foo")==NULL)
         if (calls[from][to]) totals[from]++;

    }
    
    

if (totals[from]) {

      fprintf( fp, "  %s [shape=rectangle]\n", functions[from].funcName );

    } else {

      fprintf( fp, "  %s [shape=ellipse]\n", functions[from].funcName );

    }

  }


  /* Emit call graph */
  for (from = 0 ; from < MAX_FUNCTIONS ; from++) {

    if (functions[from].address == 0) break;

    for (to = 0 ; to < MAX_FUNCTIONS ; to++) {
      if(strstr(functions[to].funcName,"stacktrace_foo")==NULL)
      {
      	if (calls[from][to]) {
                fprintf( fp, "  %s -> %s [label=\"%d calls\" fontsize=\"10\"]\n", 
                  functions[from].funcName, functions[to].funcName,
                  calls[from][to] );
		if((from==to) && (calls[from][to]!=0))
		{
		fprintf(my_fp,"Stack size of the function %s is %d",functions[from].funcName,(calls[from][to]*functions[from].stacksize));	
		}      	
	}
      }
      if (functions[to].address == 0) break;

    }
}
  
  fprintf( fp, "\n}\n" );
  
  fclose(fp);
  fclose(fp_tmp);
  fclose(my_fp);  	
  return;
}

