#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(const MyString& name):
    Player(name) {}

bool HumanPlayer::tryParseSquare(const char* buffer, Position& position) 
{

    if (!buffer || std::strlen(buffer) < 2) return false;

    char file = (buffer[0] >= 'A' && buffer[0] <= 'Z') ? (char)(buffer[0] - 'A' + 'a') : buffer[0];
    char rank = buffer[1];

    if (file < 'a' || file > 'h' || rank < '1' || rank > '8')     return false;
    
    position.col = file - 'a';
    position.row = 8 - (rank - '0');

    return true;

}

Move HumanPlayer::requestMove(const Board& board, Color mover)
{

    while (true) 
    {

        std::cout << name.getData() << " (" << (mover == Color::WHITE ? "White" : "Black") << ")> ";

        MyString token;

        if (!(std::cin >> token)) std::exit(0);
        const char* str = token.getData();

        if (std::strcmp(str, "O-O") == 0 || std::strcmp(str, "0-0") == 0)
        {

            Position from = (mover == Color::WHITE ? Position{ 7,4 } : Position{ 0,4 });
            Position to = (mover == Color::WHITE ? Position{ 7,6 } : Position{ 0,6 });

            return Move(from, to, SpecialMove::CASTILING_KING_SIDE);

        }

        if (std::strcmp(str, "O-O-O") == 0 || std::strcmp(str, "0-0-0") == 0)
        {

            Position from = (mover == Color::WHITE ? Position{ 7,4 } : Position{ 0,4 });
            Position to = (mover == Color::WHITE ? Position{ 7,2 } : Position{ 0,2 });

            return Move(from, to, SpecialMove::CASTILING_QUEEN_SIDE);

        }

        size_t len = std::strlen(str);

        if (len >= 4) 
        {

            char buffer1[3] = { str[0], str[1], '\0' };
            char buffer2[3] = { str[2], str[3], '\0' };
            Position from, to;

            if (tryParseSquare(buffer1, from) && tryParseSquare(buffer2, to)) 
            {

                SpecialMove specialMove = SpecialMove::NORMAL;
                FigureType promo = FigureType::QUEEN;

                if (len == 5) 
                {

                    specialMove = SpecialMove::PROMOTION;
                    char x = str[4];
                    char P = (x >= 'a' && x <= 'z') ? (x - 'a' + 'A') : x;

                    switch (P) 
                    {

                    case 'R': promo = FigureType::ROOK; break;
                    case 'N': promo = FigureType::KNIGHT; break;
                    case 'B': promo = FigureType::BISHOP; break;
                    default: promo = FigureType::QUEEN; break;

                    }

                }

                return Move(from, to, specialMove, promo);

            }

        }

        std::cout << "Invalid input, try again." << std::endl;

    }

}

