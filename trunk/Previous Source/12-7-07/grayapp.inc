;=======================================================================================================
;Revolution Grayscale Package & (Gray)Mapper
;
;By James Montelengo
;Modified by Jason Lee
;
;To use this, you must have
;#include "grayapp.inc" at the top of your code near the header
;and in at least one other place in your code
;
;To use these defines, put them ABOVE the 1ST inclusion of grayapp.inc
;#define INCLUDEMAPPER  		<- includes GrayMapper
;#define INCLUDERGP				<- includes Revolution Grayscale Package
;#define APP					<- modifies the inclusions so that it is APPsafe
;#define AllocStart				<- if you use #define APP, you MUST define this also
;							   	   this is the address where you will allocate RAM for 
;							       the parts of RGP and GrayMapper that use Self Modifying Code
;FOR RGP
;#define TI83					<- Use if coding for Ti-83
;#define VTI					<- Use if testing code in VTI
;#define TILEM					<- Use if testing code in TILEM
;#define EMU 					<- Use if testing code in an emulator
;#define NODOUBLEBUFFER			<- Use if you want to directly manipulate active buffers
;							   	   Faster, but will look worse in most cases.
;#define ALIGNEDSPRITE			<- include gsAlignedSprite routine
;#define ALIGNEDMASKEDSPRITE	<- include gsAlignedMaskedSprite routine
;#define REVERSEMASK			<- gsAlignedMaskedSprite uses light layer as mask instead of dark


;========================================================================================================
#ifndef HEADER

#ifdef INCLUDEMAPPER
.echo "Defines!!"
#DEFINE Num_Layers	MapperRam
#DEFINE MapWidth		Num_Layers+1
#DEFINE MapY		MapWidth+2
#DEFINE MapX 		MapY+2
#DEFINE TilePtr		MapX+2
#DEFINE MapBufPtr		TilePtr+2
#DEFINE MapPtr		MapBufPtr+2
#DEFINE AniTab		MapPtr+2
#DEFINE OldMapY		AniTab+2
#DEFINE OldMapX		OldMapY+2
#DEFINE MapStructEnd	OldMapX+2
#DEFINE PreMapPTR		OldMapX+2
#DEFINE Tilemasks1	PreMapPTR+2
#DEFINE Tilemasks		Tilemasks1
#DEFINE Tilemask2		Tilemasks1
#DEFINE Tilemask1		Tilemasks1+1
#DEFINE Tilemasks2	Tilemask1+1
#DEFINE Tilemask4		Tilemasks2
#DEFINE Tilemask3		Tilemasks2+1
#DEFINE RotTileqck	Tilemask3+1
#DEFINE MapperRamEnd	RotTileqck+5
#Define MapStructSize	MapStructEnd-MapperRam
#DEFINE MapperRamSize	MapperRamEnd-MapperRam
#endif

#define HEADER

#else

#ifdef INCLUDERGP
#IFNDEF TI83
#IFNDEF TI83P
#DEFINE TI83P		;Falls back onto TI-83Plus if nothing is chosen
#ENDIF
#ENDIF
#IFDEF TI83P
#IFDEF TI83
#UNDEFINE TI83
#ENDIF
#ENDIF

#IFDEF VTI
#IFDEF TILEM
#UNDEFINE TILEM
#ENDIF
#IFNDEF EMU
#DEFINE EMU
#ENDIF
#ENDIF
#IFDEF TILEM
#IFNDEF EMU
#DEFINE EMU
#ENDIF
#ENDIF
#IFNDEF EMU
Frame_skip = 4
#ELSE
#IFDEF VTI
Frame_skip = 2
#ENDIF
#IFDEF TILEM
Frame_skip = 1
#ENDIF
#ENDIF
#IFDEF TI83
interrupt_entry =$8787		;uses statvars
interrupt_byte	=$87
interrupt_table =$8600
interrupt_reg	=$86
#ENDIF
#IFDEF TI83P
interrupt_entry =$8a8a		;uses statvars
interrupt_byte	=$8a
interrupt_table =$8b00
interrupt_reg	=$8b
#ENDIF

gbuf1	= $9340		;gbuf aka Plotsscreen
gbuf2	= $86EC		;Savesscreen aka saferam1 aka apdbuf

#ifdef NODOUBLEBUFFER
gsActivebuf1 = gbuf1
gsActivebuf2 = gbuf2
#endif 


#ifdef ALIGNEDSPRITE
;==========================================================
; gsAlignedSprite
;==========================================================
; 66 bytes
; puts 8x8 aligned sprite
; a=x l=y ix=sprite
gsAlignedSprite:
	sla l			;*2
	sla l			;*4
	ld h,0
	ld b,h
	ld c,l
	add hl,bc		;*8
	add hl,bc		;*12
	ld c,a
	add hl,bc
	push hl		;now we got right spot in hl
	push ix		;tile moved to DE for speed
	pop de
	ld bc,gbuf1 ;
	add hl,bc
	ld bc,12
	call write8
	pop hl
	ld bc,gbuf2 ;
	add hl,bc
	ld bc,12
write8:
	ld a,(de)
	ld (hl),a
	add hl,bc
	inc de
	ld a,(de)
	ld (hl),a
	add hl,bc
	inc de
	ld a,(de)
	ld (hl),a
	add hl,bc
	inc de
	ld a,(de)
	ld (hl),a
	add hl,bc
	inc de
	ld a,(de)
	ld (hl),a
	add hl,bc
	inc de
	ld a,(de)
	ld (hl),a
	add hl,bc
	inc de
	ld a,(de)
	ld (hl),a
	add hl,bc
	inc de
	ld a,(de)
	ld (hl),a
	inc de
	ret
#endif




#IFDEF ALIGNEDMASKEDSPRITE
;==========================================================
; gsAlignedMaskedSprite
;==========================================================
;187 bytes
; mask/draw 8x8 aligned sprite
; a=number + 64*(HORIZFLIP) + 128*(VERTFLIP)
; h=x l=y de=sprite table
; output: everything destroyed, but ix=ix+8
;
;I optimising alot here, I got rid of the second tempsprite
;I'm making it faster were I can, As far as the main loop
;I just completely reworte it, Duck's code was very awkward.
;I dropped the size about 50 bytes and made it MUCH FASTER.

gsAlignedMaskedSprite:
	push hl
;---------------------------------KV83 way of commenting
;Finds the tile
	ld h,0
	ld l,a
	sla l				;clears 
	sla l				;upper bits.
	add hl,hl
	add hl,hl
	add hl,de
;---------------------------------
;copys to temp area
	ld de,gsTempSprite
	ld bc,16
	ldir
;---------------------------------
;flips it if needed
	ld c,a
	bit 6,a
	call nz,gsFlipHoriz
	bit 7,c
	call nz,gsFlipVert
	ld ix,gsTempSprite
	pop hl
