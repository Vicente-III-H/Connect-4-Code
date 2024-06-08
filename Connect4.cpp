/*
Vicente III Hernandez

Acknowledgements:
The criteria for the scoring system were based on the scoring system from the video:
https://www.youtube.com/watch?v=y7AKtWGOPAE&list=PLFCB5Dp81iNV_inzM-R9AKkZZlePCZdtV

The minimax algorithm was modelled after the pseudocode presented in the video:
https://www.youtube.com/watch?v=l-hh51ncgDI
*/

//Libraries
#include <iostream>
#include <cstdlib>
using namespace std;

//Constants
const int BOARD_ROWS = 6;
const int BOARD_COLUMNS = 7;

const int HUMAN_TOKEN_TYPE = 1;
const int ROBOT_TOKEN_TYPE = 2;

const int MIDDLE_COLUMN = 4;
const int TWO_LINE = 2;
const int THREE_LINE = 5;
const int OPPO_THREE_LINE = 4;
const int FOUR_LINE = 900;

const int DEPTH = 2; //default: 2

//Type Structs
typedef struct
{
  int score, columnOfMove;
} minimaxReturns;

//Function Prototypes
void displayBoard(int boardArray[BOARD_ROWS][BOARD_COLUMNS]);

int gameWon(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int currentPlayer);
bool legalMove(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int choiceCol);
void dropToken(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int columnHeights[BOARD_COLUMNS], int choiceCol, int tokenType);

void addTokenToArray(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int columnHeights[BOARD_COLUMNS], int row, int column, int tokenType);
void removeTokenInArray(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int columnHeights[BOARD_COLUMNS], int row, int column);

int humanMove(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int columnHeights[BOARD_COLUMNS], int currentCol);
int robotMove(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int columnHeights[BOARD_COLUMNS]);

void minimaxAlg(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int columnHeights[BOARD_COLUMNS], int depth, bool maxPlayer, minimaxReturns &values, int columnOfMove = 0);
void displayScore(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int columnOfMove, int score);

int scoreBoard(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int columnOfMove, int playerToken, int oppoToken);
int scorePoints(int sum, bool oppoCheck);

int horizontalCheck(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int row, int column, int playerToken, int oppoToken, bool oppoCheck = false);
int verticalCheck(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int row, int column, int playerToken, int oppoToken, bool oppoCheck = false);
int positiveSlopeCheck(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int row, int column, int playerToken, int oppoToken, bool oppoCheck = false);
int negativeSlopeCheck(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int row, int column, int playerToken, int oppoToken, bool oppoCheck = false);

//Main
int main()
{
  //In boardArray: 0 = empty, 1 = red, 2 = yellow;
  int boardArray[BOARD_ROWS][BOARD_COLUMNS] = {0};
  int columnHeights[BOARD_COLUMNS] = {0};
  
  int currentCol = 1;
  int currentPlayer = 1;
  
  while(!gameWon(boardArray, currentPlayer))
  {
  	currentPlayer = 1;
    currentCol = humanMove(boardArray, columnHeights, currentCol);
    displayBoard(boardArray);
    

    if(!gameWon(boardArray, currentPlayer))
    {
      currentPlayer = 2;
      currentCol = robotMove(boardArray, columnHeights);
      displayBoard(boardArray);
    }
	}
  
  return EXIT_SUCCESS;
}

//Human Player Functions
int humanMove(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int columnHeights[BOARD_COLUMNS], int currentCol)
{
  int choiceCol = currentCol;
  
  do {
		cout << "Enter player move column: ";
    cin >> choiceCol;
	} while(!legalMove(boardArray, choiceCol));
  
  dropToken(boardArray, columnHeights, choiceCol, HUMAN_TOKEN_TYPE);
  
  return choiceCol;
}

bool legalMove(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int choiceCol)
{
  if(boardArray[0][choiceCol - 1] == 0 && choiceCol >= 1 && choiceCol <= 7)
  {
    return true;
  }
  return false;
}

