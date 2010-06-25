;LLegends.inc
;============

; Include useful macros
#include "macros/var.inc"
#include "macros/relocate.inc"
#include "macros/debug.inc"
#include "macros/breakpoints.inc"
#include "macros/app.inc"
#include "macros/sizestatistics.inc"

#macro import_bmp_grayscale()
 .option bm_shd=4
#endmacro
#macro import_bmp_bw()
 .option bm_shd=2
#endmacro

;General Defines
#define TI83P
#define DOFRAMECOUNT		;activates a counter in case you want one, we need it for delay
#define DEBUG

; Variables
corevar = appbackupscreen
 varloc(appbackupscreen, 768)
; py and px are tile coordinates.
py = var(1)
px = var(1)
ctile = var(2)			;an pointer to current tile entry in map buffer
sanime1 = var(2)
sanime1Mask = var(2)
sanime2 = var(2)
sanime2Mask = var(2)
randData = var(2)
facingf = var(1)	;player Direction
randomseed = var(1)
stacksave = var(2)
stacksave2 = var(2)
#define corevarend stacksave2+2

; Flags
#define mygfxflags	asm_flag2
#define gs16maskedflag	1
#define spriteanimation	2

#define myFontFlags	asm_Flag2
#define LGFont	3
#define DGFont	4

#define myMiscFlags	asm_Flag2
#define temp	7

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
#define FULLSCREEN		;make use of other masks = less flicker (better?)

;use safe ram areas to alloc less ram?
gs_ram		  = $9d95			;userMem
gsActivebuf1  = gs_ram
gsActivebuf2  = gsActivebuf1 + 768
gsRamEnd	  = gsActivebuf2 + 768
gsRamSize	  = gsRamEnd - gs_ram
AllocSize	  = gsRamSize

gsmasknum	  = var(1)
GSapppage	  = var(1)
skip83		  = var(1)
gs_keymem	  = var(1)
frame_count   = var(1)
gsTempSprite  = var(16)
gsTempSprite2 = var(16)

;mapget.z80
;===========================================
;scrollmap
;mapDatatoRAM
;map node data for everything should be included into this file

;tiles.inc
;===========================================
;mappertiles = start of tileset
#define numTile	73
;===========================================
;map data in general
#define impa(value)	value | $80
#define imp		$80		;impassable tile
#define impabit	7
#define pasa(value)	value | $00
#define pas		$00		;walkable tile,just put elements in map at same column...

;mapper2.z80
;===========================================
;draw6x4map
;DExA
;Node related definitions
#define BiggestNode 28
#define mapW	6
#define mapH	4
#define mapS	mapW * mapH	;24
#define wordSize	2
#define tileW	2

;mapper and Node related definitions
NodeBuf = var(mapS)
NBmap = NodeBuf
NBup = var(wordSize)
NBdown = var(wordSize)
NBleft = var(wordSize)
NBright = var(wordSize)
NBobjnum = var(1)
NBobjStart = var(28)
NBobjEnd = var(0)
NConstSize = NBobjnum - NodeBuf + 1		;should be 33
#define OBJsize	4					;object size in bytes

;sprite.z80
;===========================================
;gs16masked
;other defines
#define TIMERCOUNT	60/10			;timer constant
#define gs16maskedwidth   gsTempSprite
#define gs16maskedsprite  gs16maskedwidth + 1
#define numAni	4
#define Anidelta	16 / numAni
#define numAniS	4
#define AniSdelta	12 / numAniS

;objdefine.inc
;===========================================
;defines codes for each object type
;FIRST BYTE WILL ALWAYS BE IDENTIFIER!

#define obj_reverseportal $01
;byte 2:  x 4bits : y 4 bits -> location on the new node to place character
;bytes 3-4: node pointer

#define obj_portal $02
;byte 2: x 4bits : y 4 bits -> location on the CURRENT NODE where the actual portal is
;byte 3: x 4bits : up/down 1 bit : y 3 bits -> Location on the DESTINATION node where portal leads
;byte 4: left/right 1 bit: 7 bits index for a table of Node Pointers
;NOTE: SET = up or left, RES = down or right, the flag in byte 4 simply says which axis: set = left/right, res = up/down

;customfont.z80
;===========================================
;customPutC
;fontset.inc
;===========================================
;FontData = start of fontset
#define	numChar 95
;constants for the custom font codes

#define [heart] 1
#define [sword] 2
#define [staff] 3
#define [bow] 4
#define [book] 5
#define [armor] 6
#define [gauntlet] 7
#define [ring] 8
#define [potion] 9
#define [key] 10
#define [poison] 11
#define [divider] 12

