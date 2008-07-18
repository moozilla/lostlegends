;LLegends.z80
;===========================================
#define vblankc appbackupscreen
#define corevar vblankc +2 
#define py corevar
#define px py + 1
;NOTE: py and px are tile coordinates.
#define ctile px + 1
#define sanime1 ctile + 2
#define sanime1Mask sanime1 + 2
#define sanime2 sanime1Mask + 2
#define sanime2Mask sanime2 + 2
#define corevarend sanime2Mask + 2

;flags
;===========================================
#define mygfxflags asm_flag1
#define gs16maskedflag 1
#define spriteanimation 2

#define myFontFlags asm_Flag2
#define LGFont 1
#define DGFont 2

;sprite.z80
;===========================================
;gs16masked
;other defines
#define TIMERCOUNT 250			;timer constant
#define gs16maskedwidth gstempsprite
#define gs16maskedsprite gs16maskedwidth + 1
#define numAni 4
#define Anidelta 16 / numAni
#define numAniS 4
#define AniSdelta 12 / numAniS

;mapper2.z80
;===========================================
;draw6x4map
;DExA
;Node related definitions
#define BiggestNode 28
#define mapW 6
#define mapH 4
#define mapS mapW * mapH
#define wordSize 2
#define tileW 2

;mapper and Node related definitions
#define NodeBuf corevarend
#define NBmap NodeBuf
#define NBup NodeBuf + mapS
#define NBdown NBup + wordSize
#define NBleft NBdown +  wordSize
#define NBright NBleft + wordSize
#define NBobjnum NBright + wordSize
#define NBobjStart NBobjnum + 1

#define NConstSize NBobjnum - NodeBuf + 1			;should be 33
#define OBJsize 4			;object size in bytes

;mapget.z80
;===========================================
;scrollmap
;mapDatatoRAM
;map node data for everything should be included into this file

;tiles.inc
;===========================================
;mappertiles = start of tileset
#define numTile 73
;===========================================
;map data in general
#define impa(value) value | $80
#define imp $80
#define impabit 7

;GrayLibx.inc
;===========================================
;gsAlignedSprite
;gsAlignedMaskedSprite
;gshorizontalflip
;gsverticalflip
;gsclearbuffer
;gscopybuffer
;gsbuf1tobuf2
;gsgetk
;gsgetcsc
;gsputsprite		USES SMC!
;gslargesprite		USES SMC!
;gs16masked
;gsenable
;gsdisable
gsmasknum	= $9d95
GSapppage	= gsmasknum + 1
skip83	= GSapppage + 1
stacksave	= skip83 + 1
gs_keymem	= stacksave + 2
frame_count	= gs_keymem + 1
gsTempSprite = frame_count + 1
gsTempSprite2 = gsTempSprite + 16
;I added this
stacksave2 = gsTempSprite2 + 16
;end I added this
gsActivebuf1  = stacksave2 + 2
gsActivebuf2  = gsActivebuf1 + 768
gsRamEnd	  = gsActivebuf2 + 768
GsRamSize	  = gsRamEnd - gsmasknum
AllocSize	= gsRamEnd - gsmasknum

;customfont.z80
;===========================================
;customPutC
;fontset.inc
;===========================================
;FontData = start of fontset
#define numChar 96
#include "fontconstants.inc"

;keyboard.z80
;===========================================
;keyget
;the following defines courtesy of Halifax
;;;;;KeyGroup1() keys
#define K_GRAPH $FE
#define K_TRACE $FD
#define K_ZOOM $FB
#define K_WINDOW $F7
#define K_YEQUAL $EF
#define K_2ND $DF
#define K_MODE $BF
#define K_DEL $7F

#define B_GRAPH  0
#define B_TRACE  1
#define B_ZOOM 	2
#define B_WINDOW  3
#define B_YEQUAL  4
#define B_2ND  5
#define B_MODE  6
#define B_DEL  7

;;;;;KeyGroup2() keys
#define K_STORE $FD
#define K_LN $FB
#define K_LOG $F7
#define K_XSQUARED $EF
#define K_XINVERSE $DF
#define K_MATH $BF
#define K_ALPHA $7F

#define B_STORE  1
#define B_LN  2
#define B_LOG  3
#define B_XSQUARED  4
#define B_XINVERSE  5
#define B_MATH  6
#define B_ALPHA  7

;;;;;KeyGroup3() keys
#define K_0 $FE
#define K_1 $FD
#define K_4 $FB
#define K_7 $F7
#define K_COMMA $EF
#define K_SIN $DF
#define K_APPS $BF
#define K_X $7F

#define B_0  0
#define B_1  1
#define B_4  2
#define B_7  3
#define B_COMMA  4
#define B_SIN  5
#define B_APPS  6
#define B_X  7

;;;;;KeyGroup4() keys
#define K_PERIOD $FE
#define K_2 $FD
#define K_5 $FB
#define K_8 $F7
#define K_LEFTPAREN $EF
#define K_COS $DF
#define K_PRGM $BF
#define K_STAT $7F

#define B_PERIOD  0
#define B_2  1
#define B_5  2
#define B_8  3
#define B_LEFTPAREN  4
#define B_COS  5
#define B_PRGM  6
#define B_STAT  7

;;;;;KeyGroup5() keys
#define K_NEGATIVE $FE
#define K_3 $FD
#define K_6 $FB
#define K_9 $F7
#define K_RIGHTPAREN $EF
#define K_TAN $DF
#define K_VARS $BF

#define B_NEGATIVE  0
#define B_3  1
#define B_6  2
#define B_9  3
#define B_RIGHTPAREN  4
#define B_TAN  5
#define B_VARS  6

;;;;;KeyGroup6() keys
#define K_ENTER $FE
#define K_PLUS $FD
#define K_MINUS $FB
#define K_MULTIPLY $F7
#define K_DIVIDE $EF
#define K_EXPONENT $DF
#define K_CLEAR $BF

#define B_ENTER  0
#define B_PLUS  1
#define B_MINUS  2
#define B_MULTIPLY  3
#define B_DIVIDE  4
#define B_EXPONENT  5
#define B_CLEAR  6

;;;;;KeyGroup7() keys
#define K_DOWN $FE
#define K_LEFT $FD
#define K_RIGHT $FB
#define K_UP $F7

#define B_DOWN  0
#define B_LEFT  1
#define B_RIGHT  2
#define B_UP  3

;######################################
;# KG_[1...7]()
;#
;#   Specify the key group that you would
;#   like to scan for keys
;######################################

#define KG_1 $BF
#define KG_2 $DF
#define KG_3 $EF
#define KG_4 $F7
#define KG_5 $FB
#define KG_6 $FD
#define KG_7 $FE
