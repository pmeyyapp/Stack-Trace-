#include <stdio.h>
#include <string.h>
#include "stack.h"
#include "symbols.h"

extern func_t functions[MAX_FUNCTIONS];
extern char fileName[10][50];
extern int noOfFiles;
static int firstTime=0;
int inject()
{
	
	int j,i;
	
	for(j=0;j<noOfFiles;j++)
	{	
		firstTime=1;
		for(i=0;functions[i].address!=0;i++)
		{
	
			injectCode (fileName[j],i);
		}
		
	}
}
int injectCode (char* argv,int i)
{
  char str[500];
  char * pch,*line;
  char *fname=(char*)malloc(strlen(argv));
  FILE *fp,*fp_tmp;
  int position,flag,opencount=0;
  
  strcpy(fname,argv);
  flag=0;
  fp = fopen(fname, "r+");
  fp_tmp=fopen("tmp.txt","w+");
  fgets(str,500, fp); 
  while(! feof(fp))
  {


  line=(char*)malloc(strlen(str));
  strcpy(line,str);
  
  //have to defer writing until check wehter its func exit.
  if((strchr(line, '}') == NULL) && (strstr(line, "return") == NULL))
  fputs(line,fp_tmp);

  if((strstr(line,"<stdio.h>")!=NULL)&&(firstTime==1))
  {	

  	firstTime=0;
  	fputs("FILE *stacktrace_fp;\n",fp_tmp);
  	fputs("int stacktrace_foo()\n",fp_tmp);
  	fputs("{\n",fp_tmp);
  	fputs("int dummy;\n",fp_tmp);
  	fputs("return &dummy;\n",fp_tmp);
  	fputs("}\n",fp_tmp);
  }
  pch = strtok (str," (),.-\n");
  position=1;
  if(strstr(line, "return") != NULL)
  {

  	if((flag==2) && (functions[i].closed == 0))
  	{	
  		flag=0;
  		functions[i].closed = 1;   //a new flag which denotes when a return is found.
		
  		fprintf(fp_tmp,"int sai_b=(int)stacktrace_foo();\n");  
  		char *buf1="fprintf(stacktrace_fp,\"";
  		char *buf2="-%d\\n\",(int)&sai_a-sai_b);\n";
  		fprintf(fp_tmp,"%s%s%s",buf1,functions[i].funcName,buf2);
  		if((strcmp(functions[i].funcName,"main")==0))
  		{
  			fputs("fclose(stacktrace_fp);\n",fp_tmp); 
  		}
  	 
	}
fputs(line,fp_tmp); 
  }	
	
  while (pch != NULL)
  	{
	
      	if(strchr(pch, '{') != NULL) //increments all curly counter at all instances.
      	{
      		opencount++;
		
	}
      	//check whether its starting of function def. assumes the opening braces 
      	//of func is on different line
      	if((strcmp(pch,functions[i].funcName) == 0) && (strchr(line, ';') == NULL) && (position ==2))
      	{
      		flag=1;
        	position++;
        	opencount=0;
        	functions[i].closed=0;
		
		break;
      	}
      
      	//this is the func entering place.
      	if((flag==1)&&(strcmp(pch,"{") == 0))
      	{
	
      		//if it is main the app has to open a file to be shared with stacktrace.
      		if(strcmp(functions[i].funcName,"main")==0)
      		{
  				
  				fputs("stacktrace_fp=fopen(\"stacktrace_tmp.txt\",\"w+\");\n",fp_tmp);  				
      		}
      		fputs("int sai_a;\n",fp_tmp);
      		flag=2;
		
      		break;
      	}
      	//check whether } denotes func exit.assumes tat we dont have {} on same line
      	if(strchr(pch, '}') != NULL)
      	{
      		opencount--;
		
      		if((opencount==0) && (flag==2) && (functions[i].closed == 0))
      		{
      			flag=0;
      			functions[i].closed = 1;
//      			fputs("int sai_b;\n",fp_tmp);
      			fprintf(fp_tmp,"int sai_b=(int)stacktrace_foo();\n");  
  				char *buf1="fprintf(stacktrace_fp,\"";
      			char *buf2="-%d\\n\",(int)&sai_a-sai_b);\n";
      			fprintf(fp_tmp,"%s%s%s",buf1,functions[i].funcName,buf2);
      			if((strcmp(functions[i].funcName,"main")==0))
      			{
      				fputs("fclose(stacktrace_fp);\n",fp_tmp); 
      			}
      		
      		}
		
			fputs(line,fp_tmp);     		
      		         break;
      	}
    	pch = strtok (NULL, " (),.-\n");
    	position++;
  	}
  	
  fgets(str,500, fp); 
  }
  fseek ( fp_tmp , 0 , SEEK_SET );
  fseek ( fp , 0 , SEEK_SET );

fgets(str,500,fp_tmp);
  while(!feof(fp_tmp)) 
  {

  fputs(str,fp);
  fgets(str,500,fp_tmp);
  }
  fclose(fp);
  fclose(fp_tmp);
  free(line);
  return 0;
}

