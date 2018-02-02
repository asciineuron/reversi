#include <iostream>
#include <fstream> //interesting to add a function to write a log file to record game outcomes
#include <utility>
#include <string>

using namespace std;

//disk values:
const int WHITE = 1;
const int BLACK = -1;
const int EMPTY = 0;

int board[8][8]; //board 8x8 array with disk representation (1 for white, -1 for black, no objects needed!)
//rows: 1-8, columns: A-H

bool is_end = false; //true once neither player has valid move
int player_to_move = WHITE;
pair<int, int> score;
pair<bool, bool> has_move; //records if each player has a valid move (game end when both false), first white, second black
string input;

//functions:
void print_board();
pair<int, int> current_score(); //first element # white disks, second # black disks
pair<int, int> string_to_move(string input); //parses string into board position, e.g. E3 -> (4,2)
void parse_input(); //will parse user input and break down into further commands (e.g. string_to_move)
void check_end();
void quit(); //sequence of quit events
void next_turn(); //simply flips player_to_turn
bool is_valid_move(pair<int, int> position); //checks in each direction to see if touching opposite color and bounding them in a straight line with another of player's disk
void enact_move(pair<int, int> position); //same as above, but changes color of bound disks
//is valid move broken into each direction:
bool test_right(pair<int, int> position);
bool test_left(pair<int, int> position);
bool test_up(pair<int, int> position);
bool test_down(pair<int, int> position);
bool test_up_left(pair<int, int> position);
bool test_up_right(pair<int, int> position);
bool test_down_left(pair<int, int> position);
bool test_down_right(pair<int, int> position);


int main()
{
	has_move.first = true;
	has_move.second = true;

	//starting positions of disks:
	board[3][3] = WHITE;
	board[4][4] = WHITE;
	board[3][4] = BLACK;
	board[4][3] = BLACK;

	cout << "Welcome to reversi!" << endl;
	score = current_score(); //these lines set up initial view correctly	
	print_board();
/*
    std::pair<int, int> testt;
    testt.first = 0;
    testt.second = 1;
    cout << test_up(testt) << endl;
 */   
	while (!is_end)
	{
		//score = current_score();
		check_end(); //checks if either player has move, if not then game over
		if (is_end) break;
		parse_input();
		if (is_end) break; //try to cleanup later, check at each point game might end
		next_turn();
		score = current_score();
        print_board();
	}
	//QUIT: return 0;	
	return 0;
}

void parse_input()
{
	RETRY: //returns to here if incorrect input
	cin >> input; //reads first word of input
	//getline(cin, input);
	if (input == "quit")
	{
		quit();	
	} else if (input.find("pass") != std::string::npos)
	{
		if (player_to_move == WHITE)
		{
			has_move.first = false;
		} else if (player_to_move == BLACK)
		{
			has_move.second = false;
		}
	} else if (input.find("play") != std::string::npos)
	{
		cin >> input; //takes second word of input (movement position)
		if (is_valid_move(string_to_move(input)))
		{
			enact_move(string_to_move(input));	
		} else
		{
			cout << "not valid move, please try again: ";
			goto RETRY;
		}
	}
	
}

void next_turn()
{
	player_to_move *= -1;
}

bool test_right(pair<int, int> position)
{
    int tester = EMPTY;
    int opposite_color = -1 * player_to_move;
    int i = 1;
    
    bool result = false;
    
    if (board[position.second][position.first + i] != opposite_color) return false;
    
    for (; (position.first + i) < sizeof(board[position.second])/sizeof(*board[position.second]) && board[position.second][position.first + i] == opposite_color; i++) {}
    
	if ((position.first + i) < sizeof(board[position.second])/sizeof(*board[position.second]))
	{
		//if next spot still within board (where matching piece would be)
		tester = board[position.second][position.first + i];
		if (tester == player_to_move)
		{
			result = true;
		}	
	} else
    {
        result = false;
    }
    return result;
}

bool test_left(pair<int, int> position)
{
    int tester = EMPTY;
    int opposite_color = -1 * player_to_move;
    int i = 1;
    
    bool result = false;
    
    if (board[position.second][position.first - i] != opposite_color) return false;
    
    for (; (position.first - i) > -1 && board[position.second][position.first - i] == opposite_color; i++) {}
	
	if (position.first - i > -1)
	{
		tester = board[position.second][position.first - i];
		if (tester == player_to_move)
		{
			result = true;
		}
	} else
    {
        result = false;
    }
    
    return result;
}

