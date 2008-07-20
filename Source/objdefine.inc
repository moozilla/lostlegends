;defines codes for each object type
;FIRST BYTE WILL ALWAYS BE IDENTIFIER!

#define obj_reverseportal $01
;byte 2:  x 4bits : y 4 bits -> location on the new node to place character
;bytes 3-4: node pointer
