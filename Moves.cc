#include "Moves.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace chess {
namespace {
int sign(int x) { return (x > 0) - (x < 0); }
Color opposite(Color c) { return c == Color::White ? Color::Black : Color::White; }
char symbol(Piece p) {
    char c = '.';
    switch (p.type) {
        case PieceType::King: c='k'; break;
        case PieceType::Queen: c='q'; break;
        case PieceType::Rook: c='r'; break;
        case PieceType::Bishop: c='b'; break;
        case PieceType::Knight: c='n'; break;
        case PieceType::Pawn: c='p'; break;
        default: return c;
    }
    return p.color == Color::White ? char(c - 'a' + 'A') : c;
}
}

bool operator==(Point a, Point b) { return a.file == b.file && a.rank == b.rank; }
bool operator!=(Point a, Point b) { return !(a == b); }
bool operator==(const Move &a, const Move &b) {
    return a.from==b.from && a.to==b.to && a.promotion==b.promotion;
}

bool Board::inside(Point s) {
    return s.file >= 0 && s.file < 8 && s.rank >= 0 && s.rank < 8;
}
int Board::index(Point s) { return s.rank*8+s.file; }
Board::Board() { clear(); }
void Board::clear() {
    cells_.fill(Piece{});
    turn_=Color::White;
    setCastlingRights(false,false,false,false);
    enPassantTarget_.reset();
}
void Board::setCastlingRights(bool wk, bool wq, bool bk, bool bq) {
    whiteCastleKing_=wk;whiteCastleQueen_=wq;
    blackCastleKing_=bk;blackCastleQueen_=bq;
}
void Board::put(Point s, Piece p) {
    if (!inside(s)) throw std::out_of_range("invalid Point");
    cells_[index(s)]=p;
}
Piece Board::at(Point s) const {
    if (!inside(s)) throw std::out_of_range("invalid Point");
    return cells_[index(s)];
}
void Board::standardSetup() {
    clear();
    const PieceType back[] = {PieceType::Rook,PieceType::Knight,PieceType::Bishop,
        PieceType::Queen,PieceType::King,PieceType::Bishop,PieceType::Knight,PieceType::Rook};
    for(int f=0; f<8; ++f) {
        put({f,0},{back[f],Color::White});
        put({f,1},{PieceType::Pawn,Color::White});
        put({f,6},{PieceType::Pawn,Color::Black});
        put({f,7},{back[f],Color::Black});
    }
    setCastlingRights(true,true,true,true);
}
std::string Board::draw() const {
    std::string out;
    for(int r=7;r>=0;--r) {
        out+=std::to_string(r+1)+" ";
        for(int f=0;f<8;++f) {
            Piece p=at({f,r});
            out+=(p.empty() ? ((f+r)%2==0?' ':'_') : symbol(p));
        }
        out+='\n';
    }
    return out+"  abcdefgh\n";
}
bool Board::clearPath(Point from, Point to) const {
    int dx=sign(to.file-from.file), dy=sign(to.rank-from.rank);
    Point p{from.file+dx, from.rank+dy};
    while(p!=to) {
        if(!inside(p) || !at(p).empty()) return false;
        p.file+=dx; p.rank+=dy;
    }
    return true;
}

// Attack detection is NOT based on legalMoves(): otherwise check detection
// would recurse forever. Pawn attacks are diagonal even on empty points.
bool Board::attacked(Point target, Color by) const {
    for(int r=0;r<8;++r) for(int f=0;f<8;++f) {
        Point from{f,r}; Piece p=at(from);
        if(p.empty() || p.color!=by) continue;
        int dx=target.file-f, dy=target.rank-r;
        int ax=std::abs(dx), ay=std::abs(dy);
        switch(p.type) {
            case PieceType::Pawn:
                if(ax==1 && dy==(by==Color::White?1:-1)) return true;
                break;
            case PieceType::Knight:
                if((ax==1&&ay==2)||(ax==2&&ay==1)) return true;
                break;
            case PieceType::King:
                if((ax<=1 && ay<=1) && (ax+ay>0)) return true;
                break;
            case PieceType::Bishop:
                if(ax==ay && ax>0 && clearPath(from,target)) return true;
                break;
            case PieceType::Rook:
                if(((dx==0&&dy!=0)||(dy==0&&dx!=0)) && clearPath(from,target)) return true;
                break;
            case PieceType::Queen:
                if(((ax==ay&&ax>0)||(dx==0&&dy!=0)||(dy==0&&dx!=0)) && clearPath(from,target)) return true;
                break;
            default: break;
        }
    }
    return false;
}
bool Board::hasKing(Color side) const {
    for(const auto &p:cells_)
        if(p.type==PieceType::King && p.color==side) return true;
    return false;
}
bool Board::inCheck(Color side) const {
    for(int r=0;r<8;++r) for(int f=0;f<8;++f) {
        Point s{f,r}; Piece p=at(s);
        if(p.type==PieceType::King && p.color==side) return attacked(s,opposite(side));
    }
    return false; // incomplete setups should be rejected by setup validation
}

