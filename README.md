# wokebar

Portable and simple X11 status bar written in C99.

## Building

You will need the XCB, Fontconfig and FreeType 2 development libraries installed on your system.

Then with meson:
```
meson setup build --buildtype=release
meson compile -C build
```
