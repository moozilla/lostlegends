
;MAPPER.z80
;===========================================
#define mapRam appbackupscreen
#define camY mapRam
#define camX camY + 1
#define mapBase camX + 1
#define mapWidth mapBase + 2
#define mapRamEnd mapWidth + 1