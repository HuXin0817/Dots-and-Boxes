import { RotateCcw, X } from "lucide-react";
import {
  useCallback,
  useEffect,
  useMemo,
  useRef,
  useState,
} from "react";

import { readGameConfig } from "./config";
import { Board, Owner } from "./game/board";
import { BoardModel } from "./game/model";
import { PlayerType } from "./game/robots";

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

function GameBoard({
  board,
  interactive,
  thinking,
  unitSize,
  onEdgeClick,
}) {
  const { model } = board;
  const logicalWindowSize = model.size * 10 + 18;
  const percent = (value) => `${(value / logicalWindowSize) * 100}%`;
  const stageStyle = {
    "--preferred-size": `${logicalWindowSize * unitSize}px`,
  };

  return (
    <div
      className="board-stage"
      style={stageStyle}
      aria-busy={thinking}
    >
      {Array.from({ length: model.boxCount }, (_, box) => (
        <div
          className={`box ${ownerClass(board.boxOwner(box))}`}
          key={`box-${box}`}
          style={{
            left: percent(model.boxX(box) * 10 + 10),
            top: percent(model.boxY(box) * 10 + 10),
            width: percent(8),
            height: percent(8),
          }}
        />
      ))}

      {Array.from({ length: model.edgeCount }, (_, edge) => {
        const dot = model.edgeDot1[edge];
        const x = model.dotX(dot);
        const y = model.dotY(dot);
        const rotated = model.edgeRotated(edge);
        const claimed = board.contains(edge);
        const last = board.lastEdge() === edge;
        return (
          <button
            type="button"
            className={[
              "edge",
              ownerClass(board.edgeOwner(edge)),
              last ? "edge-last" : "",
            ].join(" ")}
            key={`edge-${edge}`}
            style={{
              left: percent(8 + x * 10 + (rotated ? 0 : 1)),
              top: percent(8 + y * 10 + (rotated ? 1 : 0)),
              width: percent(rotated ? 2 : 10),
              height: percent(rotated ? 10 : 2),
            }}
            aria-label={`Edge ${edge}`}
            aria-disabled={!interactive || claimed}
            onClick={() => onEdgeClick(edge)}
          />
        );
      })}

      {Array.from({ length: model.dotCount }, (_, dot) => (
        <div
          className="dot"
          key={`dot-${dot}`}
          style={{
            left: percent(8 + model.dotX(dot) * 10),
            top: percent(8 + model.dotY(dot) * 10),
            width: percent(2),
            height: percent(2),
          }}
        />
      ))}
    </div>
  );
}

function ResultDialog({ board, onClose, onRestart }) {
  let message = "Draw!";
  if (board.relativeScore > 0) {
    message = `Blue Team Win! (Score ${board.player1Score()}:${board.player2Score()})`;
  } else if (board.relativeScore < 0) {
    message = `Red Team Win! (Score ${board.player1Score()}:${board.player2Score()})`;
  }

  return (
    <div className="dialog-backdrop">
      <section
        className="result-dialog"
        role="dialog"
        aria-modal="true"
        aria-labelledby="result-message"
      >
        <p id="result-message">{message}</p>
        <div className="dialog-actions">
          <button type="button" onClick={onRestart}>
            <RotateCcw aria-hidden="true" size={17} />
            Restart
          </button>
          <button type="button" onClick={onClose}>
            <X aria-hidden="true" size={18} />
            Close
          </button>
        </div>
      </section>
    </div>
  );
}

export default function App() {
  const config = useMemo(readGameConfig, []);
  const model = useMemo(
    () => new BoardModel(config.boardSize),
    [config.boardSize],
  );
  const board = useMemo(() => new Board(model), [model]);
  const boardRef = useRef(board);
  const workerRef = useRef(null);
  const pendingRequest = useRef(0);
  const [revision, setRevision] = useState(0);
  const [thinking, setThinking] = useState(false);
  const [closed, setClosed] = useState(false);
  const [workerGeneration, setWorkerGeneration] = useState(0);
  const [unitSize, setUnitSize] = useState(6 + Math.floor(16 / model.size));

  const applyMove = useCallback((edge) => {
    const currentBoard = boardRef.current;
    if (!currentBoard.gaming() || currentBoard.contains(edge)) {
      return;
    }
    currentBoard.add(edge);
    setRevision((value) => value + 1);
  }, []);

  useEffect(() => {
    const worker = new Worker(
      new URL("./game/robot-worker.js", import.meta.url),
      { type: "module" },
    );
    workerRef.current = worker;
    worker.onmessage = ({ data }) => {
      if (data.requestId !== pendingRequest.current) {
        return;
      }
      setThinking(false);
      applyMove(data.edge);
    };
    return () => {
      worker.terminate();
      if (workerRef.current === worker) {
        workerRef.current = null;
      }
    };
  }, [applyMove, workerGeneration]);

  useEffect(() => {
    const currentBoard = boardRef.current;
    if (closed || thinking || !currentBoard.gaming()) {
      return;
    }
    const playerType = currentBoard.isPlayer1Turn()
      ? config.player1Type
      : config.player2Type;
    if (playerType !== PlayerType.Robot || !workerRef.current) {
      return;
    }

    const requestId = pendingRequest.current + 1;
    pendingRequest.current = requestId;
    const request = {
      requestId,
      board: currentBoard.toSnapshot(),
    };
    setThinking(true);
    workerRef.current.postMessage(request);
  }, [
    closed,
    config.player1Type,
    config.player2Type,
    revision,
    thinking,
    workerGeneration,
  ]);

  useEffect(() => {
    const handleKeyDown = (event) => {
      if (!event.ctrlKey && !event.metaKey) {
        return;
      }
      if (event.code === "Minus") {
        event.preventDefault();
        setUnitSize((current) => {
          const next = Math.min(current - 1, Math.floor(current * 0.9));
          if (next <= 0) {
            return 1;
          }
          return next;
        });
      } else if (event.code === "Equal") {
        event.preventDefault();
        setUnitSize((current) =>
          Math.max(current + 1, Math.floor(current * 1.1)),
        );
      } else if (event.code === "KeyR" || event.code === "Digit0") {
        event.preventDefault();
        setUnitSize(6 + Math.floor(16 / model.size));
      }
    };
    window.addEventListener("keydown", handleKeyDown);
    return () => window.removeEventListener("keydown", handleKeyDown);
  }, [model.size]);

  const currentPlayerType = boardRef.current.isPlayer1Turn()
    ? config.player1Type
    : config.player2Type;
  const interactive =
    !thinking &&
    !closed &&
    boardRef.current.gaming() &&
    currentPlayerType !== PlayerType.Robot;

  const handleEdgeClick = (edge) => {
    if (interactive) {
      applyMove(edge);
    }
  };

  const restart = () => {
    pendingRequest.current += 1;
    boardRef.current.reset();
    setThinking(false);
    setClosed(false);
    setRevision((value) => value + 1);
    setWorkerGeneration((value) => value + 1);
  };

  const close = () => {
    pendingRequest.current += 1;
    workerRef.current?.terminate();
    setClosed(true);
    window.close();
  };

  if (closed) {
    return (
      <main className="closed-window" aria-label="Dots and Boxes closed" />
    );
  }

  return (
    <main className="app-shell">
      <GameBoard
        board={boardRef.current}
        interactive={interactive}
        thinking={thinking}
        unitSize={unitSize}
        onEdgeClick={handleEdgeClick}
      />
      {!boardRef.current.gaming() && (
        <ResultDialog
          board={boardRef.current}
          onRestart={restart}
          onClose={close}
        />
      )}
    </main>
  );
}
