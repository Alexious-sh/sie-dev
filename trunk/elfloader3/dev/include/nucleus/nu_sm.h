
extern void NU_SUPERVISOR_MODE_ASM(void *ct);
extern void NU_USER_MODE_ASM();

#define NU_IS_SUPERVISOR_MODE() (NU_TRUE)
#define NU_SUPERV_USER_VARIABLES  extern VOID *TCD_Current_Thread; \

#define NU_SUPERVISOR_MODE() NU_SUPERVISOR_MODE_ASM(TCD_Current_Thread)
#define NU_USER_MODE() NU_USER_MODE_ASM()
