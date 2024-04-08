#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <map>


bool isWin(const std::array<std::string, 9> &board, const std::string &player) {
    // Check rows
    for (int i = 0; i < 9; i += 3) {
        if (board[i] == player && board[i] == board[i + 1] && board[i + 1] == board[i + 2]) {
            return true;
        }
    }

    // Check columns
    for (int i = 0; i < 3; i++) {
        if (board[i] == player && board[i] == board[i + 3] && board[i + 3] == board[i + 6]) {
            return true;
        }
    }

    // Check diagonals
    if (board[0] == player && board[0] == board[4] && board[4] == board[8]) {
        return true;
    }
    if (board[2] == player && board[2] == board[4] && board[4] == board[6]) {
        return true;
    }

    return false;
}

bool isFull(const std::array<std::string, 9> &board) {
    for (int i = 0; i < 9; i++) {
        if (board[i] == "") {
            return false;
        }
    }
    return true;
}

std::string getBoardString(const std::array<std::string, 9> &board) {
    std::string board_str = "";
    for (int i = 0; i < 9; i++) {
        if (board[i] == "") {
            board_str += std::to_string(i + 1);
        } else {
            board_str += board[i];
        }
        if (i % 3 == 2) {
            board_str += "\n";
            board_str += "---------\n";
        } else {
            board_str += " | ";
        }
    }
    return board_str;
}

std::unordered_map<std::string, int> scores;

int minimax(std::array<std::string, 9> board, std::string player, std::string ai_player) {
    if (isWin(board, ai_player)) {
        return 1;
    }  
    if (isWin(board, ai_player == "X" ? "O" : "X")) {
        return -1;
    }
    if (isFull(board)) {
        return 0;
    }

    int best_score = player == ai_player ? -2 : 2;
    for (int i = 0; i < 9; i++) {
        if (board[i] == "") {
            std::array<std::string, 9> new_board = board;
            new_board[i] = player;
            int score;
            if (scores.find(getBoardString(new_board) + player) != scores.end()) {
                score = scores[getBoardString(new_board) + player];
            } else {
                score = minimax(new_board, player == "X" ? "O" : "X", ai_player);
                scores[getBoardString(new_board) + player] = score;
            }
            if (player == ai_player) {
                best_score = std::max(score, best_score);
            } else {
                best_score = std::min(score, best_score);
            }
        }
    }

    return best_score;
}

int getBestMove(const std::array<std::string, 9> &board, const std::string &player) {
    int best_move = -1;
    int best_score = -2;
    for (int i = 0; i < 9; i++) {
        if (board[i] == "") {
            std::array<std::string, 9> new_board = board;
            new_board[i] = player;
            int score = minimax(new_board, player == "X" ? "O" : "X", player);
            // printf("Move: %d, Score: %d, Player: %s\n", i + 1, score, player.c_str());
            if (score > best_score) {
                best_score = score;
                best_move = i;
            }
        }
    }
    return best_move;
}

std::string findTurn(const std::array<std::string, 9> &board) {
    int x_count = 0;
    int o_count = 0;
    for (int i = 0; i < 9; i++) {
        if (board[i] == "X") {
            x_count++;
        } else if (board[i] == "O") {
            o_count++;
        }
    }
    return x_count > o_count ? "O" : "X";
}

void game(std::array<std::string, 9> &board, const std::string &player1_char, const std::string &player2_char) {
    std::string player = player1_char;
    int turn = 0;

    while (turn < 9) {
        int move;
        std::cout << "Player " << player << std::endl; 
        std::cout << getBoardString(board) << std::endl;
        std::cout << "Enter your move (1-9): ";
        std::cin >> move;
        if (move < 1 || move > 9) {
            std::cout << "Invalid move. Please enter a number between 1 and 9." << std::endl;
            continue;
        }
        if (board[move - 1] != "") {
            std::cout << "Invalid move. That spot is already taken." << std::endl;
            continue;
        }
        board[move - 1] = player;
        turn++;
        if (isWin(board, player)) {
            std::cout << "Player " << player << " wins!" << std::endl;
            return;
        }
        player = player == player1_char ? player2_char : player1_char;
    }
    if (turn == 9) {
        std::cout << "It's a tie!" << std::endl;
    }
}

void aiGame(std::array<std::string, 9> &board, const std::string &player1_char, const std::string &player2_char) {
    std::string player = player1_char;
    while (!isFull(board)) {
        int move;
        std::cout << "Player " << player << std::endl; 
        std::cout << getBoardString(board) << std::endl;
        if (player == player1_char) {
            std::cout << "Enter your move (1-9): ";
            std::cin >> move;
            if (move < 1 || move > 9) {
                std::cout << "Invalid move. Please enter a number between 1 and 9." << std::endl;
                continue;
            }
            if (board[move - 1] != "") {
                std::cout << "Invalid move. That spot is already taken." << std::endl;
                continue;
            }
            board[move - 1] = player;
        } else {
            std::cout << "Player " << player << std::endl; 
            move = getBestMove(board, player);
            board[move] = player;
            std::cout << getBoardString(board) << std::endl;
        }
        if (isWin(board, player)) {
            std::cout << getBoardString(board) << std::endl;
            std::cout << "Player " << player << " wins!" << std::endl;
            return;
        }
        player = player == player1_char ? player2_char : player1_char;
    }
    std::cout << "It's a tie!" << std::endl;
}