;---------------------------------
;find point on buffer
	ld a,h
	ld h,0
	sla l				;*2
	sla l				;*4
	ld b,h
	ld c,l
	add hl,bc			;*8
	add hl,bc			;*12
	ld c,a
	add hl,bc			;x offset
	ld b,h
	ld c,l
	ld hl,gbuf2 
	add hl,bc			;de=layer2
	ex de,hl
	ld hl,gbuf1
	add hl,bc			;hl=layer1
;----------------------------------
;Duck wrote some of the most cryptic
;code I have ever seen. So I just deleted
;it, and wrote my own. Dark grey is
;transparent.
	ld b,8
gsMaskedSprite_loop:	;this could be made faster calling 8 times.
	push bc
#ifndef REVERSEMASK 
	ld a,(ix)		;getmask
	ld b,a			;save for later
	and (ix+8)		;strip drakgrey away
#else
	ld a,(ix+8)
	ld b,a
	and (ix)
#endif
	ld c,a			;save spr data
	ld a,(hl)		;get screen
	and b			;mask it
#ifndef REVERSEMASK 
	or c			;or black
#else
	or (ix)
#endif
	ld (hl),a		;save it
	ex de,hl		;light buffer
	ld a,(hl)		;get screen
	and b			;mask it
#ifndef REVERSEMASK 
	or (ix+8)		;or LG and black
#else
	or c
#endif
	ld (hl),a		;save it
	ld bc,12		;
	add hl,bc		;next row in buffer
	ex de,hl		;gbuf1 = hl
	add hl,bc		;next row in buffer
	inc ix			;next byte in sprite
	pop bc
	djnz gsMaskedSprite_loop
	ret

;=== horizontal sprite flip
gsFlipHoriz:
	ld de,gsTempSprite+7
	ld de,gsTempSprite
	call Horiswap
	ld de,gsTempSprite+7
	ld de,gsTempSprite
Horiswap:
	ld a,(de)
	ld b,(hl)
	ld (hl),a
	ld a,b
	ld (de),a
	dec de
	inc hl
	ld a,(de)
	ld b,(hl)
	ld (hl),a
	ld a,b
	ld (de),a
	dec de
	inc hl
	ld a,(de)
	ld b,(hl)
	ld (hl),a
	ld a,b
	ld (de),a
	dec de
	inc hl
	ld a,(de)
	ld b,(hl)
	ld (hl),a
	ld a,b
	ld (de),a
	ret

;=== vertical sprite flip
gsFlipVert:
	ld hl,gsTempSprite
	ld b,16
gsFlipVert_loop:
	ld c,(hl)
	xor a
	rr c
	rla
	rr c
	rla
	rr c
	rla
	rr c
	rla
	rr c
	rla
	rr c
	rla
	rr c
	rla
	rr c
	rla
	ld (hl),a
	inc hl
	djnz gsFlipVert_loop
	ret

#ENDIF



;-------------------------------------
;Most people won't use the sp register
;becuase that means you can't have any
;calls or interrupts, thats not true.
;Since sp will point to buffers, it is
;safe mem to write in and won't be
;noticed by coder or user, so using it
;to clear the screen would be faster
;than ldir. 1/3 the time actually

gsClearbuffer:
	ld hl,0
	ld d,h
	ld e,l
	add hl,sp
	ld bc,$2020
	ld sp,gbuf1+768
gsCB1:
	push de
	push de 		;this clears the buf
	push de 		;in a third of the time
	push de 		;ldir does.
	push de
	push de
	push de
	push de
	push de
	push de
	push de
	push de
	djnz gsCB1
	ld b,c
	ld sp,gbuf2+768
gsCB2:
	push de
	push de
	push de
	push de
	push de
	push de
	push de
	push de
	push de
	push de
	push de
	push de
	djnz gsCB2
	ld sp,hl
	ret



gsCopybuffer:
#ifndef NODOUBLEBUFFER
	ld hl,gbuf1
	ld de,gsActivebuf1
	ld bc,768
	call gsCopy

	ld hl,gbuf2
	ld de,gsActivebuf2
	ld bc,768
	jp gsCopy
#else
	ret
#endif


; copies buffer 1 to buffer 2
gsbuf1Tobuf2:
	ld hl,gbuf1
	ld de,gbuf2
	ld bc,768
;This can be used as an entry for a faster ldir routine
;MUST BE DIVISIBLE BY 16 BYTES, save 4000 clocks
gsCopy:
	ldi \ ldi \ ldi \ ldi
	ldi \ ldi \ ldi \ ldi
	ldi \ ldi \ ldi \ ldi
	ldi \ ldi \ ldi \ ldi
	jp pe,gsCopy
	ret

;---------------------------------------------
;gsGetcsc by James Montelongo 
;43 bytes, Shaved off 132 bytes :)
;I stared long and hard a Ducks getk routine.
;Theres was alot of work for something that 
;should have been relatively simple. So I 
;just completely rewrote the routine. This 
;faster and smaller. :)
gsGetK:
gsGetCSC:
	push hl
	push de
	push bc
	ld e,$fe		;frist group
	ld c,$01		;key port
	ld l,0		;l holds key pressed
cscloop:
	ld a,$ff		;For some reason emulator really wants it in the loop
	out (1),a		;reset keyport
	ld h,$fe
	out (c),e		;set keygroup
	ld b,8		;loop, Delay needed when work with key driver
	in a,(c)		;read key
cscbit:
	inc l			;inc to get key pressed
	rra 			; if key pressed done
	jp nc,donecsc
	rlc h
	djnz cscbit 	;loop 8
	rlc e			;next key group
	jp m,cscloop	;if bit 7 set loop
	ld l,0		;if no key pressed 0
donecsc:
	ld a,$ff
	out (1),a
	ld a,e
	cpl
	out (1),a
	nop
	nop
	in a,(1)
	inc a
	jp z,nootherkeypressed
	ld l,0
nootherkeypressed:
	ld a,$ff
	out (1),a
	nop
	ld a,e
	out (1),a
	nop
	nop
	in a,(1)
	cp h
	jr z,only1key
	ld l,0
only1key:
	ld a,l		;
	or a
	ld (gs_keymem),a
	pop bc
	pop de
	pop hl
	ret




;-------------------------------------------------------------------------
;This version gsenable, detects wether or not you are on an SE, this allows
;for SE owner to take advantage of the better quality grey that the SE can
;provide with out the coders havinging to make seperat verions of the game. 
gsEnable:
	di
	ld a,interrupt_reg
	ld i,a
	ld hl,interrupt_table
	ld de,interrupt_table+1
	ld bc,256
	ld (hl),interrupt_byte
	ldir
	set 7,(iy+20)		;something about text...

	ld hl,Intstart
	ld bc,Intend-Intstart
#IFDEF TI83P
	in a,($6)
	ld (GSapppage),a
	in a,(2)
	and %10000000
	jp z,notse		;83+se, 84+, 84+se 
