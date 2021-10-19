
// These adjust how long the strip is in pixels, and the inter-frame delay
// Smaller number is a faster animation.  Here's some recommended values
// for a Digispark 16MHz:
// 60/m   2000
// 100/m
// 144/m  1000

#define NUMPIXELS 144  // Number of pixels in the string
#define FRAMEDELAY 1000  // How long to wait between draw updates

// These adjust what color to use.  0,0,0 is black/off.  The number is
// up to 255, but note that high brightness will overload your power
// supply and potentially crash the microprocessor. Keep this to 10 and
// below.
#define EVENPATTERN  0,0,0 // RGB value of even/zero/false/off
#define ODDPATTERN   0,0,1 // RGB value of odd/one/true/on

// This adjusts the probability of lighting an LED. 2 sides is a 50/50
// coinflip. 6 would be a die, and you'd only light an LED if you rolled
// a 1.  2 is normal, 4 is nice. Numbers at 20 look like traffic from the
// air.  200 is silly and often blank.
// By the way, if you want to invent this, just swap the PATTERNs above.
#define COINSIDES    2  // Number of sides of coin to flip. Eg, 2 is a
                               

// Globals
char bitmap[NUMPIXELS];
int pos = 0;


void setup() {
  ledsetup();
}

void loop() {
  cli();
  for (int i = 0; i < NUMPIXELS; i++) {
    if ( bitmap[((pos + i) % NUMPIXELS)])
    {
      sendPixel( EVENPATTERN );
    }
    else
    {
      sendPixel( ODDPATTERN );
    }
  }
  delay(FRAMEDELAY); // This is also our SHOW
  bitmap[pos] = (random(255) % COINSIDES);
  pos = ((pos + 1) % NUMPIXELS);
  sei();
}
