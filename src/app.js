import { BoardSize, readPlayerConfig } from "./config";
import { Board, Owner } from "./game/board";
import { BoardTopology } from "./game/board-topology";
import { PlayerType } from "./game/robots";

const DefaultUnitSize = 8;

function ownerClass(owner) {
  switch (owner) {
    case Owner.Player1:
      return "owner-player-1";
    case Owner.Player2:
      return "owner-player-2";
    default:
      return "owner-none";
  }
}

function createBoardView(model, onEdgeClick) {
  const element = document.createElement("div");
  element.className = "board-stage";
  const logicalWindowSize = model.size * 10 + 18;
  const percent = (value) => `${(value / logicalWindowSize) * 100}%`;

  function addElement(tag, className, left, top, width, height) {
    const child = document.createElement(tag);
    child.className = className;
    Object.assign(child.style, {
      left: percent(left),
      top: percent(top),
      width: percent(width),
      height: percent(height),
    });
    element.append(child);
    return child;
  }

  const boxes = Array.from({ length: model.boxCount }, (_, box) =>
    addElement(
      "div", "box",
      model.boxX(box) * 10 + 10, model.boxY(box) * 10 + 10,
      8, 8,
    ),
  );
  const edges = Array.from({ length: model.edgeCount }, (_, edge) => {
    const dot = model.edgeDot1[edge];
    const rotated = model.edgeRotated(edge);
    const button = addElement(
      "button", "edge",
      8 + model.dotX(dot) * 10 + (rotated ? 0 : 1),
      8 + model.dotY(dot) * 10 + (rotated ? 1 : 0),
      rotated ? 2 : 10, rotated ? 10 : 2,
    );
    button.type = "button";
    button.setAttribute("aria-label", `Edge ${edge}`);
    button.addEventListener("click", () => onEdgeClick(edge));
    return button;
  });
  for (let dot = 0; dot < model.dotCount; dot += 1) {
    addElement(
      "div", "dot",
      8 + model.dotX(dot) * 10, 8 + model.dotY(dot) * 10,
      2, 2,
    );
  }

  return {
    element,
    render(board, interactive, thinking, unitSize) {
      element.setAttribute("aria-busy", String(thinking));
      element.style.setProperty(
        "--preferred-size",
        `${logicalWindowSize * unitSize}px`,
      );
      boxes.forEach((box, index) => {
        box.className = `box ${ownerClass(board.boxOwner(index))}`;
      });
      const lastEdge = board.lastEdge();
      edges.forEach((edge, index) => {
        edge.className = `edge ${ownerClass(board.edgeOwner(index))}${
          index === lastEdge ? " edge-last" : ""
        }`;
        edge.setAttribute(
          "aria-disabled",
          String(!interactive || board.contains(index)),
        );
      });
    },
  };
}

function createResultDialog(board, onRestart, onClose) {
  const backdrop = document.createElement("div");
  backdrop.className = "dialog-backdrop";
  backdrop.innerHTML = `
    <section class="result-dialog" role="dialog" aria-modal="true"
      aria-labelledby="result-message">
      <p id="result-message"></p>
      <div class="dialog-actions">
        <button type="button" data-action="restart">
          <svg aria-hidden="true" width="17" height="17" viewBox="0 0 24 24"
            fill="none" stroke="currentColor" stroke-width="2"
            stroke-linecap="round" stroke-linejoin="round">
            <path d="M3 12a9 9 0 1 0 9-9 9.75 9.75 0 0 0-6.74 2.74L3 8"/>
            <path d="M3 3v5h5"/>
          </svg>
          Restart
        </button>
        <button type="button" data-action="close">
          <svg aria-hidden="true" width="18" height="18" viewBox="0 0 24 24"
            fill="none" stroke="currentColor" stroke-width="2"
            stroke-linecap="round" stroke-linejoin="round">
            <path d="m18 6-12 12M6 6l12 12"/>
          </svg>
          Close
        </button>
      </div>
    </section>
  `;
  let message = "Draw!";
  if (board.relativeScore > 0) {
    message = `Blue Team Win! (Score ${board.player1Score()}:${board.player2Score()})`;
  } else if (board.relativeScore < 0) {
    message = `Red Team Win! (Score ${board.player1Score()}:${board.player2Score()})`;
  }
  backdrop.querySelector("#result-message").textContent = message;
  backdrop.querySelector('[data-action="restart"]').addEventListener("click", onRestart);
  backdrop.querySelector('[data-action="close"]').addEventListener("click", onClose);
  return backdrop;
}