#IFNDEF EMU

	in a,($20)
	push af
	xor a
	out ($20),a
	ld bc,12000
testSEloop:
	call TestSE
	dec bc
	ld a,b
	or c
	jp nz,testSEloop
	pop af
	out ($20),a


	call settimer
	ld hl,IntStartSE
	ld bc,IntEndSE-IntStartSE
#ENDIF
notse:
#ENDIF
	ld de,interrupt_entry
	ldir
#ifdef DOFRAMECOUNT
	xor a
	ld (frame_count),a
#ENDIF

	xor a		;Fastest speed
	out (4),a
	ld a,8
	out (3),a
	ld a,10
	out (3),a
	im 2
	ei

	ret



#IFDEF TI83P

TestSE:
	in a,($29)
	and $FC
	out ($29),a
waittestse1:
	in a,($10)
	rla
	jp c,waittestse1
	ld a,$80
	out ($10),a
waittestse2:
	in a,($10)
	rla
	jp c,waittestse2
	xor a
	out ($11),a
	ld h,0
	ld l,0
	add hl,hl
	add hl,hl
	add hl,hl
	in a,($10)
	rla
	ret nc
	in a,($29)
	add a,4
	out ($29),a
	jp nc,TestSE

	ld a,$FF
	out ($29),a
	ret
#ENDIF




;---------------------------
;Frquency handle will display a screen on
;84+s and 83+SEs.  This will allow the user
;to adjust quality. Speed might vary with this!!!

gsSetFreq:			;No need to set the frequency.
	ret


settimer:			;the heart of the SE routine
	xor a			;clear
	out ($30),a
	ld a,$40		;10922.67 Hz
	out ($30),a
	ld a,2		;generate interrupt
	out ($31),a
	ld c,$B0
	in a,(2)
	bit 5,a
	jr z,skipdifffreq
	ld c,$BB
skipdifffreq:
	ld a,c
	out ($32),a 	;screen draws at 60-64hz 
	ret





;-----------------------------
;Turns off grey and set im 1
;also enable normal interrupts
;disables timers and lowers speed
;A MUST ON EXIT

gsDisable:
	di
	im 1
	ld a,6
	out (4),a
	ld a,$0b		;regular interrupts occur
	out (3),a
#IFDEF TI83P
	in a,(2)
	and %10100000
	ret z
	xor a			;disable timer
	out ($30),a
#ENDIF
	ret




#IFDEF EMU
EMUDELAY:
	push bc
	ld b,240
vtidelayloop:
#IFDEF TILEM
	push hl
	pop hl
	nop
#ENDIF
	push hl
	pop hl
	djnz vtidelayloop
	pop bc
	ret
#ENDIF





;-----------------------------------------------------
;This is for the SE it uses one of the crystal timers
;to sync with the refresh rate of the lcd reducing the
;noise.
#IFDEF TI83P
IntStartSE:
	push af
	ex af,af'
	push af
	in a,($32)		;This is the work around for the halt bug
	cp $bc
	jr nc,greyintSE
	in a,(4)
	and $20 	;Timers only for grey
	jr z,GreyskipSE
greyintSE:
	exx
	ld b,13 	;this is the fine tuning
	djnz $		;15,000,000/(105*13)= 10,989
	in a,($20)
	push af
	xor a
	out ($20),a
	in a,($6)
	push af
	ld a,(GSapppage)
	out ($6),a
	call settimer	;sets time for next interrupt
	call lcd
#ifdef DOFRAMECOUNT
	ld hl,frame_count
	inc (hl)
#endif
	pop af
	out ($6),a
	pop af
	out ($20),a
	exx
GreyskipSE:
	xor a
	out (4),a
	ld a,8
	out (3),a
	ld a,%00001110
	out (3),a
	pop af
	ex af,af'
	pop af
	ei
	ret
IntEndSE:

#endif




;----------------------------------------------------
;The interupts on an 83P are usually not balance out
;But when both timers are running the 560hz timer is 
;evenly spaced and can be detected. So 1120hz timer
;is skipped, and every 3 560hz timer is skiped to get
;as close as possible to the refresh rate.

IntStart:
	push af
#IFDEF EMU
	call EMUDELAY
#ENDIF
#IFDEF TI83
	in a,(3)
#ELSE
	in a,(4)
#ENDIF
	and 2			;Timers only for grey
	jr z,Greyskip
	ld a,(skip83)
	dec a
	ld (skip83),a
	jr nz,Greyskip
	ld a,Frame_skip
	ld (skip83),a
	exx
#IFDEF TI83P
	in a,($6)
	push af
	ld a,(GSapppage)
	out ($6),a
#ENDIF
	call lcd
#ifdef DOFRAMECOUNT
	ld hl,frame_count
	inc (hl)
#endif
#IFDEF TI83P
	pop af
	out ($6),a
#ENDIF
	exx
Greyskip:

	ld a,0
	out (4),a
	ld a,8
	out (3),a
	ld a,%00001010
	out (3),a
	pop af
	ei
	ret
IntEnd:


;FreqTab:
; .db 0,8-5		;560/8  = 70
; .db 0,9-5		;560/9  = 62.2222
; .db 2,4-3		;248/4  = 62
; .db 6,2		;118/2  = 59
; .db 4,3-2		;170/3  = 56.6666
; .db 0,10-5	;560/10 = 56

;Freq:
; .db 1
;OutFreq:
; .db 0
;Skipfreq:
; .db 9-5



;-------------------------------------------------
;4 level Grey interlace routine
;by James Montelongo 
lcd:					;52744
	ld (stacksave),sp
	ld	a,$80
	out ($10),a
	ld a,(gsmasknum)
	inc a
	cp 3
	jr c,skipmaskswap
	xor a
skipmaskswap:
	ld (gsmasknum),a
	ld e,a
	ld d,0
	ld hl,gsmasks
	add hl,de
	ld d,(hl)
#IFNDEF FULLSCREEN
	inc hl
#ENDIF
	ld a,(hl)
	cpl
	ld e,a
	ld hl,gsActivebuf1-12
	ld sp,12
	ld a,$20
	ld c,a
colloop:

	out ($10),a
	ld b,32
rowloop:
	add hl,sp
	ld a,(hl)
	inc h
	inc h
	inc h
	xor (hl)
	and d
	xor (hl)
	out ($11),a
	add hl,sp
	nop
	ld a,(hl)
	dec h
	dec h
	dec h
	xor (hl)
	and e
	xor (hl)
	out ($11),a
	djnz rowloop
	inc c
	dec h
	dec h
	dec h
	inc hl
	ld a,c
	cp $2c
	jr nz,colloop
	ld sp,(stacksave)
	ret



gsmasks:

#IFNDEF FULLSCREEN
 .db %11011011
 .db %10110110
 .db %01101101
 .db %11011011
#ELSE
 .db $FF
 .db $FF
 .db $00
 .db $FF
#ENDIF

#ifdef XORSPRITE

