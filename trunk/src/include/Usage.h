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
");
}
