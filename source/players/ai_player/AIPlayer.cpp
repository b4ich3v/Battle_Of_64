#include "AIPlayer.h"
#include "Figure.h"
#include <climits>
#include <cstdlib>     

namespace 
{

    const int DIRS[4][2] = { {0,1},{0,-1},{1,0},{-1,0} };

    bool inBounds(int x, int y, int rows, int cols)
    {

        return x >= 0 && x < rows && y >= 0 && y < cols;

    }

    int minimumEffortPath(MyVector<MyVector<int>>& heights)
    {

        int rowsCount = heights.size(); if (!rowsCount) return 0;
        int colsCount = heights[0].size();

        MyVector<MyVector<int>> best;

        for (int currentRowIndex = 0; currentRowIndex < rowsCount; currentRowIndex++)
        {

            MyVector<int> row;

            for (int currentColIndex = 0; currentColIndex < colsCount; currentColIndex++)
            {

                row.push_back(INT_MAX);

            }

            best.push_back(row);

        }

        best[0][0] = 0;
        MyPriorityQueue<Node> pq;
        pq.push(Node(0, 0, 0), 0);

        while (!pq.empty())
        {

            Node currentNode = pq.top(); pq.pop();

            if (currentNode.effort != best[currentNode.x][currentNode.y]) continue;
            if (currentNode.x == rowsCount - 1 && currentNode.y == colsCount - 1) return currentNode.effort;

            for (int directionIndex = 0; directionIndex < 4; directionIndex++)
            {

                int newX = currentNode.x + DIRS[directionIndex][0];
                int newY = currentNode.y + DIRS[directionIndex][1];

                if (!inBounds(newX, newY, rowsCount, colsCount)) continue;

                int diff = std::abs(heights[currentNode.x][currentNode.y] - heights[newX][newY]);
                int newEffort = std::max(currentNode.effort, diff);

                if (newEffort < best[newX][newY])
                {

                    best[newX][newY] = newEffort;
                    pq.push(Node(newX, newY, newEffort), newEffort);

                }

            }

        }

        return 0;

    }

}

Node::Node(int x, int y, int effort): x(x), y(y), effort(effort) {}

bool Node::operator < (const Node& other) const 
{ 

    return effort > other.effort; 

} 

AIPlayer::AIPlayer(const MyString& name) :
    Player(name) {}

Move AIPlayer::requestMove(Board& board, Color mover)
{

    MyVector<Move> legalMoves = board.generateAllLegalMoves(mover);
    if (legalMoves.empty()) return Move();               

    MyPriorityQueue<Move, int> pq;

    for (std::size_t i = 0; i < legalMoves.size(); i++)
    {

        const Move& mv = legalMoves[i];
        board.applyMove(mv);
        MyVector<MyVector<int>> h;

        for (int currentRowIndex = 0; currentRowIndex < 8;currentRowIndex++)
        {

            MyVector<int> row;

            for (int currentColIndex = 0;currentColIndex < 8;currentColIndex++) row.push_back(0);
            {

                h.push_back(row);

            }

        }

        for (int currentRowIndex = 0;currentRowIndex < 8;currentRowIndex++)
        {

            for (int currentColIndex = 0;currentColIndex < 8;currentColIndex++)
            {

                const Figure* currentFigure = board.at({ (int8_t)currentRowIndex,(int8_t)currentColIndex });
                if (currentFigure) h[currentRowIndex][currentColIndex] = (currentFigure->getColor() == mover ? 1 : 2);

            }

        }

        int cost = minimumEffortPath(h);            
        board.undoMove(mv);
        pq.push(mv, cost); 
       
    }

    return pq.top();    
    
}
