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
return 0;
}