// std::map<std::string, int> boardStates;
void generateAllGameStates(std::array<std::string, 9> board, std::string player, std::map<std::string, int> &boardStates, std::vector<std::array<std::string, 9>> &states) {
    // If the board is in a terminal state (win, lose, or draw), stop recursion
    std::string boardString = getBoardString(board);

    if (isWin(board, "X") || isWin(board, "O") || isFull(board)) {
        return;
    }

    // For each empty spot on the board, make a move and recursively generate the game states
    for (int i = 0; i < 9; ++i) {
        if (board[i] == "") {
            board[i] = player;
            std::string bstring = getBoardString(board);
            if (boardStates.find(bstring) == boardStates.end()) {
                states.push_back(board);
                boardStates[bstring] = getBestMove(board, player == "X" ? "O" : "X");
            }
            generateAllGameStates(board, player == "X" ? "O" : "X", boardStates, states);
            board[i] = "";
        }
    }
}

void generateAi(const std::vector<std::array<std::string, 9>> &states, const std::map<std::string, int> &boardStates) {
    std::unordered_map<std::string, int> scores;
    std::ofstream myfile;
    myfile.open("tictactoe_ai.cpp");
    myfile << 
    R"(
    #include <iostream>
    #include <string>
    #include <array>
    #include <vector>

    bool isWin(const std::array<std::string, 9> &board, const std::string &player) {
        // Check rows
        for (int i = 0; i < 9; i += 3) {
            if (board[i] == player && board[i] == board[i + 1] && board[i + 1] == board[i + 2]) {
                return true;
            }
        }

        // Check columns
        for (int i = 0; i < 3; i++) {
            if (board[i] == player && board[i] == board[i + 3] && board[i + 3] == board[i + 6]) {
                return true;
            }
        }

        // Check diagonals
        if (board[0] == player && board[0] == board[4] && board[4] == board[8]) {
            return true;
        }
        if (board[2] == player && board[2] == board[4] && board[4] == board[6]) {
            return true;
        }

        return false;
    }

    bool isFull(const std::array<std::string, 9> &board) {
        for (int i = 0; i < 9; i++) {
            if (board[i] == "") {
                return false;
            }
        }
        return true;
    }

    std::string getBoardString(const std::array<std::string, 9> &board) {
        std::string board_str = "";
        for (int i = 0; i < 9; i++) {
            if (board[i] == "") {
                board_str += std::to_string(i + 1);
            } else {
                board_str += board[i];
            }
            if (i % 3 == 2) {
                board_str += "\n";
                board_str += "---------\n";
            } else {
                board_str += " | ";
            }
        }
        return board_str;
    }

    int getBestMove(const std::array<std::string, 9> &board, const std::string &player) {

    )";
    for (int i = 0; i < states.size(); i++) {
        std::array<std::string, 9> board = states[i];
        // int best_move = getBestMove(board, i % 2 ? "O" : "X");
        std::string board_str = getBoardString(board);
        int best_move = boardStates.at(board_str);
        std::istringstream iss(board_str);
        std::string line;
        if (i == 0) {
            myfile << "if (getBoardString(board) == \"";
            while (std::getline(iss, line)) {
                myfile << line << "\\n";
            }
            myfile << "\") {\n      return " << best_move << ";\n    }\n";
        } else {
            myfile << "    else if (getBoardString(board) == \"";
            while (std::getline(iss, line)) {
                myfile << line << "\\n";
            }
            myfile << "\") {\n      return " << best_move << ";\n    }\n";
        }
    }
    myfile <<
    R"(
        return 0;
    }
    )";

    myfile << 
    R"(
    
    int main() {

        std::cout << "Welcome to Tic Tac Toe!" << std::endl;
        std::cout << "Select player 1 or 2 (X or O): ";
        int player_num;
        std::cin >> player_num;
        std::string player1_char = "X";
        std::string player2_char = "O";

        if (player_num != 1 && player_num != 2) {
            std::cout << "Invalid player number. Please enter 1 or 2." << std::endl;
            return 1;
        }

        std::string player = player1_char;
        int move;
        std::array<std::string, 9> board = {"", "", "", "", "", "", "", "", ""};

        if (player_num == 2) {
            // std::cout << "Player " << player << std::endl; 
            // move = getBestMove(board, player);
            // board[move] = player;
            // std::cout << getBoardString(board) << std::endl;
            player = player == player1_char ? player2_char : player1_char;
        }

        while (!isFull(board)) {
            std::cout << "Player " << player << std::endl; 
            std::cout << getBoardString(board) << std::endl;
            if (player == player1_char) {
                std::cout << "Enter your move (1-9): ";
                std::cin >> move;
                if (move < 1 || move > 9) {
                    std::cout << "Invalid move. Please enter a number between 1 and 9." << std::endl;
                    continue;
                }
                if (board[move - 1] != "") {
                    std::cout << "Invalid move. That spot is already taken." << std::endl;
                    continue;
                }
                board[move - 1] = player;
            } else {
                // std::cout << "Player " << player << std::endl; 
                move = getBestMove(board, player);
                board[move] = player;
                // std::cout << getBoardString(board) << std::endl;
            }
            if (isWin(board, player)) {
                std::cout << getBoardString(board) << std::endl;
                std::cout << "Player " << player << " wins!" << std::endl;
                return 0;
            }
            player = player == player1_char ? player2_char : player1_char;
        }
        std::cout << "It's a tie!" << std::endl;
        return 0;
    }
    )";

    myfile.close();
    std::cout << "AI generated!" << std::endl;
}


int main() {
    std::cout << "Let me cook..." << std::endl;
    
    std::array<std::string, 9> initial_board = {"", "", "", "", "", "", "", "", ""};

    std::vector<std::array<std::string, 9>> states{};
    std::map<std::string, int> boardStates;
    generateAllGameStates(initial_board, "X", boardStates, states);
    printf("Number of game states (X): %d\n", boardStates.size());

    generateAllGameStates(initial_board, "O", boardStates, states);
    printf("Number of game states (Y): %d\n", boardStates.size());

    generateAi(states, boardStates);
    
}