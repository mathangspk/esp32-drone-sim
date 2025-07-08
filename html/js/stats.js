// Statistics calculation for timestamp gaps
let gapMin = Infinity;
let gapMax = 0;
let gapSum = 0;
let gapCount = 0;

function updateGapStats(gap) {
  gapMin = Math.min(gapMin, gap);
  gapMax = Math.max(gapMax, gap);
  gapSum += gap;
  gapCount++;

  document.getElementById("gapMin").textContent = gapMin.toFixed(1);
  document.getElementById("gapMax").textContent = gapMax.toFixed(1);
  document.getElementById("gapAvg").textContent = (gapSum / gapCount).toFixed(1);
}

function resetStats() {
  gapMin = Infinity;
  gapMax = 0;
  gapSum = 0;
  gapCount = 0;
  
  document.getElementById("gapMin").textContent = "-";
  document.getElementById("gapMax").textContent = "-";
  document.getElementById("gapAvg").textContent = "-";
} 