// Utility functions and data processing
let lastTimestamp = null;
const recentSamples = [];

function log(msg) {
  const logDiv = document.getElementById("log");
  recentSamples.push(msg);
  if (recentSamples.length > 1000) {
    recentSamples.shift();
  }
  logDiv.textContent = recentSamples.join('\n');
  logDiv.scrollTop = logDiv.scrollHeight;
}

function processSensorData(sample) {
  const [ts, roll, pitch, yaw, gx, gy, gz, thr, r_roll, r_pitch, r_yaw, aux1, aux2] = sample;
  
  // Check timestamp gap
  if (lastTimestamp) {
    const gap = ts - lastTimestamp;
    if (gap > 15) {
      log(`[⚠️ GAP] Timestamp gap: ${gap}ms`);
    }
    updateGapStats(gap);
  }
  lastTimestamp = ts;

  // Format and display data
  const line = `⏱ ${ts}ms | IMU: R=${roll.toFixed(1)} P=${pitch.toFixed(1)} Y=${yaw.toFixed(1)} | G=(${gx.toFixed(1)}, ${gy.toFixed(1)}, ${gz.toFixed(1)}) | RC: Th=${thr} R=${r_roll} P=${r_pitch} Y=${r_yaw} Aux=${aux1}/${aux2}`;
  log(line);

  // Add to CSV if logging is active
  addToCSV(ts, roll, pitch, yaw, gx, gy, gz, thr, r_roll, r_pitch, r_yaw, aux1, aux2);
}

function clearLog() {
  const logDiv = document.getElementById("log");
  logDiv.textContent = "";
  recentSamples.length = 0;
  log("[INFO] Log cleared.");
} 