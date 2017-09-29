//A sorting program that will be used to sort lines of text
//Collaborated with Vickie, Moe
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//the maximum line length is 128 as specified 			
int maxLineLength = 128;
//the word number used as the key to sort upon
int sortByNum = 0;
//we allocate 100 lines to start - we reallocate later if we need more			
int maxLines = 100; 

int wordCompare(const void *string1, const void *string2) {

	//convert to char pointers - cast
	string1 = * (char **) string1;
	//allocated memory for the line to be stored in an array
	char * str1copy = (char *) malloc(strlen(string1) + 1);
	//and we create a copy of the original string line 1 for later use because strtok is destructive
	strcpy(str1copy,string1);
	
	//chop lines into words using strtok by space
	char * chopped1 = strtok((char *) str1copy, " ");
	//allocate memory in a char array for 128 words in the line
	char * str1[maxLineLength];
	//allocate memory for the first word to be stored in

	str1[0] = (char *) malloc(strlen(chopped1) + 1);
	//strcpy(str1[0], chopped1);
	str1[0] = chopped1;
	
	//initialize the pointer to the word that we will be comparing
	char *word1 = NULL;
	int i = 1; 
	//if the word we're on currently is less than the word we have to sort by 
	//and there are still tokens in the line to be read
	while((i <= sortByNum) && (chopped1 != NULL)) {
		//in order to get next token and to continue with the same string NULL 
		//is passed as first argument since strtok maintains a static pointer to your previous passed string.
		chopped1 = strtok(NULL, " ");
		//if the next word is not null i.e. there is still line left to be read
		if (chopped1 != NULL) {
			//allocate memory & set it to continue looking for the word to compare with
			str1[i] = (char *) malloc(strlen(chopped1) + 1);
			str1[i] = chopped1; 
		}
		//if the next word is not null i.e. we're at the end of the line 
		else {
			//we use the previous word as the word to compare with instead
			//using strdup instead of strcpy because word1 is a pointer
			char * memword1 = (char *) malloc(strlen(str1[i - 1])); 
			strcpy(memword1,str1[i - 1]);
			
			word1 = (char *) memword1;
		}
		// //if the next word is not null i.e. we're at the end of the line
		// //we use the previous word as the word to compare with instead
		// /*else 
		// 	word1 = strdup(str1[i - 1]);*/

		i++;
	}
	

	//We traversed the entire line and we did not find the word that we were to sort on
	//just use the last word on the line
	if (word1 == NULL) {
		char * word1mem = (char *) malloc(strlen(chopped1) + 1); 
		strcpy(word1mem, chopped1);
		word1 = (char *) word1mem; 
	}
	//Similar to how we created a copy of the line, chopped it up by spaces, and set the word to compare by
	//we do the same for the second word to compare by
	string2 = * (char **) string2;
	char * str2copy = (char *) malloc(strlen(string2) + 1);
	strcpy(str2copy,string2);
	
	char * chopped2 = strtok((char *) str2copy, " ");
	char * str2[maxLineLength];
	
	char * word2 = NULL;

	str2[0] = (char *)malloc(strlen(chopped2) + 1);
	str2[0] = chopped2;
	
	int j = 1; 

	while((j <= sortByNum) && (chopped2 != NULL)) {
		chopped2 = strtok(NULL, " ");
		if (chopped2 != NULL) {
			str2[j]=(char *)malloc(strlen(chopped2) + 1);
			str2[j]=chopped2;
		}
		else {
			
			char * memword2 = (char *) malloc(strlen(str2[j - 1])); 
			strcpy(memword2, str2[j - 1]);
			word2 = (char *) memword2;
			//word2 = strdup(str2[j - 1]); 
		}

		j++; 
	}

	if (word2 == NULL) {
		char * word2mem = (char *) malloc(strlen(chopped2) + 1); 
		strcpy(word2mem, chopped2);
		word2= (char *) word2mem; 
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
		sortByNum = -atoi(argv[1]);
		if (sortByNum < 0) {// we convert the character entry into an integer if its less than 0	
			printf("file: %s\n", argv[2]);
			printf("sortByNum: %d. Please check it fits the format of a dash followed by a number.\n", sortByNum); 
		}
		sortByNum = sortByNum - 1;
		//if it's -1 then you sort on the first word
		//Since we are -1 from the sortByNum
		//if we dont have a word to sort by - then it would be initialized to 0 
		//therefore, it would be -1: we want it to know that its to sort by the 0th word
		if (sortByNum == -1)
			sortByNum = 0;

		else {
			input_file = argv[2];
			printf("file: %s\n", argv[2]);
		}
	}
	else {//if there are not 2 or 3 parameters, then we must either be missing or have too many arguments
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
		exit(1);
	}
	
	char **textArray = (char **) malloc(sizeof(char*) * maxLines);	
	
	//verify malloc was successful
	if (textArray == NULL)
	{
		fprintf(stderr, "Error: Memory allocation failed for textArray.\n");
		exit(1);
	}

	int line = 0;
	while(1)
	{
		if (line >= maxLines)
		{						
			//if we need more memory, double our orginal memory alloc size
			textArray = (char **) realloc(textArray, sizeof(char*)*(maxLines * 2));
			//verify realloc was successful
			if (textArray == NULL)
			{
				fprintf(stderr, "Error: Memory reallocation failed for textArray.\n");
				exit(1);
			}
			maxLines = maxLines*2;
		}

		//alloc memory for line
		textArray[line] = (char *) malloc(maxLineLength);						
		
		//verify memory alloc was successful
		if (textArray[line] == NULL)
		{
			fprintf(stderr, "Error: Memory allocation failed for line.\n");
			exit(1);
		}

		//Check length of the line
		if (strlen(textArray[line]) > 128) {
		  fprintf(stderr, "Error: Line %d is too long\n", line+1); 
		  exit(1);
		}
		//If we reach the end of the file, exit the loop
		if (fgets(textArray[line], 5000, in_file) == NULL)
			break;

		line++; 
	}

	fclose(in_file);
	int lines = line - 1;					

	printf("Unsorted text:\n");
	for (int i = 0; i <= lines; i++)
		printf("%s\n", textArray[i]);
	
	//sorts the lines using wordCompare 
	qsort(textArray, lines+1, sizeof(textArray), wordCompare);

	printf("\n\n\nText sorted by word %d:\n", sortByNum + 1);
	for (int i = 0; i <= lines; i++)
		printf("%s\n", textArray[i]);

	//free all memory
	for (int i = 0; i <= lines; i++)
	{
		free(textArray[i]);
		textArray[i] = NULL;
	}

	free(textArray);
    return 0;
}




