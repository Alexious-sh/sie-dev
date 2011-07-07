  RSEG  DATA_N
  RSEG  SWILIB_FUNC1B8_1BB:CODE
  EXTERN  EXT2_AREA
  EXTERN  pngtop
  EXTERN  pLIB_TOP
  DCD EXT2_AREA
  DCD pngtop
  DCD pLIB_TOP
  DCD SFE(DATA_N)


defadr  MACRO a,b
  PUBLIC  a
a EQU b
  ENDM

        RSEG  CODE:CODE
 
  PUBLIC  OldOnClose
OldOnClose:
  DCD 0xA02A7A68+1

  PUBLIC  OldOnCreate
OldOnCreate:
  DCD 0xA02A781C+1

  PUBLIC  ESIOld
ESIOld:
  DCD 0xA0A85B50+1
      
  PUBLIC  PITgetN
PITgetN:
  DCD 0xA0801A52+1
  
  PUBLIC  PITret
PITret:
  DCD 0xA0801A68+1
        
  PUBLIC  KEYBret
KEYBret:
  DCD 0xA07F697E+1

  PUBLIC  KEYBfunc
KEYBfunc:
  DCD 0xA0803994+1

  PUBLIC  FReadOld
FReadOld:
  DCD 0xA07CC5F0

  PUBLIC  FWriteOld
FWriteOld:
  DCD 0xA07CC6D8
        
  defadr  StoreErrInfoAndAbort,0xA07CD184
  defadr  StoreErrString,0xA07CCFA8

  END