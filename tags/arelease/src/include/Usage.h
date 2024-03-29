/* 
   CHANGELOG 
   
   5-4-2002/Niels :

   Added Joyport to Usage() for selecting the Joystick port.
   Added Joystick option for selecting Joystick as movement device.



*/

void Usage()
{
printf(
"Usage: ./cygne romfile options
 Where options are:

 romfile           : Filename of the romfile. It can have several
                     extensions (.ws/.wsc/.rom/.bin). Gzipped and
                     zipped (.gz and .zip) are also supported.
 -double           : Scale the screen 2 times the normal size.
 -scanline         : Allows scanline output with scaling.
 -joystick         : Allow joystick/joypad as movement device.
 -joyport 0 - 3    : Select Joystick GamePort. 0 is default.
");
}
