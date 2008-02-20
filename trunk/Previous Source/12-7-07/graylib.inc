


#IFNDEF TI83
#IFNDEF TI83P
#DEFINE TI83P		;Falls back onto TI-83Plus if nothing is chosen
#ENDIF
#ENDIF

#IFDEF TI83P
#UNDEF TI83
#ENDIF

#IFDEF VTI
#UNDEF TILEM
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
Frame_skip = 3
#ELSE
#IFDEF VTI
Frame_skip = 2
#ENDIF
#IFDEF TILEM
Frame_skip = 1
#ENDIF
#ENDIF


#IFDEF TI83
interrupt_entry =$8787
interrupt_byte	=$87
interrupt_table =$8600
interrupt_reg	=$86
#ENDIF
#IFDEF TI83P
;uses statvars
interrupt_entry =$8a8a
interrupt_byte	=$8a
interrupt_table =$8b00
interrupt_reg	=$8b
#ENDIF



gbuf1	= gbuf			;these are defined in ION.inc
gbuf2	= saferam1

#ifdef NODOUBLEBUFFER
gActivebuf1 = gbuf1
gActivebuf2 = gbuf2
#endif 



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
	push hl ;now we got right spot in hl
	push ix ;tile moved to DE for speed
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


gsTempSprite:
gsTempSprite2:
	.db 0,0,0,0,0,0,0,0
	.db 0,0,0,0,0,0,0,0



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

;---------------------
;No changes to these

gsCopybuffer:
#ifndef NODOUBLEBUFFER
	ld hl,gbuf1
	ld de,gActivebuf1
	ld bc,768
	call gsCopy

	ld hl,gbuf2
	ld de,gActivebuf2
	ld bc,768
	jp gsCopy
#endif

gsSetFreq:			;No need to set the frequency.
	ret

;Since my routine does not have a varible
;frequency it won't except any input on it
;my routine will try to be as tuned as possible
;for all calculators.
;----------------------------------


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
;


gsGetK:
gsGetCSC:
	push hl
	push de
	push bc
	ld a,$ff		;
	out (1),a		;reset keyport
	ld e,$fe		;frist group
	ld c,$01		;key port
	ld l,0		;l holds key pressed
cscloop:
	out (c),e		;set keygroup
	ld b,8		;loop, Delay needed when work with key driver
	in a,(c)		;read key
cscbit:
	inc l			;inc to get key pressed
	rra 			; if key pressed done
	jp nc,donecsc
	djnz cscbit 	;loop 8
	rlc e			;next key group
	jp m,cscloop	;if bit 7 set loop
	ld l,0		;if no key pressed 0
donecsc:
	ld a,l		;
	or a
	ld (gs_keymem),a
	pop bc
	pop de
	pop hl
	ret

gs_keymem:
	.db 0




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
	Set 7,(iy+20)

	ld hl,Intstart
	ld bc,Intend-Intstart
#IFDEF TI83P
	in a,(2)
	and %10100000
	jp z,notse		;83+se, 84+, 84+se 
#IFNDEF EMU
	in a,($29)


	call settimer
	ld hl,IntStartSE
	ld bc,IntEndSE-IntStartSE
#ENDIF
notse:
#ENDIF
	ld de,interrupt_entry
	ldir

	xor a		;Fastest speed
	out (4),a
	ld a,8
	out (3),a
	ld a,10
	out (3),a
	im 2
	ei
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

;-----------------------------------------------------
;This is for the SE it uses one of the crystal timers
;to sync with the refresh rate of the lcd reducing the
;noise.
#IFDEF TI83P
IntStartSE:
	push af
	ex af,af'
	push af
	in a,($32)		;THis is the work around for the halt bug
	cp $b1
	jr nc,greyintSE
	in a,(4)
	and $20 	;Timers only for grey
	jr z,GreyskipSE
greyintSE:
	exx
	ld b,13 	;this is the fine tuning
	djnz $		;15,000,000/(105*13)= 10,989
	call settimer	;sets time for next interrupt
	in a,($20)
	push af
	xor a
	out ($20),a
	call LCD
	pop af
	out ($20),a
#ifdef CONTRASTCTRL
	in a,(4)
	bit 3,a
	call z,ConKey
