# Portable Portuguese accents

The rightmost BASE thumb key taps `'` and holds the `ACCENT` layer (hold-preferred).
firmware chooses the encoder from the persisted active Bluetooth profile:

| Profiles | Backend |
| --- | --- |
| 1, 2 | Linux `Ctrl+Shift+U` Unicode entry |
| 3 | Android best-effort `Ctrl+Shift+U` Unicode entry |
| 4, 5 | Windows `Alt+0nnn` keypad entry |

This also applies over USB: the selected Bluetooth profile remains the host
hint after reboot. No separate OS-mode state can drift out of sync.

Hold ACCENT and press: `Q W E R T` for `á é í ó ú`; `A S D F` for
`ã õ ç à`; and `Z X C` for `â ê ô`. Hold Shift as well for uppercase.

The main thumb row is `NUM thumb | Enter || Space | NAV thumb | SYM`; SYM is
a dedicated immediate momentary key, not a combo.

`~` and backtick remain on SYM (and the existing NUM/SYM locations) as normal
HID keys; their rendering still depends on the host keyboard layout.

Linux Unicode entry is broadly supported by GTK/Qt applications and many
Wayland/X11 desktops, but some terminals, Chromium/Electron fields, or IMEs
may not implement it. Android has no standard physical-keyboard Unicode entry,
so profile 3 deliberately uses the same sequence only as best effort. Windows
uses legacy Alt+numpad codes: Num Lock must be on and applications that capture
Alt sequences may prevent input.