;==========================================================
; gsPutSprite
;==========================================================
;=== gsPutSprite 2.0 by James Montelongo
;104 bytes, pretty fast. backwards Compatible with Durk's
;
; puts XOR grayscale sprite
; b=Height
; l=yc
; a=xc
; ix holds pointer

gsPutSprite:
	sla l			;*2
	sla l			;*4
	ld e,l
	ld h,$00
	ld d,h
	add hl,de		;*8
	add hl,de		;*12
	ld e,a
	and $07
	xor 7
	ld (gsputrotate2),a
	add a,a
	ld (gsputrotate1),a
	ld a,$ff
gsputrotate1 = $+1
	jr $
	srl a
	srl a
	srl a
	srl a
	srl a
	srl a
	srl a
	ld (gsputmask1),a
	cpl
	ld (gsputmask2),a
	srl e
	srl e
	srl e
	add hl,de
	push hl
	ld	de,gbuf1
	add hl,de
	push bc
	ld	de,11
	call gsPutSpriteLoop1
	pop bc
	pop hl
	ld	de,gbuf2
	add hl,de
	ld	de,11
gsPutSpriteLoop1:
	ld	a,(ix)
gsputrotate2 = $+1
	jr $
	rrca
	rrca
	rrca
	rrca
	rrca
	rrca
	rrca
	ld c,a
gsputmask1 = $+1
	and 0
	xor (hl)
	ld (hl),a
	inc hl
	ld a,c
gsputmask2 = $+1
	and 0
	xor (hl)
	ld	(hl),a
	add hl,de
	inc ix
	djnz gsPutSpriteLoop1
	ret
#endif

#ifdef LARGESPRITE

gsLargeSprite:
	sla l			;*2
	sla l			;*4
	ld e,l
	ld h,$00
	ld d,h
	add hl,de		;*8
	add hl,de		;*12
	ld e,a
	and $07
	xor 7
	ld (gslargerotate2),a
	add a,a
	ld (gslargerotate1),a
	ld a,$ff
gslargerotate1 = $+1
	jr $
	srl a
	srl a
	srl a
	srl a
	srl a
	srl a
	srl a
	ld (gslargemask1),a
	cpl
	ld (gslargemask2),a
	srl e
	srl e
	srl e
	add hl,de
	ld a,c
	ld (gslargewidth),a
	ld c,b

	push hl
	ld	de,gbuf1
	add hl,de
	push bc
	call gsLargeRow
	pop bc
	pop hl
	ld	de,gbuf2
	add hl,de

gsLargeRow:
	push hl
gslargewidth = $+1
	ld b,0
gsLargemask1 = $+1
	ld d,0
gsLargeCol:
	ld a,(ix)
	inc ix
gsLargerotate2 = $+1
	jr $
	rrca
	rrca
	rrca
	rrca
	rrca
	rrca
	rrca
	ld e,a
	and d
	xor (hl)
	ld (hl),a
	inc hl
	ld a,e
gsLargemask2 = $+1
	and 0
	xor (hl)
	ld	(hl),a
	djnz gsLargeCol
	pop hl
	ld de,12
	add hl,de
	dec c
	jp nz,gsLargeRow
	ret

#endif

#ifndef APP
;---------------------------
;temp-no need to save on exit
;---------------------------

;Current mask to use.
gsmasknum:
 .db 0

#IFDEF TI83P
;this is the app page where the gs code should be
GSapppage:
 .db 0
#ENDIF


;for the 83(+) handls interrupt skips
skip83:		;1 byte
 .db 3

;saves stack, used in routine LCD
stacksave:		;2 bytes
 .dw 0

;gsgetcsc, copy of pressed key( consider using it!! )
gs_keymem:		;1 byte
 .db 0

#ifdef DOFRAMECOUNT
frame_count:
 .db 0
#endif



#IFDEF ALIGNEDMASKEDSPRITE

gsTempSprite:		;16 bytes
gsTempSprite2:
 .db 0,0,0,0,0,0,0,0
 .db 0,0,0,0,0,0,0,0

#endif


#ifndef NODOUBLEBUFFER

;both buffers are 768 bytes

gsActivebuf1:
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
gsActivebuf2:
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
#endif

#endif

#endif

#ifdef INCLUDEMAPPER
;===================================================================
;GrayMapper by James Montelengo Starts Here
;
.echo "Map code!!"
PMAPHEIGHT = PMAPHIEGHT 
PMAPBUFSIZE = PMAPWIDTH*PMAPHIEGHT*8*Tile_width*Tile_width
PREMAPBUFSIZE = (PMAPWIDTH+1)*PMAPHIEGHT


Startmapsize:



; finds map location based
;in
; hl = map y
; bc = map x
;out
; ix = map loc




find:
	ld a,l
#IF Map_Entry_Size=2
#IF Tile_width=2
	and $f0
#ELSE
	and $f8
#ENDIF
#ELSE
	sra h
	rra
#ENDIF
	sra h
	rra
	sra h
	rra
#IF Tile_width=2
	sra h
	rra
#ENDIF
	push af
	bit 7,a
	jr z,$+2+2
	neg
	ld h,a
	ld a,(Mapwidth)
	ld e,a
	call HxE
	pop af
	bit 7,a
	jr z,$+2+3
	call neghl
	ld a,c
#IF Map_Entry_Size=2
#IF Tile_width=2
	and $f0
#ELSE
	and $f8
#ENDIF
#ELSE
	sra b
	rra
#ENDIF
	sra b
	rra
	sra b
	rra
#IF Tile_width=2
	sra b
	rra
#ENDIF
	ld c,a
	add hl,bc
	ex de,hl
	ld ix,(MapPtr)
	add ix,de
	ret


#IFDEF Animated
DoAnimate:
	push hl
	push de
#IF Map_Entry_Size=2
	ld e,(ix)
#IF MAP_MASK=$FF
	ld d,(ix+1)
#ELSE
	ld a,(ix+1)		;16 bit haha
	and MAP_MASK
	ld d,a
#ENDIF
#ELSE
#IF MAP_MASK=$FF
	ld e,(ix)
#ELSE
	ld a,(ix)
	and MAP_MASK
	ld e,a
#ENDIF
	ld d,0
#ENDIF
	ld hl,(AniTab)
	add hl,de
	add hl,de
	ld a,(hl)
	or a
	jr nz,itsanimated
	pop de
	pop hl
	ret

itsanimated:
	inc hl
	ld a,(hl)
	pop hl
	ld l,a
	ld h,0
	add hl,de
	ex de,hl
	jp animatget

#ENDIF

;reads map pointed by ix, makes DE point to tile
;hl is preserved
get:
	push hl
#IF Map_Entry_Size=2
	ld e,(ix)
#IF MAP_MASK=$FF
	ld d,(ix+1)
#ELSE
	ld a,(ix+1)		;16 bit haha
	and MAP_MASK
	ld d,a
