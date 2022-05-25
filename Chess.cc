/* Project Started March 2022 */
// g++ Chess.cc && ./a.out --- use this to run it in Bash

// **FOR LATER**
// TODO uncoment purple pawn set up and remove the extra blue pawns
// TODO BLue team moves don't work properly
// TODO King Needs to count danger spots as invalid moves. --- This will be a pain.

// **FOCUS ON THESE**
// TODO Queen needs move set
// putting in e3 then d3 will cause seg fault.... need to put in a catch for when an nulptr is the start postion -- user proof it

#include <iostream>
#include <string>

#define DEBUG std::cout << "DEBUG: " << __LINE__ << endl
#define DEBUG_FUNC std::cout << "FUNC: " << __FUNCTION__ << endl
// #define DEBUG std::cout
// #define DEBUG_FUNC std::cout

using namespace std;

void Turn();

// parse the points that the player inputs.
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
      // Subtract a 1 from here to match the array 0 starting point.
      y = atoi(positionIn + 1) - 1;
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
    // This uses bash termanial colour conventions to display team colours.
    // If you are running a diffrent terminal language it may not display correct. __WARNING__
    if (colourIn == "white")
    {
      printPiece = "\033[34;1;4m" + letterIn + "\033[0m";
    }
    printPiece = "\033[34;1;4m" + letterIn + "\033[0m";
  }

  virtual bool ValidMove(Position start, Position end)
  {
    return false;
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
        if ((Board[x][start.y] != nullptr))
        {
          cout << "Invalid Rook move, piece in path." << endl;
          return false;
        }
      }
    }
    else if (start.x == end.x)
    {
      for (int y = min(start.y, end.y) + 1; y < max(start.y, end.y); y++)
      {
        if ((Board[start.x][y] != nullptr))
        {
          cout << "Invalid Rook move, piece in path." << endl;
          return false;
        }
      }
    }
    else
    {
      std::cout << "Invalid Rook move." << endl;
      return false;
    }

    if (Board[end.x][end.y] != nullptr && (Board[start.x][start.y]->color == Board[end.x][end.y]->color))
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
    if (Board[end.x][end.y] != nullptr && (Board[start.x][start.y]->color == Board[end.x][end.y]->color))
    {
      std::cout << "Invalid Knight move, team kill." << endl;
      return false;
    }
    else if (((end.x == start.x + 2) && (end.y == start.y - 1)) || ((end.x == start.x + 2) && (end.y == start.y + 1)) ||
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
      if (Board[x][y] == nullptr)
      {
        std::cout << "Invalid Bishop move, piece in the way" << endl;
        return false;
      }
    }

    // Checks if the bishop is moveing diagnal by subtracting the end postion from the start.
    // and checking both x and y difrances are equal.
    if (abs(start.x - end.x) != abs(start.y - end.y))
    {
      std::cout << "Invalid Bishop move" << endl;
      return false;
    }
    else if (Board[end.x][end.y] != nullptr && (Board[start.x][start.y]->color == Board[end.x][end.y]->color))
    {
      std::cout << "Invalid Bishop move, team piece in the way" << endl;
      return false;
    }

    y = start.y - 1;
    for (int x = min(start.x, end.x) + 1; x < max(start.x, end.x); x++)
    {
      y++;
      if (Board[x][y] == nullptr)
      {
        std::cout << "Invalid Bishop move, piece in the way" << endl;
        return false;
      }
    }
    return true;
  }
};

class King : public Piece
{
public:
  King() {}
  King(string colourIn) : Piece("G", colourIn) {}

  bool ValidMove(Position start, Position end) override
  {
    if (Board[end.x][end.y] != nullptr && (Board[start.x][start.y]->color == Board[end.x][end.y]->color))
    {
      std::cout << "Invalid King move, team kill." << endl;
      return false;
    }
    else if ((abs(end.x - start.x) <= 1) && (abs(start.y - end.y) <= 1))
    {
      return true;
    }
    std::cout << "Invalid King move." << endl;
    return false;
  }
};

class Queen : public Piece
{
public:
  Queen() {}
  Queen(string colourIn) : Piece("Q", colourIn) {}

