#include "chess_movement.h"
#include <algorithm>
#include <cassert>
#include <iostream>
using namespace chess;

bool contains(const Board &b, Color c, const char *from, const char *to, PieceType p=PieceType::None) {
    auto moves=b.legalMoves(c);
    return std::find(moves.begin(),moves.end(),parseMove(from,to,p))!=moves.end();
}
Board setup(const char *white="h1", const char *black="a8") {
    Board b; b.clear();
    b.put(parseSquare(white),{PieceType::King,Color::White});
    b.put(parseSquare(black),{PieceType::King,Color::Black});
    return b;
}
int main() {
    {
        Board b; b.standardSetup();
        assert(b.legalMoves(Color::White).size()==20);
        assert(contains(b,Color::White,"e2","e4"));
        assert(!contains(b,Color::White,"e2","e5"));
        assert(!contains(b,Color::White,"c1","g5")); // blocked bishop
        assert(contains(b,Color::White,"b1","c3")); // knight jumps
        assert(!b.makeMove(parseMove("e2","e5")));
        assert(b.sideToMove()==Color::White);
        assert(b.makeMove(parseMove("e2","e4")));
        assert(b.sideToMove()==Color::Black);
        assert(b.at(parseSquare("e2")).empty());
    }
    {
        Board b=setup();
        b.put(parseSquare("d4"),{PieceType::Rook,Color::White});
        assert(contains(b,Color::White,"d4","d7"));
        assert(!contains(b,Color::White,"d4","f6"));
        b.put(parseSquare("d5"),{PieceType::Pawn,Color::White});
        assert(!contains(b,Color::White,"d4","d6"));
    }
    {
        Board b=setup();
        b.put(parseSquare("d4"),{PieceType::Bishop,Color::White});
        assert(contains(b,Color::White,"d4","g7"));
        assert(!contains(b,Color::White,"d4","d7"));
        b.put(parseSquare("e5"),{PieceType::Pawn,Color::Black});
        assert(contains(b,Color::White,"d4","e5"));
        assert(!contains(b,Color::White,"d4","f6"));
    }
    {
        Board b=setup();
        b.put(parseSquare("d4"),{PieceType::Queen,Color::White});
        assert(contains(b,Color::White,"d4","g7"));
        assert(contains(b,Color::White,"d4","d7"));
        assert(!contains(b,Color::White,"d4","e6"));
    }
    {
        Board b=setup();
        b.put(parseSquare("d4"),{PieceType::Knight,Color::White});
        b.put(parseSquare("d5"),{PieceType::Pawn,Color::White});
        assert(contains(b,Color::White,"d4","e6"));
        assert(contains(b,Color::White,"d4","f5"));
        assert(!contains(b,Color::White,"d4","d6"));
    }
    {
        Board b=setup("e1","a8");
        b.put(parseSquare("e2"),{PieceType::Pawn,Color::White});
        assert(contains(b,Color::White,"e2","e4"));
        b.put(parseSquare("e3"),{PieceType::Pawn,Color::Black});
        assert(!contains(b,Color::White,"e2","e4"));
    }
    {
        Board b=setup("h1","a8");
        b.put(parseSquare("e7"),{PieceType::Pawn,Color::White});
        assert(contains(b,Color::White,"e7","e8",PieceType::Queen));
        assert(contains(b,Color::White,"e7","e8",PieceType::Knight));
        assert(!contains(b,Color::White,"e7","e8"));
        assert(b.makeMove(parseMove("e7","e8",PieceType::Queen)));
        assert(b.at(parseSquare("e8")).type==PieceType::Queen);
    }
    {
        // Pinned rook cannot move off the e-file because its king would be in check.
        Board b=setup("e1","a8");
        b.put(parseSquare("e2"),{PieceType::Rook,Color::White});
        b.put(parseSquare("e8"),{PieceType::Rook,Color::Black});
        assert(!contains(b,Color::White,"e2","d2"));
        assert(contains(b,Color::White,"e2","e3"));
    }
    {
        Board b=setup("e1","a8");
        b.put(parseSquare("h1"),{PieceType::Rook,Color::White});
        b.setCastlingRights(true,false,false,false);
        assert(contains(b,Color::White,"e1","g1"));
        assert(b.makeMove(parseMove("e1","g1")));
        assert(b.at(parseSquare("g1")).type==PieceType::King);
        assert(b.at(parseSquare("f1")).type==PieceType::Rook);
    }
    {
        Board b=setup("e1","a8");
        b.put(parseSquare("h1"),{PieceType::Rook,Color::White});
        b.put(parseSquare("f8"),{PieceType::Rook,Color::Black});
        b.setCastlingRights(true,false,false,false);
        assert(!contains(b,Color::White,"e1","g1")); // f1 is attacked
    }
    {
        // Black d7-d5, then White e5xd6 en passant.
        Board b=setup("h1","a8");
        b.put(parseSquare("e5"),{PieceType::Pawn,Color::White});
        b.put(parseSquare("d7"),{PieceType::Pawn,Color::Black});
        b.setSideToMove(Color::Black);
        assert(b.makeMove(parseMove("d7","d5")));
        assert(contains(b,Color::White,"e5","d6"));
        assert(b.makeMove(parseMove("e5","d6")));
        assert(b.at(parseSquare("d5")).empty());
        assert(b.at(parseSquare("d6")).type==PieceType::Pawn);
    }
    {
        // Fool's mate: correct checkmate evaluation.
        Board b; b.standardSetup();
        assert(b.makeMove(parseMove("f2","f3")));
        assert(b.makeMove(parseMove("e7","e5")));
        assert(b.makeMove(parseMove("g2","g4")));
        assert(b.makeMove(parseMove("d8","h4")));
        assert(b.isCheckmate(Color::White));
    }
    std::cout << "All movement tests passed.\n";
}