#ENDIF
#ELSE
#IF MAP_MASK=$FF
	ld e,(ix)
#ELSE
	ld a,(ix)
	and MAP_MASK
	ld e,a
#ENDIF
	ld d,0
#ENDIF
	ld hl,0
animatget:
	ld a,(Num_Layers)
	bit 2,a
	jr z,$+2+1
	add hl,de
	add hl,hl
	bit 1,a
	jr z,$+2+1
	add hl,de
	add hl,hl
	rra
	jr nc,$+2+1
	add hl,de
	add hl,hl
	add hl,hl
	add hl,hl
#IF Tile_width=2
	add hl,hl
	add hl,hl
#ENDIF
	ld de,(TilePtr)
	add hl,de
	ex de,hl
	pop hl
	scf
	ret








;------------------
;Find map loc
;	ld a,(MapY)
;	and 7
;	ld h,a
;	ld e,PMAPWIDTH
;	call HxE



UpdateMap:
	ld hl,(mapY)
	ld de,(OldMapy)
	or a
	sbc hl,de
	bit 7,h
	call nz,neghl
	ld de,32
	or a
	sbc hl,de
	jp nc,DrawMapFull

	ld hl,(mapx)
	ld de,(OldMapx)
	or a
	sbc hl,de
	bit 7,h
	call nz,neghl
	ld de,48
	or a
	sbc hl,de
	jp nc,DrawMapFull
	ld hl,(PreMapPTR)
	ld e,l
	ld d,h
	ld (hl),0
	inc de
	ld bc,PREMAPBUFSIZE-1
	ldir
	call shiftmap
	call DrawMappartial
	ret


DrawMapFull:
	ld hl,(PreMapPTR)
	ld e,l
	ld d,h
	ld (hl),1
	inc de
	ld bc,PREMAPBUFSIZE-1
	ldir
DrawMappartial:
	ld hl,(MapY)
	ld bc,(MapX)
	call find
	call setdrawtile

	ld a,$ff
	ld (Tilemask1),a
	ld hl,(MapBufPtr)
	dec hl

#IF Tile_width=2
	ld a,(mapx)
	and $08
	jr z,skip16add1
	ld a,$ff
	ld (Tilemask2),a
	ld (Tilemask3),a
	dec hl
skip16add1:
#endif

	ld de,(PreMapPTR)
	push ix
	ld b,PMAPHIEGHT 
	call domapCol
	pop ix
#IF Tile_width=2
	ld a,(Tilemask4)
	ld (Tilemask2),a
	cpl
	ld (Tilemask1),a
	ld (Tilemask3),a
#else
	ld a,(Tilemask2)
	cpl
	ld (Tilemask1),a
#endif
	inc ix
#if Map_Entry_Size=2
	inc ix
#endif
	ld hl,(MapBufPtr)


#IF Tile_width=2
	ld a,(mapx)
	and $08
	jr nz,skip16add1a
	inc hl
skip16add1a:
#endif


	ld de,(PreMapPTR)
	inc de
	ld b,PMAPWIDTH-1
loopinnercol:
	push bc
	push ix
	push hl
	push de
	ld b,PMAPHIEGHT 
	call domapCol
	pop de
	pop hl
	pop ix
	inc de
	inc hl
#IF Tile_width=2
	inc hl
#endif
	inc ix
#if Map_Entry_Size=2
	inc ix
#endif
	pop bc
	djnz loopinnercol


#IF Tile_width=2
	ld a,$FF
	ld (Tilemask4),a
	ld a,(mapx)
	and $08
	jr nz,skip16add2
	ld a,$ff
	ld (Tilemask2),a
	ld (Tilemask3),a
skip16add2:
#else

	ld a,$FF
	ld (Tilemask2),a
#endif

	ld b,PMAPHIEGHT 
	call domapCol
	ld hl,(Mapy)
	ld (OldMapy),hl
	ld hl,(Mapx)
	ld (OldMapx),hl
	ret

domapCol:
	push bc
	push hl
	push de
#IFDEF Animated
	call DoAnimate
	jr c,Thistileisanimated
#ENDIF
	ld a,(de)
	or a
	jr z,dontdrawthisone
	call get
Thistileisanimated:

	ld a,(Num_Layers)
	ld b,a
drawlayers:
	push bc
	call Drawtile
	ld bc,PMAPBUFSIZE
	add hl,bc
	pop bc
	djnz drawlayers

dontdrawthisone:


	pop de
	xor a
	ld (de),a
	ld hl,PMAPWIDTH+1
	add hl,de
	ex de,hl
	pop hl
	ld bc,PMAPWIDTH*8*Tile_width*Tile_width
	add hl,bc
	ld bc,(Mapwidth)
#if Map_Entry_Size=2
	add ix,bc
#endif
	add ix,bc
	pop bc
	djnz domapCol
	ret





#IF Tile_width=2

SetDrawTile:
	push ix
	push hl
	push de
	push bc
	ld a,(MapX)
	and 7
	ld hl,Shiftlefttab
	bit 2,a
	jr z,skiplefttab
	ld hl,Shiftrighttab
skiplefttab:
	and 3
	add a,l
	ld l,a
	jr nc,SDTnoinch
	inc h
SDTnoinch:
	push hl
	pop ix
	ld de,RotTileqck
	ld b,4
SDTloop:
	ld a,(hl)
	inc hl
	or a
	jr z,skiploadsdt
	ld (de),a
	inc de
skiploadsdt:
	djnz SDTloop
	ld a,$c9
	ld (de),a
	ld a,(ix+11)
	ld (Tilemask1),a
	ld (Tilemask3),a
	cpl
	ld (Tilemask2),a
	ld (Tilemask4),a
	pop bc
	pop de
	pop hl
	pop ix
	ret
#ELSE


SetDrawTile:
	push ix
	push hl
	push de
	push bc
	ld a,(MapX)
	and 7
	ld hl,Shiftlefttab
	bit 2,a
	jr z,skiplefttab
	ld hl,Shiftrighttab
skiplefttab:
	and 3
	add a,l
	ld l,a
	jr nc,SDTnoinch
	inc h
SDTnoinch:
	push hl
	pop ix
	ld de,RotTileqck
	ld b,4
SDTloop:
	ld a,(hl)
	inc hl
	or a
	jr z,skiploadsdt
	ld (de),a
	inc de
skiploadsdt:
	djnz SDTloop
	ld a,$c9
	ld (de),a
	ld a,(ix+11)
	ld (Tilemask1),a
	cpl
	ld (Tilemask2),a
	pop bc
	pop de
	pop hl
	pop ix
	ret
#ENDIF
Shiftrighttab:
	rrca
	rrca
	rrca
	rrca
Shiftlefttab:
 .db 0,0,0,0
	rlca
	rlca
	rlca
 .db $F0,$E0,$C0,$80
 .db $FF,$FE,$FC,$F8


#IF Tile_width=2


