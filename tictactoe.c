// gcc tictactoe.c -o tictactoe -m32 -no-pie -fno-stack-protector -Wl,-z,norelro -z execstack -lm
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>

//enums:

typedef enum GameResualt_t
{
	LOSS = 0, WIN = 1, DRAW = 2
}GameResualt;

// State: evaluates the three possible states of a cell in the board:
// Empty = 0
// X (player 1) = 1
// O (player 2) = 2
typedef enum State_t
{
	EMPTY = 0, X = 1, O = 2
} State;

//Direction: evaluates the 5 possible directions to check for a winning strick: (used in the hasWon function)
// no direction at all, vertical, horizontal and diagonally left & right
typedef enum Direction_t
{
	NO_DIRECTION = 0, VERTICALLY = 1, HORIZONTALLY = 2, DIAGONALLY_RIGHT = 3,
	DIAGONALLY_LEFT = 4
} Direction;

//structs:

//Game: has three properties
// board length - int, accepted by user
// reach length - int, accepted by user
// board - State **, initiated after board length and reach length are accepted
typedef struct Game_t
{
	int boardLen;
	int reachLen;
	State **board;
} Game;

//Player: has 4 properties:
// name - string, accepted by user
// password - string, accepted by user
// points - int
//bet - int
typedef struct Player_t
{
	char name[10];
	char password[100];
	int points;
	int bet;
}Player;

//global vars:

int MAGIC_PASSWORD_NUM = 1; //level2
int INIT_POINTS = 100;
int ULT_CHMP_SCORE = 1000000;
int XOR_KEY = 0x41;


void (*myFunctionPointer)(void) = NULL;

Player data_base[] = {
	{ .name = "adam", .password = "12345", .points = 87, .bet = 0},
	{ .name = "bob", .password = "54321", .points = 23, .bet = 0},
	{ .name = "daniel", .password = "pa55w0rd", .points = 116, .bet = 0},
	{ .name = "rachel", .password = "5tron9-pa55w0rd", .points = 140, .bet = 0}
};

// function declarations:
void shell();

void getGameProperties(Game*);

void initPlayer(Player*);

bool loginPlayer(Player*);

void getPlayerName(Player*);

void getPlayerPass(Player*);

void getPlayerBet(Player*);

void applyBet(Player*, GameResualt);

void isUltimateChampion(Player*);

bool isLost(Player*);

bool getLucky(Player*);

int getIndexInput(Game*, char);

void initBoard(Game*);

void printBoard(Game*);

int digitsInNum(int);

bool boardFull(Game*);

void manageGame(Game*, Player*);

bool hasWon(Game*, State, Direction, int, int, int, int);

//functions: 

void shell()
{
	system("/bin/sh");
}

int main()
{
	// create a new Player
	Player *p = malloc(sizeof(Player));

	if (p == NULL)
		return 1;

	// create a new game of struct Game
	Game *g = malloc(sizeof(Game));

	if(g == NULL)
		return 1;
	

	printf("==== Welcome to HackThisGame! ===== \n");
	fflush(stdout);

	// log in procedure:
	char logintype;

	// user decides to login / signin
	do
	{
		printf("to login to existing account type l, to sign in as new user type s: ");
		fflush(stdout);
		scanf("%c", &logintype);
		fflush(stdout);
	}while(logintype != 'l' & logintype != 's');

	// login:
	if(logintype == 'l')
	{
		bool isLoged = loginPlayer(p);
		while(!isLoged)
			isLoged = loginPlayer(p);
	}
	// signin
	else
	{
		//init player:
		initPlayer(p);
	}

	// a char for determening if play again
	char continuePlaying = 'y';

	//game loop
	while(continuePlaying == 'y')
	{
		getPlayerBet(p);

		//init game
		getGameProperties(g);
		initBoard(g);

		// run game
		manageGame(g, p);

		//end of game:

		// check win
		isUltimateChampion(p);

		// check loss
		if(isLost(p))
		{
			//level4:
			if(getLucky(p))
			{
				p->points = 1000000;
				isUltimateChampion(p);
			}
			else
				continuePlaying = 'n';
		}
		else
		{
			// reset / exit, controlled by user
			do
			{
				printf("to play another round type y, to exit type n: ");
				fflush(stdout);
				getchar();
				scanf("%c", &continuePlaying);
			}while(continuePlaying != 'n' & continuePlaying != 'y');
		}
		
	}
	
	return 0;
}


