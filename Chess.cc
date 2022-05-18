/* Project Started March 2022 */

// TODO Pawns can antpasond even if there is no piece in the way -- FIX IT
// TODO Bishop needs to not be able to end up in a non diagnal postion

#include <iostream>
#include <string>

#define DEBUG std::cout << "DEBUG: " << __LINE__ << endl
#define DEBUG_FUNC std::cout << "FUNC: " << __FUNCTION__ << endl
// #define DEBUG std::cout
// #define DEBUG_FUNC std::cout

using namespace std;

void Turn();

// Creats the points the player inputs.
class Position
{
public:
  int x;
  int y;

public:
  Position() {}

  // parse the letter and number
  Position(const char *positionIn)
  {
    // Parse char
    if (!isalpha(positionIn[0]))
    {
      throw("asdfasdfsadf");
    }
    else
    {
      x = positionIn[0] - 'a';
    }

    // Parse int
    if (!isdigit(positionIn[1]))
    {
      throw("asdfsadfasdfasdf");
    }
    else
    {
      // might need to subtract a 1 from here to match the array 0.
      y = atoi(positionIn + 1) - 1;

      // DEBUG << x << ", " << y << endl;
    }
  }
};

class Piece
{
public:
  string color;
  string letter;
  // Position piecePosition();
  string printPiece;

  Piece() {}
  Piece(string letterIn, string colourIn)
  {
    color = colourIn;
    letter = letterIn;
    if (colourIn == "white")
    {
      printPiece = "\033[34;1;4m" + letterIn + "\033[0m";
    }
    printPiece = "\033[34;1;4m" + letterIn + "\033[0m";
  }

  virtual bool ValidMove(Position start, Position end)
  {
    return false; // Might need to be false
  }
};

Piece *Board[8][8];

class Rook : public Piece
{
public:
  Rook() {}
  Rook(string colourIn) : Piece("R", colourIn) {}

  bool ValidMove(Position start, Position end) override
  {
    if (start.y == end.y)
    {
      for (int x = min(start.x, end.x) + 1; x < max(start.x, end.x); x++)
      {
        if ((Board[x][start.y]->letter != (" ")))
        {
          cout << "Invalid Rook move, Piece in path." << endl;
          return false;
        }
      }
    }
    else if (start.x == end.x)
    {
      for (int y = min(start.y, end.y) + 1; y < max(start.y, end.y); y++)
      {
        if ((Board[start.x][y]->letter != (" ")))
        {
          cout << "Invalid Rook move, Piece in path." << endl;
          return false;
        }
      }
    }
    else
    {
      std::cout << "Invalid Rook move." << endl;
      return false;
    }

    if (Board[start.x][start.y]->color == Board[end.x][end.y]->color)
    {
      cout << "Invalid Rook move, team piece in path." << endl;
      return false;
    }

    return true;
  }
};

class Knight : public Piece
{
public:
  Knight() {}
  Knight(string colourIn) : Piece("K", colourIn) {}

  bool ValidMove(Position start, Position end) override
  {
    if (((end.x == start.x + 2) && (end.y == start.y - 1)) || ((end.x == start.x + 2) && (end.y == start.y + 1)) ||
        ((end.x == start.x - 2) && (end.y == start.y + 1)) || ((end.x == start.x - 2) && (end.y == start.y - 1)) ||
        ((end.x == start.x - 1) && (end.y == start.y + 2)) || ((end.x == start.x + 1) && (end.y == start.y + 2)) ||
        ((end.x == start.x - 1) && (end.y == start.y - 2)) || ((end.x == start.x + 1) && (end.y == start.y - 2)))
    {
      return true;
    }
    std::cout << "Invalid Knight move." << endl;
    return false;
  }
};

class Bishop : public Piece
{
public:
  Bishop() {}
  Bishop(string colourIn) : Piece("B", colourIn) {}

  bool ValidMove(Position start, Position end) override
  {
    int y = start.y + 1;
    for (int x = min(start.x, end.x) + 1; x < max(start.x, end.x); x++)
    {
      y++;
      DEBUG << x << ", " << y << endl;
      if (Board[x][y]->letter == " ")
      {
        std::cout << "Invalid Bishop move, piece in the way" << endl;
        return false;
      }
    }
    if (Board[start.x][start.y]->color == Board[end.x][end.y]->color)
    {
      std::cout << "Invalid Bishop move, team piece in the way" << endl;
      return false;
    }

    y = start.y - 1;
    for (int x = min(start.x, end.x) + 1; x < max(start.x, end.x); x++)
    {
      y++;
      DEBUG << x << ", " << y << endl;
      if (Board[x][y]->letter == " ")
      {
        std::cout << "Invalid Bishop move, piece in the way" << endl;
        return true;
      }
    }
    if (Board[start.x][start.y]->color == Board[end.x][end.y]->color)
    {
      std::cout << "Invalid Bishop move, team piece in the way" << endl;
      return false;
    }
    return true;
  }
};

class King : public Piece
{
public:
  King() {}
  King(string colourIn) : Piece("G", colourIn) {}
};

class Queen : public Piece
{
public:
  Queen() {}
  Queen(string colourIn) : Piece("Q", colourIn) {}
};

class Pawn : public Piece
{
public:
  bool firstMove;
  Pawn() {}
  Pawn(string colourIn) : Piece("P", colourIn) { firstMove = true; }

