import { createApp } from "./app";
import "./styles.css";

const root = document.getElementById("root");
if (!root) {
  throw new Error("root element was not found");
}

const destroy = createApp(root);
if (import.meta.hot) {
  import.meta.hot.dispose(destroy);
}
