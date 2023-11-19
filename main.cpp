#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <chrono>

// Starting square must be less than 64 
// or program will crash on the first line of update.
// No safety net
#define STARTING_SQUARE 10
// Calculating time defines the amount of time (in s) allocated to find solutions
#define CALCULTING_TIME 0
#define SOLUTION_COUNT_ORDER 33554432

void solver(
    std::array<unsigned char, 64>  board, std::array<unsigned char, 64>  legalPossibilities,
    unsigned char moveNumber, unsigned char square, int& solutionCount,
    std::chrono::steady_clock::time_point startTime
);
void update(
    std::array<unsigned char, 64> board, std::array<unsigned char, 64> legalPossibilities,
    unsigned char moveNumber, unsigned char square, int& solutionCount,
    std::chrono::steady_clock::time_point startTime
);
void printBoard(const std::array<unsigned char, 64>& board);

int main() {
    // test 2
    int solutionCount = 0;
    std::array<unsigned char, 64> board,
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
    update(board, legalPossibilities, 1, STARTING_SQUARE, solutionCount, startTime);

    return 0;
}

void solver(
    std::array<unsigned char, 64>  board, std::array<unsigned char, 64>  legalPossibilities,
    unsigned char moveNumber, unsigned char square, int& solutionCount,
    std::chrono::steady_clock::time_point startTime
) {
    unsigned char x = square % 8, y = square / 8;
    std::array<std::array<unsigned char, 2>, 8> indexPair;
    indexPair.fill(std::array<unsigned char, 2>{(unsigned char)255, (unsigned char)255});

    // Fill indexPair with valid index and possibility pairs
    if (x <= 6 && y >= 2) {
        indexPair[0] = std::array<unsigned char, 2>{(unsigned char)(square - 15), legalPossibilities[square - 15]};
    }

    if (x <= 5 && y >= 1) {
        indexPair[1] = std::array<unsigned char, 2>{(unsigned char)(square - 6), legalPossibilities[square - 6]};
    }

    if (x <= 5 && y <= 6) {
        indexPair[2] = std::array<unsigned char, 2>{(unsigned char)(square + 10), legalPossibilities[square + 10]};
    }

    if (x <= 6 && y <= 5) {
        indexPair[3] = std::array<unsigned char, 2>{(unsigned char)(square + 17), legalPossibilities[square + 17]};
    }

    if (x >= 1 && y <= 5) {
        indexPair[4] = std::array<unsigned char, 2>{(unsigned char)(square + 15), legalPossibilities[square  + 15]};
    }

    if (x >= 2 && y <= 6) {
        indexPair[5] = std::array<unsigned char, 2>{(unsigned char)(square + 6), legalPossibilities[square + 6]};
    }
    
    if (x >= 2 && y >= 1) {
        indexPair[6] = std::array<unsigned char, 2>{(unsigned char)(square - 10), legalPossibilities[square - 10]};
    }

    if (x >= 1 && y >= 2) {
        indexPair[7] = std::array<unsigned char, 2>{(unsigned char)(square - 17), legalPossibilities[square - 17]};
    }

    // Order indexes on their possibilites
    std::sort(
        indexPair.begin(),
        indexPair.end(),
        [](std::array<unsigned char, 2> a, std::array<unsigned char, 2> b) {
            return a[1] > b[1];
        }
    );

    // Loop throigh the indexPair
    for (auto [index, possibility]: indexPair) {
        if (possibility > 100) continue;
        update(board, legalPossibilities, moveNumber+1, index, solutionCount, startTime);
    }
}

void update(
    std::array<unsigned char, 64> board, std::array<unsigned char, 64> legalPossibilities,
    unsigned char moveNumber, unsigned char square, int& solutionCount,
    std::chrono::steady_clock::time_point startTime
) { board[square] = moveNumber;
    legalPossibilities[square] = 255;

    if (moveNumber == 64) {
        solutionCount++;
        auto endTime = std::chrono::steady_clock::now(); // No time penalty
        if (   solutionCount
        //     !(solutionCount % 10000)  && solutionCount < 100000  ||
        //     !(solutionCount % 100000) && solutionCount < 1000000 ||
        //     !(solutionCount % 1000000)
        ) {
        //     bool usingSeconds = true;
            u_int64_t timeTaken = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
        //     if (timeTaken < 300) {
        //         usingSeconds = false;
        //         timeTaken = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
        //     }

            std::cout << solutionCount << " " << timeTaken << "\n";

            if (/*usingSeconds &&*/ timeTaken > CALCULTING_TIME) {
                printBoard(board);
                exit(EXIT_SUCCESS);
            }
        }

        return;
    }
    
    unsigned char x = square % 8, y = square / 8;
    std::vector<unsigned char> disconnected;

    if (x <= 6 && y >= 2) {
        legalPossibilities[square - 15]--;
        if (legalPossibilities[square - 15] == 0) {
            disconnected.push_back(square - 15);
        }
    }

    if (x <= 5 && y >= 1) {
        legalPossibilities[square - 6]--;
        if (legalPossibilities[square - 6] == 0) {
            disconnected.push_back(square - 6);
        }
    }

    if (x <= 5 && y <= 6) {
        legalPossibilities[square + 10]--;
        if (legalPossibilities[square + 10] == 0) {
            disconnected.push_back(square + 10);
        }
    }

    if (x <= 6 && y <= 5) {
        legalPossibilities[square + 17]--;
        if (legalPossibilities[square + 17] == 0) {
            disconnected.push_back(square + 17);
        }
    }

    if (x >= 1 && y <= 5) {
        legalPossibilities[square  + 15]--;
        if (legalPossibilities[square + 15] == 0) {
            disconnected.push_back(square + 15);
        }
    }

    if (x >= 2 && y <= 6) {
        legalPossibilities[square + 6]--;
        if (legalPossibilities[square + 6] == 0) {
            disconnected.push_back(square + 6);
        }
    }
    
    if (x >= 2 && y >= 1) {
        legalPossibilities[square - 10]--;
        if (legalPossibilities[square - 10] == 0) {
            disconnected.push_back(square - 10);
        }
    }

    if (x >= 1 && y >= 2) {
        legalPossibilities[square - 17]--;
        if (legalPossibilities[square - 17] == 0) {
            disconnected.push_back(square - 17);
        }
    }

    if (disconnected.size() >= 2) return;
    if (disconnected.size() == 1) {
        update(board, legalPossibilities, moveNumber+1, disconnected.front(), solutionCount, startTime);
        return;
    }

    solver(board, legalPossibilities, moveNumber, square, solutionCount, startTime);
}

void printBoard(const std::array<unsigned char, 64>& board) {
    for (unsigned char i = 0; i < 64; i++) {
        if (i % 8 == 0 && i != 0) {
            std::cout << "\n";
        }

        if (board[i] < 10) { std::cout << " "; } // Alignment
        std::cout << (int)board[i] << " ";
    }

    std::cout << "\n\n";
}