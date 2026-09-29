// Turns a nonstop audio stream into repeated "transcribe the last N seconds" calls.
class StreamingPipeline {
  constructor({ audioWindow, transcribe, onText,
                sampleRate = 16000, hopSec = 3, minSec = 2 }) {
    this.win = audioWindow;          // the C++ AudioWindow (WASM)
    this.transcribe = transcribe;    // async (Float32Array) => string (Whisper)
    this.onText = onText;            // called with each new piece of text
    this.hopSamples = hopSec * sampleRate;
    this.minSamples = minSec * sampleRate;
    this.sinceLast = 0;              // samples received since last Whisper run
    this.busy = false;               // is Whisper working right now?
    this.lastText = "";
    this.stats = { runs: 0, skipped: 0, lastMs: 0 };
  }

  // Module B calls this every time it has new audio (16 kHz mono float32)
  pushAudio(float32Chunk) {
    this.win.push(float32Chunk);
    this.sinceLast += float32Chunk.length;

    const enoughNew = this.sinceLast >= this.hopSamples;
    const enoughTotal = this.win.size() >= this.minSamples;
    if (!enoughNew || !enoughTotal) return;

    if (this.busy) {                 // Whisper still working: skip this round
      this.stats.skipped++;
      return;
    }
    this.sinceLast = 0;
    this._run();
  }

  async _run() {
    this.busy = true;
    const t0 = performance.now();
    try {
      const audio = this.win.snapshot();          // copy of the last N seconds
      const text = (await this.transcribe(audio)).trim();
      if (text && text !== this.lastText) {       // ignore repeated results
        this.lastText = text;
        this.onText(text);
      }
    } catch (e) {
      console.error("Transcription failed:", e);
    } finally {
      this.stats.runs++;
      this.stats.lastMs = performance.now() - t0;
      this.busy = false;
    }
  }
}

if (typeof module !== "undefined") module.exports = { StreamingPipeline };