Drawtile:
	push ix
	push hl
	push bc

	push de
	pop ix

	ld b,16
drawtileloop:
	ld de,(Tilemasks1)
	ld a,(ix)
	inc ix
	call RotTileqck
	ld c,a
	ld a,(hl)
	xor c
	and d
	xor c
	ld (hl),a
	inc hl
	ld a,(hl)
	xor c
	and e
	xor c
	ld (hl),a

	ld de,(Tilemasks2)
	ld a,(ix)
	inc ix
	call RotTileqck
	ld c,a
	ld a,(hl)
	xor c
	and d
	xor c
	ld (hl),a
	inc hl
	ld a,(hl)
	xor c
	and e
	xor c
	ld (hl),a

	ld de,(PMAPWIDTH*Tile_width)-2
	add hl,de

	djnz drawtileloop
	push ix
	pop de
	pop bc
	pop hl
	pop ix
	ret

#ELSE

Drawtile:
	push ix
	push hl
	push bc

	push de
	pop ix

	ld de,(Tilemasks)
	ld b,8

drawtileloop:
	ld a,(ix)
	inc ix
	call RotTileqck
	ld c,a
	ld a,(hl)
	xor c
	and d
	xor c
	ld (hl),a
	inc hl
	ld a,(hl)
	xor c
	and e
	xor c
	ld (hl),a
	ld a,d
	ld c,e
	ld de,PMAPWIDTH-1
	add hl,de
	ld d,a
	ld e,c
	djnz drawtileloop
	push ix
	pop de
	pop bc
	pop hl
	pop ix
	ret

#ENDIF



Shiftmap:
	ld hl,(mapY)
	ld a,l
#IF Tile_width=2
	and $F0
#else
	and $F8
#endif
	ld l,a
	ld de,(oldmapY)
	ld a,e
#IF Tile_width=2
	and $f0
#else
	and $f8
#endif
	ld e,a
	sbc hl,de
	jp z,Shiftx
	bit 7,h
	jp nz,ShiftDown

	sra h
	rr l
	sra h
	rr l
	sra h
	rr l
#IF Tile_width=2
	sra h
	rr l
#endif
	push hl
	ld b,l
	inc b
	ld hl,(PreMapPTR)
	ld de,PREMAPBUFSIZE-1
	add hl,de
suppremap:
	push bc
	ld b,PMAPWIDTH 
suppremapsub:
	ld (hl),1
	dec hl
	djnz suppremapsub
	pop bc
	djnz suppremap
	pop hl
	ld h,l
	ld e,PMAPWIDTH*8*TILE_WIDTH*Tile_width
	call HxE

	ex de,hl

	ld hl,PMAPBUFSIZE
	or a
	sbc hl,de
	ld c,l
	ld b,h
	ex de,hl
	ld de,(MapBufPtr)
	add hl,de
	ld a,(Num_Layers)

shiftuploop:
	push bc
	push hl
	push de
	ldir
	pop hl
	ld bc,PMAPBUFSIZE
	add hl,bc
	ex de,hl
	pop hl
	add hl,bc
	pop bc
	dec a
	jr nz,shiftuploop
	jp ShiftX


ShiftDown:
	call neghl

	sra h
	rr l
	sra h
	rr l
	sra h
	rr l
#IF Tile_width=2
	sra h
	rr l
#endif

	push hl
	ld b,l
	inc b
	ld hl,(PreMapPTR)
sdownpremap:
	push bc
	ld b,PMAPWIDTH 
sdownpremapsub:
	ld (hl),1
	inc hl
	djnz sdownpremapsub
	pop bc
	djnz sdownpremap
	pop hl
	ld h,l
	ld e,PMAPWIDTH*8*Tile_width*Tile_width
	call HxE

	ex de,hl

	ld hl,PMAPBUFSIZE
	or a
	sbc hl,de
	ld c,l
	ld b,h

	push de
	ld hl,PMAPBUFSIZE-1
	ld de,(MapBufPtr)
	add hl,de
	pop de
	push hl
	or a
	sbc hl,de
	pop de
	ld a,(Num_layers)
shiftdownloop:
	push bc
	push hl
	push de
	lddr
	pop hl
	ld bc,PMAPBUFSIZE
	add hl,bc
	ex de,hl
	pop hl
	add hl,bc
	pop bc
	dec a
	jr nz,shiftdownloop


ShiftX:
	ld hl,(mapX)
	ld de,(oldmapx)
	or a
	sbc hl,de
	ret z
	bit 7,h
	jp nz,Shiftright
	push hl
	ld a,l
	rra
	rra
	rra
	and %00011111

	call ShiftLeftAbytes
	pop hl
	ld a,l
	and 7
	add a,a
	ld b,a
	add a,a
	add a,b
	ld e,a
	ld d,0
	ld hl,ShiftLeftcalltab
	add hl,de
	ld b,3
shiftLeftloop:
	push bc
	ld e,(hl)
	inc hl
	ld d,(hl)
	inc hl
	push hl
	ld a,1
	call CallDE
	pop hl
	pop bc
	djnz shiftLeftloop


	ld hl,(mapx)
	ld a,l
#IF Tile_width=2
	and $f0
#ELSE
	and $f8
#ENDIF
	ld l,a
	ld de,(oldmapx)
	ld a,e
#IF Tile_width=2
	and $f0
#ELSE
	and $f8
#ENDIF
	ld e,a
	or a
	sbc hl,de
	ld a,l
	rrca
	rrca
	rrca
#IF Tile_width=2
	rrca
#ENDIF
	inc a
	ld hl,(PreMapPTR)
	ld de,PMAPWIDTH+1-1		;yes because the premap is 1 byte larger than the buffer
	add hl,de				;so to get to the last col 1 must be subtracted, +1-1
	ld b,PMAPHIEGHT 
sleftpremap:
	push bc
	ld b,a
	push hl
sleftpremapsub:
	ld (hl),$1
	dec hl
	djnz sleftpremapsub
	pop hl
	ld bc,PMAPWIDTH+1
	add hl,bc
	pop bc
	djnz sleftpremap
	ret

Shiftright:
	call neghl
	push hl
	ld a,l
	rra
	rra
	rra
	and %00011111

	call ShiftRightAbytes
	pop hl
	ld a,l
	and 7
	add a,a
	ld b,a
	add a,a
	add a,b
	ld e,a
	ld d,0
	ld hl,ShiftRightcalltab
	add hl,de
	ld b,3
shiftrightloop:
	push bc
	ld e,(hl)
	inc hl
	ld d,(hl)
	inc hl
	push hl
	ld a,1
	call CallDE
	pop hl
	pop bc
	djnz shiftrightloop


.echo $
	ld hl,(oldmapx)
	ld a,l
#IF Tile_width=2
	and $f0
#ELSE
	and $f8
#ENDIF
	ld l,a
	ld de,(mapx)
	ld a,e
#IF Tile_width=2
	and $f0
