target extended-remote localhost:3333
symbol-file build/mp1corea7/medium/main.elf
restore build/main.uimg binary 0xC0000000
set *((unsigned int)0x5C00A118) = 0xC0000000
print "Image loaded. Type `continue` to boot."
