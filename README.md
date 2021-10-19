# Bitfall/Randchase

## Hardware
The core hardware is a Digispark Mini clone -- the typically-blue, six pin ATTINY85-based Arduino-like development platform with a microusb socket. You could probably run this on almost any AVR Arduino-like platform, but it uses a library to directly drive the NeoPixel/WS2812 strips that is timing critical, and only this 16MHz board has been tested.

A NeoPixel strip is connected to the board at VIN, P0, and GND. Note that this is a technically-illegal configuration, where the LED strip is free to pull as much power as it wants from the USB port, without checking in with the power delivery system first. I recommend not leaving the LEDs plugged in while the board is plugged into a computer for programming, and should only unplug or plug in LEDs while the power is off.

That said, if you keep the brightness in the recommended range, the power usage isn't terrible, even with super-dense 144 LED strips, you can keep the budget under the 150mA. With a single-color set to '1', these are the total costs that I saw:

 * 60 leds - 60mA
 * 100 leds - 70mA
 * 144 leds - 130mA

If you use the 5V pin, instead, you might brown out the little voltage regulator onboard. If you try to use a different board, you may need some technical documents to find the actual, internal name of the port -- not the Arduino convenience names -- to configure in rawpixel.ino.

Similarly, this all assumes that you're going to get power through the microusb port and NOT add additional power to the strips.

 * Power to the USB port is assumed filtered and goes straight to the ATTINY, out the +5V pin, and somehow through the regulator to the VIN pin?
 * Power to the VIN pin must be above 7v, and goes through the onboard regulator to bring it down to 5v, after which it goes to the ATTINY and the +5v pin. Above 7v would burn your WS2812 chips, and the regulator might not be strong enough to run the strip. I would avoid this configuration.
 * Power to the 5v pin is the same as power to the USB port. It should be externally regulated.

## Software
Most of the user-serviceable parts are in the top of randchase-rawpixel.ino, where I hope the comments are sufficient. But here's way too much info about them:

### NUMPIXELS
Normally, you can't run more than about 83 pixels from an ATTINY85, but thanks to the rawpixel library, you can run at least 144 and maybe up to 250. The refresh will get slower with more pixels, of course, but otherwise there's not a lot of risk if this number is larger than your number of pixels.  I've tested 60, 100, and 144.

### FRAMEDELAY
This is how long it stops after doing a refresh before advancing a frame and doing a refresh.  There's a bare minimum required here by the NeoPixel protocol, but a minimum of 10 seems like enough. Still, I like the effects in the 1000-2000 range.  You'll probably want this smaller for denser LED strips, and reasonably large for coarse, 60LED/m strips. Note that NUMPIXELS introduces it own per-pixel delay, so this isn't a firm duration.

### EVENPATTERN
and ODDPATTERN are three numbers, 0-255, that define how bright to set the RED, GREEN, and BLUE leds.  You might be tempted to stick large numbers in here, but you should fight that urge. With high brightness comes power problems, and I think you'll find that a setting from 1-10 is more than sufficient.

   * Red 1,0,0
   * Yellow 1,1,0
   * Green 0,1,0
   * Cyan 0,1,1
   * Blue 0,0,1
   * Magenta 1,0,1
   * White 1,1,1
   * Black 0,0,0

### COINSIDES
The random stream comes from the built-in pseudorandom number generator, with all the caveats that that brings. It does a modulo operation on the random number, and if the result is a 0, it lights a LED.  If the result is anything else, it darkens a LED. When set to 2, it's a 50/50 coinflip, and gives a nice, dense, random display. When set to higher and higher values, the display gets less and less dense, until it changes from a stream to just a bit going by every few seconds.

Here's examples of what that density looks like in practice. Note that the whole example 128-line *is* blank, there's more lit on the 64-line than the 32-line, and that's how random works. If you don't believe me, then there's a high probability that roulette is not your game.
```
2:   * ****  ** * ** **** ** *   *  * ** *     **** ***  **  **  * ****   *
4:     *    ***  * **  * *            *   **     *             **  *       *
8:             *  *           *  **     *              *           * *
16: *  *                                            *        **      *     *
32:                  *                     *                   *
64:                        *          *                                 **
128:
```

