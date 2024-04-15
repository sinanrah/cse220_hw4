#include "hw4.h"

void initialize_game(ChessGame *game) {
    strcpy(game->chessboard[0], "rnbqkbnr");
    strcpy(game->chessboard[1], "pppppppp");
    for (int i = 2; i < 6; i++) {
        strcpy(game->chessboard[i], "........");
    }
    strcpy(game->chessboard[6], "PPPPPPPP");
    strcpy(game->chessboard[7], "RNBQKBNR");

    game->moveCount = 0;
    game->capturedCount = 0;
    game->currentPlayer = WHITE_PLAYER;

}

bool is_white_piece(char current_char) {
    const char *white_pieces = "RNBQKP";
    return  strchr(white_pieces, current_char);
}

bool is_black_piece(char current_char) {
    const char *black_pieces = "rnbqkp";
    return  strchr(black_pieces, current_char);
}

bool is_empty_space(char current_char) {
    return current_char == '.';
}

void chessboard_to_fen(char fen[], ChessGame *game) {
    int fen_index;
    int current_row, current_col;
    int empty_spaces;

    // iterate each current_row
    for (current_row = 0; current_row < 8; current_row++) {
        empty_spaces = 0;
        // iterate current_col
        for (current_col = 0; current_col < 8; current_col++) {
            char piece = game->chessboard[current_row][current_col];

            if (is_empty_space(piece)) {
                empty_spaces++;
            } else {
                // if current space isnt empty but there are empty spaces before, append to fen
                if (empty_spaces > 0) {
                    fen[fen_index++] = empty_spaces + '0'; // turn it into char
                    empty_spaces = 0; // put empty space count back to 0
                }

                // append the piece character to the fen
                fen[fen_index++] = piece;
            }
        }

        // if empty space at end of current_row, append count to fen
        if (empty_spaces > 0) {
            fen[fen_index++] = empty_spaces + '0';
        }

        // append / for every current_row but last
        if (current_row < 7) {
            fen[fen_index++] = '/';
        }
    }

    // append space for turn
    fen[fen_index++] = ' ';
    fen[fen_index++] = (game->currentPlayer == WHITE_PLAYER) ? 'w' : 'b';

    fen[fen_index] = '\0';
}


bool is_valid_pawn_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {    
    int row_difference = dest_row - src_row;
    int col_difference = abs(dest_col - src_col);
    int row_change = (piece == 'p') ? 1 : -1; 

    // check if pawn is moving more than 2 rows or more than 1 column
    if (abs(row_difference) > 2 || col_difference > 1) return false;

    // check if pawn direction is correct
    if ((piece == 'P' && row_difference > 0) || (piece == 'p' && row_difference < 0)) return false;

    // moving 2 spaces
    if (abs(row_difference) == 2) {
        if (col_difference > 0) return false; // can't move diagonally and move 2 spaces
        // check if at starting current_row
        if ((piece == 'P' && src_row != 6) || (piece == 'p' && src_row != 1)) return false;
        // check if pieces are in the way
        if (!is_empty_space(game->chessboard[src_row + row_change][src_col]) ||
            !is_empty_space(game->chessboard[dest_row][src_col])) return false;
        return true;
    }
    // check if moving diagonally and capturing correctly
    if (col_difference == 1) {
        if (abs(row_difference) != 1 || is_empty_space(game->chessboard[dest_row][dest_col])) return false; // cant diag move to empty space
        if ((is_white_piece(piece) && is_white_piece(game->chessboard[dest_row][dest_col])) ||
            (is_black_piece(piece) && is_black_piece(game->chessboard[dest_row][dest_col]))) return false; // can't capture own piece
        return true;
    } else if (col_difference == 0) {
        // check if pieces are in the way if moving straight
        if (!is_empty_space(game->chessboard[dest_row][dest_col])) return false;
    }

    return true;
}