#define [!] 33
#define [?] 63
#define [elip] 39
#define [...] 46
#define [:] 58
#define ['] 41
#define [,] 45
#define [right_quotation] 34
#define [left_quotation] 36
#define [left_quotation_period] 64
#define [-] 45
#define [/] 47
#define [*] 42
#define [#] 35
#define [+] 43

#define [li] 21
#define [il] 22
#define [ll] 23
#define [right_arrow] 24
#define [left_arrow] 25
#define [up_arrow] 26
#define [up_double1] 27
#define [half_up_l] 27
#define [up_double2] 28
#define [half_up_r] 28

#define [bc_top_l] 13
#define [bs_top] 14
#define [bc_top_r] 15
#define [bs_left] 16
#define [bs_right] 17
#define [bc_down_l] 18
#define [bs_down] 19
#define [bc_down_r] 20
#define [status0] 59
#define [status1] 60
#define [status2] 61
#define [status3] 62
#define [equip0] 91
#define [equip1] 92
#define [equip2] 93
#define [item0] 29
#define [item1] 30
#define [item2] 31
#define [command0] 37
#define [command1] 38
#define [command2] 40
#define [command3] 41
#define [b_icon_l] 94
#define [b_icon_r] 95

;keyboard.z80
;===========================================
;keyget
;the following defines courtesy of Halifax
;Galandros: sorry Halifax, made my custom defines, I prefer them
; Direct Input Keys
keyport	=	1
diresetkey	=	$FF

;Group 1
diKeyRowPad	= $FE			;%11111110
diGroup1	= $FE
diDown	= %11111110
diLeft	= %11111101
diRight	= %11111011
diUp		= %11110111
bDown		= 0
bLeft		= 1
bRight	= 2
bUp		= 3
;Group 2
diKeyRow5	= $FD			;%11111101
diGroup2	= $FD
diEnter	= %11111110
diPlus	= %11111101
diMinus	= %11111011
diMul		= %11110111
diDiv		= %11101111
diPower	= %11011111
diClear	= %10111111
bEnter	= 0
bPlus		= 1
bMinus	= 2
bMul		= 3
bDiv		= 4
bPower	= 5
bClear	= 6
;Group 3
diKeyRow4 	= $FB			;%11111011
diGroup3	= $FB
diNegative	= %11111110
diThree	= %11111101
diSix		= %11111011
diNine	= %11110111
diRbracket	= %11101111
diTan		= %11011111
diVars	= %10111111
bNegative	= 0
bThree	= 1
bSix		= 2
bNine		= 3
bRbracket	= 4
bTan		= 5
bVars		= 6
;Group 4
diKeyRow3	= $F7			;%11110111
diGroup4	= $F7
diPoint	= %11111110
diTwo		= %11111101
diFive	= %11111011
diEight	= %11110111
diLbracket	= %11101111
diCos		= %11011111
diPrgm	= %10111111
diStat	= %01111111
bPoint	= 0
bTwo		= 1
bFive		= 2
bEight	= 3
bLbracket	= 4
bCos		= 5
bPrgm		= 6
bStat		= 7
;Group 5
diKeyRow2 	= $EF			;%11101111
diGroup5	= $EF
diZero	= %11111110
diOne		= %11111101
diFour	= %11111011
diSeven	= %11110111
diComma	= %11101111
diSin		= %11011111
diMatrx	= %10111111
diX		= %01111111
bZero		= 0
bOne		= 1
bFour		= 2
bSeven	= 3
bComma	= 4
bSin		= 5
bMatrx	= 6
bX		= 7
;Group 6
diKeyRow1 	= $DF			;%11011111
diGroup6	= $DF
diSto		= %11111101
diLn		= %11111011
diLog		= %11110111
diX2		= %11101111
diX_1		= %11011111
diMath	= %10111111
diAlpha	= %01111111
bSto		= 1
bLn		= 2
bLog		= 3
bX2		= 4
bX_1		= 5
bMath		= 6
bAlpha	= 7
;Group 7
diKeyRowTop	= $BF			;%10111111
diGroup7	= $BF
diGraph	= %11111110
diTrace	= %11111101
diZoom	= %11111011
diWindow	= %11110111
diYequal	= %11101111
di2nd		= %11011111
diMode	= %10111111
diDel		= %01111111
bGraph	= 0
bTrace	= 1
bZoom		= 2
bWindow	= 3
bYequal	= 4
b2nd		= 5
bMode		= 6
bDel		= 7

;%11111110	$FE	254
;%11111101	$FD	253
;%11111011	$FB	251
;%11110111	$F7	247
;%11101111	$EF	239
;%11011111	$DF	223
;%10111111	$BF	191
;%01111111	$7F	127

;-END