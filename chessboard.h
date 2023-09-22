// see for background
//  https://users.cs.northwestern.edu/~riesbeck/programming/c++/stl-iterator-define.html
// CS 341: Fall 2023
// Jimmy Patel
//

#include <iostream>
#include <iterator>
using namespace std;

// define separate containers for Color, Piece (enums recommended)
enum Color { Black, White };
enum Piece { Rook, Knight, Bishop, Queen, King, Pawn };

// a chessboard contains squares
class Square {
private:
  Color color;
  Piece piece;
  bool occupied;

public:
  Square() { // default, square unoccupied
    occupied = false;
  }
  Square(Color c, Piece p) { // an occupied square
    // fill out
    color = c;
    piece = p;
    occupied = true;
  }
  int get(bool &o, Color &c, Piece &p) {
    // fill out
    o = occupied;
    c = color;
    p = piece;

    return 0;
  }

  void print();
};

class ChessboardIterator;

class Chessboard {
  // make the container iterator friendly
  typedef ChessboardIterator iterator;

private:
  Square chessboard[8][8];

public:
  friend class ChessboardIterator;

  Chessboard() {} // Square does all the initialization

  // return an x,y square from chessboard
  Square square(int x, int y) { return chessboard[x][y]; }

  ChessboardIterator begin();
  ChessboardIterator end();

  // place a piece on a x,y square in the chessboard
  int place(int x, int y, Color c, Piece p) {
    // fill out
    if (x < 0 || x > 7) {
      return -1;
    }
    if (y < 0 || y > 7) {
      return -2;
    }
    bool occupied;
    Color co;
    Piece pi;
    chessboard[x][y].get(occupied, co, pi);
    if (occupied == true) {
      return -3;
    }
    if (c != White && c != Black) {
      return -4;
    }

    if (p != Rook && p != Knight && p != Bishop && p != Queen && p != King &&
        p != Pawn) {
      return -5;
    }
    chessboard[x][y] = Square(c, p);
    return 1;
  }
  int get(int x, int y, Color &c, Piece &p) {
    // fill out

    if (x < 0 || x > 7) {
      return -1;
    }
    if (y < 0 || y > 7) {
      return -2;
    }
    if (p == Rook && p == Knight && p == Bishop && p == Queen && p == King && p == Pawn) {
      return -3;
    }
    bool occupied;
    chessboard[x][y].get(occupied, c, p);
    return 1;
  }

  int move(int fromX, int fromY, int toX, int toY) {
    // fill out
    if (fromX < 0 || fromX > 7) {
      return -1;
    }
    if (fromY < 0 || fromY > 7) {
      return -2;
    }
    if (toX < 0 || toX > 7) {
      return -3;
    }
    if (toY < 0 || toY > 7) {
      return -4;
    }
    bool occupiedFrom, occupiedTo;
    Color fromColor, toColor;
    Piece fromPiece, toPiece;
    chessboard[fromX][fromY].get(occupiedFrom, fromColor, fromPiece);

    if (occupiedFrom == false) {
      return -5;
    }

    chessboard[toX][toY].get(occupiedTo, toColor, toPiece);

    if (occupiedTo == true && fromColor == toColor) {
      return -6;
    }
    // -7 remaining - check of any intermediate piece in between the move
    // -7 implementation
    int diffValX = abs(toX - fromX);
    int diffValY = abs(toY - fromY);

    // Bishop

    if (fromPiece == Bishop && diffValX == diffValY) {
      int stepX = (toX > fromX) ? 1 : -1;
      int stepY = (toY > fromY) ? 1 : -1;

      int x = fromX + stepX;
      int y = fromY + stepY;

      while (x != toX && y != toY) {
        bool occupiedCheck;
        Color c;
        Piece p;
        chessboard[x][y].get(occupiedCheck, c, p);

        if (occupiedCheck) {
          return 1;
        }

        x += stepX;
        y += stepY;
      }
    } else if (fromPiece == Knight &&
               ((diffValX != 2 && diffValY != 1) ||
                (diffValX != 1 && diffValY != 2))) { // Knight
      return 1;
    } else if (fromPiece == Rook && (diffValX == 0 || diffValY == 0)) { // Rook
      int stepX = (toX > fromX) ? 1 : (toX < fromX) ? -1 : 0;
      int stepY = (toY > fromY) ? 1 : (toY < fromY) ? -1 : 0;

      int x = fromX + stepX;
      int y = fromY + stepY;

      while (x != toX || y != toY) {
        bool occupiedCheck;
        Color c;
        Piece p;
        chessboard[x][y].get(occupiedCheck, c, p);

        if (occupiedCheck) {
          return 1;
        }

        x += stepX;
        y += stepY;
      }

    } else if (fromPiece == Queen && (diffValX == 0 || diffValY == 0 ||
                                      diffValX == diffValY)) { // Queen
      int stepX = (toX > fromX) ? 1 : (toX < fromX) ? -1 : 0;
      int stepY = (toY > fromY) ? 1 : (toY < fromY) ? -1 : 0;

      int x = fromX + stepX;
      int y = fromY + stepY;

      while (x != toX || y != toY) {
        bool occupiedCheck;
        Color c;
        Piece p;
        chessboard[x][y].get(occupiedCheck, c, p);

        if (occupiedCheck) {
          return 1;
        }

        x += stepX;
        y += stepY;
      }
    } else if (fromPiece == King && !(diffValX <= 1 && diffValY <= 1)) { // King
      return 1;
    } else if (fromPiece == Pawn) { // Pawn
      int direction = (fromColor == White) ? 1 : -1;

      int dx = toX - fromX;
      int dy = toY - fromY;

      if (!(dx == 0 && dy == direction)) {
        return 1;
      } else if (!(fromY == (fromColor == White ? 1 : 6) && dx == 0 &&
                   dy == 2 * direction)) {
        return 1;
      } else if (!(abs(dx) == 1 && dy == direction)) {
        return 1;
      }
    }
    place(toX, toY, fromColor, fromPiece);
    chessboard[fromX][fromY] = Square();
    return -7;
  }

  void print();
};

class ChessboardIterator {
private:
  int position; // a number in [0..64]
  Chessboard chessboard;

public:
  ChessboardIterator(Chessboard &board,
                     int pos) { // start out with an empty board
    // fill out
    this->chessboard = board;
    this->position = pos;
  }

  void xy(int &x, int &y) { // break position down into x and y
    // fill out
    x = position / 8;
    y = position % 8;
  }

  // return the square
  Square operator*() {
    // fill out
    int x, y;
    xy(x, y);
    return chessboard.square(x, y);
  }

  // pre-increment
  ChessboardIterator &operator++() {
    // fill out
    ++position;
    return *this;
  }

  // needed to support foreach
  int operator!=(const ChessboardIterator &a) {
    // fill out
    return position != a.position;
  }
};
