#include <Bridge.h>


Process p;

void setup() {
  Bridge.begin();
  p.runShellCommand("espeak \"Hello from an Arduino sketch. Lets count down from five.\"");
  
  for (int t=5; t>0; t--) {
    String num = String(t);
    p.runShellCommand("espeak " + num);
    delay(500);
  }
  
  p.runShellCommand("espeak \"Hooray, we did it.\"");
  p.close();  
}

void loop() {
}