bool is_valid_rook_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    if (src_row != dest_row && src_col != dest_col) { return false;}        // ensure rook is moving H or V
    if (src_row == dest_row && src_col == dest_col) { return false;}        // ensure it's moving at all
    
    // set increment / decrement to current_row or current_col depending on what direction the rook is moving, both l/r and u/d
    int row_change = (dest_row > src_row) ? 1 : (dest_row < src_row) ? -1 : 0; 
    int col_change = (dest_col > src_col) ? 1 : (dest_col < src_col) ? -1 : 0;
    int current_row = src_row + row_change;
    int current_col = src_col + col_change;

    // check if there are pieces in the way
    while (current_row != dest_row || current_col != dest_col) {
        if (!is_empty_space(game->chessboard[current_row][current_col])) return false; 
        current_row += row_change;
        current_col += col_change;
    }    

    if ((is_white_piece(game->chessboard[src_row][src_col]) && is_white_piece(game->chessboard[dest_row][dest_col])) ||
    (is_black_piece(game->chessboard[src_row][src_col]) && is_black_piece(game->chessboard[dest_row][dest_col]))) return false;               

    return true;
}

bool is_valid_knight_move(int src_row, int src_col, int dest_row, int dest_col) {
    int row_difference = abs(src_row - dest_row);
    int col_difference = abs(src_col - dest_col);
    // change in rows / cols have to be 2 and 1 or 1 and 2
    return (row_difference == 1 && col_difference == 2) || (row_difference == 2 && col_difference == 1);
}

bool is_valid_bishop_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    int row_difference = abs(dest_row - src_row);
    int col_difference = abs(dest_col - src_col);
    int row_increment = (dest_row - src_row > 0) ? 1 : -1;
    int col_increment = (dest_col - src_col > 0) ? 1 : -1;

    if (row_difference != col_difference) return false; // needs to move the same amount of rows and cols
    int current_row = src_row + row_increment;
    int current_col = src_col + col_increment;
    // checck if pieces are in the way
    while (current_row != dest_row) {
        if (!is_empty_space(game->chessboard[current_row][current_col])) return false;
        current_row += row_increment;
        current_col += col_increment;
    }


    return true;
}

bool is_valid_queen_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    // can move either like a bishop or rook
    return is_valid_rook_move(src_row, src_col, dest_row, dest_col, game) ||
           is_valid_bishop_move(src_row, src_col, dest_row, dest_col, game);
}

bool is_valid_king_move(int src_row, int src_col, int dest_row, int dest_col) {
    int row_difference = abs(src_row - dest_row);
    int col_difference = abs(src_col - dest_col);
    // can only more one space H, V, or D
    return row_difference <= 1 && col_difference <= 1;
}

bool is_valid_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    switch (piece) {
        case 'p':
        case 'P':
            return is_valid_pawn_move(piece, src_row, src_col, dest_row, dest_col, game);
        case 'r':
        case 'R':
            return is_valid_rook_move(src_row, src_col, dest_row, dest_col, game);
        case 'n':
        case 'N':
            return is_valid_knight_move(src_row, src_col, dest_row, dest_col);
        case 'b':
        case 'B':
            return is_valid_bishop_move(src_row, src_col, dest_row, dest_col, game);
        case 'q':
        case 'Q':
            return is_valid_queen_move(src_row, src_col, dest_row, dest_col, game);
        case 'k':
        case 'K':
            return is_valid_king_move(src_row, src_col, dest_row, dest_col);
        default:
            return false;
    }



    return false;
}

void fen_to_chessboard(const char *fen, ChessGame *game) {
    // initialize indices
    int current_row = 0;
    int current_col = 0;
    int fen_index = 0;

    // clear the borad
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            game->chessboard[i][j] = '.';
        }
    }

    // parse fen
    while (fen[fen_index] != ' ') {
        char current_char = fen[fen_index];

        if (current_char == '/') {
            current_row++;
            current_col = 0;
        } else if (isdigit(current_char)) {
            current_col += current_char - '0'; // turn to int
        } else {
            game->chessboard[current_row][current_col] = current_char;
            current_col++;
        }

        fen_index++;
    }

    // parse fen to find out whose turn it is
    fen_index++;    // space
    if (fen[fen_index] == 'w') {
        game->currentPlayer = WHITE_PLAYER;
    } else if (fen[fen_index] == 'b') {
        game->currentPlayer = BLACK_PLAYER;
    }
}

