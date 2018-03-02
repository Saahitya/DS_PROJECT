// C implementation of search and insert operations
// on Trie
#include <stdio.h>
#include <stdlib.h>
#include "DS_PROJECT.h"

// Driver



int main(int argc, char const *argv[])
{
	

	srand(time(NULL));
    TRIE *root = getNode();
    insert_all_nodes(root);
	ELEM *board = init_board();
	interface(board, root);
	return 0;
{}}