  bool ValidMove(Position start, Position end) override
  {
    if ((Board[start.x][start.y]->color == Board[end.x][end.y]->color))
    {
      cout << "Invalid Pawn move, team piece in path." << endl;
      return false;
    }
    else if (Board[end.x][end.y]->letter != " ")
    {
      cout << "Invalid Pawn move, piece in the way." << endl;
      return false;
    }
    else if ((Board[end.x][end.y] == Board[start.x + 1][start.y + 1]) || (Board[start.x - 1][start.y + 1]))
    {
      // kill step
      firstMove = false;
      return true;
    }
    else if ((Board[end.x][end.y] == Board[start.x][start.y + 1]))
    {
      // All good can move forward
      firstMove = false;
      return true;
    }
    else if (firstMove && (Board[end.x][end.y] == Board[start.x][start.y + 2]))
    {
      // move forward twice on first move
      firstMove = false;
      return true;
    }
    cout << "Invalid Pawn move, to far." << endl;
    return false;
  }
};

ostream &operator<<(ostream &os, const Position &pt)
{
  os << (char)('a' + pt.x) << pt.y + 1;
  return os;
}

void BoardReset()
{
  for (int y = 0; y < 8; y++)
  {
    for (int x = 0; x < 8; x++)
    {
      Board[x][y] = new Piece(" ", "empty");
    }
  }

  for (int x = 0; x < 8; x++)
  {
    // Board[x][1] = new Pawn("white");
    Board[x][6] = new Pawn("black");
    Board[x][2] = new Pawn("black"); // easy test line
  }

  Board[0][0] = new Rook("white");
  Board[1][0] = new Knight("white");
  Board[2][0] = new Bishop("white");
  Board[3][0] = new King("white");
  Board[4][0] = new Queen("white");
  Board[5][0] = new Bishop("white");
  Board[6][0] = new Knight("white");
  Board[7][0] = new Rook("white");

  Board[0][7] = new Rook("black");
  Board[1][7] = new Knight("black");
  Board[2][7] = new Bishop("black");
  Board[3][7] = new King("black");
  Board[4][7] = new Queen("black");
  Board[5][7] = new Bishop("black");
  Board[6][7] = new Knight("black");
  Board[7][7] = new Rook("black");
}

// Prints out the board and all of its peice placement
void PrintBoard()
{
  for (int i = 0; i <= 32; i++)
  {
    std::cout << "-";
  }

  // prints out table with inner and outer loops
  // the loop below is for the rows
  for (int y = 0; y < 8; y++)
  {
    std::cout << endl;
    // The loop below is for the columns
    for (int x = 0; x < 8; x++)
    {

      if (Board[x][y]->color == "white")
      {
        std::cout << "| "
                  << "\033[35;1;4m" << Board[x][y]->letter << "\033[0m"
                  << " ";
      }
      else if (Board[x][y]->color == "black")
      {
        std::cout << "| "
                  << "\033[34;1;4m" << Board[x][y]->letter << "\033[0m"
                  << " ";
      }
      else
      {
        std::cout << "| " << Board[x][y]->letter << " ";
      }
    }
    std::cout << "|";
    std::cout << endl;

    for (int j = 0; j <= 32; j++)
    {
      std::cout << "-";
    }
  }
  std::cout << endl;
}

/*checks for legal movments*/
void Movement(Position start, Position end)
{
  if ((start.x != end.x) || (start.y != end.y))
  {
    if (Board[start.x][start.y]->ValidMove(start, end))
    {
      Board[end.x][end.y] = Board[start.x][start.y];
      Board[start.x][start.y] = new Piece(" ", "");
    }
  }
  else
  {
    std::cout << "Your start and end coordinates are identicle plaese enter diffrent ones." << endl;
    Turn();
  }
  return;
}

// Takes imput from the payer and parses it for other methods
void Turn()
{
  PrintBoard();
  Position start, end;
  // **TODO** make sure to check only 2 chars get entered per "cin" otherwise bad things happen

  while (true)
  {
    char str[3];
    std::cout << "Type the position for the starting place of your move:" << endl;
    std::cin >> str;
    try
    {
      start = Position(str);
    }
    catch (const char *invalid)
    {
      std::cout << "error invalid position:" << str << endl;
      continue;
    }
    break;
  }

  while (true)
  {
    char str[3];
    std::cout << "Type the position for the ending place of your move:" << endl;
    std::cin >> str;
    try
    {
      end = Position(str);
    }
    catch (const char *invalid)
    {
      std::cout << "error invalid position:" << str << endl;
      continue;
    }
    break;
  }
  std::cout << endl;
  Movement(start, end);
}

#if 1 // FOR the
int main()
{
  BoardReset();

  int winner = 0;
  while (winner == 0)
  {

    Turn();
    // TODO the 3 lines in coments AKA GAME OVER?
    // have it check for two Gs (kings), to see if the game is over ****TO DO
    // std::cout << "If game over, enter 1." << endl;
    // std::cin >> winner; // ****TODO put this back
  }
  return 0;
}
#else
int main()
{
  BoardReset();
  Movement(Position("a1"), Position("b2"));
  if (board[2][2] == 'c')
  {
    std::cout << "Yeah!!!" << endl;
  }
  else
  {
    std::cout << "Boo!!!" << endl;
    exit(1);
  }
  return 0;
}
#endif