int parse_move(const char *move, ChessMove *parsed_move) {
    int length = strlen(move);
    // PARSE_MOVE_INVALID_FORMAT
    if (length != 4 && length != 5) return PARSE_MOVE_INVALID_FORMAT;
    if (move[0] < 'a' || move[0] > 'h') return PARSE_MOVE_INVALID_FORMAT;
    if (move[2] < 'a' || move[2] > 'h') return PARSE_MOVE_INVALID_FORMAT;
    // PARSE_OUT_OF_BOUNDS
    if (move[1] < '1' || move[1] > '8') return PARSE_MOVE_OUT_OF_BOUNDS;
    if (move[3] < '1' || move[3] > '8') return PARSE_MOVE_OUT_OF_BOUNDS;

    if (length == 5) {
        // PARSE_MOVE_INVALID_DESTINATION
        if (move[3] != '1' && move[3] != '8') return PARSE_MOVE_INVALID_DESTINATION;

        // PARSE_MOVE_INVALID_PROMOTION
        if (move[4] != 'q' && move[4] != 'r' && move[4] != 'b' && move[4] != 'n') 
            return PARSE_MOVE_INVALID_PROMOTION;
    }

    // populate parsed_move fields
    parsed_move->startSquare[0] = move[0];
    parsed_move->startSquare[1] = move[1];
    parsed_move->startSquare[2] = '\0';

    parsed_move->endSquare[0] = move[2];
    parsed_move->endSquare[1] = move[3];

    if (length == 5) {
        parsed_move->endSquare[2] = move[4];
        parsed_move->endSquare[3] = '\0';
    } else {
        parsed_move->endSquare[2] = '\0';
    }

    return 0;
}

int make_move(ChessGame *game, ChessMove *move, bool is_client, bool validate_move) {
    // cast move chars to int
    int src_row = '8' - move->startSquare[1]; // subtract from '8' because it is inverted and also 0 indexed
    int src_col = move->startSquare[0] - 'a';
    int dest_row = '8' - move->endSquare[1];
    int dest_col = move->endSquare[0] - 'a';

    char piece = game->chessboard[src_row][src_col];

    if (validate_move) {
        // MOVE OUT OF TURN
        if ((is_client && game->currentPlayer != WHITE_PLAYER) || (!is_client && game->currentPlayer != BLACK_PLAYER)) {
            return MOVE_OUT_OF_TURN;
        }
        // MOVE NOTHING
        if (is_empty_space(piece)) {
            return MOVE_NOTHING;
        }
        // MOVE WRONG COLOR
        if ((is_client && is_black_piece(piece)) || (!is_client && is_white_piece(piece))) {
            return MOVE_WRONG_COLOR;
        }
        // MOVE SUS
        if ((is_white_piece(piece) && is_white_piece(game->chessboard[dest_row][dest_col])) ||
            (is_black_piece(piece) && is_black_piece(game->chessboard[dest_row][dest_col]))) {
            return MOVE_SUS;
        }
        // MOVE NOT A PAWN
        if (strlen(move->endSquare) == 5 && !((piece == 'P' && is_client) || (piece == 'p' && !is_client))) {
            return MOVE_NOT_A_PAWN;
        }
        // MOVE MISSING PROMOTION
        if (strlen(move->endSquare) == 4 && ((piece == 'P' && dest_row == 0) || (piece == 'p' && dest_row == 7))) {
            return MOVE_MISSING_PROMOTION;
        }
        // MOVE WRONG
        if (!is_valid_move(piece, src_row, src_col, dest_row, dest_col, game)) {
            return MOVE_WRONG;
        }
    }

    // update game moves and movecount
    game->moves[game->moveCount] = *move;
    game->moveCount++;

    if (!is_empty_space(game->chessboard[dest_row][dest_col])) {
        game->capturedPieces[game->capturedCount] = game->chessboard[dest_row][dest_col];
        game->capturedCount++;
    }

    game->chessboard[dest_row][dest_col] = piece;
    game->chessboard[src_row][src_col] = '.';

    if (strlen(move->endSquare) == 5) {
        game->chessboard[dest_row][dest_col] = move->endSquare[2];
    }

    game->currentPlayer = (game->currentPlayer == WHITE_PLAYER) ? BLACK_PLAYER : WHITE_PLAYER;

    return 0;

}

