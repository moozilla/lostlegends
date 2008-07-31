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
	.dw overworld_1x0
	.dw 0
	.dw 0
	.db 0	
overworld_0x1:
	.db $14|imp,$14|imp,$11|imp,$14|imp,$14|imp,$14|imp
	.db $14|imp,$14|imp,$14|imp,$1B|imp,$14|imp,$11|imp
	.db $14|imp,$14|imp,$01,$01,$1B|imp,$14|imp
	.db $14|imp,$11|imp,$01,$11|imp,$14|imp,$14|imp
	.dw 0
	.dw overworld_1x1
	.dw 0
	.dw 0
	.db 0	
overworld_0x2:
	.db $01|imp,$12|imp,$12|imp,$11|imp,$12|imp,$01|imp
	.db $12|imp,$12|imp,$01,$13,$13,$11|imp
	.db $12|imp,$01,$00,$00,$00,$00
	.db $12|imp,$13,$00,$00,$01,$11|imp
	.dw 0
	.dw overworld_1x2
	.dw 0
	.dw overworld_0x3
	.db 0	
overworld_0x3:
	.db $14|imp,$14|imp,$3E|imp,$3F|imp,$40|imp,$14|imp
	.db $12|imp,$12|imp,$41,$42,$43|imp,$12|imp
	.db $00,$00,$00,$00,$00,$00
	.db $12|imp,$12|imp,$00,$00,$00,$12|imp
	.dw 0		;FOR TESTING PURPOSES, THIS NODE HAS BEEN SECTIONED OFF
	.dw overworld_1x3
	.dw overworld_0x2
	.dw overworld_0x4
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
	.dw overworld_1x4
	.dw overworld_0x3
	.dw 0
	.db 0	
overworld_1x0:
	.db $12|imp,$00,$00,$01,$00,$11|imp
	.db $01|imp,$12|imp,$00,$01,$00,$12|imp
	.db $12|imp,$00,$01,$00,$12|imp,$01|imp
	.db $12|imp,$00,$00,$00,$00,$11|imp
	.dw overworld_0x0
	.dw overworld_2x0
	.dw 0
	.dw 0
	.db 0
overworld_1x1:
	.db $14|imp,$14|imp,$01,$14|imp,$11|imp,$14|imp
	.db $14|imp,$11|imp,$01,$01,$01,$14|imp
	.db $14|imp,$14|imp,$14|imp,$14|imp,$01,$14|imp
	.db $14|imp,$00,$00,$00,$00,$14|imp
	.dw overworld_0x1
	.dw overworld_2x1
	.dw 0
	.dw 0
	.db 0	 
overworld_1x2:
	.db $12|imp,$00,$00,$00,$00,$12|imp
	.db $12|imp,$00,$00,$00,$00,$11|imp
	.db $11|imp,$01,$00,$00,$01,$12|imp
	.db $12|imp,$01,$00,$01,$12|imp,$01|imp
	.dw overworld_0x2
	.dw overworld_2x2
	.dw 0
	.dw 0
	.db 0
overworld_1x3:
	.db $12|imp,$11|imp,$00,$00,$01,$11|imp
	.db $12|imp,$01,$00,$00,$01,$01
	.db $12|imp,$12|imp,$01,$00,$01,$11|imp
	.db $11|imp,$01,$00,$00,$00,$12|imp
	.dw overworld_0x3
	.dw overworld_2x3
	.dw 0
	.dw overworld_1x4
	.db 0
overworld_1x4:
	.db $12|imp,$00,$00,$00,$00,$12|imp
	.db $01,$00,$11|imp,$12|imp,$00,$11|imp 
	.db $12|imp,$00,$00,$00,$00,$12|imp
	.db $12|imp,$11|imp,$00,$12|imp,$01,$11|imp 
	.dw overworld_0x4
	.dw overworld_2x4
	.dw overworld_1x3
	.dw 0
	.db 0
overworld_2x0:
	.db $14|imp,$03,$03,$03,$03,$14|imp 
	.db $14|imp,$03,$03,$03,$03,$14|imp 
	.db $14|imp,$14|imp,$03,$03,$14|imp,$14|imp
	.db $14|imp,$14|imp,$04,$04,$14|imp,$14|imp
	.dw overworld_1x0
	.dw overworld_3x0
	.dw 0
	.dw 0
	.db 0	
overworld_2x1:
	.db $14|imp,$03,$03,$03,$03,$14|imp
	.db $14|imp,$03,$03,$03,$03,$14|imp
	.db $14|imp,$14|imp,$03,$03,$14|imp,$14|imp
 	.db $14|imp,$14|imp,$04,$04,$14|imp,$14|imp
	.dw overworld_1x1
	.dw overworld_3x1
	.dw 0
	.dw 0
	.db 0
overworld_2x2:
	.db $11|imp,$00,$00,$01,$11|imp,$12|imp
	.db $12|imp,$12|imp,$01,$11|imp,$12|imp,$11|imp
	.db $11|imp,$12|imp,$3C,$12|imp,$11|imp,$01|imp
	.db $01|imp,$12|imp,$12|imp,$11|imp,$12|imp,$12|imp
	.dw overworld_1x2
	.dw 0
	.dw 0
	.dw 0
	.db 0
