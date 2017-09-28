#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int wordNum = 0;						//we use a global variable for the word number parameter
											//so we can use it in our string comparison function
int maxLineLength = 129;			//one extra character for a line end
int lineAllocation = 100;											//we allocate 100 lines to start - we reallocate later if we need more

//comparison funtion
int wordcompare(const void *a, const void *b) {

	//we need to convert our comparison function inputs to char pointers
	char *string1 = *(char **) a;
	char *string2 = *(char **) b;

	//and we need to allocate some memory for variables to copy the strings to
		//this is so that we can tokenize iA and iB without changing them
	char * str1copy = (char *) malloc(strlen(string1) + 1);
	//make sure to make a copy of the original string line 1 for later use.
	strcpy(str1copy, string1);
	char * str2copy = (char *) malloc(strlen(string2) + 1);
	//make sure to make a copy of the original string line 2 for later use.
	strcpy(str2copy, string2);

	//now we tokenize string A and create a string (strA) where we keep a substring
		//of it - we start by just the first word
	char * tokenA = strtok((char *) str1copy, " ");
	char * strA[129];
	strA[0] = (char *) malloc(strlen(tokenA) + 1);
	strcpy(strA[0], tokenA);
	char * wordToCompareA = NULL;
	int i = 1; 

	//and then we add a number of words so that the number of words in our substring
		//is equal to the numeric parameter passed in
	while((i <= wordNum) && (tokenA != NULL)) {
		//we set tokenA to the next word
		tokenA = strtok(NULL, " ");
		//if we're not at the end of the current line yet, we add a word to strA
			//and continue looping, looking for the word to compare
		if (tokenA != NULL) {
			strA[i] = (char *) malloc(strlen(tokenA) + 1);
			strcpy(strA[i], tokenA);
			//strA[i] = strdup(tokenA);
		}
		//if we're at the end of the current line, we store the last word on the
			//current line and use that as the word to compare
		else {
			//using strdup instead of strcpy because wordToCompareA is a pointer to a character
			wordToCompareA = strdup(strA[i - 1]);
			break;
		}
		i++;
	}

	//if we still don't have a word to compare, we use the last token we were on
	if (wordToCompareA == NULL) {
		wordToCompareA = strdup(tokenA);
	}

	//and then we do the same thing for the string we're comparing with
	char * tokenB = strtok((char *) str2copy, " ");
	char * strB[129];
	char * wordToCompareB = NULL;

	strB[0] = (char *)malloc(strlen(tokenB) + 1);
	strcpy(strB[0], tokenB);
	int j = 1; 

	while((j <= wordNum) && (tokenB != NULL)) {
		tokenB = strtok(NULL, " ");
		if (tokenB != NULL) {
			strB[j] = (char *)malloc(strlen(tokenB) + 1);
			strcpy(strB[j], tokenB);
		}
		else {
			wordToCompareB = strdup(strB[j - 1]);
			break;
		}
		j++; 
	}

	if (wordToCompareB == NULL) {
		wordToCompareB = strdup(tokenB);
	}

	return strcmp(wordToCompareA, wordToCompareB);
}


int main(int argc, char **argv)
{
	//initialize the input file 
	char *input_file = NULL;
	if (argc == 2) {
		//if there are only 2 parameters then the 2nd command line arg
		//is the file name 
		printf("file: %s\n", argv[1]);
		input_file = argv[1];
	}
	else if (argc == 3) {
		//if there are 3 command line parameters then the 2rd arg
		//is the word key to be sorted upon
		//and the 3rd arg is the filename
		wordNum = -atoi(argv[1]);
		if (wordNum < 0) {// we convert the character entry into an integer if its less than 0
			
			printf("file: %s\n", argv[2]);
			printf("wordNum: %d. Please check it fits the format of a dash followed by a number.\n", wordNum); 
		}
		wordNum = wordNum - 1;
		//if it's -1 then you sort on the first word
		if (wordNum == -1){ 
		//Since we are -1 from the wordNum
		//if we dont have a word to sort by - then it would be initialized to 0 
		//therefore, it would be -1: we want it to know that its to sort by the 0th word
			wordNum = 0;
			}

		else{
			input_file = argv[2];
			printf("file: %s\n", argv[2]);
		}
	}
	else {//if there are are not 2 or 3 parameters, then we must either be missing or have too many arguments
		fprintf(stderr, "Error: Bad command line parameters\n");
		exit(1);
	}

	if (input_file == NULL) {//if there is nothing in the input file or no such file exists
	  fprintf(stderr, "Error: Bad command line parameters.\n");
	  exit(1);
	}

	FILE *in_file = fopen(input_file, "r");
	if (in_file == NULL) {
	  fprintf(stderr, "Error: Cannot open file %s\n", input_file);
	  //fprintf(stderr, "Error: Bad command line parameters\n");
		exit(1);
	}
//*************
	
	char **textArray = (char **) malloc(sizeof(char*) * lineAllocation);		//allocating a pointer to pointers of char - will be used as
																					//an array of text with one entry per line of text
	if (textArray == NULL)
	//the data is too big and memory could not be allocated
	{
		fprintf(stderr, "malloc failed.\n");
		exit(1);
	}

	int line=0;
	while(1)												//rather than get the number of lines in the file in one pass and increment
	{																				//line that many times, we just keep incrementing it until fgets gives
	  int lineLength = 0;																//us NULL - then we know we're done

		if (line >= lineAllocation)
		{
			int moreLineAllocation = lineAllocation * 2;							//if it turns out that we didn't allocate enough lines,
																						//double our allocation and try again
			textArray = (char **) realloc(textArray, sizeof(char*) * moreLineAllocation);

			if (textArray == NULL)
			{
				fprintf(stderr, "malloc failed.\n");
				exit(1);
			}

			lineAllocation = moreLineAllocation;
		}

		textArray[line] = (char *) malloc(maxLineLength);							//allocate memory for the current line

		if (textArray[line] == NULL)
		{
			fprintf(stderr, "malloc failed.\n");
			exit(1);
		}

		if (fgets(textArray[line], 100000, in_file) == NULL)					//break out of the loop if we're at the end of the file
		{
			free(textArray[line]);
			textArray[line] = NULL;
			break;
		}

		if (strlen(textArray[line]) > 128) {
		  fprintf(stderr, "Line too long\n"); 
		  exit(1);
		}

		//strip line feeds and carriage returns from the end of lines and replace them with null chars so the tokenizer will work
		for (lineLength = strlen(textArray[line]) - 1; lineLength >= 0 && ((textArray[line][lineLength] == '\n') || (textArray[line][lineLength] == '\r')); lineLength--)
		{
			textArray[line][lineLength] = '\0';
		}
		line++; 
	}

	fclose(in_file);								//We've got what we need in an array now, so we close the file

	int lines = line - 1;					//we broke earlier because the line number was 1 too high
	
	line = 0;


	printf("%s\n", "Original File:\n");
	for (line = 0; line <= lines; line++)
	{
		printf("%s\n", textArray[line]);
	}

	qsort(textArray, lines + 1, sizeof(textArray), wordcompare);

	line = 0;

	printf("\n%s\n", "\n----------\n");
	printf("%s%d%s\n", "File sorted on word #", (wordNum + 1),":\n");
	for (line = 0;
 line <= lines; line++)
	{
		printf("%s\n", textArray[line]);
	}

	line = 0;

	//we've printed the sorted array - now we can free the memory
	for (line = 0; line <= lines; line++)
	{
		free(textArray[line]);
		textArray[line] = NULL;
	}
	free(textArray);

    return 0;
}




