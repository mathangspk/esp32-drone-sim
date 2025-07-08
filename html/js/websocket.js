// WebSocket connection management
let ws = null;

function connectWS() {
  if (ws) ws.close();
  ws = new WebSocket(`ws://192.168.4.1/ws`);

  ws.onopen = () => {
    log("[INFO] WebSocket connected");
  };

  ws.onclose = () => {
    log("[INFO] WebSocket disconnected");
  };

  ws.onerror = err => {
    log("[ERROR] WebSocket: " + err.message);
  };

  ws.onmessage = event => {
    const json = JSON.parse(event.data);
    if (json.type === "batch") {
      json.data.forEach(sample => {
        processSensorData(sample);
      });
    }
  };
}

function disconnectWS() {
  if (ws && ws.readyState === WebSocket.OPEN) {
    ws.close();
  }
}

// Cleanup on page unload
window.addEventListener("beforeunload", () => {
  if (ws && ws.readyState === WebSocket.OPEN) {
    ws.close();
  }
}); 