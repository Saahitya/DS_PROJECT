// C implementation of search and insert operations
// on Trie
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

// trie node

typedef struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
    // isEndOfWord is true if the node represents
    // end of a word
    bool isEndOfWord;
}TRIE;

typedef struct elem{
	char letter;
	int isTerr;//is -1 and 1 for the resp. players 0 for unnoccupied territory  1 blue  -1 red
	bool isWord; //whether the element is part of some word or not
}ELEM;
// Returns new trie node (initialized to NULLs)
bool puts_words(char * string, ELEM *board, int pos);
void mark_territory(int pos, char dir, char len, struct TrieNode *root,ELEM *board, int player);


struct TrieNode *getNode()
{
    struct TrieNode *pNode = NULL;

    pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));

    if (pNode)
    {
        int i;

        pNode->isEndOfWord = false;

        for (i = 0; i < ALPHABET_SIZE; i++)
            pNode->children[i] = NULL;
    }

    return pNode;
}

int LEN_OF_FILE=0;
// If not present, inserts key into trie
// If the key is prefix of trie node, just marks leaf node
void insert(struct TrieNode *root, const char *key)
{
    int level;
    int length = strlen(key);
    int index;

    struct TrieNode *pCrawl = root;

    for (level = 0; level < length; level++)
    {
        index = CHAR_TO_INDEX(key[level]);
        if(index>26 ||index<0)
            break;
        if (!pCrawl->children[index])
            pCrawl->children[index] = getNode();

        pCrawl = pCrawl->children[index];
    }

    // mark last node as leaf
    pCrawl->isEndOfWord = true;
}

// Returns true if key presents in trie, else false
bool search(struct TrieNode *root, const char *key)
{
    int level;
    int length = strlen(key);
    int index;
    struct TrieNode *pCrawl = root;

    for (level = 0; level < length; level++)
    {
        index = CHAR_TO_INDEX(key[level]);
        if(index<0 || index>25)
            break;
        else if(!pCrawl->children[index])
            return false;

        pCrawl = pCrawl->children[index];
    }

    return (pCrawl != NULL && pCrawl->isEndOfWord);
}

void insert_all_nodes(struct TrieNode *root)
{
    char str[100];
    int count=0;
    FILE *fp=fopen("words.txt","r");
    while(fgets(str,30,fp)!=NULL)
    {
        insert(root,str);
        //puts(str);
        ++count;
    }
    printf("No of words=%d\n",count);
    fclose(fp);
}

char * addInEnd(char * str, char ch)
{
    int len;
    for(len=0;str[len]!='\0';len++);
    str[len]=ch;
    str[len+1]='\0';
    return str;
}

void removeFromEnd(char * s)
{
    int len;
    for(len=0;s[len]!='\0';++len);
    s[len-1]='\0';
}

void strperms(struct TrieNode * noode,int *hashish,char * wsf)
{
    if(noode->isEndOfWord)
    {   //printf("%s\n",wsf);
		FILE *fp;

		fp = fopen("test.txt", "a");
		//fprintf(fp, "This is testing for fprintf...\n");
		if(wsf!="\0")
		{	fputs(wsf, fp);
			fputs("\n", fp);
			LEN_OF_FILE+=1;
		}
		fclose(fp);
	}

    for(int i=0;i<ALPHABET_SIZE;++i)
        if(hashish[i]!=0 && noode->children[i]!=NULL)
        {
            hashish[i]--;
            strperms(noode->children[i],hashish,addInEnd(wsf,i+'a'));
            hashish[i]++;
            removeFromEnd(wsf);
        }
    //for(int i=0;i<ALPHABET_SIZE;i++)printf("%d",hashish[i]);
}


void generate_words(struct TrieNode * root, char s[])
{
	LEN_OF_FILE = 0;
	
    int hashish[ALPHABET_SIZE];
    char wsf[100];
    strcpy(wsf,"\0");
    for(int i=0;i<ALPHABET_SIZE;i++)
        hashish[i]=0;
    for(int i=0;s[i]!='\0';i++)
        hashish[s[i]-'a']++;
//    for(int i=0;i<ALPHABET_SIZE;i++)
//        printf("%d,",hashish[i]);
    strperms(root,hashish,wsf);
}


// BOARD FUNCTIONS AND INTERFACE

