# Simulasi SmartSafe 🔐

An Arduino Uno–based **smart safe** prototype. The user enters a 4-digit PIN on a keypad, an LCD shows the status, and a servo acts as the lock that opens when the PIN is correct.

The project was designed and simulated in **Tinkercad Circuits**.

![SmartSafe simulation circuit](Simulasi%20arduino%20smartsafe.png)

---

## How It Works

The system uses **two Arduino Unos** that talk to each other over Serial (UART), because a single Arduino doesn't have enough pins to handle the keypad, LCD and servo at the same time.

```
┌──────────────┐   Serial (TX → RX)   ┌──────────────────┐
│  Arduino #1  │ ───────────────────▶ │    Arduino #2    │
│   (Keypad)   │                      │ (Main Controller)│
└──────▲───────┘                      └───┬──────────┬───┘
       │                                  │          │
   Keypad 4x3                         LCD 16x2     Servo
```

1. **Arduino #1 – Keypad** reads the pressed key and sends the character to Arduino #2 over Serial.
2. **Arduino #2 – Main Controller** receives the character and runs the safe logic:
   - The LCD shows `Welcome :)` and `Press Any Key`.
   - Press any key to start → the LCD shows `Enter Your PIN`.
   - Enter the 4-digit PIN (shown as `****`). Made a typo? Press `*` to clear and start over.
   - **Correct PIN** → the LCD shows `Unlocked` and the servo rotates 0° → 90° (lock opens).
   - Press `*` or `#` to lock it again (servo returns to 0°).
   - **Wrong PIN** → the LCD shows `Wrong Password`, then returns to the welcome screen.
   - **3 wrong attempts in a row** → the system locks out for 10 seconds (`Too many tries`).

| Key | Action |
|---|---|
| Any key (welcome screen) | Start entering the PIN |
| `0`–`9` | PIN digit |
| `*` (while entering PIN) | Clear input |
| `*` / `#` (while unlocked) | Lock again |

Settings can be changed through the constants at the top of `Arduino Uno LCD.ino`:

| Constant | Default | Purpose |
|---|---|---|
| `PASS` | `"2468"` | Safe PIN |
| `MAX_ATTEMPTS` | `3` | Wrong attempts allowed before lockout |
| `LOCKOUT_MS` | `10000` | Lockout duration (ms) |
| `UNLOCKED_ANGLE` | `90` | Servo angle when unlocked |

---

## Components

| Component | Qty | Notes |
|---|---|---|
| Arduino Uno | 2 | One for the keypad, one as the main controller |
| Matrix keypad | 1 | Code uses a 4x3 layout (`1-9`, `*`, `0`, `#`) |
| 16x2 LCD (HD44780) | 1 | 4-bit mode |
| Potentiometer | 1 | LCD contrast |
| Resistor | 1 | LCD backlight |
| Servo motor (SG90 / micro servo) | 1 | Lock mechanism |
| Breadboard & jumper wires | — | |

---

## Wiring

### Arduino #1 – Keypad

| Keypad | Arduino Pin |
|---|---|
| Row 1 – Row 4 | D8, D7, D6, D5 |
| Col 1 – Col 3 | D4, D3, D2 |
| **TX (D1)** | → RX (D0) on Arduino #2 |
| **GND** | → GND on Arduino #2 (common ground) |

### Arduino #2 – Main Controller

| Device | LCD / Servo Pin | Arduino Pin |
|---|---|---|
| LCD | RS | D12 |
| LCD | E | D11 |
| LCD | D4, D5, D6, D7 | D5, D4, D3, D2 |
| LCD | RW | GND |
| LCD | V0 (contrast) | Potentiometer wiper |
| Servo | Signal | D6 |
| Servo | VCC / GND | 5V / GND |
| **RX (D0)** | | ← TX (D1) on Arduino #1 |

---

## File Structure

```
Simulasi-SmartSafe/
├── Arduino Uno KeyPad.ino         # Arduino #1: reads the keypad, sends keys over Serial
├── Arduino Uno LCD.ino            # Arduino #2: PIN logic, LCD and servo
└── Simulasi arduino smartsafe.png # Tinkercad circuit screenshot
```

---

## Getting Started

### In Tinkercad
1. Create a new circuit in [Tinkercad Circuits](https://www.tinkercad.com/circuits) following the picture and wiring tables above.
2. Paste `Arduino Uno KeyPad.ino` into Arduino #1 and `Arduino Uno LCD.ino` into Arduino #2 (use **Text** mode in the Code editor).
3. Click **Start Simulation**, press any key, then enter the PIN `2468`.

### On real hardware
1. Install the libraries from the Arduino IDE **Library Manager**:
   - `Keypad` (by Mark Stanley & Alexander Brevig)
   - `LiquidCrystal` and `Servo` come built in with the Arduino IDE.
2. Upload each sketch to its board. **Disconnect the TX/RX wire while uploading**, since pins D0/D1 are shared with USB.
3. Reconnect TX → RX and make sure both boards share GND.

---

## Development Notes

- **Keypad layout**: the picture uses a 4x4 keypad (with `A`–`D` keys), while the code maps a 4x3 layout, so the `A`–`D` column is unused.
- Ideas for next features: change the PIN from the keypad, add a buzzer/LED indicator, and store the PIN in EEPROM so it survives power loss.

---

## License

This project was made for learning purposes. Feel free to use and modify it.
