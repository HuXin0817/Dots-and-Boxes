import { Board } from "./board";
import { BoardModel } from "./model";
import { Robot } from "./robots";

const models = new Map();
const robots = new Map();

self.onmessage = ({ data }) => {
  let model = models.get(data.board.size);
  if (!model) {
    model = new BoardModel(data.board.size);
    models.set(data.board.size, model);
  }

  const board = new Board(model);
  for (const edge of data.board.moves) {
    board.add(edge);
  }

  const robotKey = `${data.board.size}:${board.turn}`;
  let robot = robots.get(robotKey);
  if (!robot) {
    robot = new Robot(model);
    robots.set(robotKey, robot);
  }

  self.postMessage({
    requestId: data.requestId,
    edge: robot.move(board),
  });
};
