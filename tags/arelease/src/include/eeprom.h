/*
 ===================================================================================
		Cygne WIN  v 2.1a      (c) Dox 2002     dox@space.pl
 ===================================================================================

   NEC cpu core  by Bryan McPhail,Oliver Bergmann, Fabrice Frances and David Hedley
   Zlib  by  Jean-loup Gailly and Mark Adler

 ===================================================================================
*/

BYTE iEEPROM[]={
 255,255,255,255,255,255,192,255,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,127,0,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255, 
 0,252,255,1,255,253,255,253,255,253,255,253,
 255,253,255,253,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 0,0,3,3,0,0,0,64,128,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,
 135,5,140,9,5,12,139,12,144,0,0,2,
 0,76,165,0,128,0,0,0,255,127,255,127,
 255,127,255,127,255,127,255,127,255,127,255,127,
 255,127,255,127,255,127,255,127,255,127,255,127,
 255,127,255,127,255,127,255,127,255,127,255,127,
 255,127,255,127,255,127,255,127,255,127,255,127,
 255,127,255,127,255,127,255,127,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 0,0,6,6,6,6,6,0,0,0,0,0,
 1,128,15,0,1,1,1,15,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,
 'C'-54,'Y'-54,'G'-54,'N'-54,'E'-54,0,0,0,0,0,0,0,0,0,0,
 0,32,1,1,33,1,4,0,1,
 0,152,60,127,74,1,53,1,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255,255,255,255,255,255,255,255,255,
 255,255,255,255};