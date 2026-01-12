#ifndef XNET_EXAMPLES_SUDOKU_SUDOKU_H
#define XNET_EXAMPLES_SUDOKU_SUDOKU_H

#include "xnet/base/Types.h"
#include "xnet/base/StringPiece.h"

xnet::string solveSudoku(const xnet::StringPiece &puzzle);
const int kCells = 81;
extern const char kNoSolution[];

#endif // XNET_EXAMPLES_SUDOKU_SUDOKU_H
