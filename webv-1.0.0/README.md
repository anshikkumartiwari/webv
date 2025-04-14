# webv — Minimal Terminal-to-GUI Web Launcher for any Debian based Operating System (Ubuntu, Kali Linux, Raspberry Pi OS, MX Linux, Parrot OS)


`webv` is a lightweight(8kb package size), terminal-based web launcher built using GTK3 + WebKitGTK 4.1 and C language. Run it from your terminal and get instant GUI browsing — no clutter, just pure speed and utility.

---

## Install via APT

You can install `webv` directly from the custom APT repo.

Run the following commands:

```bash
echo "deb [trusted=yes] https://anshikkumartiwari.github.io/webv stable main" | sudo tee /etc/apt/sources.list.d/webv.list
sudo apt update
sudo apt install webv
```

---

## Usage

```bash
webv https://wikipedia.org
```
```bash
webv what's the weather toady
```
```bash
webv              # opens https://www.google.com by default
```

---

## Features

- Launch full websites directly from terminal
- Enter search queries and it auto-searches Google
- Minimalist, elegant interface
- Back, forward, refresh, and info buttons
- Keyboard shortcuts:
  - Ctrl + L: Focus the URL bar
  - Ctrl + R: Reload page
  - Alt + ← / Alt + →: Back / Forward
- Auto-fallback to google.com if no URL/query is provided

---

## How it works

- `webv` checks if your input is a valid URL
  - If yes → opens it
  - If no → assumes it’s a Google search
- GUI opens in a clean GTK window using WebKitGTK 4.1
- Info button shows shortcut help

---

## Screenshot

![Screenshot](annotely_image%20(1).png)

---

## Dependencies

No additional dependencies needed when installing via above API.

But if you are building from source, make sure you have:

```bash
sudo apt install libgtk-3-dev libwebkit2gtk-4.1-dev
```

---

## Build From Source (Although we've not kept it public)

```bash
gcc browser.c -o webv `pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.1`
```

---

## Contributing

Pull requests, issues, stars — all welcome.  
If you have ideas to improve or extend, go for it.

---

## License

MIT License

---

Project Website: https://anshikkumartiwari.github.io/webv

```
webv-1.0.0
├─ 📁debian
│  ├─ 📄changelog
│  ├─ 📄changelogZone.Identifier
│  ├─ 📄compat
│  ├─ 📄control
│  ├─ 📄controlZone.Identifier
│  ├─ 📄copyright
│  ├─ 📄copyrightZone.Identifier
│  ├─ 📄install
│  ├─ 📄rules
│  ├─ 📄rulesZone.Identifier
│  ├─ 📄webv.desktop
│  ├─ 📄webv.desktopZone.Identifier
│  ├─ 📄webv.install
│  └─ 📄webv.installZone.Identifier
├─ 📄.gitignore
├─ 📄browser.c
├─ 📄index.html
├─ 📄Makefile
└─ 📄README.md
```