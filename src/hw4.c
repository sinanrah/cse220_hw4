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
    int row_diff = dest_row - src_row;
    int col_diff = abs(dest_col - src_col);
    int row_change = (piece == 'p') ? 1 : -1; 

    // check if pawn is moving more than 2 rows or not at all, or more than 1 column
    if (abs(row_diff) > 2 || row_diff == 0 || col_diff > 1) return false;

    // check if pawn direction is correct
    if ((piece == 'P' && row_diff >= 0) || (piece == 'p' && row_diff <= 0)) return false;

    // moving 2 spaces
    if (abs(row_diff) == 2) {
        // check if at starting row
        if ((piece == 'P' && src_row != 6) || (piece == 'p' && src_row != 1)) return false;
        // check if pieces are in the way
        if (!is_empty_space(game->chessboard[src_row + row_change][src_col]) ||
            !is_empty_space(game->chessboard[dest_row][src_col])) {
            return false;
        }
    }

    // check if moving diagonally and capturing correctly
    if (col_diff == 1) {
        if (abs(row_diff) != 1 || is_empty_space(game->chessboard[dest_row][dest_col])) return false; // cant diag move to empty space
        if ((is_white_piece(piece) && is_white_piece(game->chessboard[dest_row][dest_col])) ||
            (is_black_piece(piece) && is_black_piece(game->chessboard[dest_row][dest_col]))) return false; // can't capture own piece
    } else if (col_diff == 0) {
        // check if pieces are in the way if moving straight
        int current_row = src_row + row_change;
        while ((piece == 'P' ? current_row > dest_row : current_row < dest_row)) {
            if (!is_empty_space(game->chessboard[current_row][src_col])) return false;
            current_row += row_change;
        }
    }

    return true;
}


bool is_valid_rook_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    if (src_row != dest_row && src_col != dest_col) { return false;}        // ensure rook is moving H or V
    if (src_row == dest_row && src_col == dest_col) { return false;}        // ensure it's moving at all
    
    // set increment / decrement to row or col depending on what direction the rook is moving, both l/r and u/d
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
