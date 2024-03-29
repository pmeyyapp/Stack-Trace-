#ifndef __SYMBOLS_H
#define __SYMBOLS_H

#define MAX_FUNCTIONS		200
#define MAX_FUNCTION_NAME	50

typedef struct {
  unsigned int address;
  char funcName[MAX_FUNCTION_NAME+1];
  int closed;	
  int stacksize;
} func_t;


void initSymbol( char *imageName );

int lookupSymbol( unsigned int address );

void addSymbol( unsigned int address );

void addCallTrace( unsigned int address );

void emitSymbols( void );

#endif /* __SYMBOLS_H */