//Game State Functions
int gameWon(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int currentPlayer)
{

// assumption: the red = 1 = human, yellow = 2 = robot

	const int draw = 3, notWon = 0;
  int win = 0;
  
	if(boardArray[0][0] != 0 && boardArray[0][1] != 0 && boardArray[0][2] != 0 && boardArray[0][3] != 0
		&& boardArray[0][4] != 0 && boardArray[0][5] != 0 && boardArray[0][6] != 0)
	{
		return draw;
	}
  if(currentPlayer == 1)
  {
		win = 1; // human won
	}
	else if(currentPlayer == 2)
	{
		win = 2; // robot won
	}
	

	for(int row = 5; row >= 0 ; row--)
	{
		for(int col = 0; col < 7; col++)
		{
			if(row > 2)
			{
				if(boardArray[row][col] == currentPlayer
					&& boardArray[row - 1][col] == currentPlayer
					&& boardArray[row - 2][col] == currentPlayer
					&& boardArray[row - 3][col] == currentPlayer)
				{
					return win;
				} // checks vertical
				
				if(col < 4)
				{
					if(boardArray[row][col] == currentPlayer
						&& boardArray[row][col + 1] == currentPlayer
						&& boardArray[row][col + 2] == currentPlayer
						&& boardArray[row][col + 3] == currentPlayer)
					{
						return win;
					} // checks horizontal
					
					if(boardArray[row][col] == currentPlayer
						&& boardArray[row - 1][col + 1] == currentPlayer
						&& boardArray[row - 2 ][col + 2] == currentPlayer
						&& boardArray[row - 3][col + 3] == currentPlayer)
					{
						return win;
					} // checks +ve slope
				}
			 
			}
			else if(col < 4)
			{
				if(boardArray[row][col] == currentPlayer
					&& boardArray[row][col + 1] == currentPlayer
					&& boardArray[row][col + 2] == currentPlayer
					&& boardArray[row][col + 3] == currentPlayer)
				{
					return win;
				} // checks horizontal
				
				if(boardArray[row][col] == currentPlayer
					&& boardArray[row + 1][col + 1] == currentPlayer
					&& boardArray[row + 2 ][col + 2] == currentPlayer
					&& boardArray[row + 3][col + 3] == currentPlayer)
				{
					return win;
				} // checks -ve slope
			}

		}
	}

	return notWon; //not won if neither the robot nor human has a connect 4

}

int scorePoints(int sum, bool oppoCheck)
{
  if(!oppoCheck)
  {
    if(sum == 2)
    {
      return TWO_LINE;
    }
    else if(sum == 3)
    {
      return THREE_LINE;
    }
  }
  else
  {
    if(sum == 3)
    {
      return -1 * OPPO_THREE_LINE;
    }
  }
  
  return 0;
}

int horizontalCheck(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int row, int column, int playerToken, int oppoToken, bool oppoCheck)
{
  int score = 0;
   
  const int START_ROW = row;
  const int START_COLUMN = column;
  
  int sum = 1;
  
  for(int colOffset = 1; colOffset < 4; colOffset++)
  {
    int currentToken = boardArray[START_ROW][START_COLUMN + colOffset];
    
    if(currentToken == oppoToken || START_COLUMN + colOffset == BOARD_COLUMNS)
    {
      return 0;
    }
    else if(currentToken == playerToken)
    {
      sum++;
    }
  }
  
  score += scorePoints(sum, oppoCheck);
  
  return score;
  
}

int verticalCheck(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int row, int column, int playerToken, int oppoToken, bool oppoCheck)
{
  int score = 0;
  
  const int START_ROW = row;
  const int START_COLUMN = column;
  
  int sum = 1;
  
  for(int rowOffset = 1; rowOffset < 4; rowOffset++)
  {
    int currentToken = boardArray[START_ROW - rowOffset][START_COLUMN];
    
    if(currentToken == oppoToken || START_ROW - rowOffset == -1)
    {
      return 0;
    }
    else if(currentToken == playerToken)
    {
      sum++;
    }
  }
  
  score += scorePoints(sum, oppoCheck);
  
  return score;
  
}

int positiveSlopeCheck(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int row, int column, int playerToken, int oppoToken, bool oppoCheck)
{
  int score = 0;
   
  const int START_ROW = row;
  const int START_COLUMN = column;
  
  int sum = 1;
  
  for(int slopeOffset = 1; slopeOffset < 4; slopeOffset++)
  {
    int currentToken = boardArray[START_ROW - slopeOffset][START_COLUMN + slopeOffset];
    
    if(currentToken == oppoToken || START_ROW - slopeOffset == -1 || START_COLUMN + slopeOffset == BOARD_COLUMNS)
    {
      return 0;
    }
    else if(currentToken == playerToken)
    {
      sum++;
    }
  }
  
  score += scorePoints(sum, oppoCheck);
  
  return score;
  
}

int negativeSlopeCheck(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int row, int column, int playerToken, int oppoToken, bool oppoCheck)
{
  int score = 0;
   
  const int START_ROW = row;
  const int START_COLUMN = column;
  
  int sum = 1;
  
  for(int slopeOffset = 1; slopeOffset < 4; slopeOffset++)
  {
    int currentToken = boardArray[START_ROW + slopeOffset][START_COLUMN + slopeOffset];
    
    if(currentToken == oppoToken || START_ROW + slopeOffset == BOARD_ROWS || START_COLUMN + slopeOffset == BOARD_COLUMNS)
    {
      return 0;
    }
    else if(currentToken == playerToken)
    {
      sum++;
    }
  }
  
  score += scorePoints(sum, oppoCheck);
  
  return score;
  
}

