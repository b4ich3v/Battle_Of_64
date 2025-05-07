#include "AIPlayer.h"

static const int DIRS[4][2] = { { 0, 1}, { 0, -1}, { 1, 0}, {-1, 0} };

Node::Node(int x, int y, int effort) 
{

    this->x = x;
    this->y = y;
    this->effort = effort;

}

bool Node::operator < (const Node& other) const 
{

    return effort > other.effort;

}

static bool isValid(int x, int y, int rows, int cols) 
{

    return x >= 0 && x < rows && y >= 0 && y < cols;

}

int minimumEffortPath(MyVector<MyVector<int>>& heights) 
{

    int rows = (int)heights.size();
    if (rows == 0) return 0;
    int cols = (int)heights[0].size();

    MyVector<MyVector<int>> effortSoFar;

    for (int row = 0; row < rows; row++)
    {

        MyVector<int> currentRow;

        for (int col = 0; col < cols; col++) 
        {

            currentRow.push_back(INT_MAX);

        }

        effortSoFar.push_back(currentRow);

    }

    MyPriorityQueue<Node> pq;
    effortSoFar[0][0] = 0;
    pq.push(Node(0,0,0), 0);

    while (!pq.empty())
    {

        Node current = pq.top();
        pq.pop();

        if (current.effort != effortSoFar[current.x][current.y]) continue;
        if (current.x == rows - 1 && current.y == cols - 1)  return current.effort;
           
        for (int i = 0; i < 4; i++) 
        {

            int newX = current.x + DIRS[i][0];
            int newY = current.y + DIRS[i][1];

            if (!isValid(newX, newY, rows, cols)) continue;
                
            int diff = heights[current.x][current.y] - heights[newX][newY];
            if (diff < 0) diff = -diff;
            int newEffort = current.effort > diff ? current.effort : diff;

            if (newEffort < effortSoFar[newX][newY]) 
            {

                effortSoFar[newX][newY] = newEffort;
                pq.push(Node(newX, newY, newEffort), newEffort);

            }

        }

    }

    return effortSoFar[rows - 1][cols - 1] == INT_MAX ? 0: effortSoFar[rows - 1][cols - 1];

}

AIPlayer::AIPlayer(const MyString& name):
    Player(name) {}

Move AIPlayer::requestMove(Board& board, Color mover)  
{

    MyVector<Move> pseudo = board.generateAllLegalMoves(mover);
    if (pseudo.empty()) return Move{ {-1,-1},{-1,-1} };

    MyPriorityQueue<Move, int> pq;

    for (size_t i = 0; i < pseudo.size(); i++) 
    {

        const Move move = pseudo[i];
        board.applyMove(move);
        MyVector<MyVector<int>> heights;

        for (int row = 0; row < 8; row++)
        {

            MyVector<int> currentRow;

            for (int col = 0; col < 8; col++)
            {

                currentRow.push_back(0);

            }

            heights.push_back(currentRow);

        }

        for (int currentRowIndex = 0; currentRowIndex < 8; currentRowIndex++) 
        {

            for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++)
            {

                auto* figure = board.at({ currentRowIndex,currentColIndex });
                heights[currentRowIndex][currentColIndex] = figure ? (figure->getColor() == mover ? 1 : 2) : 0;

            }

        }

        int cost = minimumEffortPath(heights);
        board.undoMove(move);
        pq.push(move, cost);

    }

    return pq.top();

}
