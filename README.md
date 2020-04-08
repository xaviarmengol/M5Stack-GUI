# M5Stack-GUI
A modular GUI library for M5Stack (ESP32).

This library has evolved from cfGUI library. The intention is to make it simpler, richer and more modular.

It's composed of a simple hierarchy of graphical widget that can be drawn on screen. Each widget has a standard view, and a zoomed view.

The following widgets have been created:

  - Screen and AppScreen (screen with a top bar and a bottom bar)
  - Bar
  - StatusBar: Bar with uptime, clock and Wifi signal
  - ButtonInfoBar: display the function of the physical button
  - Button: Shows a value. In zoomed mode can also show a graph of the last samples
  - UpDownButton: Used to modify a value.
  - Log Screen: display logging text when in zoom mode
  - Mosaic of widgets: In its zoomed version contains another layer of widgets, including other mosaics.
  
In any widget, a variable (integer) can be provided at widget definition and it is updated automatically. If no variable is provided, the widget text should be updated manually from the application.

# How To (Platform.io)
To use this library with your platform.io project, simply clone or download the library into the directory 'lib' of your project.
Then, you just need to include the headers (e.g. #include <Screen.h>) and write some code.


Here is an example of `platformio.ini` file:

```
[env:m5stack-core-esp32]
platform = espressif32
board = m5stack-core-esp32
framework = arduino
build_flags=-w
```
  
# Todo
  - Remove -w flag requeriment
  - Solve graphical details
  - Create more widgets
  
# Changelog

## 0.1.0
  - First version of the library, cloned from cfGUI
  - Added some additional Widgets (Graphical Button, Logging System)
  - Extended functionalities with the composable Mosaic
  