int scoreBoard(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int columnOfMove, int playerToken, int oppoToken)
{
  int score = 0;
  int scoreCoefficient = 1;
  if(playerToken == HUMAN_TOKEN_TYPE)
  {
    scoreCoefficient = -1;
  }
  
  if(columnOfMove == 3)
  {
    score += MIDDLE_COLUMN;
  }
  
  for(int row = 5; row >= 0 ; row--)
	{
		for(int col = 0; col < 7; col++)
		{
      int currentToken = boardArray[row][col];
      
			if(currentToken != 0 && row > 2)
			{
        //check Vertical
				if(currentToken == playerToken)
        {
          score += verticalCheck(boardArray, row, col, playerToken, oppoToken);
        }
        else
        {
          score += verticalCheck(boardArray, row, col, oppoToken, playerToken, true);
        }
				
				if(col < 4)
				{
          //check Horizontal and +Slope
          if(currentToken == playerToken)
          {
            score += horizontalCheck(boardArray, row, col, playerToken, oppoToken);
					  score += positiveSlopeCheck(boardArray, row, col, playerToken, oppoToken);
          }
          else
          {
            score += horizontalCheck(boardArray, row, col, oppoToken, playerToken, true);
					  score += positiveSlopeCheck(boardArray, row, col, oppoToken, playerToken, true);
          }
          
				}
			 
			}
			else if(currentToken != 0 &&  col < 4)
			{
        //check Horizontal and -Slope
        if(currentToken == playerToken)
        {
          score += horizontalCheck(boardArray, row, col, playerToken, oppoToken);
				  score += negativeSlopeCheck(boardArray, row, col, playerToken, oppoToken);
        }
        else
        {
          score += horizontalCheck(boardArray, row, col, oppoToken, playerToken, true);
				  score += negativeSlopeCheck(boardArray, row, col, oppoToken, playerToken, true);
        }
        
			}

		}
	}
  
  score *= scoreCoefficient;
  
  return score;
  
}

void displayScore(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int columnOfMove, int score)
{
  cout << endl;
  displayBoard(boardArray);
  cout << "Column: " << columnOfMove << ", Score: " << score << endl;
  
  return;
}

//Robot Player Functions
int robotMove(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int columnHeights[BOARD_COLUMNS])
{
  
  minimaxReturns values;
  minimaxAlg(boardArray, columnHeights, DEPTH, true, values);
  
  dropToken(boardArray, columnHeights, values.columnOfMove, ROBOT_TOKEN_TYPE);
  
  return values.columnOfMove;
  
}