// use pointer arithmetic for arg
int send_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    if (strncmp(message, "/move", 5) == 0) {
        ChessMove move;
        if (parse_move(message + 6, &move) == 0) {
            if (make_move(game, &move, is_client, true) == 0) {
                send(socketfd, message, strlen(message), 0);
                return COMMAND_MOVE;
            }
        }
        return COMMAND_ERROR;
    } else if (strcmp(message, "/forfeit") == 0) {
        send(socketfd, message, strlen(message), 0);
        return COMMAND_FORFEIT;
    } else if (strcmp(message, "/chessboard") == 0) {
        display_chessboard(game);
        return COMMAND_DISPLAY;
    } else if (strncmp(message, "/import", 7) == 0 && !is_client) {
        fen_to_chessboard(message + 8, game);
        send(socketfd, message, strlen(message), 0);
        return COMMAND_IMPORT;
    } else if (strncmp(message, "/load", 5) == 0) {
        char arg[256];
        strncpy(arg, message + 6, sizeof(arg) - 1);
        arg[sizeof(arg) - 1] = '\0';
        
        char *user = strtok(arg, " ");
        char *save_number_string = strtok(NULL, " ");
        if (user != NULL && save_number_string != NULL) {
            int save_number = atoi(save_number_string);
            if (load_game(game, user, "game_database.txt", save_number) == 0) {
                send(socketfd, message, strlen(message), 0);
                return COMMAND_LOAD;
            }
        }
        return COMMAND_ERROR;
    } else if (strncmp(message, "/save", 5) == 0) {
        char user[256];
        strncpy(user, message + 6, sizeof(user) - 1);
        user[sizeof(user) - 1] = '\0';
        
        if (save_game(game, user, "game_database.txt") == 0) {
            return COMMAND_SAVE;
        }
        return COMMAND_ERROR;
    }
    
    return COMMAND_UNKNOWN;
}

int receive_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    if (strncmp(message, "/move", 5) == 0) {
        ChessMove move;
        if (parse_move(message + 6, &move) == 0) {
            make_move(game, &move, is_client, false);
            return COMMAND_MOVE;
        }
        return COMMAND_ERROR;
    } else if (strcmp(message, "/forfeit") == 0) {
        close(socketfd);
        return COMMAND_FORFEIT;
    } else if (strncmp(message, "/import", 7) == 0 && is_client) {
        fen_to_chessboard(message + 8, game);
        return COMMAND_IMPORT;
    } else if (strncmp(message, "/load", 5) == 0) {
        char arg[256];
        strncpy(arg, message + 6, sizeof(arg) - 1);
        arg[sizeof(arg) - 1] = '\0';
        
        char *user = strtok(arg, " ");
        char *save_number_string = strtok(NULL, " ");
        if (user != NULL && save_number_string != NULL) {
            int save_number = atoi(save_number_string);
            if (load_game(game, user, "game_database.txt", save_number) == 0) {
                return COMMAND_LOAD;
            }
        }
        return COMMAND_ERROR;
    }
    
    return -1;
}

int save_game(ChessGame *game, const char *user, const char *db_filename) {
    // check user
    if (strlen(user) == 0 || user == NULL || strchr(user, ' ') != NULL) return -1; // strchr checks for a char in string
    
    FILE *file = fopen(db_filename, "a"); // append
    if (file == NULL) {
        return -1;
    }
    char fen[256];
    chessboard_to_fen(fen, game); // get fen of current game
    fprintf(file, "%s:%s\n", user, fen);
    fclose(file);
    return 0;
}

int load_game(ChessGame *game, const char *user, const char *db_filename, int save_number) {
    FILE *file = fopen(db_filename, "r"); // read   
    if (file == NULL) {
        return -1;
    }
    char line[256];
    int save_count = 0; // save count for user

    while (fgets(line, sizeof(line), file)) { // fgets until end
        char *token = strtok(line, ":"); // split lines between :
        if (strcmp(token, user) == 0) { // check for same user n increment save count for the user 
            save_count++;
            if (save_count == save_number) {
                token = strtok(NULL, "\n"); // end of fen string should be newline
                if (token == NULL) {
                    fclose(file);
                    return -1; // missing fen
                }
                fen_to_chessboard(token, game); // parse fen, load into game
                fclose(file);
                return 0;
            }
        }
    }

    fclose(file);
    return -1;
}

void display_chessboard(ChessGame *game) {
    printf("\nChessboard:\n");
    printf("  a b c d e f g h\n");
    for (int i = 0; i < 8; i++) {
        printf("%d ", 8 - i);
        for (int j = 0; j < 8; j++) {
            printf("%c ", game->chessboard[i][j]);
        }
        printf("%d\n", 8 - i);
    }
    printf("  a b c d e f g h\n");
}
