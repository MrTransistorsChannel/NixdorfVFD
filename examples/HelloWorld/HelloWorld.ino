#include <NixdorfVFD.h>
#include <SoftwareSerial.h>

NixdorfVFD VFD;
SoftwareSerial vfd(2, 3, true); // SoftwareSerial port on pins #2 and #3, inverted logic

void setup() {
  vfd.begin(9600);              // Starting serial communication at 9600bod
  VFD.begin(vfd);               // Initialising the display
  VFD.clear();                  // Clearing screen
  VFD.home();                   // Returning cursor to the start position
  VFD.setCodePage(CP_866);      // Enabling cyrillic code page

  VFD.setCursor(4, 0);          // Moving cursor to 5 column in the first row
  VFD.print("Hello world!");    // Printing text
  VFD.setCursor(4, 1);
  VFD.print("Привет, мир!");

}

void loop() {
  // Nothing here
}