// Initializes the all the cells in the board with(*-letter, 0-isWord,0-isTerr) 
ELEM* init_board()
{
	ELEM * board = (ELEM *)malloc(sizeof(ELEM)*10*10);
	if(board){
	for(int i =0; i < 10; i++)
	{
		for(int j = 0; j < 10; j++)
		{
			(board+(10*i+j)*sizeof(ELEM))->letter = '*';
			(board+(10*i+j)*sizeof(ELEM))->isWord = 0; 
			(board+(10*i+j)*sizeof(ELEM))->isTerr = 0;
		}
	}
	return board;
	}
	else{
		return NULL;
	}

}
//with the given word and the position func. figures out which orientation fits 
//to place the words without overlapping with other words and then returns a charcter array of possible orientations
char * possible_orient(char *string, ELEM *board, int pos,char *orient)
{
	int length = 0;
	int k =0;
	while(*(string+length*sizeof(char))!='\0')
	{
		length+=1;
	}
	int flag;
	if((pos - (pos%10))/10 + 1 >= length)			//enter NORTH
	{
		flag = 1;
		int j = 0;
		for(int i =pos;j<length  && i>0;i -= 10)
		{
			j+=1;
			if((board+(i)*sizeof(ELEM))->isWord==1)
			{
				flag=0;
				break;
			}
		}
		if(flag==1)
		orient[k++]='N';
	}
	
	if(pos%10 + 1 >= length)	//enter WEST
	{
		flag = 1;
		int j =0;
		for(int i =pos;j<length && i>0 ; i -= 1)
		{
			j+=1;
			if((board+(i)*sizeof(ELEM))->isWord==1)
			{
				flag=0;
				break;
			}
		}
		if(flag==1)
			orient[k++]='W';
	}
	
	if(10 - (pos%10) >=length)	//enter EAST
	{		
		flag = 1;
		int j =0;
		for(int i =pos;j<length && i<99 ; i += 1)
		{
			j+=1;
			if((board+(i)*sizeof(ELEM))->isWord==1)
			{
				flag=0;
				break;
			}
		}
		if(flag==1)
			orient[k++] = 'E';
	}
	
	if(10 - (pos - pos%10)/10 >=length)		//enter SOUTH	
	{
		flag = 1;
		int j =0;
		for(int i =pos;j<length && i<99; i += 10)
		{
			j+=1;
			if((board+(i)*sizeof(ELEM))->isWord==1)
			{
				flag=0;
				break;
			}
		}
		if(flag==1)
			orient[k++] = 'S';
		
		}
	
	
	orient[k++] = '\0';

	return orient;
}

//given a word-it figures out random position from possible orientation to place it in ,
//verifies it and then places it in right orientation
void place_it_in_board(char * string, ELEM *board)
{
	int length;
	int * arr;
	int success = 0;
	int random_pos = rand()%99+1; //random pos to start the word from
	int string_length=0;
	while(*(string+string_length*sizeof(char))!='\0')string_length+=1;
	int count = 0;
	do{
		count ++;
		random_pos = rand()%99+1;
		
		char orienti[10];
		char *orient = possible_orient(string, board, random_pos,orienti);
		int length = 0;
		while(*(orient+length*sizeof(char))!='\0')length+=1;
		if(length==0){success=0;continue;}
		int random_dir = rand()%length;
		int j=0;
		//printf("\n%d %c \n\n", random_dir,*(orient+random_dir*sizeof(char)) );
		switch(*(orient+random_dir*sizeof(char)))
		{
			case 'N':
			
					for(int i =random_pos;j<string_length;i -= 10,j+=1)
					{
						//printf("%s",string);
						(board+(i)*sizeof(ELEM))->letter = *(string + j*sizeof(char));
						(board + i*sizeof(ELEM))->isWord=1;
						
					}
					success=1;
					break;
			case 'S':
					for(int i =random_pos;j<string_length;i += 10,j+=1)
					{
						(board + i*sizeof(ELEM))->letter = *(string + j*sizeof(char));
						(board + i*sizeof(ELEM))->isWord=1;
					}
					success=1;
					break;
			case 'E':
					for(int i =random_pos; j<string_length;i+=1,j+=1)
					{
						(board + i*sizeof(ELEM))->letter = *(string + j*sizeof(char));
						(board + i*sizeof(ELEM))->isWord=1;
					}
					success=1;
					break;
			case 'W':
					for(int i =random_pos; j<string_length;i-= 1,j+=1)
					{
						(board + i*sizeof(ELEM))->letter = *(string + j*sizeof(char));
						(board + i*sizeof(ELEM))->isWord=1;
					}
					success=1;
					break;
			
		}
		
	}while(!success && count<10);
	

}

