#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include <conio.h>
#include <time.h>


#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])

// Alphabet size (# of symbols)
#define ALPHABET_SIZE (26)

// Converts key current character into index
// use only 'a' through 'z' and lower case
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

typedef struct elem{
	char letter;
	int isTerr;//is -1 and 1 for the resp. players 0 for unnoccupied territory  1 blue  -1 red
	bool isWord; //whether the element is part of some word or not
}ELEM;

typedef struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
    // isEndOfWord is true if the node represents
    // end of a word
    bool isEndOfWord;
}TRIE;

TRIE *getNode();

bool puts_words(char * string, ELEM *board, int pos);

void mark_territory(int pos, char dir, char len, TRIE *root,ELEM *board, int player);

void insert(TRIE *root, const char *key);

bool search(TRIE *root, const char *key);

void insert_all_nodes(TRIE *root);

char * addInEnd(char * str, char ch);

void removeFromEnd(char * s);

void strperms(TRIE * noode,int *hashish,char * wsf);

void generate_words(TRIE * root, char s[]);

ELEM* init_board();

char * possible_orient(char *string, ELEM *board, int pos,char *orient);

void place_it_in_board(char * string, ELEM *board);

void form_board(ELEM* board,TRIE *root);

bool word_check(int pos, char dir, char len, TRIE *root,ELEM *board, int player );

void interface(ELEM *board, TRIE *root);