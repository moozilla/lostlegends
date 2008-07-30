;data for map entries should go here... after the mapget routine
;naming convention: row x column
;next nodes: up,down,left,right
;null places where you shouldn't be able to move through, ie walls
grendale_castle_0x0:
	.db $08|imp,$08|imp,$08|imp,$08|imp,$08|imp,$08|imp
	.db $08|imp,$1D|imp,$1B|imp,$1A|imp,$1B|imp,$08|imp
	.db $08|imp,$02,$02,$02,$02,$08|imp
	.db $08|imp,$08|imp,$08|imp,$0E,$08|imp,$08|imp
	.dw 0
	.dw grendale_castle_1x0
	.dw 0
	.dw 0
	.db 0						
grendale_castle_0x1:
	.db $08|imp,$0C|imp,$1F|imp,$20|imp,$0C|imp,$08|imp
	.db $0C|imp,$0D|imp,$21|imp,$22|imp,$0D|imp,$0C|imp
	.db $0D|imp,$1B|imp,$02,$02,$1B|imp,$0D|imp
	.db $23|imp,$02,$02,$02,$02,$24|imp
	.dw 0
	.dw grendale_castle_1x1
	.dw 0
	.dw 0
	.db 0
grendale_castle_0x2:
	.db $08|imp,$08|imp,$08|imp,$08|imp,$08|imp,$08|imp
	.db $08|imp,$1D|imp,$1D|imp,$1D|imp,$39,$08|imp
	.db $08|imp,$02,$02,$02,$02,$08|imp
	.db $08|imp,$08|imp,$0E,$08|imp,$08|imp,$08|imp
	.dw 0
	.dw grendale_castle_1x2
	.dw 0
	.dw 0
	.db 1
;portal to 0x3
	.db obj_portal, $41, $42, $02
grendale_castle_0x3:
	.db $08|imp,$08|imp,$08|imp,$08|imp,$08|imp,$08|imp
	.db $08|imp,$1B|imp,$1A|imp,$08|imp,$3A,$08|imp
	.db $08|imp,$02,$02,$02,$02,$08|imp
	.db $08|imp,$08|imp,$08|imp,$08|imp,$08|imp,$08|imp
	.dw 0
	.dw 0
	.dw 0
	.dw 0
	.db 1
;portal to 0x2
	.db obj_portal, $41, $42, $03
grendale_castle_1x0:
	.db $08|imp,$08|imp,$08|imp,$0E,$08|imp,$08|imp
	.db $08|imp,$1B|imp,$26|imp,$02,$02,$0E
	.db $08|imp,$02,$02,$02,$02,$08|imp
	.db $08|imp,$08|imp,$08|imp,$08|imp,$08|imp,$08|imp
	.dw grendale_castle_0x0
	.dw 0
	.dw 0
	.dw grendale_castle_1x1
	.db 0
grendale_castle_1x1:
	.db $08|imp,$02,$02,$02,$02,$08|imp
	.db $0E,$02,$02,$02,$02,$0E
	.db $08|imp,$02,$02,$02,$02,$08|imp
	.db $0C|imp,$02,$02,$02,$02,$0C|imp
	.dw grendale_castle_0x1
	.dw grendale_castle_2x1
	.dw grendale_castle_1x0
	.dw grendale_castle_1x2
	.db 0
grendale_castle_1x2:
	.db $08|imp,$08|imp,$0E,$08|imp,$08|imp,$08|imp
	.db $0E,$02,$02,$1D|imp,$1D|imp,$08|imp
	.db $08|imp,$02,$02,$02,$1D|imp,$08|imp
	.db $08|imp,$08|imp,$08|imp,$08|imp,$08|imp,$08|imp
	.dw grendale_castle_0x2
	.dw 0
	.dw grendale_castle_1x1
	.dw 0
	.db 0
grendale_castle_1x3:
	.db $11|imp,$14|imp,$14|imp,$14|imp,$14|imp,$11|imp
	.db $14|imp,$14|imp,$29|imp,$01,$14|imp,$14|imp
	.db $14|imp,$00,$01,$00,$00,$14|imp
	.db $14|imp,$00,$01,$01,$00,$14|imp
	.dw 0
	.dw grendale_castle_2x3
	.dw 0
	.dw 0
	.db 0