//seeds the original board with words from the players choice of letters 
void form_board(ELEM* board,struct TrieNode *root)
{
	
	char letters[6]; 
	FILE *fp;
	
	printf("\nPLAYER 1:Enter 3 letters to form words\n");
	/*for(int i =0;i<3;i++)
	{
		scanf("%c", &letters[i]) ;
	}
	*/
		scanf(" %c", &letters[0]) ;
		scanf(" %c", &letters[1]) ;
		scanf(" %c", &letters[2]) ;		
	printf("\nPLAYER 2:Enter 3 letters to form words\n");
	/*for(int i =3;i<6;i++)
	{
		scanf("%c", &letters[i]);
	}*/
		scanf(" %c", &letters[3]) ;
		scanf(" %c", &letters[4]) ;
		scanf(" %c", &letters[5]) ;		
	
	
	strcat(letters, "\0");
	generate_words(root, letters);
	
	for(int i =0;i<5;i++)
	{
		fp =fopen("test.txt","r+");
		char line[50];
	
		int randnum = rand()%(LEN_OF_FILE-1)+2;
		int linenumber = 0;
		
		
		while(fgets(line, 60, fp))
		{
			
			++linenumber;

			if(linenumber == randnum)
			{
				
				char word[10];
				int k = 0;
				for(int j = 1;line[j]!='\0';j++)
				{
				word[k++] = line[j-1];
				}
				word[k] = '\0';
				printf("\t%s\t\n", word);

				place_it_in_board(word, board);
				
			}
		}
		fclose(fp);
	}
		remove("test.txt");
		LEN_OF_FILE = 0;
	
	
}