void Board::addMovesFor(Point from, std::vector<Move> &out) const {
    Piece p=at(from);
    if(p.empty()) return;
    auto add=[&](Point to, bool captureAllowed=true) {
        if(!inside(to)) return;
        Piece dest=at(to);
        if(!dest.empty() && (dest.color==p.color || dest.type==PieceType::King)) return;
        if(!captureAllowed && !dest.empty()) return;
        if(p.type==PieceType::Pawn && (to.rank==0 || to.rank==7)) {
            for(PieceType pr:{PieceType::Queen,PieceType::Rook,PieceType::Bishop,PieceType::Knight})
                out.push_back({from,to,pr});
        } else out.push_back({from,to,PieceType::None});
    };
    const int dir = p.color==Color::White ? 1 : -1;
    if(p.type==PieceType::Pawn) {
        Point one{from.file,from.rank+dir};
        if(inside(one)&&at(one).empty()) {
            add(one,false);
            Point two{from.file,from.rank+2*dir};
            if(from.rank==(p.color==Color::White?1:6) && at(two).empty())
                add(two,false);
        }
        for(int dx:{-1,1}) {
            Point to{from.file+dx,from.rank+dir};
            if(!inside(to)) continue;
            Piece q=at(to);
            if(!q.empty() && q.color!=p.color) add(to);
            // En passant: target point is empty, captured pawn is behind it.
            if(enPassantTarget_ && to==*enPassantTarget_ && q.empty()) {
                Piece victim=at({to.file,from.rank});
                if(victim.type==PieceType::Pawn && victim.color!=p.color) add(to);
            }
        }
        return;
    }
    if(p.type==PieceType::Knight) {
        for(int dx:{-2,-1,1,2}) for(int dy:{-2,-1,1,2})
            if(std::abs(dx)!=std::abs(dy)) add({from.file+dx,from.rank+dy});
        return;
    }
    if(p.type==PieceType::King) {
        for(int dx=-1;dx<=1;++dx) for(int dy=-1;dy<=1;++dy)
            if(dx||dy) add({from.file+dx,from.rank+dy});
        // Castling must not pass through or finish on attacked points.
        const int home = p.color==Color::White?0:7;
        if(from==Point{4,home} && !inCheck(p.color)) {
            bool k=p.color==Color::White?whiteCastleKing_:blackCastleKing_;
            bool q=p.color==Color::White?whiteCastleQueen_:blackCastleQueen_;
            if(k && at({5,home}).empty() && at({6,home}).empty() &&
               at({7,home}).type==PieceType::Rook && at({7,home}).color==p.color &&
               !attacked({5,home},opposite(p.color)) && !attacked({6,home},opposite(p.color)))
                add({6,home});
            if(q && at({1,home}).empty() && at({2,home}).empty() && at({3,home}).empty() &&
               at({0,home}).type==PieceType::Rook && at({0,home}).color==p.color &&
               !attacked({3,home},opposite(p.color)) && !attacked({2,home},opposite(p.color)))
                add({2,home});
        }
        return;
    }
    // Sliding pieces: stop at the first occupied point.
    for(int dx=-1;dx<=1;++dx) for(int dy=-1;dy<=1;++dy) {
        if(dx==0&&dy==0) continue;
        bool diagonal=dx!=0 && dy!=0;
        if(p.type==PieceType::Bishop && !diagonal) continue;
        if(p.type==PieceType::Rook && diagonal) continue;
        Point to{from.file+dx,from.rank+dy};
        while(inside(to)) {
            Piece dest=at(to);
            if(!dest.empty() && dest.color==p.color) break;
            add(to);
            if(!dest.empty()) break;
            to.file+=dx; to.rank+=dy;
        }
    }
}
std::vector<Move> Board::pseudoLegalMoves(Color side) const {
    std::vector<Move> result;
    for(int r=0;r<8;++r) for(int f=0;f<8;++f) {
        Point sq{f,r}; Piece p=at(sq);
        if(!p.empty()&&p.color==side) addMovesFor(sq,result);
    }
    return result;
}
std::vector<Move> Board::legalMoves(Color side) const {
    std::vector<Move> result;
    if(!hasKing(side) || !hasKing(opposite(side))) return result;
    for(const Move &m:pseudoLegalMoves(side)) {
        Board next=*this;
        next.applyUnchecked(m);
        if(!next.inCheck(side)) result.push_back(m);
    }
    return result;
}
void Board::applyUnchecked(const Move &m) {
    Piece moving=at(m.from);
    Piece captured=at(m.to);
    // Capturing a rook on its original point eliminates that castling right.
    if(captured.type==PieceType::Rook) {
        if(m.to==Point{0,0}) whiteCastleQueen_=false;
        if(m.to==Point{7,0}) whiteCastleKing_=false;
        if(m.to==Point{0,7}) blackCastleQueen_=false;
        if(m.to==Point{7,7}) blackCastleKing_=false;
    }
    if(moving.type==PieceType::King) {
        if(moving.color==Color::White) whiteCastleKing_=whiteCastleQueen_=false;
        else blackCastleKing_=blackCastleQueen_=false;
    }
    if(moving.type==PieceType::Rook) {
        if(m.from==Point{0,0}) whiteCastleQueen_=false;
        if(m.from==Point{7,0}) whiteCastleKing_=false;
        if(m.from==Point{0,7}) blackCastleQueen_=false;
        if(m.from==Point{7,7}) blackCastleKing_=false;
    }
    // En passant capture happens on an empty destination point.
    if(moving.type==PieceType::Pawn && enPassantTarget_ &&
       m.to==*enPassantTarget_ && m.from.file!=m.to.file && captured.empty())
        put({m.to.file,m.from.rank},{});
    // King moves two points on castling; the rook follows it.
    if(moving.type==PieceType::King && std::abs(m.to.file-m.from.file)==2) {
        int rookFile=m.to.file>m.from.file?7:0;
        int rookTo=m.to.file>m.from.file?5:3;
        put({rookTo,m.from.rank},at({rookFile,m.from.rank}));
        put({rookFile,m.from.rank},{});
    }
    put(m.from,{});
    if(m.promotion!=PieceType::None) moving.type=m.promotion;
    put(m.to,moving);
    enPassantTarget_.reset();
    if(moving.type==PieceType::Pawn && std::abs(m.to.rank-m.from.rank)==2)
        enPassantTarget_=Point{m.from.file,(m.from.rank+m.to.rank)/2};
    turn_=opposite(turn_);
}
bool Board::makeMove(const Move &move) {
    auto options=legalMoves(turn_);
    if(std::find(options.begin(), options.end(), move)==options.end()) return false;
    applyUnchecked(move);
    return true;
}
bool Board::isCheckmate(Color side) const { return inCheck(side) && legalMoves(side).empty(); }
bool Board::isStalemate(Color side) const { return !inCheck(side) && legalMoves(side).empty(); }
Point parsePoint(const std::string &s) {
    if(s.size()!=2 || s[0]<'a'||s[0]>'h'||s[1]<'1'||s[1]>'8')
        throw std::invalid_argument("point must be like e2");
    return {s[0]-'a',s[1]-'1'};
}
Move parseMove(const std::string &from,const std::string &to,PieceType promotion) {
    return {parsePoint(from),parsePoint(to),promotion};
}
std::string describeMove(const Move &m) {
    auto sq=[](Point s) { return std::string(1,char('a'+s.file))+char('1'+s.rank); };
    std::string result=sq(m.from)+" -> "+sq(m.to);
    if(m.promotion!=PieceType::None) result += std::string(" promote to ") + symbol({m.promotion,Color::White});
    return result;
}
} // namespace chess