overworld_2x3:
	.db $14|imp,$03,$03,$03,$03,$14|imp
	.db $14|imp,$03,$03,$03,$03,$14|imp
	.db $14|imp,$14|imp,$03,$03,$14|imp,$14|imp
	.db $14|imp,$14|imp,$04,$04,$14|imp,$14|imp
	.dw overworld_1x3
	.dw overworld_3x3
	.dw 0
	.dw 0
	.db 0
overworld_2x4:
	.db $11|imp,$12|imp,$01,$11|imp,$00,$12|imp
	.db $12|imp,$13,$00,$00,$13,$11|imp
	.db $11|imp,$01,$13,$1B|imp,$01,$12|imp
	.db $11|imp,$12|imp,$12|imp,$11|imp,$12|imp,$11|imp
	.dw overworld_1x4
	.dw 0
	.dw 0
	.dw 0
	.db 0
overworld_3x0:
	.db $04|imp,$15|imp,$04,$04,$15|imp,$04|imp
	.db $15|imp,$16|imp,$03,$03,$16|imp,$15|imp
	.db $16|imp,$03,$03,$03,$15|imp,$16|imp
	.db $15|imp,$03,$16|imp,$15|imp,$03,$16|imp
	.dw overworld_2x0
	.dw overworld_4x0
	.dw 0
	.dw 0
	.db 0
overworld_3x1:
	.db $15|imp,$15|imp,$04,$04,$15|imp,$15|imp
	.db $15|imp,$03,$03,$03,$03,$15|imp
	.db $16|imp,$03,$15|imp,$03,$03,$16|imp
	.db $15|imp,$03,$03,$03,$03,$15|imp
	.dw overworld_2x1
	.dw overworld_4x1
	.dw 0
	.dw 0
	.db 0
overworld_3x2:
	.db $16|imp,$16|imp,$04|imp,$16|imp,$15|imp,$16|imp
	.db $04|imp,$04|imp,$16|imp,$15|imp,$3C,$15|imp
	.db $04|imp,$16|imp,$04|imp,$16|imp,$03,$16|imp
	.db $16|imp,$04|imp,$16|imp,$15|imp,$03,$15|imp
	.dw 0
	.dw overworld_4x2
	.dw 0
	.dw 0
	.db 0
overworld_3x3:
	.db $04|imp,$15|imp,$04,$04,$15|imp,$15|imp
	.db $16|imp,$03,$03,$03,$03,$03
	.db $15|imp,$16|imp,$03,$03,$16|imp,$03
 	.db $15|imp,$04|imp,$15|imp,$15|imp,$04|imp,$15|imp
 	.dw overworld_2x3
	.dw 0
	.dw 0
	.dw overworld_3x4
	.db 0
overworld_3x4:
	.db $15|imp,$16|imp,$15|imp,$15|imp,$16|imp,$15|imp
	.db $03,$03,$03,$03,$44,$16|imp
	.db $03,$03,$03,$03,$16|imp,$15|imp
	.db $16|imp,$03,$16|imp,$16|imp,$15|imp,$04|imp
	.dw 0
	.dw overworld_4x4
	.dw overworld_3x3
	.dw 0
	.db 0
overworld_4x0:
	.db $16|imp,$03,$15|imp,$16|imp,$03,$16|imp
	.db $15|imp,$03,$03,$16|imp,$03,$03
	.db $16|imp,$03,$03,$03,$03,$16|imp
	.db $04|imp,$16|imp,$16|imp,$16|imp,$16|imp,$04|imp
 	.dw overworld_3x0
	.dw 0
	.dw 0
	.dw overworld_4x1
	.db 0
overworld_4x1:
	.db $16|imp,$03,$03,$03,$03,$15|imp
	.db $03,$03,$03,$15|imp,$03,$03
	.db $16|imp,$03,$03,$03,$03,$16|imp
	.db $04|imp,$15|imp,$16|imp,$15|imp,$15|imp,$04|imp
  	.dw overworld_3x1
	.dw 0
	.dw overworld_4x0
	.dw overworld_4x2
	.db 0
overworld_4x2:
	.db $15|imp,$16|imp,$16|imp,$16|imp,$03,$15|imp
	.db $03,$03,$16|imp,$16|imp,$03,$16|imp
	.db $15|imp,$03,$03,$03,$03,$03
	.db $16|imp,$16|imp,$16|imp,$15|imp,$16|imp,$15|imp
 	.dw overworld_3x2
	.dw 0
	.dw overworld_4x1
	.dw overworld_4x3
	.db 0
overworld_4x3:
	.db $04|imp,$16|imp,$16|imp,$15|imp,$15|imp,$16|imp
	.db $16|imp,$03,$03,$03,$03,$45
	.db $03,$03,$03,$03,$16|imp,$15|imp
	.db $15|imp,$16|imp,$15|imp,$16|imp,$04|imp,$15|imp
 	.dw 0
	.dw 0
	.dw overworld_4x2
	.dw overworld_4x4
	.db 0
overworld_4x4:
	.db $15|imp,$03,$16|imp,$15|imp,$16|imp,$04|imp
	.db $45,$03,$03,$03,$03,$16|imp 
	.db $16|imp,$03,$16|imp,$03,$16|imp,$04|imp 
	.db $15|imp,$16|imp,$15|imp,$16|imp,$15|imp,$16|imp
 	.dw overworld_3x4
	.dw 0
	.dw overworld_4x3
	.dw 0
	.db 0
