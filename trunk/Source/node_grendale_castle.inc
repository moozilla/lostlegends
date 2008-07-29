;data for map entries should go here... after the mapget routine
;naming convention: row x column
;next nodes: up,down,left,right
;null places where you shouldn't be able to move through, ie walls
grendale_castle_0x0:
	.db $08|imp,$08|imp,$08|imp,$08|imp,$08,$08
	.db $08|imp,$1D,$1B,$1A,$1B|imp,$08
	.db $08|imp,$02,$02,$02,$02,$08
	.db $08|imp,$08|imp,$08|imp,$0E,$08|imp,$08|imp
	.dw 0
	.dw grendale_castle_1x0
	.dw 0
	.dw grendale_castle_0x1
	.db 0						
grendale_castle_0x1:
	.db $08,$0C,$1F,$20,$0C,$08
	.db $0C,$0D,$21,$22,$0D,$0C
	.db $0D,$1B,$02,$02,$1B,$0D
	.db $23,$02,$02,$02,$02,$24
	.dw 0
	.dw grendale_castle_1x1
	.dw grendale_castle_0x0
	.dw grendale_castle_0x2
	.db 0
grendale_castle_0x2:
	.db $08,$08,$08,$08,$08,$08
	.db $08,$1D,$1D,$1D,$39,$08
	.db $08,$02,$02,$02,$02,$08
	.db $08,$08,$0E,$08,$08,$08
	.dw 0
	.dw grendale_castle_1x2
	.dw grendale_castle_0x1
	.dw 0
	.db 0
grendale_castle_0x3:
	.db $08,$08,$08,$08,$08,$08
	.db $08,$1B,$1A,$08,$3A,$08
	.db $08,$02,$02,$02,$02,$08
	.db $08,$08,$08,$08,$08,$08
	.dw 0
	.dw 0
	.dw 0
	.dw 0
	.db 0
grendale_castle_1x0:
	.db $08,$08,$08,$0E,$08,$08
	.db $08,$1B,$26,$02,$02,$0E
	.db $08,$02,$02,$02,$02,$08
	.db $08,$08,$08,$08,$08,$08
	.dw grendale_castle_0x0
	.dw 0
	.dw 0
	.dw grendale_castle_1x1
	.db 0
grendale_castle_1x1:
	.db $08,$02,$02,$02,$02,$08
	.db $0E,$02,$02,$02,$02,$0E
	.db $08,$02,$02,$02,$02,$08
	.db $0C,$02,$02,$02,$02,$0C
	.dw grendale_castle_0x1
	.dw grendale_castle_2x1
	.dw grendale_castle_1x0
	.dw grendale_castle_1x2
	.db 0
grendale_castle_1x2:
	.db $08,$08,$0E,$08,$08,$08
	.db $0E,$02,$02,$1D,$1D,$08
	.db $08,$02,$02,$02,$1D,$08
	.db $08,$08,$08,$08,$08,$08
	.dw grendale_castle_0x2
	.dw 0
	.dw grendale_castle_1x1
	.dw 0
	.db 0
grendale_castle_1x3:
	.db $11,$14,$14,$14,$14,$11
	.db $14,$14,$29,$01,$14,$14
	.db $14,$00,$01,$00,$00,$14
	.db $14,$00,$01,$01,$00,$14
	.dw 0
	.dw grendale_castle_2x3
	.dw 0
	.dw 0
	.db 0
grendale_castle_2x0:
	.db $08,$1A,$1B,$08,$12,$08
	.db $08,$28,$27,$10,$1C,$08
	.db $08,$0F,$0F,$08,$01,$00
	.db $11,$00,$01,$00,$00,$11
	.dw 0
	.dw grendale_castle_3x0
	.dw 0
	.dw grendale_castle_2x1
	.db 0
grendale_castle_2x1:
	.db $0D,$02,$02,$02,$02,$0D
	.db $26,$02,$02,$02,$02,$26
	.db $01,$01,$01,$01,$01,$01
	.db $12,$01,$01,$01,$01,$12
	.dw grendale_castle_1x1
	.dw grendale_castle_3x1
	.dw grendale_castle_2x0
	.dw grendale_castle_2x2
	.db 0
grendale_castle_2x2:
	.db $08,$12,$11,$12,$11,$12
	.db $08,$00,$00,$00,$00,$00
	.db $01,$01,$01,$01,$00,$01
	.db $11,$12,$11,$11,$12,$11
	.dw 0
	.dw 0
	.dw grendale_castle_2x1
	.dw grendale_castle_2x3
	.db	0
grendale_castle_2x3:
	.db $12,$00,$00,$01,$00,$11
	.db $00,$00,$01,$01,$00,$11
	.db $01,$01,$01,$01,$01,$12
	.db $11,$08,$0E,$08,$12,$01
	.dw grendale_castle_1x3
	.dw grendale_castle_3x3
	.dw grendale_castle_2x2
	.dw 0
	.db 0
grendale_castle_3x0:
	.db $12,$00,$00,$00,$00,$12
	.db $12,$13,$28,$01,$13,$12
	.db $12,$12,$13,$13,$12,$12
	.db $01,$12,$12,$12,$01,$12
	.dw grendale_castle_2x0
	.dw 0
	.dw 0
	.dw 0
	.db 0
grendale_castle_3x1:
	.db $11,$00,$00,$00,$01,$12
	.db $12,$01,$00,$00,$00,$11
	.db $01,$12,$00,$00,$12,$11
	.db $11,$11,$01,$01,$11,$12
	.dw grendale_castle_2x1
	.dw 0	;FUTURE LINK TO ANOTHER MAP IN ANOTHER FILE!!!!!
	.dw 0
	.dw 0
	.db 1
	.db obj_reverseportal, $22
	.dw overworld_0x3
grendale_castle_3x2:
	.db $08,$08,$08,$08,$08,$08
	.db $08,$3A,$08,$1D,$1D,$08
	.db $08,$02,$02,$02,$1D,$08
	.db $08,$08,$08,$08,$08,$08
	.dw 0
	.dw 0
	.dw 0
	.dw 0
	.db 1
;actual portal to 3x3, see objdefine.inc for format definitions
	.db obj_portal, $11, $22, $01|%10000000
grendale_castle_3x3:
	.db $08,$08,$0E,$08,$08,$08
	.db $08,$02,$02,$2C,$1E,$08
	.db $08,$39,$02,$02,$02,$08
	.db $08,$08,$08,$08,$08,$08
	.dw grendale_castle_2x3
	.dw 0
	.dw 0
	.dw 0
	.db 1
;actual portal to 3x2, see objdefine for format definitions
	.db obj_portal, $12, $12, $00
	 
	
	
