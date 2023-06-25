#include <iostream>
#include <array>
#include <vector>
#include <algorithm>

void solver(std::array<int, 64> board, std::array<int, 64> legalPossibilities, int moveNumber, int square);
bool update(std::array<int, 64> &board, std::array<int, 64> &legalPossibilities, int moveNumber, int square);
void initialize(std::array<int, 64> &board, std::array<int, 64> &legalPossibilities);
void printBoard(std::array<int, 64> board);

int main() {
    std::cout << "Hello world!" << std::endl;
    std::array<int, 64> board, legalPossibilities;
    initialize(board, legalPossibilities);
    update(board, legalPossibilities, 1, 35);
    solver(board, legalPossibilities, 1, 35);

    return 0;
}

void solver(std::array<int, 64> board, std::array<int, 64> legalPossibilities, int moveNumber, int square) {    
    int x = square % 8, y = square / 8;
    std::array<std::array<int, 2>, 8> indexPair;
    indexPair.fill(std::array<int, 2>{-1, 255});
    // Change to single dimensional array and pass legalP to sort?

    // Fill indexPair with valid index and possibility pairs
    if (x <= 6 && y >= 2) {
        indexPair[0] = std::array<int, 2>{square - 15, legalPossibilities[square - 15]};
    }

    if (x <= 5 && y >= 1) {
        indexPair[1] = std::array<int, 2>{square - 6, legalPossibilities[square - 6]};
    }

    if (x <= 5 && y <= 6) {
        indexPair[2] = std::array<int, 2>{square + 10, legalPossibilities[square + 10]};
    }

    if (x <= 6 && y <= 5) {
        indexPair[3] = std::array<int, 2>{square + 17, legalPossibilities[square + 17]};
    }

    if (x >= 1 && y <= 5) {
        indexPair[4] = std::array<int, 2>{square + 15, legalPossibilities[square  + 15]};
    }

    if (x >= 2 && y <= 6) {
        indexPair[5] = std::array<int, 2>{square + 6, legalPossibilities[square + 6]};
    }
    
    if (x >= 2 && y >= 1) {
        indexPair[6] = std::array<int, 2>{square - 10, legalPossibilities[square - 10]};
    }

    if (x >= 1 && y >= 2) {
        indexPair[7] = std::array<int, 2>{square - 17, legalPossibilities[square - 17]};
    }


    // Order indexes on their possibilites
    std::sort(
        indexPair.begin(),
        indexPair.end(),
        [](std::array<int, 2> a, std::array<int, 2> b) {
            return a[1] < b[1];
        }
    );

    // Loop throigh the indexPair
    for (auto [index, possibility]: indexPair) {
        if (possibility > 100) continue;
        auto state = update(board, legalPossibilities, moveNumber+1, index);
        if (state) {
            solver(board, legalPossibilities, moveNumber+1, index);
        }
    }
}

bool update(std::array<int, 64> &board, std::array<int, 64> &legalPossibilities, int moveNumber, int square) {
    board[square] = moveNumber;

    if (moveNumber == 64) {
        printBoard(board);
        exit(EXIT_SUCCESS);
    }

    auto copy = legalPossibilities;
    int x = square % 8, y = square / 8;
    std::vector<int> disconnected;

    if (x <= 6 && y >= 2) {
        copy[square - 15]--;
        if (copy[square - 15] == 0) {
            disconnected.push_back(square - 15);
        }
    }

    if (x <= 5 && y >= 1) {
        copy[square - 6]--;
        if (copy[square - 6] == 0) {
            disconnected.push_back(square - 6);
        }
    }

    if (x <= 5 && y <= 6) {
        copy[square + 10]--;
        if (copy[square + 10] == 0) {
            disconnected.push_back(square + 10);
        }
    }

    if (x <= 6 && y <= 5) {
        copy[square + 17]--;
        if (copy[square + 17] == 0) {
            disconnected.push_back(square + 17);
        }
    }

    if (x >= 1 && y <= 5) {
        copy[square  + 15]--;
        if (copy[square + 15] == 0) {
            disconnected.push_back(square + 15);
        }
    }

    if (x >= 2 && y <= 6) {
        copy[square + 6]--;
        if (copy[square + 6] == 0) {
            disconnected.push_back(square + 6);
        }
    }
    
    if (x >= 2 && y >= 1) {
        copy[square - 10]--;
        if (copy[square - 10] == 0) {
            disconnected.push_back(square - 10);
        }
    }

    if (x >= 1 && y >= 2) {
        copy[square - 17]--;
        if (copy[square - 17] == 0) {
            disconnected.push_back(square - 17);
        }
    }

    if (disconnected.size() >= 2) {
        board[square] = 0;
        return false;
    } else if (disconnected.size() == 1) {
        auto state = update(board, copy, moveNumber+1, disconnected.front());
        if (state) {
            solver(board, copy, moveNumber+1, disconnected.front());
        }

        board[square] = 0;
        return false;
    }

    legalPossibilities = copy;
    legalPossibilities[square] = 255;
    return true;
}

void initialize(std::array<int, 64> &board, std::array<int, 64> &legalPossibilities) {
    // fill board with 0s
    board.fill(0);

    // initialize legalPossibilities
    legalPossibilities.fill(8);

    for (int i = 2; i < 6; i++) {
        legalPossibilities[i] = 4;
    }

    for (int i = 10; i < 14; i++) {
        legalPossibilities[i] = 6;
    }

    for (int i = 50; i < 54; i++) {
        legalPossibilities[i] = 6;
    }

    for (int i = 58; i < 62; i++) {
        legalPossibilities[i] = 4;
    }

    for (int i = 16; i < 41; i+=8) {
        legalPossibilities[i] = 4;
    }

    for (int i = 17; i < 42; i+=8) {
        legalPossibilities[i] = 6;
    }

    for (int i = 22; i < 47; i+=8) {
        legalPossibilities[i] = 6;
    }

    for (int i = 23; i < 58; i+=8) {
        legalPossibilities[i] = 4;
    }

    legalPossibilities[ 9] = 4;
    legalPossibilities[14] = 4;
    legalPossibilities[49] = 4;
    legalPossibilities[54] = 4;

    legalPossibilities[ 0] = 2;
    legalPossibilities[ 7] = 2;
    legalPossibilities[56] = 2;
    legalPossibilities[63] = 2;

    legalPossibilities[ 1] = 3;
    legalPossibilities[ 6] = 3;
    legalPossibilities[ 8] = 3;
    legalPossibilities[15] = 3;
    legalPossibilities[48] = 3;
    legalPossibilities[55] = 3;
    legalPossibilities[57] = 3;
    legalPossibilities[62] = 3;
}

void printBoard(std::array<int, 64> board) {
    for (int i = 0; i < 64; i++) {
        if (i % 8 == 0) {
            std::cout << "\n";
        }

        if (board[i] < 10) {
            std::cout << " ";
        }
        std::cout << board[i] << " ";
    }
}