grendale_castle_2x0:
	.db $08|imp,$1A|imp,$1B|imp,$08|imp,$12|imp,$08|imp
	.db $08|imp,$28|imp,$27|imp,$10|imp,$1C|imp,$08|imp
	.db $08|imp,$0F|imp,$0F|imp,$08|imp,$01,$00
	.db $11|imp,$00,$01,$00,$00,$11|imp
	.dw 0
	.dw grendale_castle_3x0
	.dw 0
	.dw grendale_castle_2x1
	.db 0
grendale_castle_2x1:
	.db $0D|imp,$02,$02,$02,$02,$0D|imp
	.db $26|imp,$02,$02,$02,$02,$26|imp
	.db $01,$01,$01,$01,$01,$01
	.db $12|imp,$01,$01,$01,$01,$12|imp
	.dw grendale_castle_1x1
	.dw grendale_castle_3x1
	.dw grendale_castle_2x0
	.dw grendale_castle_2x2
	.db 0
grendale_castle_2x2:
	.db $08|imp,$12|imp,$11|imp,$12|imp,$11|imp,$12|imp
	.db $08|imp,$00,$00,$00,$00,$00
	.db $01,$01,$01,$01,$00,$01
	.db $11|imp,$12|imp,$11|imp,$11|imp,$12|imp,$11|imp
	.dw 0
	.dw 0
	.dw grendale_castle_2x1
	.dw grendale_castle_2x3
	.db	0
grendale_castle_2x3:
	.db $12|imp,$00,$00,$01,$00,$11|imp
	.db $00,$00,$01,$01,$00,$11|imp
	.db $01,$01,$01,$01,$01,$12|imp
	.db $11|imp,$08|imp,$0E,$08|imp,$12|imp,$01|imp
	.dw grendale_castle_1x3
	.dw grendale_castle_3x3
	.dw grendale_castle_2x2
	.dw 0
	.db 0
grendale_castle_3x0:
	.db $12|imp,$00,$00,$00,$00,$12|imp
	.db $12|imp,$13,$28|imp,$01,$13,$12|imp
	.db $12|imp,$12|imp,$13,$13,$12|imp,$12|imp
	.db $01|imp,$12|imp,$12|imp,$12|imp,$01|imp,$12|imp
	.dw grendale_castle_2x0
	.dw 0
	.dw 0
	.dw 0
	.db 0
grendale_castle_3x1:
	.db $11|imp,$00,$00,$00,$01,$12|imp
	.db $12|imp,$01,$00,$00,$00,$11|imp
	.db $01|imp,$12|imp,$00,$00,$12|imp,$11|imp
	.db $11|imp,$11|imp,$01,$01,$11|imp,$12|imp
	.dw grendale_castle_2x1
	.dw 0	;FUTURE LINK TO ANOTHER MAP IN ANOTHER FILE!!!!!
	.dw 0
	.dw 0
	.db 1
	.db obj_reverseportal, $22
	.dw overworld_0x3
grendale_castle_3x2:
	.db $08|imp,$08|imp,$08|imp,$08|imp,$08|imp,$08|imp
	.db $08|imp,$3A,$08|imp,$1D|imp,$1D|imp,$08|imp
	.db $08|imp,$02,$02,$02,$1D|imp,$08|imp
	.db $08|imp,$08|imp,$08|imp,$08|imp,$08|imp,$08|imp
	.dw 0
	.dw 0
	.dw 0
	.dw 0
	.db 1
;actual portal to 3x3, see objdefine.inc for format definitions
	.db obj_portal, $11, $22, $01|%10000000
grendale_castle_3x3:
	.db $08|imp,$08|imp,$0E,$08|imp,$08|imp,$08|imp
	.db $08|imp,$02,$02,$2C|imp,$1E|imp,$08|imp
	.db $08|imp,$39,$02,$02,$02,$08|imp
	.db $08|imp,$08|imp,$08|imp,$08|imp,$08|imp,$08|imp
	.dw grendale_castle_2x3
	.dw 0
	.dw 0
	.dw 0
	.db 1
;actual portal to 3x2, see objdefine for format definitions
	.db obj_portal, $12, $12, $00
	 
	
	