export function createApp(root) {
  const model = new BoardTopology(BoardSize);
  const board = new Board(model);
  const { player1Type, player2Type } = readPlayerConfig();
  const main = document.createElement("main");
  main.className = "app-shell";
  const boardView = createBoardView(model, handleEdgeClick);
  main.append(boardView.element);
  root.replaceChildren(main);

  let worker = null;
  let pendingRequest = 0;
  let thinking = false;
  let closed = false;
  let unitSize = DefaultUnitSize;
  let resultDialog = null;

  function isRobotTurn() {
    return (board.isPlayer1Turn() ? player1Type : player2Type) === PlayerType.Robot;
  }

  function render() {
    if (closed) {
      return;
    }
    const robotTurn = isRobotTurn();
    if (!thinking && board.gaming() && robotTurn && worker) {
      thinking = true;
      worker.postMessage({
        requestId: ++pendingRequest,
        board: board.toSnapshot(),
      });
    }
    boardView.render(
      board,
      !thinking && board.gaming() && !robotTurn,
      thinking,
      unitSize,
    );
    if (!board.gaming() && !resultDialog) {
      resultDialog = createResultDialog(board, restart, close);
      main.append(resultDialog);
    }
  }

  function applyMove(edge) {
    if (
      closed || !board.gaming() ||
      !Number.isInteger(edge) || edge < 0 || edge >= model.edgeCount ||
      board.contains(edge)
    ) {
      return;
    }
    board.add(edge);
    render();
  }

  function handleEdgeClick(edge) {
    if (!thinking && !isRobotTurn()) {
      applyMove(edge);
    }
  }

  function startWorker() {
    const nextWorker = new Worker(
      new URL("./game/robot-worker.js", import.meta.url),
      { type: "module" },
    );
    worker = nextWorker;
    nextWorker.onmessage = ({ data }) => {
      if (closed || worker !== nextWorker || data.requestId !== pendingRequest) {
        return;
      }
      thinking = false;
      applyMove(data.edge);
    };
  }

  function restart() {
    if (closed) {
      return;
    }
    pendingRequest += 1;
    worker?.terminate();
    board.reset();
    thinking = false;
    resultDialog?.remove();
    resultDialog = null;
    startWorker();
    render();
  }

  function destroy() {
    closed = true;
    pendingRequest += 1;
    worker?.terminate();
    worker = null;
    window.removeEventListener("keydown", handleKeyDown);
  }

  function close() {
    destroy();
    main.className = "closed-window";
    main.setAttribute("aria-label", "Dots and Boxes closed");
    main.replaceChildren();
    window.close();
  }

  function handleKeyDown(event) {
    if (!event.ctrlKey && !event.metaKey) {
      return;
    }
    if (event.code === "Minus") {
      event.preventDefault();
      unitSize = Math.max(1, Math.min(unitSize - 1, Math.floor(unitSize * 0.9)));
    } else if (event.code === "Equal") {
      event.preventDefault();
      unitSize = Math.max(unitSize + 1, Math.floor(unitSize * 1.1));
    } else if (event.code === "Digit0") {
      event.preventDefault();
      unitSize = DefaultUnitSize;
    } else {
      return;
    }
    render();
  }

  window.addEventListener("keydown", handleKeyDown);
  startWorker();
  render();
  return destroy;
}
