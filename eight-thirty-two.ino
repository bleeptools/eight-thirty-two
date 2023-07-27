#include "core_cm7.h"

#if true
#include "arp8.h"
static Arp8<16, 24> arp;
#else
#include "arp32.h"
static Arp32<16, 24> arp;
#endif

void setup() {
  // setup measurement
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->LAR = 0xC5ACCE55;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void loop() {
  Serial.begin(9600);

  // measure - start
	DWT->CYCCNT = 0;

  arp.NoteOn(9, 127);
  arp.NoteOn(10, 127);
  arp.NoteOn(5, 127);
  arp.NoteOn(18, 127);
  arp.NoteOn(6, 127);
  arp.NoteOn(5, 127);
  arp.NoteOn(3, 127);
  arp.NoteOn(20, 127);
  arp.NoteOn(11, 127);
  arp.NoteOn(4, 127);

  // measure - stop
  Serial.println(DWT->CYCCNT);

  delay(5000);
}
