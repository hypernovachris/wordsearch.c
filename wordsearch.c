#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

enum Direction {NW, N, NE, W, E, SW, S, SE, Fail};

int iterate(enum Direction state, int cols)
{
	switch (state) {
	case NW:
		return -cols - 1;
	case N:
		return -cols;
	case NE:
		return -cols + 1;
	case W:
		return -1;
	case E:
		return 1;
	case SW:
		return cols - 1;
	case S:
		return cols;
	case SE:
		return cols + 1;
	}
}

enum Direction find_word(char *str, char *word, int cols, int start, int filesize, int wordsize)
{
	enum Direction final_direction = Fail;
	for (enum Direction current_direction = NW; current_direction < Fail; current_direction++) {
		bool word_found = true;
		if (!((start + iterate(current_direction, cols)) >= 0 && (start + iterate(current_direction, cols)) < filesize && 1 < wordsize)) {
			word_found = false;
		}
		else {
			for (int i = start + iterate(current_direction, cols), j = 1; i >= 0 && i < filesize && j < wordsize; i += iterate(current_direction, cols)) {
				if (toupper(word[j]) != toupper(str[i])) {
					word_found = false;
				}
				j++;
			}
		}
		if (word_found == true) {
			final_direction = current_direction;
			break;
		}
	}
	return final_direction;
}

void printWord(char *word, int start, enum Direction direction, int filesize, int columns)
{
	// make an empty string full of dashes and line breaks
	char *str = malloc(filesize * sizeof(char) + 1);
	for (int i = 0; i < filesize; i++) {
		if (i % columns == columns - 1) {
			str[i] = '\n';
		} else {
			str[i] = '-';
		}
	}
	// insert word into string
	for (int i = 0, j = start; word[i] != '\0'; i++) {
		str[j] = word[i];
		j += iterate(direction, columns);
	}
	// print string
	for(int i = 0; i < filesize; i++) {
		putchar(str[i]);
	}
	free(str);
	return;
}

int main(int argc, char *argv[])
{
	// open file and read it into array
	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL) {
		printf("Error opening file\n");
		return 0;
	}
	fseek(fp, 0, SEEK_END);
	int filesize = ftell(fp);
	rewind(fp);
	char *word_search = malloc(filesize * sizeof(char));
	fread(word_search, filesize, sizeof(char), fp);
	fclose(fp);
	// find column count
	int cols = 0;
	for (int i = 0; i < filesize; i++) {
		if (word_search[i] == '\n') {
			cols = i + 1;
			break;
		}
	}
	// find size of word
	int wordsize = 0;
	for (int i = 0;; i++) {
		if (argv[2][i] == '\0') {
			wordsize = i;
			break;
		}
	}
	// search for first letter of word to be found
	for (int i = 0; i < filesize; i++) {
		enum Direction found = Fail;
		if (toupper(word_search[i]) == toupper(argv[2][0])) {
			found = find_word(word_search, argv[2], cols, i, filesize, wordsize);
		}
		if (found != Fail) {
			printWord(argv[2], i, found, filesize, cols);
			break;
		}
	}
	free(word_search);
	return 0;
}