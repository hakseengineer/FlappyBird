# Echo Flap — Ultrasonic Flappy Bird

Play Flappy Bird with your **hand**. An ultrasonic distance sensor (HC-SR04) connected to an Arduino streams your hand distance to the browser over USB, and the bird follows your hand height in real time. No install, no libraries: it is a single HTML file.

## Demo video

[![Echo Flap demo](https://img.youtube.com/vi/5DXqrWZ9imw/maxresdefault.jpg)](https://www.youtube.com/watch?v=5DXqrWZ9imw)

> Click the thumbnail to watch the demo on YouTube.

**Live demo:** https://hakseengineer.github.io/FlappyBird/ *(optional, see GitHub Pages below)*

## Features

- **Two control modes**
  - *Position:* hand height equals bird height.
  - *Flap:* dip your hand toward the sensor to flap.
- **Easy calibration:** set the Near and Far distances with sliders, or put your hand where you want and press *Set near here* / *Set far here*. An Invert option flips the direction.
- **Out-of-range filter:** readings beyond *Far + custom range* are ignored, so the bird holds its last height instead of jumping when your hand leaves the sensor.
- **Game speed:** 0.5x to 3x slider, and the speed also ramps up the longer you survive.
- **Adaptive smoothing** to remove sensor jitter while staying responsive.
- **Difficulty levels:** Easy, Normal, Hard.
- **Themes:** Day, Sunset, Night.
- **Landscape and portrait layouts** (press `P` to toggle).
- **Synthesized sound effects** with volume control and mute.
- **Mouse / touch test mode** so you can try the game without hardware.
- **Best score** is saved in your browser.

## What you need

| Item | Notes |
|---|---|
| Arduino (Uno / Nano / similar) | Any board with a USB serial connection |
| HC-SR04 ultrasonic sensor | 5 V version |
| Jumper wires + USB cable | |
| Chrome or Edge on desktop | Web Serial is not supported in Firefox or Safari |

## Wiring (example)

| HC-SR04 | Arduino |
|---|---|
| VCC | 5V |
| GND | GND |
| TRIG | D9 |
| ECHO | D10 |

> Using a 3.3 V board? The HC-SR04 ECHO pin outputs 5 V, so use a voltage divider or a 3.3 V-compatible sensor.

## Arduino sketch

The game reads one line per measurement in the format `D:<distance in cm>` at **115200 baud**.

```cpp
const int TRIG = 9;
const int ECHO = 10;

void setup() {
  Serial.begin(115200);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}

void loop() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long us = pulseIn(ECHO, HIGH, 30000);   // 30 ms timeout
  if (us > 0) {
    float cm = us * 0.0343 / 2.0;
    Serial.print("D:");
    Serial.println(cm, 1);
  }
  delay(30);
}
```

## How to run

1. Upload the sketch above to your Arduino.
2. Open `ultrasonic_flappy.html` in **Chrome or Edge** (double-click it, or use the live demo link).
3. Close the Arduino Serial Monitor if it is open, since only one program can use the port.
4. Click **Connect sensor** and pick your Arduino's port.
5. Open **Settings** and set Near and Far for your hand range.
6. Click the game or press **Space** to start.

No sensor? Open Settings and change **Input** to *Mouse / touch (test)*.

## Controls

| Action | Input |
|---|---|
| Move the bird (Position mode) | Hand distance from the sensor |
| Flap (Flap mode) | Quick dip of your hand toward the sensor |
| Start / retry | Click, tap, or `Space` |
| Toggle portrait / landscape | `P` or the orientation button |

## Settings reference

| Setting | What it does |
|---|---|
| Control mode | Position or Flap |
| Input | Ultrasonic sensor or Mouse / touch |
| Difficulty | Pipe gap, base speed and spacing |
| Theme | Day, Sunset, Night |
| Near / Far (cm) | Distance range mapped to the bottom and top of the screen |
| Ignore beyond Far + (cm) | Readings past this limit are discarded and the bird holds its height |
| Game speed | 0.5x to 3x base multiplier (also ramps up during play) |
| Smoothing | Higher is more responsive, lower is smoother |
| Volume | Sound effect volume |
| Invert | Closer hand means higher bird |

## Troubleshooting

- **"Web Serial needs Chrome or Edge":** use a desktop Chromium browser.
- **Port not listed or fails to open:** close the Arduino IDE Serial Monitor and any other program using the port, then replug the board.
- **Bird jitters:** lower the Smoothing value, or aim the sensor at a flat surface (your palm).
- **Bird jumps when your hand leaves the sensor:** reduce *Ignore beyond Far +* so stray far readings are discarded.
- **Game too hard:** lower Game speed or choose Easy.

## Host it online (GitHub Pages)

Web Serial works on `https://` pages, so GitHub Pages is a good way to share the game.

1. Rename `ultrasonic_flappy.html` to `index.html`.
2. In the repo go to **Settings > Pages**.
3. Under **Build and deployment**, choose *Deploy from a branch*, select `main` and `/ (root)`, then save.
4. After a minute your game is live at `https://YOUR_USERNAME.github.io/YOUR_REPO/`.

## Credits

Created by **Hak Se Engineer**.

## License

Add a license of your choice (for example MIT) in a `LICENSE` file.
