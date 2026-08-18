// background.js
// This is the "service worker" — a script that runs in the background,
// separate from any web page, for as long as Chrome needs it (it can be
// stopped and restarted automatically, so never assume it's "always on"
// or store important state only in variables here — use chrome.storage).

// Runs once, the moment the extension is installed or updated.
chrome.runtime.onInstalled.addListener((details) => {
  console.log("[LJIA] Extension installed/updated:", details.reason);

  // Set sensible defaults the first time the extension is installed.
  if (details.reason === "install") {
    chrome.storage.local.set({
      settings: {
        extensionEnabled: true,
        showEnglishSubtitle: true
      }
    });
  }
});

// Central message hub. Content scripts (caption reader, subtitle overlay,
// etc.) and the popup/options pages will all send messages here when they
// need something — e.g. a dictionary lookup that has to go through the
// WASM-backed hash table. For now this just proves the wiring works.
chrome.runtime.onMessage.addListener((message, sender, sendResponse) => {
  console.log("[LJIA] Message received in background:", message);

  if (message.type === "PING") {
    sendResponse({ type: "PONG", receivedAt: Date.now() });
    return true; // keep the message channel open for the async response
  }

  // TODO (later weeks): route lookup requests to the WASM dictionary,
  // route "save word" actions to the storage layer, etc.
});