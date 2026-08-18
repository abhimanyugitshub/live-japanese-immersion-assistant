// content-scripts/caption-reader.js
//
// This file gets injected directly into the video page (Netflix, Hotstar,
// YouTube — see the "matches" list in manifest.json) as soon as the page
// is idle. Anything you write here can read and touch that page's DOM,
// same as a browser extension's DevTools console could.
//
// This is currently a STUB. Module C (Caption-Track Reader) builds the
// real DOM-observation logic here: detecting the platform's existing
// subtitle element and reading its text live as it changes.
//
// What's proven working right now:
//   1. The script is being injected on the right sites at the right time.
//   2. It can talk back to background.js through chrome.runtime messages.

console.log("[LJIA] caption-reader.js injected on:", window.location.hostname);

// Quick round-trip test so Module C can confirm messaging works before
// writing any real caption-detection logic.
chrome.runtime.sendMessage({ type: "PING" }, (response) => {
  if (chrome.runtime.lastError) {
    console.warn("[LJIA] Could not reach background script:", chrome.runtime.lastError.message);
    return;
  }
  console.log("[LJIA] Background responded:", response);
});

// TODO (Module C): replace this with real caption detection, e.g.
//   const observer = new MutationObserver(() => { ...read subtitle text... });
//   observer.observe(document.body, { childList: true, subtree: true });