bool test_up(pair<int, int> position)
{
    int tester = EMPTY;
    int opposite_color = -1 * player_to_move;
    int i = 1;
    
    bool result = false;
    
    if (board[position.second - i][position.first] != opposite_color) return false;
    
    for (; (position.second - i > -1) && board[position.second - i][position.first] == opposite_color; i++) {}
       	
    if (position.second - i > -1)
	{
		tester = board[position.second - i][position.first];
        //cout << "tester" << tester << " player " << player_to_move << endl;
		if (tester == player_to_move)
		{
            result = true;
		}
	} else
    {
        result = false;
    }
    return result;
}

bool test_down(pair<int, int> position)
{
    int tester = EMPTY;
    int opposite_color = -1 * player_to_move;
    int i = 1;
    
    bool result = false;
    //check that adjacent is opposite color before checking all
    if (board[position.second + i][position.first] != opposite_color) return false;
    
    for (; (position.second + i) < sizeof(board)/sizeof(*board) && board[position.second + i][position.first] == opposite_color; i++) {}
	
	if (position.second + i < sizeof(board)/sizeof(*board))
	{
		tester = board[position.second + i][position.first];
		//cout << "tester" << tester << " player " << player_to_move << endl;
        if (tester == player_to_move)
		{
			result = true;
		}
	} else
    {
        result = false;
    }	
    return result;
}

bool test_up_right(pair<int, int> position)
{
    int tester = EMPTY;
    int opposite_color = -1 * player_to_move;
    int i = 1;
    
    bool result = false;
    
    if (board[position.second - i][position.first + i] != opposite_color) return false;
    
    for (; (position.second - i > -1) && ((position.first + i) < sizeof(board[position.second])/sizeof(*board[position.second])) && board[position.second - i][position.first + i] == opposite_color; i++) {}
	
	if ((position.second - i > -1) && (position.first + i) < sizeof(board[position.second])/sizeof(*board[position.second]))
	{
		tester = board[position.second - i][position.first + i];
		if (tester == player_to_move)
		{
			result = true;
		}
	} else
    {
        result = false;
    }
    return result;
}

bool test_up_left(pair<int, int> position)
{
    int tester = EMPTY;
    int opposite_color = -1 * player_to_move;
    int i = 1;
    
    bool result = false;
    
    if (board[position.second - i][position.first - i] != opposite_color) return false;
    
    for (; (position.second - i > -1) && ((position.first - i) > -1) && board[position.second - i][position.first - i] == opposite_color; i++){}
    
    if ((position.second - i > -1) && (position.first - i))
    {
        tester = board[position.second - i][position.first - i];
        if (tester == player_to_move)
        {
            result = true;
        }
    } else
    {
        result = false;
    }
    return result;
}

bool test_down_left(pair<int, int> position)
{
    int tester = EMPTY;
    int opposite_color = -1 * player_to_move;
    int i = 1;
    
    bool result = false;
    
    if (board[position.second + i][position.first - i] != opposite_color) return false;
    
    for (; (position.first - i > -1) && ((position.second + i) < sizeof(board)/sizeof(*board)) && board[position.second + i][position.first - i] == opposite_color; i++) {}
    
    if (position.second - i > -1 && position.second + i < sizeof(board)/sizeof(*board))
    {
            tester = board[position.second + i][position.first - i];
            if (tester == player_to_move)
            {
                    result = true;
            }
    } else
    {
        result = false;
    }
    return result;
}

bool test_down_right(pair<int, int> position)
{
    int tester = EMPTY;
    int opposite_color = -1 * player_to_move;
    int i = 1;
    
    bool result = false;
    
    if (board[position.second + i][position.first + i] != opposite_color) return false;
    
    for (; (position.second + i < sizeof(board)/sizeof(*board)) && (position.first + i < sizeof(board[position.second])/sizeof(*board[position.second])) && board[position.second + i][position.first + i] == opposite_color; i++) {}
    
    if ((position.second + i < sizeof(board)/sizeof(*board)) && (position.first + i < sizeof(board[position.second])/sizeof(*board[position.second])))
    {
        tester = board[position.second + i][position.first + i];
        if (tester == player_to_move)
        {
            result = true;
        }
    } else
    {
        result = false;
    }
    
    return result;
    
}

bool is_valid_move(pair<int, int> position)
{
	//position.first = column or board[][], position.second = row or board[]
	
	if (board[position.second][position.first] != EMPTY) return false; //if position is occupied

	return (test_up(position) || test_down(position) || test_left(position) || test_right(position) || test_up_left(position) || test_up_right(position) || test_down_left(position) || test_down_right(position));
}