#ELSE
	and $f8
#ENDIF
	ld e,a
	or a
	sbc hl,de
	ld a,l
	rrca
	rrca
	rrca
#IF Tile_width=2
	rrca
#ENDIF
	inc a

	ld hl,(PreMapPTR)
	ld b,PMAPHIEGHT 
srightpremap:
	push bc
	ld b,a
	push hl
srightpremapsub:
	ld (hl),$1
	inc hl
	djnz srightpremapsub
	pop hl
	ld bc,PMAPWIDTH+1
	add hl,bc
	pop bc
	djnz srightpremap


	ret




CallDE:
	ex de,hl
	jp (hl)

Shiftleftcalltab:
 .dw noshift		;0
 .dw noshift
 .dw noshift
 .dw ShiftPIXLeft		;1
 .dw noshift
 .dw noshift
 .dw ShiftPIXLeft		;2
 .dw ShiftPIXLeft
 .dw noshift
 .dw ShiftPIXLeft3	;3
 .dw noshift
 .dw noshift
 .dw ShiftNIBLeft		;4
 .dw noshift
 .dw noshift
 .dw ShiftNIBLeft		;5
 .dw ShiftPIXLeft
 .dw noshift
 .dw ShiftNIBLeft		;6
 .dw ShiftPIXLeft
 .dw ShiftPIXLeft
 .dw ShiftPIXLeft3	;7
 .dw ShiftNIBLeft
 .dw noshift

ShiftRightcalltab:
 .dw noshift		;0
 .dw noshift
 .dw noshift
 .dw ShiftPIXRight	;1
 .dw noshift
 .dw noshift
 .dw ShiftPIXRight	;2
 .dw ShiftPIXRight
 .dw noshift
 .dw ShiftPIXright3	;3
 .dw noshift
 .dw noshift
 .dw ShiftNIBRight	;4
 .dw noshift
 .dw noshift
 .dw ShiftNIBRight	;5
 .dw ShiftPIXRight
 .dw noshift
 .dw ShiftNIBRight	;6
 .dw ShiftPIXRight
 .dw ShiftPIXRight
 .dw ShiftPIXright3	;7
 .dw ShiftNIBRight
 .dw noshift

noshift:
	ret

SHiftdowncalc:
	push af
	push de
	push bc
	ld ix,(PreMapPTR)
	ld de,PMAPWIDTH+1
	ld c,$ff
cntdownlp:
	inc c
	ld a,(ix)
	add ix,de
	jr nz,cntdownlp
	ld e,c
	ld h,tile_width*8
	call hxe
	ld a,PMAPHIEGHT*8*Tile_width
	sub l
	ld e,c
	pop bc
	ld b,a
	sla e
	sla e
	sla e	
	ld h,PMAPWIDTH*Tile_width*Tile_width
	call hxe
	pop de
	add hl,de
	ex de,hl
	pop af
	ret

SHiftupcalc:
	push af
	push de
	push bc
	ld ix,(PreMapPTR)
	ld de,PREMAPBUFSIZE-1
	add ix,de
	ld de,-(PMAPWIDTH+1)
	ld c,$ff
cntuplp:
	inc c
	ld a,(ix)
	add ix,de
	jr nz,cntuplp
	ld e,c
	ld h,tile_width*8
	call hxe
	ld a,PMAPHIEGHT*8*Tile_width
	sub l
	ld e,c
	pop bc
	ld b,a
	sla e
	sla e
	sla e	
	ld h,PMAPWIDTH*Tile_width*Tile_width
	call hxe
	pop de
	ex de,hl
	or a
	sbc hl,de
	ex de,hl
	pop af
	ret


ShiftLeftAbytes:
	or a
	ret z
	ld l,a
	ld a,PMAPWIDTH*Tile_width
	sub l
	ld c,a
	ld a,l
	ld de,(MapBufPtr)
	ld hl,(Num_layers)
slbytesloop:
	push hl
	push bc
	push de


	call ShiftLeftAbytesDO
	pop hl
	ld bc,PMAPBUFSIZE
	add hl,bc
	ex de,hl
	pop bc
	pop hl
	dec l
	jr nz,slbytesloop
	ret

ShiftLeftAbytesDO:
	call SHiftdowncalc
SLeftLoop1:
	push bc
	ld b,0
	ld l,a
	ld h,b
	add hl,de
	ldir
	ex de,hl
	pop bc
	djnz SLeftLoop1
	ret


ShiftRightAbytes:
	or a
	ret z
	neg
	ld e,a
	ld a,PMAPWIDTH*Tile_width
	add a,e
	ld c,a
	ld a,e
	ld hl,(MapBufPtr)
	ld de,PMAPBUFSIZE-1
	add hl,de
	ex de,hl
	ld hl,(Num_layers)
srbytesloop:
	push hl
	push bc
	push de
	call ShiftRightAbytesDO
	pop hl
	ld bc,PMAPBUFSIZE
	add hl,bc
	ex de,hl
	pop bc
	pop hl
	dec l
	jr nz,srbytesloop
	ret
ShiftRightAbytesDO:
	call SHiftupcalc
SRightLoop1:
	push bc
	ld b,0
	ld l,a
	ld h,$FF
	add hl,de
	lddr
	ex de,hl
	pop bc
	djnz SRightLoop1
	ret




ShiftNIBLeft:
	ld hl,(MapBufPtr)
	ld de,PMAPBUFSIZE-1
	add hl,de
	ld a,(Num_layers)
	ld b,a
SNIBLeftLoop:
	push bc
	push hl
	call ShiftNIBLeftDo
	pop hl
	ld de,PMAPBUFSIZE
	add hl,de
	pop bc
	djnz SNIBLeftLoop
	ret
ShiftNIBLeftDo:
	ex de,hl
	call SHiftupcalc
	ex de,hl
ShiftNIBLeftLoop1:
	call unrollrld
	djnz ShiftNIBLeftLoop1
	ret
unrollrld:
#IF (PMAPWIDTH*Tile_width)>15
	rld
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>14
	rld
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>13
	rld
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>12
	rld
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>11
	rld
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>10
	rld
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>9
	rld
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>8
	rld
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>7
	rld
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>6
	rld
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>5
	rld
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>4
	rld
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>3
	rld
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>2
	rld
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>1
	rld
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>0
	rld
	dec hl
#ENDIF
	ret

ShiftNIBRight:
	ld hl,(MapBufPtr)
	ld a,(Num_layers)
	ld b,a
SNIBrightLoop:
	push bc
	push hl
	call ShiftNIBRightDo
	pop hl
	ld de,PMAPBUFSIZE
	add hl,de
	pop bc
	djnz SNIBrightLoop
	ret
ShiftNIBRightDo:
	ex de,hl
	call SHiftdowncalc
	ex de,hl
ShiftNIBRightLoop1:
	call unrollrrd
	djnz ShiftNIBRightLoop1
	ret
