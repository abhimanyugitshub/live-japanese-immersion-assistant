// options.js

const enabledCheckbox = document.getElementById("extension-enabled");
const englishCheckbox = document.getElementById("show-english");
const saveStatus = document.getElementById("save-status");

// Load whatever is currently saved and reflect it in the checkboxes.
chrome.storage.local.get("settings", (result) => {
  const settings = result.settings || {
    extensionEnabled: true,
    showEnglishSubtitle: true
  };
  enabledCheckbox.checked = settings.extensionEnabled;
  englishCheckbox.checked = settings.showEnglishSubtitle;
});

// Save immediately whenever a checkbox changes — no separate "Save" button.
function saveSettings() {
  const settings = {
    extensionEnabled: enabledCheckbox.checked,
    showEnglishSubtitle: englishCheckbox.checked
  };
  chrome.storage.local.set({ settings }, () => {
    saveStatus.textContent = "Saved.";
    setTimeout(() => (saveStatus.textContent = ""), 1200);
  });
}

enabledCheckbox.addEventListener("change", saveSettings);
englishCheckbox.addEventListener("change", saveSettings);