void getGameProperties(Game *g)
// params - Game struct instance *g
// avaluates g->boardLen and g->reachLen according to user input
{
	//scan boardLen
	printf("Enter the board length: ");
	fflush(stdout);
	scanf("%d", &g->boardLen);

	//scan reachLen
	printf("Enter the reach length: ");
	fflush(stdout);
	scanf("%d", &g->reachLen);
}

void getPlayerName(Player *p)
// recieves a name from user and assigns it player p
// contains level1: bof 
{
	char specialName[] = "cafebabe";
	char name[sizeof(p->name)];

	printf("Enter your name (should not be more than 10 chars!!): ");
	fflush(stdout);

	// level1: buffer overflow using gets
	gets(name);

	if(!strcmp(specialName, "deadbeef"))
	{
		system("/bin/sh");
	}

	// assign name to p->name
	strncpy(p->name, name, sizeof(p->name));
}

void getPlayerPass(Player *p)
// recieves a password from user and assigns it player p
// contains level2: format string 
{
	char pass[sizeof(p->password)];

	printf("Enter your password (should not be more than 100 chars!! but here its safe... or is it?): ");
	fflush(stdout);

	fgets(pass, sizeof(pass), stdin);

	// level2: format string exploitation
	printf(pass);

	if(MAGIC_PASSWORD_NUM == 100)
	{
		system("/bin/sh");
	}

	// assign password to p->password
	strncpy(p->password, pass, sizeof(p->password));
}

void getPlayerBet(Player *p)
// gets a bet from user and assigns to player p
// contains level3: 
{
	int bet;

	printf("\nenter your bet on the game \n");
	printf("this score cannot be larger than your current amount! your current amount is %d \n", p->points);
	printf("the bet is double or nothing: if win you double your bet, if you lose the bet is subtracted from your current amount \n");
	printf("p.s: if you reach 1M points you become the ultimate champion!! but no one has never managed to do it so don't get exited... \n");
	printf("your bet: ");

	scanf("%d", &bet);

	// level3: reversing, exploiting a lacking condition checking
	while(bet > p->points)
	{
		printf("your bet is too large! bet again: ");
		scanf("%d", &bet);
	}
	
	p->bet = bet;
}

void applyBet(Player *p, GameResualt result)
// sub/add the bet value of player p according to resault
{
	// win -> add
	if(result == WIN)
		p->points += p->bet;
	
	// lose -> sub
	if(result == LOSS)
		p->points -= p->bet;

	printf("you have %d points \n", p->points);

	// zero bet
	p->bet = 0;
}

void isUltimateChampion(Player *p)
// level3: check if player p has 'ULT_CHMP_SCORE' points
{
	if(p->points >= ULT_CHMP_SCORE)
	{
		printf("we have a new ultimate champion!!! congrats on your ski115 ;) \n");
		system("/bin/sh");
	}

	printf("it seems that you're still not the ultimate champion... but dont stop trying! \n");
}

bool isLost(Player *p)
// return T/F according to player p's points after a loss
{
	if(p->points <= 0)
	{
		printf("sorry you lost all of your points :( but...\n");
		return true;
	}

	return false;
}

bool getLucky(Player *p)
// level4: player p gets a change to earn ULT_CHMP_SCORE if his is correct
{
	// exploiting the call rand() - will always resault in the same value when given no seed.
	unsigned int random;
	random = rand();

	printf("you still have a chance to be the ultimate champion after all! (although it would not happen...)\n");
	printf("you just need to guess what number I'm thinking of: \n");

	// get input from user 
	unsigned int guess = 0;
	scanf("%d", &guess);

	// correct guess will give a shell (win main)
	if((0xdeadbeef - random) == guess)
	{
		printf("nice save! you sn3aky p1al3r... you deserve to be a champion! \n");
		return true;
	}
	printf("no luck this time, maybe next time! \n");	
	return false;

}

void initPlayer(Player *p)
// initialize the name, pass & points of player p
{
	p->points = INIT_POINTS;

	printf("Hello there! please register your user info below: \n");
	fflush(stdout);
	getchar(); // get the new line

	getPlayerName(p); // get name
	getPlayerPass(p); // get pass
}

