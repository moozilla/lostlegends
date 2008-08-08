;data for map entries should go here... after the mapget routine
;naming convention: row x column
;next nodes: up,down,left,right
;null places where you shouldn't be able to move through, ie walls

living_forest_0x0:
	.db $09|imp,$09|imp,$3B,$09|imp,$09|imp,$09|imp
	.db $09|imp,$06,$06,$06,$06,$06
	.db $09|imp,$06,$06,$06,$06,$06
	.db $46|imp,$09|imp,$09|imp,$09|imp,$09|imp,$09|imp
	.dw 0
	.dw 0
	.dw 0
	.dw living_forest_0x1
	.db 1
;portal to 1x0
	.db obj_portal, $20, $11, $05	
living_forest_0x1:
	.db $09|imp,$09|imp,$09|imp,$46|imp,$46|imp,$46|imp
	.db $06,$1C|imp,$1C|imp,$09|imp,$09|imp,$09|imp
	.db $06,$06,$06,$06,$06,$06
	.db $09|imp,$09|imp,$09|imp,$09|imp,$09|imp,$09|imp
	.dw 0
	.dw 0
	.dw living_forest_0x0
	.dw living_forest_0x2
	.db 0	
living_forest_0x2:
	.db $09|imp,$0C|imp,$0D|imp,$0D|imp,$0C|imp,$09|imp
	.db $09|imp,$0D|imp,$2A|imp,$2B|imp,$0D|imp,$09|imp
	.db $02,$02,$02,$02,$02,$02
	.db $09|imp,$09|imp,$09|imp,$09|imp,$09|imp,$09|imp
	.dw 0
	.dw 0
	.dw living_forest_0x1
	.dw 0
	.db 1
;reverse portal object to gaia overworld
	.db obj_reverseportal, $21
	.dw overworld_2x2	
living_forest_1x0:
	.db $14|imp,$3C,$14|imp,$12|imp,$12|imp,$12|imp
	.db $14|imp,$01,$00,$11|imp,$00,$12|imp
	.db $12|imp,$01,$11|imp,$00,$01,$01
	.db $12|imp,$01,$11|imp,$12|imp,$01,$12|imp
	.dw 0
	.dw living_forest_2x0
	.dw 0
	.dw living_forest_1x1
	.db 1
;portal to 0x0
	.db obj_portal, $10, $21, $06		
living_forest_1x1:
	.db $12|imp,$12|imp,$12|imp,$12|imp,$12|imp,$12|imp
	.db $11|imp,$12|imp,$01,$01,$11|imp,$12|imp
	.db $01,$00,$00,$00,$01,$01
	.db $11|imp,$00,$00,$00,$12|imp,$11|imp
	.dw 0
	.dw living_forest_2x1
	.dw living_forest_1x0
	.dw living_forest_1x2
	.db 0	
living_forest_1x2:
	.db $12|imp,$11|imp,$12|imp,$11|imp,$12|imp,$01|imp
	.db $11|imp,$13,$01,$12|imp,$11|imp,$11|imp
	.db $01,$00,$13,$11|imp,$1B|imp,$12|imp
	.db $12|imp,$12|imp,$11|imp,$12|imp,$01,$11|imp
	.dw 0
	.dw living_forest_2x2
	.dw living_forest_1x1
	.dw 0
	.db 0	
living_forest_2x0:
	.db $11|imp,$00,$1B|imp,$11|imp,$01,$11|imp
	.db $12|imp,$01,$11|imp,$12|imp,$01,$12|imp
	.db $12|imp,$00,$11|imp,$12|imp,$00,$11|imp
	.db $11|imp,$01,$12|imp,$12|imp,$00,$12|imp
	.dw living_forest_1x0
	.dw living_forest_3x0
	.dw 0
	.dw 0
	.db 0	
living_forest_2x1:
	.db $12|imp,$01,$00,$00,$12|imp,$11|imp
	.db $11|imp,$12|imp,$00,$00,$01,$11|imp
	.db $01|imp,$11|imp,$00,$00,$00,$01
	.db $12|imp,$00,$00,$00,$00,$11|imp
	.dw living_forest_1x1
	.dw living_forest_3x1
	.dw 0
	.dw living_forest_2x2
	.db 0	
living_forest_2x2:
	.db $12|imp,$11|imp,$01|imp,$11|imp,$13,$11|imp
	.db $12|imp,$00,$12|imp,$11|imp,$01,$12|imp
	.db $00,$00,$11|imp,$01,$00,$12|imp
	.db $12|imp,$01,$11|imp,$12|imp,$00,$11|imp
	.dw living_forest_1x2
	.dw living_forest_3x2
	.dw living_forest_2x1
	.dw 0
	.db 0	
living_forest_3x0:
	.db $12|imp,$01,$11|imp,$11|imp,$00,$12|imp
	.db $12|imp,$01,$12|imp,$11|imp,$00,$11|imp
	.db $12|imp,$00,$01,$01,$01,$12|imp
	.db $11|imp,$12|imp,$11|imp,$12|imp,$12|imp,$12|imp
	.dw living_forest_2x0
	.dw 0
	.dw 0
	.dw 0
	.db 0	
living_forest_3x1:
	.db $11|imp,$01,$00,$00,$01,$12|imp
	.db $12|imp,$01,$00,$00,$00,$11|imp
	.db $11|imp,$12|imp,$00,$00,$11|imp,$12|imp
	.db $12|imp,$12|imp,$01,$01,$12|imp,$12|imp
	.dw living_forest_2x1
	.dw 0
	.dw 0
	.dw 0
	.db 1
;reverse portal object to gaia overworld
	.db obj_reverseportal, $21
	.dw overworld_2x2		
living_forest_3x2:
	.db $11|imp,$01,$12|imp,$11|imp,$00,$11|imp
	.db $12|imp,$01,$11|imp,$00,$00,$12|imp
	.db $12|imp,$00,$01,$00,$01,$12|imp
	.db $11|imp,$11|imp,$12|imp,$11|imp,$12|imp,$11|imp
	.dw living_forest_2x2
	.dw 0
	.dw 0
	.dw 0
	.db 0	


