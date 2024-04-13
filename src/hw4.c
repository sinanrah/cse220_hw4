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

void chessboard_to_fen(char fen[], ChessGame *game) {
    (void)fen;
    (void)game;
}

bool is_white_piece(char ch) {
    const char *white_pieces = "RNBQKP";
    return  strchr(white_pieces, ch);
}

bool is_black_piece(char ch) {
    const char *black_pieces = "rnbqkp";
    return  strchr(black_pieces, ch);
}

bool is_empty_space(char ch) {
    return ch == '.';
}

bool is_valid_pawn_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    int start_row = src_row - 1;
    int end_row = dest_row - 1;
    int start_col = src_col - 1;
    int end_col = dest_col - 1;
    char end = game-> chessboard[end_row][end_col];

    // check if pawn is moving more than 2 rows
    if (abs(dest_row - src_row > 2)) {return false;} 
   
    // check pawn direction
    if (dest_row - src_row > 0 && !is_black_piece(piece)) {return false;}
    if (dest_row - src_row < 0 && !is_white_piece(piece)) {return false;}

    // check if at starting row when moving 2 spaces
    if (abs(dest_row - src_row) == 2) {
        if (src_row != 2 || src_row != 7) {return false;}
    }

    // check if moving diagonally / capturing correctly
    if (src_col != dest_col) {
        if (is_empty_space(end)) {return false;}
        if (is_black_piece(piece) && is_black_piece(end)) {return false;}
        if (is_white_piece(piece) && is_white_piece(end)) {return false;}    
    } else {
    // if moving straight, check if there are pieces in the way
        if (is_black_piece(piece)) {
            for (int i = start_row + 1; i <= end_row; i++) {
                if (!is_empty_space(game->chessboard[i][start_col])) {return false;}
            }
        }
    }

    return true;
}

bool is_valid_rook_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    if (src_row != dest_row && src_col != dest_col) { return false;}        // ensure rook is moving H or V
    if (src_row == dest_row && src_col == dest_col) { return false;}        // ensure it's moving at all
    
    int start_row = src_row - 1;
    int end_row = dest_row - 1;
    int start_col = src_col - 1;
    int end_col = dest_col - 1;

    // if rook moving horizontally
    if (src_row == dest_row) {                                 
        for (int i = start_col + 1; i < end_col; i++) {
            // check if there is a piece in the way
            if (!is_empty_space(game->chessboard[start_row][i])) {            
                printf("Piece in the way of [%d][%d] to [%d][%d].\n",
                src_row, src_col, dest_row, dest_col);
                return false;
            }
        }
    // if rook moving vertically
    } else {    
       for (int i = start_row + 1; i < end_row; i++) {
            // check if there is a piece in the way
            if (!is_empty_space(game->chessboard[i][start_col])) {            
                printf("Piece in the way of [%d][%d] to [%d][%d].\n",
                src_row, src_col, dest_row, dest_col);
                return false;
            }
        }                     
    }

    return true;
}

bool is_valid_knight_move(int src_row, int src_col, int dest_row, int dest_col) {
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    return false;
}

bool is_valid_bishop_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    (void)game;
    return false;
}

bool is_valid_queen_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    (void)game;
    return false;
}

bool is_valid_king_move(int src_row, int src_col, int dest_row, int dest_col) {
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    return false;
}

bool is_valid_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    (void)piece;
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    (void)game;
    return false;
}

void fen_to_chessboard(const char *fen, ChessGame *game) {
    (void)fen;
    (void)game;
}

int parse_move(const char *move, ChessMove *parsed_move) {
    (void)move;
    (void)parsed_move;
    return -999;
}

int make_move(ChessGame *game, ChessMove *move, bool is_client, bool validate_move) {
    (void)game;
    (void)move;
    (void)is_client;
    (void)validate_move;
    return -999;
}

int send_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    (void)game;
    (void)message;
    (void)socketfd;
    (void)is_client;
    return -999;
}

int receive_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    (void)game;
    (void)message;
    (void)socketfd;
    (void)is_client;
    return -999;
}

int save_game(ChessGame *game, const char *username, const char *db_filename) {
    (void)game;
    (void)username;
    (void)db_filename;
    return -999;
}

int load_game(ChessGame *game, const char *username, const char *db_filename, int save_number) {
    (void)game;
    (void)username;
    (void)db_filename;
    (void)save_number;
    return -999;
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
