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
    // get piece and color at loaction x,y coordinate of board
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
    bool occupied;
    chessboard[x][y].get(occupied, c, p); // get piece and color at loaction x,y coordinate of board
    if(occupied == false){
      return -3;
    }
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
    Color fromColor, toColor;
    Piece fromPiece, toPiece;
    int occupiedFrom = get(fromX, fromY, fromColor, fromPiece);
    int occupiedTo = get(toX, toY, toColor, toPiece);

    if (occupiedFrom != 1) {
      return -5;
    }

    if (occupiedTo == 1 && (toColor == fromColor)) {
      return -6;
    }
    // -7 remaining - check of any intermediate piece in between the move
    // -7 implementation
    int diffValX = abs(toX - fromX);
    int diffValY = abs(toY - fromY);

    switch (fromPiece) {
        case Rook: {
            // Rooks can move any number of squares vertically or horizontally
            if (!(diffValX == 0 || diffValY == 0)) return -7;
            break;
        }
        case Knight: {
            // Knights move in an L-shape: 2 squares in one direction and 1 square perpendicular to it
            if (!((diffValX == 1 && diffValY == 2) || (diffValX == 2 && diffValY == 1))) return -7;
            break;
        }
        case Bishop: {
            // Bishops can move any number of squares diagonally
            if (diffValX != diffValY) return -7;
            break;
        }
        case Queen: {
            // Queens can move any number of squares vertically, horizontally, or diagonally
            if (!(diffValX == 0 || diffValY == 0 || diffValX == diffValY)) return -7;
            break;
        }
        case King: {
            // Kings can move one square in any direction
            if (diffValX > 1 || diffValY > 1) return -7;
            break;
        }
        case Pawn: {
            // Pawns can move forward one square, or two squares from their starting position
            // Pawns can also move diagonally forward one square to capture an opponent's piece
            int direction = (fromColor == White) ? 1 : -1;
            bool isForwardMove = diffValX == 0 && diffValY == direction;
            bool isDoubleForwardMove = fromY == (fromColor == White ? 1 : 6) && diffValX == 0 && diffValY == 2 * direction;
            bool isDiagonalMove = diffValX == 1 && diffValY == direction;
            
            // Forward move
            if (isForwardMove && occupiedTo == 0) {
                break;
            }

            // Double forward move from starting position
            if (isDoubleForwardMove && occupiedTo == 0) {
                // Additionally, check if the square in-between is unoccupied
                bool inBetweenOccupied;
                Color inBetweenColor;
                Piece inBetweenPiece;
                get(fromX, fromY + direction, inBetweenColor, inBetweenPiece);
                if (inBetweenOccupied) return -7;
                break;
            }

            // Diagonal capturing move
            if (isDiagonalMove && occupiedTo == 1 && fromColor != toColor) {
                break;
            }

            // If none of the above conditions are met, the move is illegal
            return -7;
        }
    }
    chessboard[toX][toY] = Square(fromColor, fromPiece);
    chessboard[fromX][fromY] = Square();
    return 1;
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
    this->chessboard = board; // initiallizing chessboard with board
    this->position = pos; // initiallizing position with pos
  }

  void xy(int &x, int &y) { // break position down into x and y
    // fill out
    x = position % 8; // converting x postioion to get 0-8 coordinates
    y = position / 8; // converting y postioion to get 0-8 coordinates
  }

  // return the square
  Square operator*() {
    // fill out
    int x, y;
    xy(x, y);
    return chessboard.square(x, y); // returning square
  }

  // pre-increment
  ChessboardIterator &operator++() {
    // fill out
    ++position; // updating the row position for iterator
    return *this;
  }

  // needed to support foreach
  int operator!=(const ChessboardIterator &a) {
    // fill out
    return position != a.position; // checking the position for iterator
  }
};