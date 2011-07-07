;(p)Dimadze


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
  DCD 0xA02A832C+1

  PUBLIC  OldOnCreate
OldOnCreate:
  DCD 0xA02A80FE+1 ; OldOnClose - 0x22E

  PUBLIC  ESIOld
ESIOld:
  DCD 0xA0A1CE60+1 
      
  PUBLIC  PITgetN
PITgetN:
  DCD 0xA07B862A+1
  
  PUBLIC  PITret
PITret:
  DCD 0xA07B8640+1
        
  PUBLIC  KEYBret
KEYBret:
  DCD 0xA07AD556+1

  PUBLIC  KEYBfunc
KEYBfunc:
  DCD 0xA07BA56C+1

  PUBLIC  FReadOld
FReadOld:
  DCD 0xA0786578

  PUBLIC  FWriteOld
FWriteOld:
  DCD 0xA0786660   ;FReadOld + 0xE8
        
  defadr  StoreErrInfoAndAbort,0xA07870A0
  defadr  StoreErrString,0xA0786EC4

  END