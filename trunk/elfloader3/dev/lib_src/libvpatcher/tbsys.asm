

                  RSEG	CODE:CODE
                  CODE32

                  PUBLIC  EnableINT

        EnableINT:

                   MRS     R1, CPSR
                   BIC     R1, R1, #0xC0
                   ORR     R1, R1, R0
                   AND     R2, R1, #0x1F
                   CMP     R2, #0x10


#ifndef NEWSGOLD
	           SWIEQ   0x04
#else
	           SWIEQ   0x00
#endif

                   MSR     CPSR_c, R1
                   BX      LR

                   //Выключает прерывания
                   PUBLIC  DisableINT

        DisableINT:

                   MRS     R0, CPSR
                   ORR     R1, R0, #0xC0
                   AND     R2, R0, #0x1F
                   CMP     R2, #0x10

                   SWIEQ   0x04

#ifndef NEWSGOLD
	           SWIEQ   0x04
#else
	           SWIEQ   0x00
#endif

                   MSR     CPSR_c, R1
                   AND     R0, R0, #0xC0
                   BX      LR





                  PUBLIC   CleanTLB

        CleanTLB:

                  STR	   LR, [SP, #-0x04]!
	          MRS	   R2,  CPSR

	          SWI      0x04


#ifndef NEWSGOLD
	          SWI      0x04
#else
	          SWI      0x00
#endif


      CTLB_loop:


                  MRC      p15, 0, PC, c7, c14, 3
                  BNE      CTLB_loop

	          MCR      p15, 0, R0, c7, c10, 4
	          MCR      p15, 0, R0, c7, c5, 0
	          MCR      p15, 0, R0, c8, c7, 0


                  MRS      R1, CPSR
	          ORR      R1, R1,#0xC0
	          MSR      CPSR_c, R1

	          MSR	   CPSR_c,R2
	          LDR	   PC,[SP], #+0x04



                  PUBLIC   CleanCache

     CleanCache:

      	          STR	   LR, [SP, #-0x04]!
      	          MRS	   R2,  CPSR

      	          SWI      0x04


#ifndef NEWSGOLD
	          SWI      0x04
#else
	          SWI      0x00
#endif


                  MOV      R1, #0x00
                  MCR      p15, 0, R1, c8, c7, 0

	          MCR      p15, 0, R0, c7, c10, 1
	          MCR      p15, 0, R0, c7, c10, 4
	          MCR      p15, 0, R0, c7, c5, 1
                  MCR      p15, 0, R0, c7, c13, 1

                  MCR      p15, 0, R0, c8, c7, 1


	          MRS      R1, CPSR
	          ORR      R1, R1,#0xC0
	          MSR      CPSR_c, R1

	          MSR	   CPSR_c,R2
	          LDR	   PC,[SP], #+0x04



                  PUBLIC   GetTBaseAddr

     GetTBaseAddr:

 	          STR	   LR, [SP, #-0x04]!
	          MRS	   R2,  CPSR

	          SWI      0x04


#ifndef NEWSGOLD
	          SWI      0x04
#else
	          SWI      0x00
#endif


	          MRS      R1, CPSR
	          ORR      R1, R1,#0xC0
	          MSR      CPSR_c, R1

                  MOV      R0, #0
                  MRC      p15, 0, R0, c2, c0

	          MSR	   CPSR_c,R2
	          LDR	   PC,[SP], #+0x04




                  PUBLIC   UnLockAccess

   UnLockAccess:

 	          STR	   LR, [SP, #-0x04]!
	          MRS	   R2,  CPSR


	          SWI      0x04


#ifndef NEWSGOLD
	          SWI      0x04
#else
	          SWI      0x00
#endif

	          MRS      R1, CPSR
	          ORR      R1, R1,#0xC0
	          MSR      CPSR_c, R1

                  MOV      R0, #0xFFFFFFFF
                  MCR      p15, 0, R0, c3, c0

                  NOP
	          NOP
	          NOP
	          NOP
                  NOP
	          NOP
	          NOP
	          NOP
                  NOP
	          NOP
	          NOP
	          NOP
                  NOP
	          NOP
	          NOP
	          NOP

	          MSR	   CPSR_c,R2
	          LDR	   PC,[SP], #+0x04




                END
