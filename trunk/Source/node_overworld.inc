;data for map entries should go here... after the mapget routine
;naming convention: row x column
;next nodes: up,down,left,right
;null places where you shouldn't be able to move through, ie walls
overworld_0x3:
	.db $08,$0C,$1F,$20,$0C,$08
	.db $0C,$0D,$21,$22,$0D,$0C
	.db $0D,$1B,$02,$02,$1B,$0D
	.db $23,$02,$02,$02,$02,$24
	.dw 0
	.dw grendale_castle_1x1
	.dw grendale_castle_0x0
	.dw grendale_castle_0x2
	.db 0
