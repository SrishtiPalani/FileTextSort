//A sorting program that will be used to sort lines of text
//Collaborated with Vickie, Moe
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//the maximum line length is 128 as specified 			
int maxLineLength = 128;
//the word number used as the key to sort upon
int wordNum = 0;
//we allocate 100 lines to start - we reallocate later if we need more			
int lineAllocation = 100; 
//comparison funtion
int wordcompare(const void *string1, const void *string2) {

	//convert to char pointers - cast
	string1 = * (char **) string1;
	//allocated memory for the line to be stored in an array
	char * str1copy = (char *) malloc(strlen(string1) + 1);
	//and we create a copy of the original string line 1 for later use because strtok is destructive
	strcpy(str1copy, string1);
	//chop lines into words using strtok by space
	char * tokenized1 = strtok((char *) str1copy, " ");
	//allocate memory in a char array for 128 words in the line
	char * str1[maxLineLength];
	//allocate memory for the first word to be stored in
	str1[0] = (char *) malloc(strlen(tokenized1) + 1);
	strcpy(str1[0], tokenized1);

	
	//initialize the pointer to the word that we will be comparing
	char *word1 = NULL;
	int i = 1; 
	//if the word we're on currently is less than the word we have to sort by 
	//and there are still tokens in the line to be read
	while((i <= wordNum) && (tokenized1 != NULL)) {
		//in order to get next token and to continue with the same string NULL 
		//is passed as first argument since strtok maintains a static pointer to your previous passed string.
		tokenized1 = strtok(NULL, " ");
		//if the next word is not null i.e. there is still line left to be read
		if (tokenized1 != NULL) {
			//allocate memory & set it to continue looking for the word to compare with
			str1[i] = (char *) malloc(strlen(tokenized1) + 1);
			strcpy(str1[i], tokenized1);
		}
		//if the next word is not null i.e. we're at the end of the line 
		else {
			//we use the previous word as the word to compare with instead
			//using strdup instead of strcpy because word1 is a pointer
			word1 = strdup(str1[i - 1]);
			
		}
		i++;
	}

	//We traversed the entire line and we did not find the word that we were to sort on
	//just use the last word on the line
	if (word1 == NULL) {
		word1 = strdup(tokenized1);
	}

	//Similar to how we created a copy of the line, chopped it up by spaces, and set the word to compare by
	//we do the same for the second word to compare by
	string2 = * (char **) string2;
	char * str2copy = (char *) malloc(strlen(string2) + 1);
	strcpy(str2copy, string2);
	char * tokenized2 = strtok((char *) str2copy, " ");
	char * str2[maxLineLength];
	
	char * word2 = NULL;
	str2[0] = (char *)malloc(strlen(tokenized2) + 1);
	strcpy(str2[0], tokenized2);
	int j = 1; 

	while((j <= wordNum) && (tokenized2 != NULL)) {
		tokenized2 = strtok(NULL, " ");
		if (tokenized2 != NULL) {
			str2[j]=(char *)malloc(strlen(tokenized2) + 1);
			strcpy(str2[j], tokenized2);
		}
		else {
			word2 = strdup(str2[j - 1]);}
		j++; 
	}

	if (word2 == NULL) {
		word2 = strdup(tokenized2);
	}

	//printf ("The word1 is: %s\n", word1); 
	//printf("The word2 is: %s\n", word2);
	return strcmp(word1, word2);
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
	for (line = 0; line <= lines; line++)
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




