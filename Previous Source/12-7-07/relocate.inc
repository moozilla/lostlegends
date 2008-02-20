#macro relocate(new_location)
 #ifdef old_location
   .echo "Error line ",__line,": You cannot nest relocate blocks."
 #else
   #define old_location eval($)
   .org new_location
 #endif
#endmacro

#macro endrelocate()
 #ifdef new_location
   .org $-new_location + old_location
   #undefine new_location
   #undefine old_location
 #else
   .echo "Error line ",__line,": No relocate statements corresponds to this endrelocate."
 #endif
#endmacro