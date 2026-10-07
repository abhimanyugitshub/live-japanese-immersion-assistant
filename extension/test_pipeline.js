const createModule = require("./wasm/audio_window.js");
const { StreamingPipeline } = require("./streaming_pipeline.js");

const sleep = (ms) => new Promise(r => setTimeout(r, ms));

// Usage: node extension/test_pipeline.js <windowSec> <hopSec> <whisperMs>
const WINDOW_SEC = Number(process.argv[2] || 10);
const HOP_SEC = Number(process.argv[3] || 3);
const WHISPER_MS = Number(process.argv[4] || 1500); // simulated Whisper time
const SPEED = 100;            // we run 100x faster than real time
const SR = 16000;
const TOTAL_SEC = 3600;       // one simulated hour

(async () => {
  const M = await createModule();
  const win = new M.AudioWindow(WINDOW_SEC * SR);

  let calls = 0;
  const fakeWhisper = async (audio) => {
    await sleep(WHISPER_MS / SPEED);          // pretend Whisper takes WHISPER_MS
    return `fake text #${++calls}`;
  };

  let outputs = 0;
  const pipe = new StreamingPipeline({
    audioWindow: win, transcribe: fakeWhisper,
    onText: () => { outputs++; }, hopSec: HOP_SEC
  });

  const chunk = new Float32Array(SR / 10);    // 0.1 s of audio per push
  const chunksPerHop = HOP_SEC * 10;
  const heapStart = M.HEAP8.length;

  for (let i = 0; i < TOTAL_SEC * 10; i++) {
    pipe.pushAudio(chunk);
    // after every hop's worth of audio, wait the matching simulated time
    if ((i + 1) % chunksPerHop === 0) await sleep((HOP_SEC * 1000) / SPEED);
  }
  await sleep(200);

  console.log(`Settings: window=${WINDOW_SEC}s hop=${HOP_SEC}s whisper=${WHISPER_MS}ms`);
  console.log("Audio received (s):", win.totalPushed() / SR);
  console.log("Samples stored    :", win.size(), "(max", win.capacity() + ")");
  console.log("WASM heap start/end:", heapStart, M.HEAP8.length);
  console.log("Whisper runs:", pipe.stats.runs, " skipped:", pipe.stats.skipped,
              " (ideal runs:", TOTAL_SEC / HOP_SEC + ")");
  console.log("Estimated subtitle delay (s):", HOP_SEC + WHISPER_MS / 1000);

  if (win.size() !== win.capacity()) throw new Error("window not full");
  if (M.HEAP8.length > heapStart * 2) throw new Error("memory grew!");
  console.log("PASS: bounded memory");
})();