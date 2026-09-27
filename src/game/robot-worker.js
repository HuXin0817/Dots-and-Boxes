import { BoardSize } from "../config";
import { Board } from "./board";
import { BoardTopology } from "./board-topology";
import { Robot } from "./robots";

const model = new BoardTopology(BoardSize);
const robots = new Map();

self.onmessage = ({ data }) => {
  const board = new Board(model);
  for (const edge of data.board.moves) {
    board.add(edge);
  }

  let robot = robots.get(board.turn);
  if (!robot) {
    robot = new Robot(model);
    robots.set(board.turn, robot);
  }

  self.postMessage({
    requestId: data.requestId,
    edge: robot.move(board),
  });
};