  bool ValidMove(Position start, Position end) override
  {
    // Turns of freindly fire
    if (Board[end.x][end.y] != nullptr && (Board[start.x][start.y]->color == Board[end.x][end.y]->color))
    {
      cout << "Invalid Queen move, team piece in path." << endl;
      return false;
    }

    // Checks if the Queen is moveing diagnal by subtracting the end postion from the start
    // and checking both x and y difrances are equal
    int y = start.y + 1;
    if (start.x == end.x || end.y == start.y)
    {
      // just here to skip the else if
    }
    else if (abs(start.x - end.x) != abs(start.y - end.y))
    {
      DEBUG;
      std::cout << "Invalid Queen move" << endl;
      return false;
    }

    y = start.y - 1;
    for (int x = min(start.x, end.x) + 1; x < max(start.x, end.x); x++)
    {
      y++;
      if (Board[x][y] == nullptr)
      {
        std::cout << "Invalid Queen move, piece in the way" << endl;
        return false;
      }
    }

    // straight row / column Queen movement
    if (start.y == end.y)
    {
      for (int x = min(start.x, end.x) + 1; x < max(start.x, end.x); x++)
      {
        if ((Board[x][start.y] != nullptr)) // y check
        {
          cout << "Invalid Queen move, piece in path." << endl;
          return false;
        }
      }
    }
    else if (start.x == end.x) // x check
    {
      for (int y = min(start.y, end.y) + 1; y < max(start.y, end.y); y++)
      {
        if ((Board[start.x][y] != nullptr))
        {
          cout << "Invalid Queen move, piece in path." << endl;
          return false;
        }
      }
    }
    // If all Ifs fail the move is valid
    return true;
  }
};

class Pawn : public Piece
{
public:
  bool firstMove;
  Pawn() {}
  Pawn(string colourIn) : Piece("P", colourIn) { firstMove = true; }

  bool ValidMove(Position start, Position end) override
  {
    if (Board[end.x][end.y] != nullptr && (Board[start.x][start.y]->color == Board[end.x][end.y]->color))
    {
      cout << "Invalid Pawn move, team piece in path." << endl;
      return false;
    }
    // En passant // TODO needs to remove dead peice
    else if ((Board[start.x][start.y + 1] != nullptr) && (Board[end.x][end.y] == nullptr))
    {
      DEBUG;
      firstMove = false;
      Board[start.x][start.y + 1] = nullptr;
      return true;
    }
    // kill move
    else if (((Board[end.x][end.y] == Board[start.x + 1][start.y + 1]) ||
              (Board[end.x][end.y] == Board[start.x - 1][start.y + 1])) &&
             (Board[end.x][end.y] != nullptr))
    {
      firstMove = false;
      return true;
    }
    else if ((Board[end.x][end.y] != nullptr))
    {
      cout << "Invalid Pawn move, piece in the way." << endl;
      return false;
    }
    // All good can move forward
    else if (start.x == end.x && start.y + 1 == end.y)
    {
      firstMove = false;
      return true;
    }
    // move forward twice on first move
    else if (firstMove && (Board[end.x][end.y] == Board[start.x][start.y + 2]))
    {
      firstMove = false;
      return true;
    }
    cout << "Invalid Pawn move." << endl;
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
      Board[x][y] = nullptr;
    }
  }

  for (int x = 0; x < 8; x++)
  {
    Board[x][1] = new Pawn("white"); // coment out when testing
    Board[x][6] = new Pawn("black");
    Board[3][3] = new Pawn("black"); // For easy move testing TODO REMOVE when ready
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

// Prints out the board and all of it's peice placement
void PrintBoard()
{
  std::cout << "  A   B   C   D   E   F   G   H  " << endl;
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
      if (Board[x][y] == nullptr)
      {
        std::cout << "|   ";
      }
      else if (Board[x][y]->color == "white")
      {
        std::cout << "| "
                  << "\033[35;1;4m" << Board[x][y]->letter << "\033[0m"
                  << " ";
      }
      else
      {
        std::cout << "| "
                  << "\033[34;1;4m" << Board[x][y]->letter << "\033[0m"
                  << " ";
      }
    }
    std::cout << "| " << y + 1;
    std::cout << endl;

    for (int j = 0; j <= 32; j++)
    {
      std::cout << "-";
    }
  }
  std::cout << endl;
  std::cout << "  A   B   C   D   E   F   G   H  " << endl;
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
      Board[start.x][start.y] = nullptr;
    }
  }
  else
  {
    std::cout << "Your start and end coordinates are identicle plaese enter diffrent ones." << endl;
    Turn();
  }
  return;
}

// Takes input from the payer and parses it for other methods
void Turn()
{
  PrintBoard();
  Position start, end;
  // **TODO** make sure to check only 2 chars get entered per "cin" otherwise bad things happen

  while (true)
  {
    char str[3];
    std::cout << "Type start postion of your move:" << endl;
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
    std::cout << "Type end postion of your move:" << endl;
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

#if 1 // FOR DEBUG purposes
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