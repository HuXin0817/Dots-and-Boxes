import { parsePlayerType } from "./game/robots";

// Supported range: 1-6; exact-search cache values must fit in one byte.
export const BoardSize = 6;

export function readPlayerConfig() {
  const params = new URLSearchParams(window.location.search);
  return {
    player1Type: parsePlayerType(params.get("player1")),
    player2Type: parsePlayerType(params.get("player2")),
  };
}
