// CSV logging functionality
let logging = false;
let csvData = "Time (ms),Roll,Pitch,Yaw,GyroX,GyroY,GyroZ,Throttle,RC_Roll,RC_Pitch,RC_Yaw,Aux1,Aux2\n";

function startLogging() {
  logging = true;
  csvData = "Time (ms),Roll,Pitch,Yaw,GyroX,GyroY,GyroZ,Throttle,RC_Roll,RC_Pitch,RC_Yaw,Aux1,Aux2\n";
  log("[INFO] Logging started.");
}

function stopLogging() {
  logging = false;
  log("[INFO] Logging stopped.");
}

function downloadCSV() {
  const blob = new Blob([csvData], { type: "text/csv" });
  const url = URL.createObjectURL(blob);
  const a = document.createElement("a");
  a.href = url;
  a.download = "sensor_log.csv";
  document.body.appendChild(a);
  a.click();
  document.body.removeChild(a);
  URL.revokeObjectURL(url);
  log("[INFO] CSV downloaded.");
}

// Add data to CSV when logging is active
function addToCSV(timestamp, roll, pitch, yaw, gx, gy, gz, thr, r_roll, r_pitch, r_yaw, aux1, aux2) {
  if (logging) {
    csvData += `${timestamp},${roll},${pitch},${yaw},${gx},${gy},${gz},${thr},${r_roll},${r_pitch},${r_yaw},${aux1},${aux2}\n`;
  }
} 