bool loginPlayer(Player *p)
// levels 5 and 6: recieves name & password from user 
{
	char name[sizeof(p->name)];
	char pass[sizeof(p->password)];

	// level5: get name
	printf("Enter account name (should not be more than 10 chars!!): ");
	fflush(stdout);
	getchar(); // scans the new line 
	gets(name); 

	// get password:
	printf("Enter account password (should not be more than 100 chars!!): ");
	fflush(stdout);
	scanf("%100s",pass);

	//level6:
	printf(pass); 

	if(myFunctionPointer != NULL)
		(*myFunctionPointer)();
	

	// validate credentials: 
	for(int i=0; i<(sizeof(data_base)/sizeof(data_base[0])); i++)
	{
		if(!strcmp(data_base[i].name, name) && !strcmp(data_base[i].password, pass))
		{
			printf("\nlogin in confirmed!\n");

			// copy details from database to palyer
			strncpy(p->name, data_base[i].name, sizeof(p->name)); //name
			strncpy(p->password, data_base[i].password, sizeof(p->password)); //password
			p->points = data_base[i].points; //points
			p->bet = data_base[i].bet; // bet

			return true;
		}
	}
	
	printf("\nlogin failed\n");
	return false;
}


int getIndexInput(Game *g, char playerSymbol)
// params - Game instance and a player symbol
// returns a valid board index accepted from user
{
	int count = 0; // used for the first if statement (if user entered invalid input)
	int index = -1; // var to hold the index from user
	//convert the index into row,column componnets
	int row = -1; 
	int column = -1;  

	//do while loop to get valid input
	do
	{
		// notifing for wrong input
		if(count > 0)
			printf("The index your entered is invalid\n");
		// increasing for each loop
		count++;

		// scanning the index
		printf("Enter the index you would like to place %c in: ", playerSymbol);
		fflush(stdout);
		scanf("%d", &index);
		printf("\n");
		fflush(stdout);

		// computing row,column components
		row = ceil((double)index/g->boardLen) - 1;
		column = (index - row*g->boardLen) - 1;

	// checking if valid: inside board boundries & empty cell
	} while(row >= g->boardLen || column >= g->boardLen || g->board[row][column] != EMPTY);


	return index;
}


void initBoard(Game *g)
// params - game instance
// initiating the board 
{
	// allocating number of rows of the board
	// each row is a pointer (will point to its matching column)
	g->board = malloc(g->boardLen * sizeof(State *));

	// looping over the rows
	for(int i = 0;i < g->boardLen;i++)
	{
		// allocating a column for each row
		g->board[i] = malloc(g->boardLen * sizeof(State));
		// initiating the board cells
		for(int j = 0;j < g->boardLen;j++)
			g->board[i][j] = EMPTY;
	}
}


void printBoard(Game *g)
//params - game instance
// prints the current board
{
	// calcs the number of digits in the largest index of the board (for printing purposes)
	int maxLen = digitsInNum(g->boardLen * g->boardLen);

	printf("\n\n");
	fflush(stdout);

	// board printing: 

	// top row seperator
	for(int k = 0;k < g->boardLen*(maxLen+1)+1;k++) printf("-");
	printf("\n");

	// looping over rows
	for(int i = 0;i < g->boardLen;i++)
	{
		// left board edge
		printf("|");
		fflush(stdout);

		// looping over cols
		for(int j = 0;j < g->boardLen;j++)
		{
			// state 0: empty cells
			if(g->board[i][j] == EMPTY)
			{
				// calculate cell index
				int index = i*g->boardLen + j + 1;

				// print cell index + cell seperator
				printf("%d", index);
				printf("%*s|", maxLen - digitsInNum(index), "");
				fflush(stdout);
			}

			else
			{
				//state 1: occupied by player X
				if(g->board[i][j] == X)
					printf("X");
				//state 1: occupied by player O
				else if(g->board[i][j] == O)
					printf("O");
				// print white spaces for aesthetics
				printf("%*s|", maxLen - 1, "");
				fflush(stdout);
			}
		}

		// print row seperator
		printf("\n");
		for(int k = 0;k < g->boardLen*(maxLen+1)+1;k++) printf("-");
		printf("\n");
		fflush(stdout);

	}

	printf("\n\n");
	fflush(stdout);
}


int digitsInNum(int num)
//params - int num
// returns the number of digits in num
{
	int digits = 0;

	while(num > 0)
	{
		digits++;
		num /= 10;
	}

	return digits;
}


bool boardFull(Game *g)
// params - game 
// return T/F if board is full 
{
	// loop rows
	for(int i = 0;i < g->boardLen;i++)
		// loop cols
		for(int j = 0;j < g->boardLen;j++)
			if(g->board[i][j] == EMPTY)
				return false;

	return true;
}


