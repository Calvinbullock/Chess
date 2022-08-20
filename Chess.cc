/* Project Started March 2022 */
// g++ Chess.cc && ./a.out --- use this to run it in Bash
// WARNING TO SHOW DEBUGs COMEMNT OUT "system("clear");"

// **FOR LATER**
// TODO King Needs to count danger spots as invalid moves. --- This will be a pain.

// **FOCUS ON THESE**
// TODO Game ends when only kings are left -- IsKingOnBoard() fuction
// TODO clock ncurses
// Special moves - look at wiki for all moves

#include <ncurses.h>
#include <iostream>
#include <string>

#define DEBUG std::cout << "DEBUG: " << __LINE__ << endl
#define DEBUG_FUNC std::cout << "FUNC: " << __FUNCTION__ << endl
// #define DEBUG std::cout
// #define DEBUG_FUNC std::cout

// GOLABLE VARIABLES / MEATHODS
using namespace std;
string ERROR_MSG = "";
int TURN_NUM = 0;

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
    // I don't think this code is ever used....
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
          ERROR_MSG = "Invalid Rook move, piece in path.";
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
          ERROR_MSG = "Invalid Rook move, piece in path.";
          return false;
        }
      }
    }
    else
    {
      ERROR_MSG = "Invalid Rook move.";
      return false;
    }

    if (Board[end.x][end.y] != nullptr && (Board[start.x][start.y]->color == Board[end.x][end.y]->color))
    {
      ERROR_MSG = "Invalid Rook move, team piece in path.";
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
      ERROR_MSG = "Invalid Knight move, team kill.";
      return false;
    }
    else if (((end.x == start.x + 2) && (end.y == start.y - 1)) || ((end.x == start.x + 2) && (end.y == start.y + 1)) ||
             ((end.x == start.x - 2) && (end.y == start.y + 1)) || ((end.x == start.x - 2) && (end.y == start.y - 1)) ||
             ((end.x == start.x - 1) && (end.y == start.y + 2)) || ((end.x == start.x + 1) && (end.y == start.y + 2)) ||
             ((end.x == start.x - 1) && (end.y == start.y - 2)) || ((end.x == start.x + 1) && (end.y == start.y - 2)))
    {
      return true;
    }
    ERROR_MSG = "Invalid Knight move.";
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
        ERROR_MSG = "Invalid Bishop move, piece in the way";
        return false;
      }
    }

    // Checks if the bishop is moveing diagnal by subtracting the end postion from the start.
    // and checking both x and y difrances are equal.
    if (abs(start.x - end.x) != abs(start.y - end.y))
    {
      ERROR_MSG = "Invalid Bishop move";
      return false;
    }
    else if (Board[end.x][end.y] != nullptr && (Board[start.x][start.y]->color == Board[end.x][end.y]->color))
    {
      ERROR_MSG = "Invalid Bishop move, team piece in the way";
      return false;
    }

    y = start.y - 1;
    for (int x = min(start.x, end.x) + 1; x < max(start.x, end.x); x++)
    {
      y++;
      if (Board[x][y] == nullptr)
      {
        ERROR_MSG = "Invalid Bishop move, piece in the way";
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
      ERROR_MSG = "Invalid King move, team kill.";
      return false;
    }
    else if ((abs(end.x - start.x) <= 1) && (abs(start.y - end.y) <= 1))
    {
      return true;
    }
    ERROR_MSG = "Invalid King move.";
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
    // Turns off freindly fire
    if (Board[end.x][end.y] != nullptr && (Board[start.x][start.y]->color == Board[end.x][end.y]->color))
    {
      ERROR_MSG = "Invalid Queen move, team piece in path.";
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
      ERROR_MSG = "Invalid Queen move";
      return false;
    }

    y = start.y - 1;
    for (int x = min(start.x, end.x) + 1; x < max(start.x, end.x); x++)
    {
      y++;
      if (Board[x][y] == nullptr)
      {
        ERROR_MSG = "Invalid Queen move, piece in the way";
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
          ERROR_MSG = "Invalid Queen move, piece in path.";
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
          ERROR_MSG = "Invalid Queen move, piece in path.";
          return false;
        }
      }
    }
    // If all ifs fail the move is valid
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
    // Gets the colour of the pawn and sets it to a boolean
    bool isWhite = Board[start.x][start.y]->color == "white";

    if (Board[end.x][end.y] != nullptr && (Board[start.x][start.y]->color == Board[end.x][end.y]->color))
    {
      ERROR_MSG = "Invalid Pawn move, team piece in path.";
      return false;
    }
    // En passant move for white
    else if ((Board[start.x][start.y + 1] != nullptr) && (Board[end.x][end.y] == nullptr) && isWhite)
    {
      DEBUG;
      Board[start.x][start.y + 1] = nullptr;
      firstMove = false;
      return true;
    }
    // En passant move for black
    else if ((Board[start.x][start.y - 1] != nullptr) && (Board[end.x][end.y] == nullptr) && !isWhite)
    {
      DEBUG;
      Board[start.x][start.y - 1] = nullptr;
      firstMove = false;
      return true;
    }
    // kill move for white
    else if ((end.x == start.x + 1 && end.y == start.y + 1 ||
              end.x == start.x - 1 && end.y == start.y + 1) &&
             (Board[end.x][end.y] != nullptr) && isWhite)

    {
      DEBUG;
      firstMove = false;
      return true;
    }
    // kill move for black
    else if ((end.x == start.x + 1 && end.y == start.y - 1 ||
              end.x == start.x - 1 && end.y == start.y - 1) &&
             (Board[end.x][end.y] != nullptr) && !isWhite)
    {
      DEBUG;
      firstMove = false;
      return true;
    }
    else if ((Board[end.x][end.y] != nullptr))
    {
      ERROR_MSG = "Invalid Pawn move, piece in the way.";
      return false;
    }
    // All good can move forward white
    else if (start.x == end.x && start.y + 1 == end.y && isWhite)
    {
      DEBUG;
      firstMove = false;
      return true;
    }
    // All good can move forward black
    else if (start.x == end.x && start.y - 1 == end.y && !isWhite)
    {
      DEBUG;
      firstMove = false;
      return true;
    }
    // move forward twice on first move, white
    else if (firstMove && end.x == start.x && end.y == start.y + 2 && isWhite)
    {
      DEBUG;
      firstMove = false;
      return true;
    }
    // move forward twice on first move black
    else if (firstMove && end.x == start.x && end.y == start.y - 2 && !isWhite)
    {
      DEBUG;
      firstMove = false;
      return true;
    }
    ERROR_MSG = "Invalid Pawn move.";
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
    Board[x][1] = new Pawn("white"); // comment out when testing
    Board[x][6] = new Pawn("black");
    // Board[3][3] = new Pawn("black"); // For easy move testing TODO REMOVE when ready
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
void PrintBoard_ncurse()
{
  // TODO move?
  initscr(); /* Start curses mode */

  clear();
  refresh(); /* Print it on to the real screen */

  // TODO MOVE?
  start_color();
  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2, COLOR_WHITE, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_BLACK);

  printw("Turn: %s\n", std::to_string(TURN_NUM).c_str());

  if (TURN_NUM % 2 == 0)
  {
    printw("Purple/White's Turn\n");
  }
  else
  {
    printw("Blue/Black's Turn\n");
  }

  attron(COLOR_PAIR(2));

  printw("\n");
  printw("  A   B   C   D   E   F   G   H  \n");

  for (int i = 0; i <= 32; i++)
  {
    printw("-");
  }

  // prints out table with inner and outer loops
  // the loop below is for the rows
  for (int y = 0; y < 8; y++)
  {
    printw("\n");

    // The loop below is for the columns
    for (int x = 0; x < 8; x++)
    {
      if (Board[x][y] == nullptr)
      {
        printw("|   ");
      }
      else if (Board[x][y]->color == "white")
      {
        printw("| %s ", Board[x][y]->letter.c_str());
      }
      else
      {
        printw("| ");
        attron(COLOR_PAIR(1));
        printw("%s ", Board[x][y]->letter.c_str());
        attroff(COLOR_PAIR(1));
      }
    }

    printw("| %s\n", std::to_string(y + 1).c_str());

    for (int j = 0; j <= 32; j++)
    {
      printw("-");
    }
  }
  printw("\n");
  printw("  A   B   C   D   E   F   G   H  \n");
  printw("\n");

  if (!ERROR_MSG.empty())
  {
    attron(COLOR_PAIR(3));
    printw("Errors: %s\n", ERROR_MSG.c_str());
    attroff(COLOR_PAIR(3));
  }
  ERROR_MSG.clear();

  attroff(COLOR_PAIR(2));
  endwin(); /* End curses mode */
}