#ENDIF
;	xor a
;	ld hl,Gwait
;	cp (hl)
;	jr z,$+3		;for vsync and halt
;	dec (hl)
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

settimer:			;the heart of the SE routine
	xor a			;clear
	out ($30),a
	ld a,$40		;10922.67 Hz
	out ($30),a
	ld a,2		;generate interrupt
	out ($31),a
	ld a,0b0h
	out ($32),a 	;screen draws at 60-64hz 
	ret

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
	call LCD
#ifdef CONTRASTCTRL
#IFDEF TI83
	in a,(3)
#ELSE
	in a,(4)
#ENDIF
	bit 3,a
	call z,ConKey
#ENDIF
;	xor a
;	ld hl,Gwait
;	cp (hl)
;	jr z,$+3		;for vsync and halt
;	dec (hl)
	exx
Greyskip:
	xor a
	out (4),a
	ld a,8
	out (3),a
	ld a,%00001010
	out (3),a
	pop af
	ei
	ret
IntEnd:


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

#ifdef CONTRASTCTRL
;this handles the contrast changing in the game
ConKey:
	ld a,$ff
	out (1),a
	ld a,$fd
	out (1),a
norepeat = $+1		;mask so keys won't repeat
	ld a,0
	cpl
	ld c,a
	in a,(1)
	ld (norepeat),a
	or c
	cpl
	ld c,a
	ld a,(contrast)
	rr c
	rr c
	adc a,0
	rr c
	sbc a,0
	cp 39
	jr nc,skipcontrast
	ld (contrast),a
	add a,$d8
	out ($10),a
skipcontrast:
	ld a,$ff
	out (1),a
	ret

#ENDIF



;-------------------------------------------------
;4 level Grey interlace routine
;by James Montelongo 
;148 bytes
;This is a reworked version of Ducks copy routine
;It does the exact same thing only faster.
;It's optimzed to a much finer degree in both
;speed and size. The timings between writes are
;73 here, Ducks was 77, plus he needs to push bc,
;I don't. He used EXX, but i used the SP register
;to gain speed. Finally rather than use a table I
;merely swap the masks from their locations and
;that save on memory.


#ifndef FASTER

lcd:					;61500
	ld (stacksave),sp
	ld	a,$80
	out ($10),a

	ld a,(rt3a+1)
	ld hl,rt1a+1
	ld b,(hl)
	ld (hl),a
	ld a,b
	ld hl,rt2a+1
	ld b,(hl)
	ld (hl),a
	ld a,b
	ld hl,rt3a+1
	ld b,(hl)
	ld (hl),a
	ld a,b
	ld (rt4a+1),a

	ld a,(rt3b+1)
	ld hl,rt1b+1
	ld b,(hl)
	ld (hl),a
	ld a,b
	ld hl,rt2b+1
	ld b,(hl)
	ld (hl),a
	ld a,b
	ld hl,rt3b+1
	ld b,(hl)
	ld (hl),a
	ld a,b
	ld (rt4b+1),a

	ld de,gActivebuf1
	ld hl,gActivebuf2
	ld a,$20
colloop:
	out ($10),a
	ex af,af'
	ex de,hl		; hl =layer1
	ld sp,12
	ld b,21
rowloop:

	ld a,(hl)		; hl =layer1
rt1a:	and %11011011
	ld c,a
	ld a,(de)
rt1b:	and %00100100
	or c
	out ($11),a
	add hl,sp
	ex de,hl
	add hl,sp

	ld a,(de)		; de =layer1
rt2a:	and %01101101
	ld c,a
	ld a,(hl)
rt2b:	and %10010010
	or c
	out ($11),a
	add hl,sp
	ex de,hl
	add hl,sp

	ld a,(hl)		; hl =layer1
rt3a:	and %10110110
	ld c,a
	ld a,(de)
rt3b:	and %01001001
	or c
	out ($11),a
	add hl,sp
	ex de,hl
	add hl,sp

	ex de,hl

	djnz rowloop

	ld a,(hl)		; hl =layer1
rt4a:	and %01011011
	ld c,a
	ld a,(de)
rt4b:	and %10100100
	or c
	out ($11),a
	ld sp,-755
	add hl,sp
	ex de,hl
	add hl,sp
	ex af,af'
	inc a
	cp $2c
	jr nz,colloop
