# metronome

metronome uses the terminal beep ('\a') to produce sound.

**Demo:**

![demo](https://raw.githubusercontent.com/greysome/metronome/master/demo.gif)

**Installation (Linux only):**

Navigate to project directory and run the following commands:

```
make
sudo make install
```

To uninstall:

```
sudo make uninstall
```



**Arguments:**

```-t <tempo>```

Tempo in bpm (default 120). If the tempo is too fast and the beeps are no longer distinct, try setting the beep length as follows: 
```xset b 100 500 10```

```-d <divisions>```

List of divisions (separated by commas). The tempo will be based on the largest division.   