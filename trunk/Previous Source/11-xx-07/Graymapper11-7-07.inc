






#ifndef MAPDEFINC

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
#define MAPDEFINC

#else

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












