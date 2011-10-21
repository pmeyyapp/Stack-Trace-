							    Stack Trace utility

							   Palaniappan Meyyappan
							        Tony Mathew											




You need to have linux installed with graphviz to run stacktrace tool.
Follow below steps to use stacktrace.
1)go to directory stacktrace 
cd stacktrace
make clean
make
sudo make install
2) go to the directory in which the code to be tested is available.
3)copy the script stacktrace_script.sh to this directory.
4)run this script .When it asks for filename enter the full filename(eg:calc.c)
5)It automatically builds the code file and gives you a callgraph with node names
as function names appended with stack size.
6)Also in the current directory you will find stacksize.txt which lists functions
which are recursive and the stack consumed by those functions.




//Assumption.
the code follows the syntax of function definition as
datatype funcname(parameters....)
{
function body;
}


Example:
The package contains two code files permute.c,calc.c which can be built using stacktrace
Upon completion you will get the callgraph and stack related details.

Currently the script can support only one code file running.If you have more than one 
file commands have to be typed on to the command line similar to script content.