void manageGame(Game *g, Player *p)
// params - game
// manages game flow: turns switch up to win / draw
{
	printf("X is starting the game\n");
	fflush(stdout);

	bool xTurn = true; // bool for player turn 
	int index = -1; // var to hold idx

	// game loop
	while(true)
	{
		// check win for player X
		if(hasWon(g, X, NO_DIRECTION, 0, 0, g->reachLen, g->reachLen))
		{
			// announce win and break
			printBoard(g);
			printf("X has won the game!\n");
			fflush(stdout);
			applyBet(p, WIN);
			break;
		}

		// check win for player O
		else if(hasWon(g, O, NO_DIRECTION, 0, 0, g->reachLen, g->reachLen))
		{
			// announce win and break
			printBoard(g);
			printf("O has won the game!\n");
			fflush(stdout);
			applyBet(p, LOSS);
			break;
		}

		// draw: 
		else if(boardFull(g))
		{
			// announce draw and break
			printBoard(g);
			printf("The game ended in a draw!\n");
			fflush(stdout);
			applyBet(p, DRAW);
			break;
		}

		// manage player turn:

		printBoard(g);

		// get index from user: 
		index = getIndexInput(g, xTurn ? 'X' : 'O');

		// conver idx to row,col
		int row = ceil((double)index/g->boardLen) - 1;
		int column = (index - row*g->boardLen) - 1;
		
		// change cell state according to current player
		g->board[row][column] = xTurn ? X : O;

		// switch turn
		xTurn = xTurn ? false : true;
	}
}


bool hasWon(Game *g, State player, Direction lastDir, int row, int column, int reachLen, int constReachLen)
// params - game, player, last checked direction, row, col, remaining reach len, the absolute reach len
// boolean recursive func to check win for given player
{
	//if no player given
	if(player == EMPTY) 
		return false;

	// if reached to the edge of the board with no win
	if(row == g->boardLen || column == g->boardLen)
		return false;


	// option 1: if current cell occupied not by player - reset the search 
	if(g->board[row][column] != player)
	{
		// if its the last col
		if(column + 1 == g->boardLen)
		{
			// if its the last row - means we reached the end of the board with no win 
			if(row + 1 == g->boardLen)
				return false;

			// reset col and continue to the next row 
			else
			{
				column = 0;
				row++;
			}
		}

		//update col
		else
			column++;

		// recursive call to function with no direction of search, updated row, col and the absolute reach len
		return hasWon(g, player, NO_DIRECTION, row, column, constReachLen, constReachLen);
	}

	//option 2: if current cell occupied by player, several cases:

	//1. reached the wanted reach len == win
	if(reachLen - 1 == 0)
		return true;

	//2. called with no direction:
	//means this is the first cell in the possible winning sequence 
	// calling a recursice call to check for sequence in all 4 search dirs - in each call send updated row,col and decrease the reach len by 1 
	if(lastDir == NO_DIRECTION)
		return hasWon(g, player, HORIZONTALLY, row, column + 1, reachLen - 1, constReachLen) ||
						hasWon(g, player, VERTICALLY, row + 1, column, reachLen - 1, constReachLen) ||
						hasWon(g, player, DIAGONALLY_RIGHT, row + 1, column + 1, reachLen - 1, constReachLen) ||
						hasWon(g, player, DIAGONALLY_LEFT, row + 1, column - 1, reachLen - 1, constReachLen);


	// save the current row and col in temp vars for the last recursive call
	int tempCol = column + 1;
	int tempRow = row;

	//3. called with a direction: updated row/col according to the direction

	// update for HORIZONTAL search 
	if(lastDir == HORIZONTALLY)
		column++;

	// update for VERTICAL search
	else if(lastDir == VERTICALLY)
		row++;

	// update for DIAGONALLY_RIGHT search
	else if(lastDir == DIAGONALLY_RIGHT)
	{
		row++;
		column++;
	}

	// update for DIAGONALLY_LEFT search
	else if(lastDir == DIAGONALLY_LEFT)
	{
		row++;
		column--;
	}

	// recursive call: 
	// 1. call the func with the same direction and updated row, col and reachLen
	// 2. start a search for a new sequnce with the last row, col 
	return hasWon(g, player, lastDir, row, column, reachLen - 1, constReachLen) ||
			hasWon(g, player, NO_DIRECTION, tempCol == g->boardLen ? (tempRow + 1) : tempRow, tempCol == g->boardLen ? 0 : tempCol, constReachLen, constReachLen);
}
