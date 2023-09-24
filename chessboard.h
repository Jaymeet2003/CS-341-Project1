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
    bool occupied;
    chessboard[x][y].get(occupied, c, p);
    if(occupied == false){
      return -3;
    }
    return 1;
  }

  int move(int fromX, int fromY, int toX, int toY) {
    // fill out
    if (fromX < 0 || fromX > 7) return -1;
    if (fromY < 0 || fromY > 7) return -2;
    if (toX < 0 || toX > 7) return -3;
    if (toY < 0 || toY > 7) return -4;

    Color fromColor, toColor;
    Piece fromPiece, toPiece;
    bool fromOccupied = get(fromX, fromY, fromColor, fromPiece) == 1;
    bool toOccupied = get(toX, toY, toColor, toPiece) == 1;

    if (!fromOccupied) return -5;
    if (toOccupied && fromColor == toColor) return -6;

    int diffValX = abs(toX - fromX);
    int diffValY = abs(toY - fromY);

    switch (fromPiece) {
        // ... (other cases remain the same)
        
        case Pawn:
            int direction = (fromColor == White) ? 1 : -1;
            bool isForwardMove = diffValX == 0 && diffValY == direction;
            bool isDiagonalMove = diffValX == 1 && diffValY == direction;

            if (isForwardMove && !toOccupied) break;
            if (isDiagonalMove && toOccupied && fromColor != toColor) break;
            return -7; // Any other Pawn move is illegal
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
    this->chessboard = board;
    this->position = pos;
  }

  void xy(int &x, int &y) { // break position down into x and y
    // fill out
    x = position % 8;
    y = position / 8;
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
