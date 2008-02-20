;LLegends.z80
;===========================================
#define saveSP appbackupscreen

;GrayLib2.inc
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
;gsputsprite
;gslargesprite
;gsenable
;gsdisable
gsmasknum	= $9d95
GSapppage	= gsmasknum + 1
skip83	= GSapppage + 1
stacksave	= skip83 + 1
gs_keymem	= stacksave + 2
frame_count	= gs_keymem + 1
gsTempSprite = frame_count + 1
gsTempSprite2 = gsTempSprite 
gsActivebuf1  = gsTempSprite + 16
gsActivebuf2  = gsActivebuf1 + 768
gsRamEnd	  = gsActivebuf2 + 768
GsRamSize	  = gsRamEnd - gsmasknum
AllocSize	= gsRamEnd - gsmasknum

;MAPPER.z80
;===========================================
;mapper
;DExA
#define mapRam appbackupscreen + 2
#define camY mapRam
#define camX camY + 1
#define mapBase camX + 1
#define mapWidth mapBase + 2
#define mapRamEnd mapWidth + 1
;tiles.z80
;===========================================
;mappertiles = start of tileset
#define numTile 73

;customfont.z80
;===========================================
;customPutC
;fontset.z80
;===========================================
;FontData = start of fontset
#define numChar 96

;keyboard.z80
;===========================================
;keyget
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