void minimaxAlg(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int columnHeights[BOARD_COLUMNS], int depth, bool maxPlayer, minimaxReturns &values, int columnOfMove)
{
  
  const int wonRobot = gameWon(boardArray, ROBOT_TOKEN_TYPE);
  const int wonHuman = gameWon(boardArray, HUMAN_TOKEN_TYPE);
  if(depth == 0 || wonRobot || wonHuman)
  {
    minimaxReturns scoreValue;
    
    if(maxPlayer)
    {
      if(wonRobot == ROBOT_TOKEN_TYPE)
      {
        values.score = FOUR_LINE;
        values.columnOfMove = scoreValue.columnOfMove;
        return;
      }
      else if(wonHuman == HUMAN_TOKEN_TYPE)
      {
        values.score = -1 * FOUR_LINE;
        values.columnOfMove = scoreValue.columnOfMove;
        return;
      }
      
      scoreValue.score = scoreBoard(boardArray, columnOfMove, HUMAN_TOKEN_TYPE, ROBOT_TOKEN_TYPE);
    }
    else
    {
      if(wonRobot == ROBOT_TOKEN_TYPE)
      {
        values.score = FOUR_LINE;
        values.columnOfMove = scoreValue.columnOfMove;
        return;
      }
      else if(wonHuman == HUMAN_TOKEN_TYPE)
      {
        values.score = -1 * FOUR_LINE;
        values.columnOfMove = scoreValue.columnOfMove;
        return;
      }
      
      scoreValue.score = scoreBoard(boardArray, columnOfMove, ROBOT_TOKEN_TYPE, HUMAN_TOKEN_TYPE);
    }
    
    values.score = scoreValue.score;
    values.columnOfMove = scoreValue.columnOfMove;
    return;
  }
  
  if(maxPlayer)
  {
    minimaxReturns maxValues;
    maxValues.score = -9999;
    maxValues.columnOfMove = 1;
    
    for(int colDropIndex = 0; colDropIndex < BOARD_COLUMNS; colDropIndex++) //Drops a token in each column to score that potential move
    {
      const int emptyTokenRow = (BOARD_ROWS - 1) - columnHeights[colDropIndex]; //Checks if a column is not full
      
      if(emptyTokenRow > -1)
      {
        addTokenToArray(boardArray, columnHeights, emptyTokenRow, colDropIndex, ROBOT_TOKEN_TYPE);        
        
        minimaxReturns possibleMoveScore;
        minimaxAlg(boardArray, columnHeights, depth - 1, false, possibleMoveScore, colDropIndex);
        
        //displayScore(boardArray, colDropIndex, possibleMoveScore.score);
        
        if(max(maxValues.score, possibleMoveScore.score) == possibleMoveScore.score && possibleMoveScore.score != maxValues.score)
        {
          maxValues.score = possibleMoveScore.score;
          maxValues.columnOfMove = colDropIndex + 1;
        }
        
        removeTokenInArray(boardArray, columnHeights, emptyTokenRow, colDropIndex);
     }
    
    }
    
    //cout << depth << ": "<< maxValues.score << endl;
    //cout << "FinalMove: " << maxValues.columnOfMove - 1 << endl;
    values.score = maxValues.score;
    values.columnOfMove = maxValues.columnOfMove;
    return;
  
  }
  else
  {
    minimaxReturns minValues;
    minValues.score = 9999;
    minValues.columnOfMove = 1;
    
    for(int colDropIndex = 0; colDropIndex < BOARD_COLUMNS; colDropIndex++) //Drops a token in each column to score that potential move
    {
      const int emptyTokenRow = (BOARD_ROWS - 1) - columnHeights[colDropIndex]; //Checks if a column is not full
      
      if(emptyTokenRow > -1)
      {
        addTokenToArray(boardArray, columnHeights, emptyTokenRow, colDropIndex, HUMAN_TOKEN_TYPE);
        
        minimaxReturns possibleMoveScore;
        minimaxAlg(boardArray, columnHeights, depth - 1, true, possibleMoveScore, colDropIndex);
        
        //displayScore(boardArray, colDropIndex, possibleMoveScore.score);
        
        if(min(minValues.score, possibleMoveScore.score) == possibleMoveScore.score && possibleMoveScore.score != minValues.score)
        {
          minValues.score = possibleMoveScore.score;
          minValues.columnOfMove = colDropIndex + 1;
        }
        
        removeTokenInArray(boardArray, columnHeights, emptyTokenRow, colDropIndex);
     }
    
    }
    
    //cout << depth << ": "<< minValues.score << endl;
    //cout << "FinalMove: " << minValues.columnOfMove - 1 << endl;
    values.score = minValues.score;
    values.columnOfMove = minValues.columnOfMove;
    return;
    
  }
  
}

//"Trivials"
void displayBoard(int boardArray[BOARD_ROWS][BOARD_COLUMNS])
{
  for (int rowIndex = 0; rowIndex < BOARD_ROWS; rowIndex++)
  {
    cout << "{";
    for (int colIndex = 0; colIndex < BOARD_COLUMNS; colIndex++)
    {
      char character = boardArray[rowIndex][colIndex];
      if(character == HUMAN_TOKEN_TYPE)
      {
        cout << " X";
      }
      else if(character == ROBOT_TOKEN_TYPE)
      {
        cout << " O";
      }
      else
      {
        cout << " -";
      }
    }
    cout << " }" << endl;
  }
  
  return;
}

void dropToken(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int columnHeights[BOARD_COLUMNS], int choiceCol, int tokenType)
{
  const int emptyTokenRow = (BOARD_ROWS - 1) - columnHeights[choiceCol - 1];
  
  addTokenToArray(boardArray, columnHeights, emptyTokenRow, choiceCol - 1, tokenType);
  
  cout << "Token dropped to: (" << emptyTokenRow + 1 << ", " << choiceCol << ")" << endl;
  
  return;
}

void addTokenToArray(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int columnHeights[BOARD_COLUMNS], int row, int column, int tokenType)
{
  boardArray[row][column] = tokenType;
  columnHeights[column] += 1;
  
  return;
}

void removeTokenInArray(int boardArray[BOARD_ROWS][BOARD_COLUMNS], int columnHeights[BOARD_COLUMNS], int row, int column)
{
  boardArray[row][column] = 0;
  columnHeights[column] -= 1;
  
  return;
}