unrollrrd:
#IF (PMAPWIDTH*Tile_width)>15
	rrd
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>14
	rrd
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>13
	rrd
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>12
	rrd
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>11
	rrd
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>10
	rrd
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>9
	rrd
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>8
	rrd
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>7
	rrd
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>6
	rrd
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>5
	rrd
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>4
	rrd
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>3
	rrd
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>2
	rrd
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>1
	rrd
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>0
	rrd
	inc hl
#ENDIF
	ret

ShiftPIXLeft:
	ld hl,(MapBufPtr)
	ld de,PMAPBUFSIZE-1
	add hl,de
	ld a,(Num_layers)
	ld b,a
SPIXLeftLoop:
	push bc
	push hl
	call ShiftPIXLeftDo
	pop hl
	ld de,PMAPBUFSIZE
	add hl,de
	pop bc
	djnz SPIXLeftLoop
	ret
ShiftPIXLeftDo:
	ex de,hl
	call SHiftupcalc
	ex de,hl
ShiftPIXLeftLoop1:
	or a
	call unrollrl
	djnz ShiftPIXLeftLoop1
	ret

unrollrl:
#IF (PMAPWIDTH*Tile_width)>15
	rl (hl)
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>14
	rl (hl)
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>13
	rl (hl)
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>12
	rl (hl)
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>11
	rl (hl)
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>10
	rl (hl)
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>9
	rl (hl)
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>8
	rl (hl)
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>7
	rl (hl)
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>6
	rl (hl)
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>5
	rl (hl)
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>4
	rl (hl)
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>3
	rl (hl)
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>2
	rl (hl)
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>1
	rl (hl)
	dec hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>0
	rl (hl)
	dec hl
#ENDIF
	ret

ShiftPIXRight:
	ld hl,(MapBufPtr)
	ld a,(Num_layers)
	ld b,a
SPIXRightLoop:
	push bc
	push hl
	call ShiftPIXRightDo
	pop hl
	ld de,PMAPBUFSIZE
	add hl,de
	pop bc
	djnz SPIXRightLoop
	ret
ShiftPIXRightDo:
	ex de,hl
	call SHiftdowncalc
	ex de,hl
ShiftPIXRightLoop1:

	call unrollrr
	djnz ShiftPIXRightLoop1
	ret

unrollrr:
#IF (PMAPWIDTH*Tile_width)>15
	rr (hl)
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>14
	rr (hl)
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>13
	rr (hl)
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>12
	rr (hl)
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>11
	rr (hl)
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>10
	rr (hl)
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>09
	rr (hl)
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>8
	rr (hl)
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>7
	rr (hl)
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>6
	rr (hl)
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>5
	rr (hl)
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>4
	rr (hl)
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>3
	rr (hl)
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>2
	rr (hl)
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>1
	rr (hl)
	inc hl
#ENDIF
#IF (PMAPWIDTH*Tile_width)>0
	rr (hl)
	inc hl
#ENDIF
	ret


ShiftPIXRight3:
	ld hl,(MapBufPtr)
	ld a,(Num_layers)
	ld b,a
SPIXRight3Loop:
	push bc
	push hl
	call Shiftright3
	pop hl
	ld de,PMAPBUFSIZE
	add hl,de
	pop bc
	djnz SPIXRight3Loop
	ret

Shiftright3:
	ex de,hl
	call SHiftdowncalc
	ex de,hl
SR3loop:
	call unrollrrd
	push hl
	dec hl
	rlca
	rlca
	rlca
	rlca
	rlca
	call unrollrl
	pop hl
	djnz SR3loop
	ret

ShiftPIXLeft3:
	ld hl,(MapBufPtr)
	ld de,PMAPBUFSIZE-1
	add hl,de
	ld a,(Num_layers)
	ld b,a
SPIXLeft3Loop:
	push bc
	push hl
	call Shiftleft3
	pop hl
	ld de,PMAPBUFSIZE
	add hl,de
	pop bc
	djnz SPIXLeft3Loop
	ret


Shiftleft3:
	ex de,hl
	call SHiftupcalc
	ex de,hl
Sl3loop:
	call unrollrld
	push hl
	inc hl
	rra
	call unrollrr
	pop hl
	djnz SL3loop
	ret


neghl:
	ld a,h
	cpl
	ld h,a
	ld a,l
	cpl
	ld l,a
	inc hl
	ret



HxE:
	ld d,0
	ld l,d
	add hl,hl
	jr nc,$+3
	add hl,de
	add hl,hl
	jr nc,$+3
	add hl,de
	add hl,hl
	jr nc,$+3
	add hl,de
	add hl,hl
	jr nc,$+3
	add hl,de
	add hl,hl
	jr nc,$+3
	add hl,de
	add hl,hl
	jr nc,$+3
	add hl,de
	add hl,hl
	jr nc,$+3
	add hl,de
	add hl,hl
	ret nc
	add hl,de
	ret


.echo "Size is: ",$-Startmapsize," bytes."

#endif

#ifdef APP

#ifdef INCLUDERGP
gsmasknum	= AllocStart
GSapppage	= gsmasknum + 1
#ifdef TI83P
skip83	= GSapppage + 1
#else
skip83	= GSapppage
#endif
stacksave	= skip83 + 1
gs_keymem	= stacksave + 2
frame_count	= gs_keymem + 1
#ifdef DOFRAMECOUNT
gsTempSprite = frame_count + 1
#else
gsTempSprite = frame_count
#endif
gsTempSprite2 = gsTempSprite
#ifdef ALIGNEDMASKEDSPRITE
gsActivebuf1  = gsTempSprite + 16
#else
gsActivebuf1  = gsTempSprite
#endif 
#ifndef NODOUBLEBUFFER
gsActivebuf2  = gsActivebuf1 + 768
gsRamEnd	  = gsActivebuf2 + 768
#else
gsActivebuf2 = gsActivebuf1
gsRamEnd = gsActivebuf2
#endif
GsRamSize	  = gsRamEnd - gsmasknum
#endif


#ifdef INCLUDEMAPPER

#ifdef INCLUDERGP
PreMap	= gsRamEnd
#else
PreMap = AllocStart
#endif
Mapbuf1	= PreMap + PREMAPBUFSIZE
Mapbuf2	= Mapbuf1 + PMAPBUFSIZE
MapperRam	= Mapbuf2 + PMAPBUFSIZE
MapRamEnd	= MapperRam + MapperRamSize

AllocSize = MapRamEnd - AllocStart

#else

#ifdef INCLUDERGP
AllocSize = gsRamEnd - AllocStart 
#endif

#endif


#endif

#ifdef INCLUDEMAPPER
#ifndef APP
PreMap:
 .fill PREMAPBUFSIZE,0
Mapbuf1:
 .fill PMAPBUFSIZE,0
Mapbuf2:
 .fill PMAPBUFSIZE,0
MapperRam:
.fill MapperRamSize,0
#endif
#endif

#endif
