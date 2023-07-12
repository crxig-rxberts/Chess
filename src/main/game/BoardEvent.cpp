#include "BoardEvent.hpp"
#include "PieceMovement.hpp"

int BoardEvent::draggedPieceArrayIndex = -1;
sf::Vector2f BoardEvent::originalPos;
sf::Vector2f BoardEvent::mouseOffset;
sf::Vector2f BoardEvent::originalScale;
std::vector<sf::Vector2i> BoardEvent::possibleMoves;

void BoardEvent::handleMouseButtonEvent(Board& board, const sf::Event::MouseButtonEvent &event, bool isPressed) {
    float x = event.x;
    float y = event.y;

    if (isPressed) {
        if (event.button == sf::Mouse::Left) {
            findClickedPiece(board, x, y);

            if (draggedPieceArrayIndex >= 0) {
                originalPos.x = x / board.tileSize;
                originalPos.y = y / board.tileSize;
                findLegalMoves(board, draggedPieceArrayIndex);
            }
        }
    } else {
        releasePiece(board, event.x, event.y);
    }
}

void BoardEvent::handleMouseMoveEvent(Board& board, const sf::Event::MouseMoveEvent &event) {
    if (draggedPieceArrayIndex != -1 && draggedPieceArrayIndex < board.pieces.size()) {
        board.pieces[draggedPieceArrayIndex].setPosition(sf::Vector2f(event.x, event.y) + mouseOffset);
    }
}


void BoardEvent::findClickedPiece(Board& board, float x, float y) {
    for (size_t i = 0; i < board.pieces.size(); ++i) {
        auto const &piece = board.pieces[i];
        if (piece.getGlobalBounds().contains(x, y) && piece.getPieceIndex() * board.currentPlayerTurn > 0) {
            draggedPieceArrayIndex = i;
            mouseOffset = piece.getPosition() - sf::Vector2f(x, y);
            originalScale = piece.getScale(); // Store the original scale
            break;
        }
    }
}

void BoardEvent::findLegalMoves(Board& board, int pieceIndex) {
    auto const& piece = board.pieces[pieceIndex];
    possibleMoves = PieceMovement::calculateMovesForPiece(piece, board);
}


void BoardEvent::releasePiece(Board& board, float x, float y) {
    if (draggedPieceArrayIndex != -1) {
        auto col = static_cast<int>((x + mouseOffset.x) / board.tileSize);
        auto row = static_cast<int>((y + mouseOffset.y) / board.tileSize);

        if (isPossibleMove(col, row)) {
            handleLegalMove(board, col, row);
        } else {
            revertMove(board);
        }
        resetDragState(board);
    }
}

bool BoardEvent::isPossibleMove(int col, int row) {
    sf::Vector2i newTile(col, row);
    return std::any_of(possibleMoves.begin(), possibleMoves.end(),
                       [newTile](const auto& move){ return move == newTile; });
}

void BoardEvent::handleLegalMove(Board& board, int col, int row) {
    Piece tempPiece = board.pieces[draggedPieceArrayIndex];
    int targetPieceIndex = getPieceIndexAtTile(board, col, row);

    if (targetPieceIndex == 0 || tempPiece.getPieceIndex() * targetPieceIndex < 0) {
        updatePieceLayout(board, col, row, tempPiece);
        removeTargetPiece(board, col, row);
        snapPieceToTileCenter(board, col, row);
        board.currentPlayerTurn *= -1;
    }
}

void BoardEvent::updatePieceLayout(Board& board, int col, int row, const Piece& piece) {
    auto oldCol = static_cast<int>(originalPos.x);
    auto oldRow = static_cast<int>(originalPos.y);
    board.pieceLayout[oldRow][oldCol] = 0;
    board.pieceLayout[row][col] = piece.getPieceIndex();
}

void BoardEvent::removeTargetPiece(Board& board, int col, int row) {
    int targetPieceArrayIndex = getTargetPieceArrayIndexAtTile(board, col, row);
    if (targetPieceArrayIndex != -1) {
        board.pieces[targetPieceArrayIndex].hide();
        board.pieces.erase(board.pieces.begin() + targetPieceArrayIndex);
        if(targetPieceArrayIndex < draggedPieceArrayIndex){
            draggedPieceArrayIndex--;
        }
    }
}

void BoardEvent::revertMove(Board& board) {
    auto oldCol = static_cast<int>(originalPos.x);
    auto oldRow = static_cast<int>(originalPos.y);
    snapPieceToTileCenter(board, oldCol, oldRow);
}

void BoardEvent::resetDragState(Board& board) {
    board.pieces[draggedPieceArrayIndex].setScale(originalScale);
    draggedPieceArrayIndex = -1;
}

void BoardEvent::snapPieceToTileCenter(Board& board, int col, int row) {
    sf::Vector2f newCenter(
            col * board.tileSize + board.tileSize / 2,
            row * board.tileSize + board.tileSize / 2
            );

    sf::FloatRect bounds = board.pieces[draggedPieceArrayIndex].getGlobalBounds();
    sf::Vector2f topLeftOffset(
            bounds.width / 2,
            bounds.height / 2
            );

    board.pieces[draggedPieceArrayIndex].setPosition(sf::Vector2f(newCenter.x - (topLeftOffset.x * originalScale.x), newCenter.y - (topLeftOffset.y * originalScale.y)));
}


int BoardEvent::getPieceIndexAtTile(Board board, int col, int row) {
    for (size_t i = 0; i < board.pieces.size(); ++i) {
        if (i == draggedPieceArrayIndex) {
            continue; // Ignore the dragged piece
        }

        const auto &piece = board.pieces[i];
        sf::Vector2f position = piece.getPosition();
        auto pieceCol = static_cast<int>(position.x / board.tileSize);
        auto pieceRow = static_cast<int>(position.y / board.tileSize);

        if (pieceCol == col && pieceRow == row) {
            return piece.getPieceIndex();
        }
    }

    return 0;
}

int BoardEvent::getTargetPieceArrayIndexAtTile(Board& board, int col, int row) {
    for (size_t i = 0; i < board.pieces.size(); ++i) {
        if (i == draggedPieceArrayIndex) {
            continue; // Ignore the dragged piece
        }

        const auto &piece = board.pieces[i];
        sf::Vector2f position = piece.getPosition();
        auto pieceCol = static_cast<int>(position.x / board.tileSize);
        auto pieceRow = static_cast<int>(position.y / board.tileSize);

        if (pieceCol == col && pieceRow == row) {
            return i;
        }
    }
    return -1;
}