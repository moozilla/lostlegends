graylibtop:
;RGP 2.0 provided by James Montelengo (Jim e)
;made APP safe thanks to James Montelengo
;----------------
;Whole bunch of intial defining crap
;
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
	add hl,bc		;buffer offset
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
	;push hl
	;ld hl,$0000
	;ld (vblankc),hl
	;pop hl
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
	res 7,(iy+20)			;Jim told me this was something that needed to be done for RGP
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
	;ld hl,(vblankc)
	;inc hl
	;ld (vblankc),hl
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
	;ld hl,(vblankc)
	;inc hl
	;ld (vblankc),hl
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
	ld de,gbuf2
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

graylibbottom:
#define graylibxinccode graylibbottom - graylibtop
#define graylibxincsize graylibxinccode
#define graylibxincdata 0

 print_sizes("graylibx.inc", graylibxincsize, graylibxincdata, graylibxinccode)