// checks for legal movments.
void Movement(Position start, Position end)
{
  if ((start.x != end.x) || (start.y != end.y))
  {
    if (Board[start.x][start.y]->ValidMove(start, end))
    {
      Board[end.x][end.y] = Board[start.x][start.y];
      Board[start.x][start.y] = nullptr;
      TURN_NUM++;
    }
    else
    {
      ERROR_MSG = "Invalid Move.";
    }
  }
  else
  {
    ERROR_MSG = "Your start and end coordinates are identicle plaese enter diffrent ones.";
  }
  return;
}

// Takes input from the payer and parses it for other methods
void TurnInput_Ncurse()
{
  Position start, end;
  // **TODO** make sure to check only 2 chars get entered per "cin" otherwise bad things happen

  // this and the next while's will repeat the quesstions until you give an none empty (null) postion
  while (true)
  {
    printw("Type the start postion of your move: ");
    char *str = new char[160];
    getstr(str);

    try
    {
      start = Position(str);
    }
    catch (const char *invalid)
    {
      ERROR_MSG = "invalid position:";
      continue;
    }
    break;
  }

  while (true)
  {
    printw("Type the end postion of your move: ");
    char *str = new char[160];
    getstr(str);

    try
    {
      end = Position(str);
    }
    catch (const char *invalid)
    {
      ERROR_MSG = "invalid position:";
      continue;
    }
    break;
  }
  std::cout << endl;

  // This will stop people from entering a nullptr as the start postion.
  if (Board[start.x][start.y] == nullptr)
  {
    ERROR_MSG = "Starting point has no piece.";
    return;
  }

  // Checks if its the turn of the colour moveing.
  if ((TURN_NUM % 2 == 0 && Board[start.x][start.y]->color == "black") ||
      (TURN_NUM % 2 == 1 && Board[start.x][start.y]->color == "white"))
  {
    ERROR_MSG = "Not your turn";
    return;
  }

  Movement(start, end);
}

