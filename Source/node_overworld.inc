;data for map entries should go here... after the mapget routine
;naming convention: row x column
;next nodes: up,down,left,right
;null places where you shouldn't be able to move through, ie walls



overworld_0x0:
	.db $14|imp,$14|imp,$14|imp,$14|imp,$14|imp,$14|imp
	.db $12|imp,$14|imp,$3C,$14|imp,$12|imp,$14|imp
	.db $14|imp,$14|imp,$01,$00,$00,$12|imp
	.db $14|imp,$00,$01,$01,$00,$12|imp
	.dw 0
	.dw 0
	.dw 0
	.dw 0
	.db 0	
overworld_0x1:
	.db $14|imp,$14|imp,$11|imp,$14|imp,$14|imp,$14|imp
	.db $14|imp,$14|imp,$14|imp,$1B|imp,$14|imp,$11|imp
	.db $14|imp,$14|imp,$01,$01,$1B|imp,$14|imp
	.db $14|imp,$11|imp,$01,$11|imp,$14|imp,$14|imp
	.dw 0
	.dw 0
	.dw 0
	.dw 0
	.db 0	
overworld_0x2:
	.db $01|imp,$12|imp,$12|imp,$11|imp,$12|imp,$01|imp
	.db $12|imp,$12|imp,$01,$13,$13,$11|imp
	.db $12|imp,$01,$00,$00,$00,$00
	.db $12|imp,$13,$00,$00,$01,$11|imp
	.dw 0
	.dw 0
	.dw 0
	.dw 0
	.db 0	
overworld_0x3:
	.db $14|imp,$14|imp,$3E|imp,$3F|imp,$40|imp,$14|imp
	.db $12|imp,$12|imp,$41,$42,$43|imp,$12|imp
	.db $00|imp,$00,$00,$00,$00,$00|imp
	.db $12|imp,$12|imp,$00|imp,$00|imp,$00|imp,$12|imp
	.dw 0		;FOR TESTING PURPOSES, THIS NODE HAS BEEN SECTIONED OFF
	.dw 0
	.dw 0
	.dw 0
	.db 2
;actual portal to Castle Grendale 3x1, see objdefine for format definitions
	.db obj_portal, $21, $23|%00001000, $04
	.db obj_portal, $31, $33|%00001000, $04
		
overworld_0x4:
	.db $14|imp,$01|imp,$01|imp,$14|imp,$12|imp,$01|imp 
	.db $01,$14|imp,$11|imp,$01,$14|imp,$14|imp
	.db $00,$00,$00,$00,$01,$14|imp
	.db $11|imp,$00,$00,$00,$00,$11|imp
	.dw 0
	.dw 0
	.dw 0
	.dw 0
	.db 0	