Note that if you want to invert this display -- that is, a sea of ON leds with OFF leds streaming through it -- you can just swap the values in the EVENPATTERN and ODDPATTERN settings.

## Strip config
At the top of rawpixel.ino are a small number of settings that I sincerely hope you do not need to adjust.

The PIXEL_PORT, PIXEL_DDR, and PIXEL_BIT defines say which pin the pixels are connected to.  These are defined using internal AVR names, and not the Arduino convenience names. The default of PORTB, DDRB, 0 works for pin 0 of the Digispark clone.

The T1H, T1L, T0H, and T0L are the raw timing for the WS2812 signals. You should not need to touch these unless your LED strip is incredibly strange.

Lastly, you signal a strip to display a frame by a RESET sequence that is just holding the line low for a sufficiently long time.  That time is MORE than taken care of in the FRAMEDELAY, but if you start trying to push the speed faster and things start freaking out, you might be running into this.


## Compiling
These are assuming you are using the Digispark or its clone. Full details can be found at the [Digistump Github](https://github.com/digistump/DigistumpArduino) or at the [Digispark Wiki](http://digistump.com/wiki/digispark).

 * Board Package: [http://digistump.com/package_digistump_index.json](http://digistump.com/package_digistump_index.json)
 * Board: Digispark (16mhz - No USB)
 * Programmer: Micronucleus

You might also use other ATTINY85 boards, like the Adafruit Gemma or Trinket, which use their own board package.

Note that all of these devices generally use a bitbang USB method that does not play well with more modern USB3 controllers. If you have problems with programming your board, try using a USB2 port (usually black instead of blue) or get a cheap USB2 hub and plug into that.  Even then, programming these chips very often just randomly fails. Keep trying, and it should work within two or three tries.



## The Future
- Add a heartbeat to the onboard Digispark LED (p1)
- Add a button to change the color
- Add a button to change the coinsides
- Add a button to change the frame delay
- Make custom hardware with all these buttons on them.

## Thank you
### Josh Levine
[SimpleNeoPixelDemo](https://github.com/bigjosh/SimpleNeoPixelDemo)
'bigjosh' wrote the SimpleNeoPixelDemo that is the root of the rawpixel.ino file included here.  This project would not exist if not for his work. The competing NeoPixel implementations have memory requirements that would mean switching to larger and more expensive ATMEGA chip to run more than 83 pixels. Since the library allows you to copy an array directly to the LEDs you also avoid the intensive and wasteful copying to get the streaming motion.

### Limor "Ladyada" Fried - [Adafruit](https://adafruit.com)
It is undeniable that the WS2812 that makes this project so incredibly simple would not have been nearly as easy and accessible if not for the work, both technical and commercial, that Adafruit has done for the community. Although this project uses no Adafruit products and no Adafruit code, I greatly appreciate their contributions. Also, consider running this on their 5V Trinket or make something wearable with a Gemma!

### Erik Kettenburg - [DigiStump](http://digistump.com)
Although Digistump seems to have abandoned the original Digispark design for their newer ATTINY87-based PRO model, they released the original design that would plug directly into a USB port. Indeed, it doesn't appear they ever released a MicroUSB+TINY85 model. It seems that's strictly from the realm of clone importers. Regardless, I am grateful for Digistump's work at making the ATTINY85 so approachable for silly throw-away blinky projects.

### [WorldSemi](http://www.world-semi.com/)
Designed and marketed the WS2812 and other RGB LED controller chips that straight-up revolutionized RGB LEDs. I tried to do this project with a pile of shift registed and a single-color LED, and the price and easy-of-use made that whole experiment silly. Yes, you could do this effect with nothing more than 7400 logic... but your time and money isn't worth it.

### Atmel, now [Microchip](http://www.microchip.com/)
I don't know what's special about the AVR chips, but they're unavoidable in the hobbyist space, and drew me into their ecosystem.

### [Arduino](https://www.arduino.cc/)
I love/hate this environment, but it has undeniably unlocked the great power of these little chips, and for that I'm grateful.