// Returns true and prints games over when black or white king dies
int IsKingOnBoard()
{
  bool isKingDead_Black = false;
  bool isKingDead_White = false;

  // Iterates through the Board and check for two kings -- IN PROGRESS
  for (int x = 0; x < 8; x++)
  {
    for (int y = 0; y < 8; y++)
    {
      if (Board[x][y] != nullptr && Board[x][y]->letter == "G")
      {
        if (Board[x][y]->color == "white")
        {
          isKingDead_White = true;
        }
        else if (Board[x][y]->color == "black")
        {
          isKingDead_Black = true;
        }
      }
    }
  }

  // Checks if king is dead
  if (!isKingDead_Black)
  {
    std::cout << "-------- Game Over --------" << endl
              << "-------- White Wins --------" << endl
              << "-------- " << TURN_NUM << " Turns ---------" << endl
              << endl;
    return 1;
  }
  else if (!isKingDead_White)
  {
    std::cout << "-------- Game Over --------" << endl
              << "-------- Black Wins --------" << endl
              << "-------- " << TURN_NUM << "Turns --------" << endl
              << endl;
    return 1;
  }

  return 0;
}

// will start the game and keep it running
void GameStart()
{
  BoardReset();

  // will cuntinue the game until IsKingOnBoard returns 0
  int winner = 0;
  while (winner == 0)
  {
    PrintBoard_ncurse();
    TurnInput_Ncurse();
    winner = IsKingOnBoard();
  }
}

#if 1 // FOR DEBUG purposes
int main()
{
  GameStart();

  return 0;
}

#else
int main()
{
  BoardReset();
  Position test1 = Position("a1");
  // Position test1 = Position("a3");
  // pawn move check
  if (Board[0][1] != nullptr && Board[0][1]->ValidMove(test1, test1))
  {
    std::cout << "Pass" << endl;
    std::cout << ERROR_MSG << endl;
  }
  else
  {
    std::cout << "Fail" << endl;
    std::cout << ERROR_MSG << endl;
  }
  if (Board[0][0] == nullptr)
  {
    std::cout << "Yeah!!!" << endl;
  }
  else
  {
    std::cout << "Boi" << endl;
    exit(1);
  }
  return 0;
}
#endif