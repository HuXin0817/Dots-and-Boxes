import { MaxBoardSize, MinBoardSize } from "./game/model";
import { parsePlayerType } from "./game/robots";

function parseBoardSize(value) {
  const boardSize = Number(value ?? 6);
  if (!Number.isFinite(boardSize)) {
    return 6;
  }
  return Math.min(
    MaxBoardSize,
    Math.max(MinBoardSize, Math.floor(boardSize)),
  );
}

export function readGameConfig() {
  const params = new URLSearchParams(window.location.search);
  return {
    boardSize: parseBoardSize(
      params.get("boardsize") ?? params.get("size"),
    ),
    player1Type: parsePlayerType(params.get("player1")),
    player2Type: parsePlayerType(params.get("player2")),
  };
}
