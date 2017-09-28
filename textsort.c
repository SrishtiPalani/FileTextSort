#include <stdio.h> 
#include <stdlib.h>

int wordNum;	
#define maxLineLength 128;	
int lineNum; 
int main( int argc, char ** argv)
{ 
	char *input_file = NULL;

 	if (argc == 2) {
		printf("file: %s\n", argv[1]);
		input_file = argv[1];
	}
	else if (argc == 3){ 
		if (atoi(argv[1]) < 0) {
			wordNum = atoi(argv[1]);
			printf("file: %s\n", argv[2]);
			printf("wordNum: %d. Please enter a number greater than 0.\n", wordNum); 
		}
		input_file = argv[2];
		printf("file: %s\n", argv[2]);
		printf("You want to sort on word: %d\n", wordNum); 
	}
	else {
		fprintf(stderr, "Error: Bad command line parameters\n");
		exit(1);
	}
	if (input_file == NULL) {
	  fprintf(stderr, "Error: Bad command line parameters\n");
	  exit(1);
	}

	FILE *in_file = fopen(input_file, "r");
	if (in_file == NULL) {
	fprintf(stderr, "Error: Empty file. Please check. %s\n", input_file);
	exit(1);
	}

	char **text = (char **) malloc(sizeof(char*) * lineNum);		//allocating a pointer to pointers of char - will be used as
																					//an array of text with one entry per line of text
	if (text == NULL)
	{
		fprintf(stderr, "Memory Allocation failed.\n");
		exit(1);
	}

	int lines =0;
	int ch =0; 
	lines++; 
	while ((ch = fgetc(in_file)) != EOF)
    {
      if (ch == '\n')
    lines = lines+ 1;
    }
    printf("There are %d lines in this file\n", lines);


	for (int line = 0; line < lines; line++) {
	 	 int lineLength = 0;			

		if (line >= lineNum)//we've run out the memory that we allocated so we double our allocated memory
		{
			int morelineNum = lineNum * 2;						
																						
			text = (char **) realloc(text, sizeof(char*) * morelineNum);

			if (text == NULL)
			{
				fprintf(stderr, "Memory Allocation for Text failed.\n");
				exit(1);
			}

			lineNum = morelineNum;
		}
return 0;
}