;defines codes for each object type
;FIRST BYTE WILL ALWAYS BE IDENTIFIER!

#define obj_reverseportal $01
;byte 2:  x 4bits : y 4 bits -> location on the new node to place character
;bytes 3-4: node pointer

#define obj_portal $02
;byte 2: x 4bits : y 4 bits -> location on the CURRENT NODE where the actual portal is
;byte 3: x 4bits : up/down 1 bit : y 3 bits -> Location on the DESTINATION node where portal leads
;byte 4: left/right 1 bit: 7 bits index for a table of Node Pointers
;NOTE: SET = up or left, RES = down or right, the flag in byte 4 simply says which axis: set = l/r, res = u/d
