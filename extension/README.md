# Chrome Extension Shell — Week 3, Module D

This is an empty but fully working Manifest V3 Chrome extension. Nothing
"smart" happens yet — this module's job was just to build the skeleton so
every other module has a place to plug into:

- Module C (Caption-Track Reader) → fills in `content-scripts/caption-reader.js`
- Week 4 Module A (Subtitle Overlay) → adds to the content script / injects UI
- Week 4 Module B (Click-to-Lookup) → builds inside `popup/`
- Week 2 Module D's WASM build → will be loaded from `background.js` or the
  content script once it's wired in

## Folder structure

```
extension/
├── manifest.json              # the extension's "config file" — Chrome reads this first
├── background.js              # service worker (background logic)
├── content-scripts/
│   └── caption-reader.js      # injected into Netflix/Hotstar/YouTube pages
├── popup/
│   ├── popup.html
│   ├── popup.css
│   └── popup.js                # shown when you click the toolbar icon
├── options/
│   ├── options.html
│   ├── options.css
│   └── options.js              # the extension's settings page
└── icons/
    ├── icon16.png
    ├── icon48.png
    └── icon128.png
```

## How to load it in Chrome (unpacked)

1. Open `chrome://extensions` in Chrome.
2. Turn on **Developer mode** (top-right toggle).
3. Click **Load unpacked**.
4. Select the `extension/` folder.
5. You should see "Live Japanese Immersion Assistant" appear as a card,
   with no errors. Click the puzzle-piece icon in the toolbar, pin it, then
   click it — the popup should open and say "Extension is active."

Any time you change a file, go back to `chrome://extensions` and hit the
refresh icon on the extension's card to reload it.