/* 
   CHANGELOG 
   
   5-4-2002/Niels :

   Added Joyport to Usage() for selecting the Joystick port.
   Added Joystick option for selecting Joystick as movement device.



*/

void Usage()
{
printf("Usage ./cygne romfile options\n");
printf("Where options are:\n\n");
printf(" romfile           : Filename of the romfile. It can have several\n");
printf("                     extensions (.ws/.wsc/.rom/.bin). Gzipped and\n");
printf("                     zipped (.gz and .zip) are also supported.\n");
printf(" -double           : Scale the screen 2 times the normal size.\n");
printf(" -scanline         : Allows scanline output with scaling.\n");
printf(" -joystick         : Allow joystick/joypad as movement device.\n");
printf(" -joyport 0 - 3    : Select Joystick GamePort. 0 is default.\n");
}
