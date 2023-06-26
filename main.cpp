#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <chrono>

// Starting square must be less than 64 
// or program will crash on the first line of update.
// No safety net
#define STARTING_SQUARE 0
// Calculating time defines the amount of time (in µs) allocated to find solutions
#define CALCULTING_TIME 1

void solver(
    std::array<int, 64>  board, std::array<int, 64>  legalPossibilities,
    const int &moveNumber, const int& square, int& solutionCount,
    const std::chrono::steady_clock::time_point &startTime
);
bool update(
    std::array<int, 64> &board, std::array<int, 64> &legalPossibilities,
    const int &moveNumber, const int& square, int& solutionCount,
    const std::chrono::steady_clock::time_point &startTime
);
void printBoard(const std::array<int, 64>& board);

int main() {
    std::array<int, 64> board,
        legalPossibilities = {
            2, 3, 4, 4, 4, 4, 3, 2,
            3, 4, 6, 6, 6, 6, 4, 3,
            4, 6, 8, 8, 8, 8, 6, 4,
            4, 6, 8, 8, 8, 8, 6, 4,
            4, 6, 8, 8, 8, 8, 6, 4,
            4, 6, 8, 8, 8, 8, 6, 4,
            3, 4, 6, 6, 6, 6, 4, 3,
            2, 3, 4, 4, 4, 4, 3, 2
        };
    auto startTime = std::chrono::steady_clock::now();
    int solutionCount = 0;
    update(board, legalPossibilities, 1, STARTING_SQUARE, solutionCount, startTime);
    solver(board, legalPossibilities, 1, STARTING_SQUARE, solutionCount, startTime);

    return 0;
}

void solver(
    std::array<int, 64>  board, std::array<int, 64>  legalPossibilities,
    const int &moveNumber, const int& square, int& solutionCount,
    const std::chrono::steady_clock::time_point &startTime
) {
    int x = square % 8, y = square / 8;
    std::array<std::array<int, 2>, 8> indexPair;
    indexPair.fill(std::array<int, 2>{-1, 255});

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
        auto copyBoard = board;
        auto copyPossibilities = legalPossibilities;
        if (update(copyBoard, copyPossibilities, moveNumber+1, index, solutionCount, startTime)) {
            solver(copyBoard, copyPossibilities, moveNumber+1, index, solutionCount, startTime);
        }
    }
}

bool update(
    std::array<int, 64> &board, std::array<int, 64> &legalPossibilities,
    const int &moveNumber, const int& square, int& solutionCount,
    const std::chrono::steady_clock::time_point &startTime
) { board[square] = moveNumber;

    if (moveNumber == 64) {
        solutionCount++;
        auto endTime = std::chrono::steady_clock::now();
        
        if (std::chrono::duration_cast<std::chrono::microseconds> (endTime - startTime).count() > CALCULTING_TIME) {
            std::cout << "Solution " << solutionCount << " took " <<
                std::chrono::duration_cast<std::chrono::microseconds> (endTime - startTime).count() << "µs\n";
            printBoard(board);
            exit(EXIT_SUCCESS);
        }

        board[square] = 0;
        return false;
    }

    auto copyPossibilities = legalPossibilities;
    
    int x = square % 8, y = square / 8;
    std::vector<int> disconnected;

    if (x <= 6 && y >= 2) {
        copyPossibilities[square - 15]--;
        if (copyPossibilities[square - 15] == 0) {
            disconnected.push_back(square - 15);
        }
    }

    if (x <= 5 && y >= 1) {
        copyPossibilities[square - 6]--;
        if (copyPossibilities[square - 6] == 0) {
            disconnected.push_back(square - 6);
        }
    }

    if (x <= 5 && y <= 6) {
        copyPossibilities[square + 10]--;
        if (copyPossibilities[square + 10] == 0) {
            disconnected.push_back(square + 10);
        }
    }

    if (x <= 6 && y <= 5) {
        copyPossibilities[square + 17]--;
        if (copyPossibilities[square + 17] == 0) {
            disconnected.push_back(square + 17);
        }
    }

    if (x >= 1 && y <= 5) {
        copyPossibilities[square  + 15]--;
        if (copyPossibilities[square + 15] == 0) {
            disconnected.push_back(square + 15);
        }
    }

    if (x >= 2 && y <= 6) {
        copyPossibilities[square + 6]--;
        if (copyPossibilities[square + 6] == 0) {
            disconnected.push_back(square + 6);
        }
    }
    
    if (x >= 2 && y >= 1) {
        copyPossibilities[square - 10]--;
        if (copyPossibilities[square - 10] == 0) {
            disconnected.push_back(square - 10);
        }
    }

    if (x >= 1 && y >= 2) {
        copyPossibilities[square - 17]--;
        if (copyPossibilities[square - 17] == 0) {
            disconnected.push_back(square - 17);
        }
    }

    if (disconnected.size() >= 2) {
        board[square] = 0;
        return false;
    } else if (disconnected.size() == 1) {
        copyPossibilities[square] = 255;
        if (update(board, copyPossibilities, moveNumber+1, disconnected.front(), solutionCount, startTime)) {
            solver(board, copyPossibilities, moveNumber+1, disconnected.front(), solutionCount, startTime);
        }

        board[square] = 0;
        return false;
    }

    legalPossibilities = copyPossibilities;
    legalPossibilities[square] = 255;
    return true;
}

void printBoard(const std::array<int, 64>& board) {
    for (int i = 0; i < 64; i++) {
        if (i % 8 == 0) {
            std::cout << "\n";
        }

        if (board[i] < 10) {
            std::cout << " ";
        }
        std::cout << board[i] << " ";
    }

    std::cout << "\n\n\n";
}