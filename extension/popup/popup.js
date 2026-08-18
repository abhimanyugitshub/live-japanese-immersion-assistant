// popup.js
// Runs when the popup window itself opens (clicking the toolbar icon).

const statusEl = document.getElementById("status");
const optionsBtn = document.getElementById("open-options");

// Confirm the popup can read saved settings from chrome.storage.
chrome.storage.local.get("settings", (result) => {
  if (result.settings && result.settings.extensionEnabled) {
    statusEl.textContent = "Extension is active.";
  } else {
    statusEl.textContent = "Extension is installed but disabled.";
  }
});

// Confirm the popup can talk to background.js.
chrome.runtime.sendMessage({ type: "PING" }, (response) => {
  console.log("[LJIA] popup got response from background:", response);
});

optionsBtn.addEventListener("click", () => {
  chrome.runtime.openOptionsPage();
});