//Searches the word in board given the pos, dir and length and the
// searches in trieTree for the word,if successfull calls mark_territory
bool word_check(int pos, char dir, char len, struct TrieNode *root,ELEM *board, int player )					//should mark the territory if word is correct with player number(1 or 2 )
{
	char word[10];
	int j=0;
	
	switch(dir)
		{
			case 'N':
					
					for(int i =pos;j < len;i -= 10)
					{
						word[ j++] = (board+(i)*sizeof(ELEM))->letter ;
						//printf("%s",string);
						//(board+(i)*sizeof(ELEM))->letter = *(string + j*sizeof(char));
						//(board + i*sizeof(ELEM))->isWord=1;
						
					}
					
					break;
			case 'S':
					for(int i = pos;j<len;i += 10)
					{
						word[ j++] = (board+(i)*sizeof(ELEM))->letter ;
						//(board + i*sizeof(ELEM))->letter = *(string + j*sizeof(char));
						//(board + i*sizeof(ELEM))->isWord=1;
					}
					
					break;
			case 'E':
					for(int i = pos; j < len ;i+=1)
					{
						word[ j++] = (board+(i)*sizeof(ELEM))->letter ;
						//*(board + i*sizeof(ELEM))->letter = *(string + j*sizeof(char));
						//(board + i*sizeof(ELEM))->isWord=1;
					}
					
					break;
			case 'W':
					for(int i = pos; j < len ; i-= 1)
					{
						word[ j++] = (board+(i)*sizeof(ELEM))->letter ;
						//(board + i*sizeof(ELEM))->letter = *(string + j*sizeof(char));
						//(board + i*sizeof(ELEM))->isWord=1;
					}
					
					break;
			
		}
		word[j] = '\0';

		printf("%s\n", word);
		bool success = search(root,word);	
		if(success == true)
		{
			mark_territory( pos, dir, len, root, board, player);
			printf("true");
		}
		else printf("false");	//if word is there I want to remove the word by putting random letters
					// there and marking the territory
		return true;
}
//marks the territory by assigning territory to player and randomizing letters there
void mark_territory(int pos, char dir, char len, struct TrieNode *root,ELEM *board, int player)
{
	int j =0;
	
	switch(dir)
		{
			case 'N':
					
					for(int i =pos;j < len;i -= 10)
					{
						(board+(i)*sizeof(ELEM))->isTerr = player;
						(board+(i)*sizeof(ELEM))->letter =  'a' + rand()%25;;
						(board+(i)*sizeof(ELEM))->isWord = 0;
						j++;
					
					}
					
					break;
			case 'S':
					for(int i = pos;j<len;i += 10)
					{
						(board+(i)*sizeof(ELEM))->isTerr = player;
						(board+(i)*sizeof(ELEM))->letter  =  'a' + rand()%25;;
						(board+(i)*sizeof(ELEM))->isWord = 0;
						j++;
					}
					
					break;
			case 'E':
					for(int i = pos; j < len ;i+=1)
					{
						(board+(i)*sizeof(ELEM))->isTerr = player;
						(board+(i)*sizeof(ELEM))->letter =  'a' + rand()%25;;
						(board+(i)*sizeof(ELEM))->isWord = 0;
						j++;
						
					}
					
					break;
			case 'W':
					for(int i = pos; j < len ; i-= 1)
					{
						(board+(i)*sizeof(ELEM))->isTerr = player;
						(board+(i)*sizeof(ELEM))->letter =  'a' + rand()%25;;
						(board+(i)*sizeof(ELEM))->isWord = 0;
						j++;
						
					}
					
					break;
			
		}
	
}
//Runs the interface by calling for all the inputs from players 
//and printing appropriate board's output
void interface(ELEM *board, struct TrieNode *root)
{
	
	
	char dir, forfeit_1,forfeit_2;
	int length, pos;
	bool correct;
	
	int terr_1, terr_2;
	terr_1 = terr_2 = 0;
	
	form_board(board, root);
																										//I think I must print the board at this stage - just once
	for(int i = 0; i<10;i++){
		for(int j =0; j<10;j++){
			if( (board + (10*i+j)*sizeof(ELEM))->isWord == 0)
				(board + (10*i+j)*sizeof(ELEM))->letter = 'a' + rand()%25;
			
		}
		
	
	}																							//I also think that i should form board after every round
	//6 rounds 
	printf("\n\n");
	for(int i =0;i<3;i++)
	{
		//_clrscr();
		system("cls");
		terr_1 = terr_2 = 0;
		printf("  ");
		for(int k=0;k<10;k+=1)printf("%d ", k);		//Print the board before every round
		printf("\n");
		for(int i = 0; i<10;i++){
			printf("%d ", i);
		for(int j =0; j<10;j++){
			printf("%c ", (board + (10*i+j)*sizeof(ELEM))->letter);
		}
		printf("\n");
	}
		
		
		fflush(stdin);
		printf("PLAYER 1 turn: \n");
		printf("Do you want to forfeit your turn-Press Y to forfeit any other key to play the turn \n\n");
		scanf("%c", &forfeit_1);
		
		if(forfeit_1!='Y' && forfeit_1!='y')
		{
			
			printf("\tEnter the position of the word \n");
			fflush(stdin);
			scanf("%d", &pos);
			
			printf("\tEnter the length of the word \n");
			fflush(stdin);
			scanf("%d", &length);
			
			printf("\tEnter the orientation of the word - N (north) or S (south) or E (east) or W (west) \n");
			fflush(stdin);
			scanf("%c", &dir);
			correct = word_check(pos, dir, length, root, board,1);
			
			//if(correct)
		}
		fflush(stdin);
		printf("PLAYER 2 turn: \n");
		printf("Do you want to forfeit your turn-Press Y to forfeit any other key to play the turn \n\n");
		scanf("%c", &forfeit_2);
		
		if(forfeit_2!='Y' && forfeit_2!='y')
		{
			printf("\tEnter the position of the word \n");
			fflush(stdin);
			scanf("%d", &pos);
			
			printf("\tEnter the length of the word \n");
			fflush(stdin);
			scanf("%d", &length);
			printf("\tEnter the orientation of the word - N (north) or S (south) or E (east) or W (west) \n");
			fflush(stdin);
			scanf("%c", &dir);
			correct = word_check(pos, dir, length, root, board, 2);
			
			//if(correct)
		}
		
		
		for(int i =0; i<10; i++)
		{
			for(int j =0;j<10;j++)
			{
				if((board + (10*i+j)*sizeof(ELEM))->isTerr==1)
					terr_1++;
				if((board + (10*i+j)*sizeof(ELEM))->isTerr==2)
					terr_2++;
				
			}
		}
		printf("PLAYER 1 territory: %d \nPLAYER 2 territory: %d\n", terr_1, terr_2);
		
		if((forfeit_2=='Y' || forfeit_2=='y') && (forfeit_1=='Y' || forfeit_1=='y'))
		{
			printf("Game forfeited because both of you forfeited turns\n");
			break;
		}
		
	}
	
	if(terr_1 > terr_2)printf("PLAYER 1 WINS\n");
	else if(terr_1 < terr_2)printf("PLAYER 2 WINS\n");
	else printf("DRAW GAME\n");
	
}
