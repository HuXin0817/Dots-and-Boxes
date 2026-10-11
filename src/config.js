import { parsePlayerType } from "./game/robots";

// Supported range: 1-6 for the compact topology and move buffers.
export const BoardSize = 6;

export function readPlayerConfig() {
  const params = new URLSearchParams(window.location.search);
  return {
    player1Type: parsePlayerType(params.get("player1")),
    player2Type: parsePlayerType(params.get("player2")),
  };
}