stacksave =$+1
	ld sp,0
	ret

#else

lcd:					;58,700
	ld (stacksave),sp
	ld	a,$80
	out ($10),a

	ld a,(m3a+1)
	ld hl,m1a+1
	ld b,(hl)
	ld (hl),a
	ld (m4a+1),a
	ld (m7a+1),a
	ld (m0a+1),a
	ld a,b
	ld hl,m2a+1
	ld b,(hl)
	ld (hl),a
	ld (m5a+1),a
	ld (m8a+1),a
	ld a,b
	ld (m3a+1),a
	ld (m6a+1),a
	ld (m9a+1),a

	ld a,(contrast)
	add a,$d8
	out ($10),a


	ld a,(m3b+1)
	ld hl,m1b+1
	ld b,(hl)
	ld (hl),a
	ld (m4b+1),a
	ld (m7b+1),a
	ld (m0b+1),a
	ld a,b
	ld hl,m2b+1
	ld b,(hl)
	ld (hl),a
	ld (m5b+1),a
	ld (m8b+1),a
	ld a,b
	ld (m3b+1),a
	ld (m6b+1),a
	ld (m9b+1),a

	ld de,gActivebuf1
	ld hl,gActivebuf2
	ld a,$20
colloop:
	out ($10),a
	ex af,af'
	ex de,hl		; hl =layer1
	ld sp,12
	ld b,7
rowloop:

	ld a,(hl)		; hl =layer1
m1a:	and %11011011
	ld c,a
	ld a,(de)
m1b:	and %00100100
	or c
	out ($11),a
	add hl,sp
	ex de,hl
	add hl,sp

	ld a,(de)		; de =layer1
m2a:	and %01101101
	ld c,a
	ld a,(hl)
m2b:	and %10010010
	or c
	out ($11),a
	add hl,sp
	ex de,hl
	add hl,sp

	ld a,(hl)		; hl =layer1
m3a:	and %10110110
	ld c,a
	ld a,(de)
m3b:	and %01001001
	or c
	out ($11),a
	add hl,sp
	ex de,hl
	add hl,sp

	ld a,(de)		; de =layer1
m4a:	and %11011011
	ld c,a
	ld a,(hl)
m4b:	and %00100100
	or c
	out ($11),a
	add hl,sp
	ex de,hl
	add hl,sp

	ld a,(hl)		; hl =layer1
m5a:	and %01101101
	ld c,a
	ld a,(de)
m5b:	and %10010010
	or c
	out ($11),a
	add hl,sp
	ex de,hl
	add hl,sp

	ld a,(de)		; de =layer1
m6a:	and %10110110
	ld c,a
	ld a,(hl)
m6b:	and %01001001
	or c
	out ($11),a
	add hl,sp
	ex de,hl
	add hl,sp


	ld a,(hl)		; hl =layer1
m7a:	and %11011011
	ld c,a
	ld a,(de)
m7b:	and %00100100
	or c
	out ($11),a
	add hl,sp
	ex de,hl
	add hl,sp

	ld a,(de)		; de =layer1
m8a:	and %01101101
	ld c,a
	ld a,(hl)
m8b:	and %10010010
	or c
	out ($11),a
	add hl,sp
	ex de,hl
	add hl,sp

	ld a,(hl)		; hl =layer1
m9a:	and %10110110
	ld c,a
	ld a,(de)
m9b:	and %01001001
	or c
	out ($11),a
	add hl,sp
	ex de,hl
	add hl,sp

	ex de,hl

	djnz rowloop

	ld a,(hl)		; hl =layer1
m0a:	and %01011011
	ld c,a
	ld a,(de)
m0b:	and %10100100
	or c
	out ($11),a
	ld sp,-755
	add hl,sp
	ex de,hl
	add hl,sp
	ex af,af'
	inc a
	cp $2c
	jp nz,colloop
stacksave =$+1
	ld sp,0
	ret

#endif






;---------------------------
;temp-no need to save on exit
;---------------------------


skip83:
 .db 3


ghalt:
 .db 0

#ifndef NODOUBLEBUFFER
gActivebuf1:
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
gActivebuf2:
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