void enact_move(pair<int, int> position)
{
    //check each direction, then clear pieces
	cout << "good!" << endl;
    int opposite_color = -1*player_to_move;
    board[position.second][position.first] = player_to_move;
    int i = 1;
    if (test_right(position))
    {
        //no need for rest of function, already tested above
        for (; (position.first + i) < sizeof(board[position.second])/sizeof(*board[position.second]) && board[position.second][position.first + i] == opposite_color; i++)
        {
            board[position.second][position.first + i] = player_to_move; 
        }
    }
    i = 1;
    if (test_left(position))
    {
        for (; (position.first - i) > -1 && board[position.second][position.first - i] == opposite_color; i++)
        {
            board[position.second][position.first - i] = player_to_move;
        }
    }
    i = 1;
    if (test_up(position))
    {
        for (; (position.second - i > -1) && board[position.second - i][position.first] == opposite_color; i++) 
        {
            board[position.second - i][position.first] = player_to_move;
        }
    }
    i = 1;
    if (test_down(position))
    {
        for (; (position.second + i) < sizeof(board)/sizeof(*board) && board[position.second + i][position.first] == opposite_color; i++)
        {
            board[position.second + i][position.first] = player_to_move;
        }
    }
    i = 1;
    if (test_up_right(position))
    {
        for (; (position.second - i > -1) && ((position.first + i) < sizeof(board[position.second])/sizeof(*board[position.second])) && board[position.second - i][position.first + i] == opposite_color; i++) 
        {
            board[position.second - i][position.first + i] = player_to_move;
        }
    }
    i = 1;
    if (test_up_left(position))
    {
        for (; (position.second - i > -1) && ((position.first - i) > -1) && board[position.second - i][position.first - i] == opposite_color; i++)
        {
            board[position.second - i][position.first - i] = player_to_move;
        }
    }
    i = 1;
    if (test_down_left(position))
    {
         for (; (position.first - i > -1) && ((position.second + i) < sizeof(board)/sizeof(*board)) && board[position.second + i][position.first - i] == opposite_color; i++) 
         {
             board[position.second + i][position.first - i] = player_to_move;
        }
    }
    i = 1;
    if (test_down_right(position))
    {
        for (; (position.second + i < sizeof(board)/sizeof(*board)) && (position.first + i < sizeof(board[position.second])/sizeof(*board[position.second])) && board[position.second + i][position.first + i] == opposite_color; i++) 
        {
            board[position.second + i][position.first + i] = player_to_move;
        }
    }
}

void quit()
{
	is_end = true;
	ofstream log;
	log.open("game_log.txt", ios::app); //append to end of file
	if (score.first > score.second)
	{
		log << "White\n";
	} else if (score.second > score.first)
	{
		log << "Black\n";
	} else if (score.first == score.second)
	{
		log << "Tie\n";
	}
	
	cout << "Game over, thank you for playing!" << endl;
	//goto QUIT;
}

void check_end()
{
	if (has_move.first == false && has_move.second == false)
	{
		quit();
	}
}

void print_board() //will add graphics later
{
	cout << "\n  A B C D E F G H" << endl;
	for (int row = 0; row < sizeof(board)/sizeof(*board); row++)
	{
		cout << (row + 1);
		for (int col = 0; col < sizeof(board[row])/sizeof(*board[row]); col++)
		{
			if (board[row][col] == WHITE)
			{
				cout << " W";
			} else if (board[row][col] == BLACK)
			{
				cout << " B";
			} else
			{
				cout << " -";
			}
		}
		cout << " " << (row + 1) << endl;
	}
	cout << "  A B C D E F G H\n" << endl;
	if (player_to_move == WHITE)
	{
		cout << "White's turn" << endl;
	} else if (player_to_move == BLACK)
	{		
		cout << "Black's turn" << endl;
	}
	cout << "White: " << score.first << " Black: " << score.second << "\n\n->";
}

pair<int, int> current_score()
{
	pair<int, int> score; //first white, second black
	for (int row = 0; row < sizeof(board)/sizeof(*board); row++)
	{
		for (int col = 0; col < sizeof(board[row])/sizeof(*board[row]); col++)
		{
			if (board[row][col] == WHITE)
			{
				score.first++;
			} else if (board[row][col] == BLACK)
			{
				score.second++;
			}
		}
	}
	return score;
}

pair<int, int> string_to_move(string position_input)
{
	pair<int, int> position;
	//parse col/letter into num:
	if (position_input[0] == 'a')
	{
		position.first = 0;
	} else if (position_input[0] == 'b')
	{
		position.first = 1;
	} else if (position_input[0] == 'c')
	{
		position.first = 2;
	} else if (position_input[0] == 'd')
	{
		position.first = 3;
	} else if (position_input[0] == 'e')
	{
		position.first = 4;
	} else if (position_input[0] == 'f')
	{
		position.first = 5;
	} else if (position_input[0] == 'g')
	{
		position.first = 6;
	} else if (position_input[0] == 'h') 
	{
		position.first = 7;
	}
	
	//parse row/num into num
	position.second = position_input[1] - '0' - 1; //convert char to int, subtract 1 to match board numbering
	return position;
}
