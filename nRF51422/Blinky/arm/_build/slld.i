#line 1 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"
 































 

#line 1 "C:\\Keil\\ARM\\ARMCC\\bin\\..\\include\\stdio.h"
 
 
 





 






 













#line 38 "C:\\Keil\\ARM\\ARMCC\\bin\\..\\include\\stdio.h"


  
  typedef unsigned int size_t;    








 
 

 
  typedef struct __va_list __va_list;





   




 




typedef struct __fpos_t_struct {
    unsigned __int64 __pos;
    



 
    struct {
        unsigned int __state1, __state2;
    } __mbstate;
} fpos_t;
   


 


   

 

typedef struct __FILE FILE;
   






 

extern FILE __stdin, __stdout, __stderr;
extern FILE *__aeabi_stdin, *__aeabi_stdout, *__aeabi_stderr;

#line 129 "C:\\Keil\\ARM\\ARMCC\\bin\\..\\include\\stdio.h"
    

    

    





     



   


 


   


 

   



 

   


 




   


 





    


 






extern __declspec(__nothrow) int remove(const char *  ) __attribute__((__nonnull__(1)));
   





 
extern __declspec(__nothrow) int rename(const char *  , const char *  ) __attribute__((__nonnull__(1,2)));
   








 
extern __declspec(__nothrow) FILE *tmpfile(void);
   




 
extern __declspec(__nothrow) char *tmpnam(char *  );
   











 

extern __declspec(__nothrow) int fclose(FILE *  ) __attribute__((__nonnull__(1)));
   







 
extern __declspec(__nothrow) int fflush(FILE *  );
   







 
extern __declspec(__nothrow) FILE *fopen(const char * __restrict  ,
                           const char * __restrict  ) __attribute__((__nonnull__(1,2)));
   








































 
extern __declspec(__nothrow) FILE *freopen(const char * __restrict  ,
                    const char * __restrict  ,
                    FILE * __restrict  ) __attribute__((__nonnull__(2,3)));
   








 
extern __declspec(__nothrow) void setbuf(FILE * __restrict  ,
                    char * __restrict  ) __attribute__((__nonnull__(1)));
   




 
extern __declspec(__nothrow) int setvbuf(FILE * __restrict  ,
                   char * __restrict  ,
                   int  , size_t  ) __attribute__((__nonnull__(1)));
   















 
#pragma __printf_args
extern __declspec(__nothrow) int fprintf(FILE * __restrict  ,
                    const char * __restrict  , ...) __attribute__((__nonnull__(1,2)));
   


















 
#pragma __printf_args
extern __declspec(__nothrow) int _fprintf(FILE * __restrict  ,
                     const char * __restrict  , ...) __attribute__((__nonnull__(1,2)));
   



 
#pragma __printf_args
extern __declspec(__nothrow) int printf(const char * __restrict  , ...) __attribute__((__nonnull__(1)));
   




 
#pragma __printf_args
extern __declspec(__nothrow) int _printf(const char * __restrict  , ...) __attribute__((__nonnull__(1)));
   



 
#pragma __printf_args
extern __declspec(__nothrow) int sprintf(char * __restrict  , const char * __restrict  , ...) __attribute__((__nonnull__(1,2)));
   






 
#pragma __printf_args
extern __declspec(__nothrow) int _sprintf(char * __restrict  , const char * __restrict  , ...) __attribute__((__nonnull__(1,2)));
   



 

#pragma __printf_args
extern __declspec(__nothrow) int snprintf(char * __restrict  , size_t  ,
                     const char * __restrict  , ...) __attribute__((__nonnull__(3)));
   















 

#pragma __printf_args
extern __declspec(__nothrow) int _snprintf(char * __restrict  , size_t  ,
                      const char * __restrict  , ...) __attribute__((__nonnull__(3)));
   



 
#pragma __scanf_args
extern __declspec(__nothrow) int fscanf(FILE * __restrict  ,
                    const char * __restrict  , ...) __attribute__((__nonnull__(1,2)));
   






























 
#pragma __scanf_args
extern __declspec(__nothrow) int _fscanf(FILE * __restrict  ,
                     const char * __restrict  , ...) __attribute__((__nonnull__(1,2)));
   



 
#pragma __scanf_args
extern __declspec(__nothrow) int scanf(const char * __restrict  , ...) __attribute__((__nonnull__(1)));
   






 
#pragma __scanf_args
extern __declspec(__nothrow) int _scanf(const char * __restrict  , ...) __attribute__((__nonnull__(1)));
   



 
#pragma __scanf_args
extern __declspec(__nothrow) int sscanf(const char * __restrict  ,
                    const char * __restrict  , ...) __attribute__((__nonnull__(1,2)));
   








 
#pragma __scanf_args
extern __declspec(__nothrow) int _sscanf(const char * __restrict  ,
                     const char * __restrict  , ...) __attribute__((__nonnull__(1,2)));
   



 

 
extern __declspec(__nothrow) int vfscanf(FILE * __restrict  , const char * __restrict  , __va_list) __attribute__((__nonnull__(1,2)));
extern __declspec(__nothrow) int vscanf(const char * __restrict  , __va_list) __attribute__((__nonnull__(1)));
extern __declspec(__nothrow) int vsscanf(const char * __restrict  , const char * __restrict  , __va_list) __attribute__((__nonnull__(1,2)));

extern __declspec(__nothrow) int _vfscanf(FILE * __restrict  , const char * __restrict  , __va_list) __attribute__((__nonnull__(1,2)));
extern __declspec(__nothrow) int _vscanf(const char * __restrict  , __va_list) __attribute__((__nonnull__(1)));
extern __declspec(__nothrow) int _vsscanf(const char * __restrict  , const char * __restrict  , __va_list) __attribute__((__nonnull__(1,2)));

extern __declspec(__nothrow) int vprintf(const char * __restrict  , __va_list  ) __attribute__((__nonnull__(1)));
   





 
extern __declspec(__nothrow) int _vprintf(const char * __restrict  , __va_list  ) __attribute__((__nonnull__(1)));
   



 
extern __declspec(__nothrow) int vfprintf(FILE * __restrict  ,
                    const char * __restrict  , __va_list  ) __attribute__((__nonnull__(1,2)));
   






 
extern __declspec(__nothrow) int vsprintf(char * __restrict  ,
                     const char * __restrict  , __va_list  ) __attribute__((__nonnull__(1,2)));
   






 

extern __declspec(__nothrow) int vsnprintf(char * __restrict  , size_t  ,
                     const char * __restrict  , __va_list  ) __attribute__((__nonnull__(3)));
   







 

extern __declspec(__nothrow) int _vsprintf(char * __restrict  ,
                      const char * __restrict  , __va_list  ) __attribute__((__nonnull__(1,2)));
   



 
extern __declspec(__nothrow) int _vfprintf(FILE * __restrict  ,
                     const char * __restrict  , __va_list  ) __attribute__((__nonnull__(1,2)));
   



 
extern __declspec(__nothrow) int _vsnprintf(char * __restrict  , size_t  ,
                      const char * __restrict  , __va_list  ) __attribute__((__nonnull__(3)));
   



 
extern __declspec(__nothrow) int fgetc(FILE *  ) __attribute__((__nonnull__(1)));
   







 
extern __declspec(__nothrow) char *fgets(char * __restrict  , int  ,
                    FILE * __restrict  ) __attribute__((__nonnull__(1,3)));
   










 
extern __declspec(__nothrow) int fputc(int  , FILE *  ) __attribute__((__nonnull__(2)));
   







 
extern __declspec(__nothrow) int fputs(const char * __restrict  , FILE * __restrict  ) __attribute__((__nonnull__(1,2)));
   




 
extern __declspec(__nothrow) int getc(FILE *  ) __attribute__((__nonnull__(1)));
   







 




    extern __declspec(__nothrow) int (getchar)(void);

   





 
extern __declspec(__nothrow) char *gets(char *  ) __attribute__((__nonnull__(1)));
   









 
extern __declspec(__nothrow) int putc(int  , FILE *  ) __attribute__((__nonnull__(2)));
   





 




    extern __declspec(__nothrow) int (putchar)(int  );

   



 
extern __declspec(__nothrow) int puts(const char *  ) __attribute__((__nonnull__(1)));
   





 
extern __declspec(__nothrow) int ungetc(int  , FILE *  ) __attribute__((__nonnull__(2)));
   






















 

extern __declspec(__nothrow) size_t fread(void * __restrict  ,
                    size_t  , size_t  , FILE * __restrict  ) __attribute__((__nonnull__(1,4)));
   











 

extern __declspec(__nothrow) size_t __fread_bytes_avail(void * __restrict  ,
                    size_t  , FILE * __restrict  ) __attribute__((__nonnull__(1,3)));
   











 

extern __declspec(__nothrow) size_t fwrite(const void * __restrict  ,
                    size_t  , size_t  , FILE * __restrict  ) __attribute__((__nonnull__(1,4)));
   







 

extern __declspec(__nothrow) int fgetpos(FILE * __restrict  , fpos_t * __restrict  ) __attribute__((__nonnull__(1,2)));
   








 
extern __declspec(__nothrow) int fseek(FILE *  , long int  , int  ) __attribute__((__nonnull__(1)));
   














 
extern __declspec(__nothrow) int fsetpos(FILE * __restrict  , const fpos_t * __restrict  ) __attribute__((__nonnull__(1,2)));
   










 
extern __declspec(__nothrow) long int ftell(FILE *  ) __attribute__((__nonnull__(1)));
   











 
extern __declspec(__nothrow) void rewind(FILE *  ) __attribute__((__nonnull__(1)));
   





 

extern __declspec(__nothrow) void clearerr(FILE *  ) __attribute__((__nonnull__(1)));
   




 

extern __declspec(__nothrow) int feof(FILE *  ) __attribute__((__nonnull__(1)));
   


 
extern __declspec(__nothrow) int ferror(FILE *  ) __attribute__((__nonnull__(1)));
   


 
extern __declspec(__nothrow) void perror(const char *  );
   









 

extern __declspec(__nothrow) int _fisatty(FILE *   ) __attribute__((__nonnull__(1)));
    
 

extern __declspec(__nothrow) void __use_no_semihosting_swi(void);
extern __declspec(__nothrow) void __use_no_semihosting(void);
    





 











#line 948 "C:\\Keil\\ARM\\ARMCC\\bin\\..\\include\\stdio.h"



 

#line 36 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"


#line 1 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"
 































 








#line 1 "..\\Spansion_SLLD_v13.2.1\\src\\slld_targetspecific.h"
 































 













 



















 


 

 
 

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

 
 
 
 
 
 
 
 
 




 

 
 
 



 

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 









 





#line 196 "..\\Spansion_SLLD_v13.2.1\\src\\slld_targetspecific.h"







#line 1 "..\\..\\Include\\boards.h"










 
 
 


 
 



#line 1 "..\\..\\Include\\boards/ant_io_board_v_5.h"

 


 




#line 1 "..\\..\\Include\\nrf_gpio.h"



#line 1 "..\\..\\Include\\nrf51.h"

 








































 





 



 









 

typedef enum {
 
  Reset_IRQn                    = -15,               
  NonMaskableInt_IRQn           = -14,               
  HardFault_IRQn                = -13,               
  SVCall_IRQn                   =  -5,               
  DebugMonitor_IRQn             =  -4,               
  PendSV_IRQn                   =  -2,               
  SysTick_IRQn                  =  -1,               
 
  POWER_CLOCK_IRQn              =   0,               
  RADIO_IRQn                    =   1,               
  UART0_IRQn                    =   2,               
  SPI0_TWI0_IRQn                =   3,               
  SPI1_TWI1_IRQn                =   4,               
  GPIOTE_IRQn                   =   6,               
  ADC_IRQn                      =   7,               
  TIMER0_IRQn                   =   8,               
  TIMER1_IRQn                   =   9,               
  TIMER2_IRQn                   =  10,               
  RTC0_IRQn                     =  11,               
  TEMP_IRQn                     =  12,               
  RNG_IRQn                      =  13,               
  ECB_IRQn                      =  14,               
  CCM_AAR_IRQn                  =  15,               
  WDT_IRQn                      =  16,               
  RTC1_IRQn                     =  17,               
  QDEC_IRQn                     =  18,               
  LPCOMP_IRQn                   =  19,               
  SWI0_IRQn                     =  20,               
  SWI1_IRQn                     =  21,               
  SWI2_IRQn                     =  22,               
  SWI3_IRQn                     =  23,               
  SWI4_IRQn                     =  24,               
  SWI5_IRQn                     =  25                
} IRQn_Type;




 


 
 
 

 




   

#line 1 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cm0.h"
 







 

























 
























 




 


 

 













#line 110 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cm0.h"


 







#line 145 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cm0.h"

#line 1 "C:\\Keil\\ARM\\ARMCC\\bin\\..\\include\\stdint.h"
 
 





 










#line 26 "C:\\Keil\\ARM\\ARMCC\\bin\\..\\include\\stdint.h"







 

     

     
typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;
typedef   signed       __int64 int64_t;

     
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;
typedef unsigned       __int64 uint64_t;

     

     
     
typedef   signed          char int_least8_t;
typedef   signed short     int int_least16_t;
typedef   signed           int int_least32_t;
typedef   signed       __int64 int_least64_t;

     
typedef unsigned          char uint_least8_t;
typedef unsigned short     int uint_least16_t;
typedef unsigned           int uint_least32_t;
typedef unsigned       __int64 uint_least64_t;

     

     
typedef   signed           int int_fast8_t;
typedef   signed           int int_fast16_t;
typedef   signed           int int_fast32_t;
typedef   signed       __int64 int_fast64_t;

     
typedef unsigned           int uint_fast8_t;
typedef unsigned           int uint_fast16_t;
typedef unsigned           int uint_fast32_t;
typedef unsigned       __int64 uint_fast64_t;

     
typedef   signed           int intptr_t;
typedef unsigned           int uintptr_t;

     
typedef   signed       __int64 intmax_t;
typedef unsigned       __int64 uintmax_t;




     

     





     





     





     

     





     





     





     

     





     





     





     

     


     


     


     

     


     


     


     

     



     



     


     
    
 



#line 197 "C:\\Keil\\ARM\\ARMCC\\bin\\..\\include\\stdint.h"

     







     










     











#line 261 "C:\\Keil\\ARM\\ARMCC\\bin\\..\\include\\stdint.h"



 



#line 147 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cm0.h"
#line 1 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cmInstr.h"
 







 

























 






 



 


 









 







 







 






 








 







 







 









 









 

__attribute__((section(".rev16_text"))) static __inline __asm uint32_t __REV16(uint32_t value)
{
  rev16 r0, r0
  bx lr
}








 

__attribute__((section(".revsh_text"))) static __inline __asm int32_t __REVSH(int32_t value)
{
  revsh r0, r0
  bx lr
}










 










 



#line 292 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cmInstr.h"


#line 684 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cmInstr.h"

   

#line 148 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cm0.h"
#line 1 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cmFunc.h"
 







 

























 






 



 


 





 
 






 
static __inline uint32_t __get_CONTROL(void)
{
  register uint32_t __regControl         __asm("control");
  return(__regControl);
}







 
static __inline void __set_CONTROL(uint32_t control)
{
  register uint32_t __regControl         __asm("control");
  __regControl = control;
}







 
static __inline uint32_t __get_IPSR(void)
{
  register uint32_t __regIPSR          __asm("ipsr");
  return(__regIPSR);
}







 
static __inline uint32_t __get_APSR(void)
{
  register uint32_t __regAPSR          __asm("apsr");
  return(__regAPSR);
}







 
static __inline uint32_t __get_xPSR(void)
{
  register uint32_t __regXPSR          __asm("xpsr");
  return(__regXPSR);
}







 
static __inline uint32_t __get_PSP(void)
{
  register uint32_t __regProcessStackPointer  __asm("psp");
  return(__regProcessStackPointer);
}







 
static __inline void __set_PSP(uint32_t topOfProcStack)
{
  register uint32_t __regProcessStackPointer  __asm("psp");
  __regProcessStackPointer = topOfProcStack;
}







 
static __inline uint32_t __get_MSP(void)
{
  register uint32_t __regMainStackPointer     __asm("msp");
  return(__regMainStackPointer);
}







 
static __inline void __set_MSP(uint32_t topOfMainStack)
{
  register uint32_t __regMainStackPointer     __asm("msp");
  __regMainStackPointer = topOfMainStack;
}







 
static __inline uint32_t __get_PRIMASK(void)
{
  register uint32_t __regPriMask         __asm("primask");
  return(__regPriMask);
}







 
static __inline void __set_PRIMASK(uint32_t priMask)
{
  register uint32_t __regPriMask         __asm("primask");
  __regPriMask = (priMask);
}


#line 271 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cmFunc.h"


#line 307 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cmFunc.h"


#line 634 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cmFunc.h"

 

#line 149 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cm0.h"








 
#line 174 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cm0.h"

 






 
#line 190 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cm0.h"

 










 


 





 


 
typedef union
{
  struct
  {

    uint32_t _reserved0:27;               





    uint32_t Q:1;                         
    uint32_t V:1;                         
    uint32_t C:1;                         
    uint32_t Z:1;                         
    uint32_t N:1;                         
  } b;                                    
  uint32_t w;                             
} APSR_Type;



 
typedef union
{
  struct
  {
    uint32_t ISR:9;                       
    uint32_t _reserved0:23;               
  } b;                                    
  uint32_t w;                             
} IPSR_Type;



 
typedef union
{
  struct
  {
    uint32_t ISR:9;                       

    uint32_t _reserved0:15;               





    uint32_t T:1;                         
    uint32_t IT:2;                        
    uint32_t Q:1;                         
    uint32_t V:1;                         
    uint32_t C:1;                         
    uint32_t Z:1;                         
    uint32_t N:1;                         
  } b;                                    
  uint32_t w;                             
} xPSR_Type;



 
typedef union
{
  struct
  {
    uint32_t nPRIV:1;                     
    uint32_t SPSEL:1;                     
    uint32_t FPCA:1;                      
    uint32_t _reserved0:29;               
  } b;                                    
  uint32_t w;                             
} CONTROL_Type;

 






 


 
typedef struct
{
  volatile uint32_t ISER[1];                  
       uint32_t RESERVED0[31];
  volatile uint32_t ICER[1];                  
       uint32_t RSERVED1[31];
  volatile uint32_t ISPR[1];                  
       uint32_t RESERVED2[31];
  volatile uint32_t ICPR[1];                  
       uint32_t RESERVED3[31];
       uint32_t RESERVED4[64];
  volatile uint32_t IP[8];                    
}  NVIC_Type;

 






 


 
typedef struct
{
  volatile const  uint32_t CPUID;                    
  volatile uint32_t ICSR;                     
       uint32_t RESERVED0;
  volatile uint32_t AIRCR;                    
  volatile uint32_t SCR;                      
  volatile uint32_t CCR;                      
       uint32_t RESERVED1;
  volatile uint32_t SHP[2];                   
  volatile uint32_t SHCSR;                    
} SCB_Type;

 















 



























 















 









 






 



 






 


 
typedef struct
{
  volatile uint32_t CTRL;                     
  volatile uint32_t LOAD;                     
  volatile uint32_t VAL;                      
  volatile const  uint32_t CALIB;                    
} SysTick_Type;

 












 



 



 









 








 
 






 

 










 









 

 



 




 

 
 










 
static __inline void NVIC_EnableIRQ(IRQn_Type IRQn)
{
  ((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->ISER[0] = (1 << ((uint32_t)(IRQn) & 0x1F));
}







 
static __inline void NVIC_DisableIRQ(IRQn_Type IRQn)
{
  ((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->ICER[0] = (1 << ((uint32_t)(IRQn) & 0x1F));
}











 
static __inline uint32_t NVIC_GetPendingIRQ(IRQn_Type IRQn)
{
  return((uint32_t) ((((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->ISPR[0] & (1 << ((uint32_t)(IRQn) & 0x1F)))?1:0));
}







 
static __inline void NVIC_SetPendingIRQ(IRQn_Type IRQn)
{
  ((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->ISPR[0] = (1 << ((uint32_t)(IRQn) & 0x1F));
}







 
static __inline void NVIC_ClearPendingIRQ(IRQn_Type IRQn)
{
  ((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->ICPR[0] = (1 << ((uint32_t)(IRQn) & 0x1F));  
}










 
static __inline void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
{
  if(IRQn < 0) {
    ((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->SHP[( ((((uint32_t)(IRQn) & 0x0F)-8) >> 2) )] = (((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->SHP[( ((((uint32_t)(IRQn) & 0x0F)-8) >> 2) )] & ~(0xFF << ( (((uint32_t)(IRQn) ) & 0x03) * 8 ))) |
        (((priority << (8 - 2)) & 0xFF) << ( (((uint32_t)(IRQn) ) & 0x03) * 8 )); }
  else {
    ((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->IP[( ((uint32_t)(IRQn) >> 2) )] = (((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->IP[( ((uint32_t)(IRQn) >> 2) )] & ~(0xFF << ( (((uint32_t)(IRQn) ) & 0x03) * 8 ))) |
        (((priority << (8 - 2)) & 0xFF) << ( (((uint32_t)(IRQn) ) & 0x03) * 8 )); }
}












 
static __inline uint32_t NVIC_GetPriority(IRQn_Type IRQn)
{

  if(IRQn < 0) {
    return((uint32_t)(((((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->SHP[( ((((uint32_t)(IRQn) & 0x0F)-8) >> 2) )] >> ( (((uint32_t)(IRQn) ) & 0x03) * 8 ) ) & 0xFF) >> (8 - 2)));  }  
  else {
    return((uint32_t)(((((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->IP[ ( ((uint32_t)(IRQn) >> 2) )] >> ( (((uint32_t)(IRQn) ) & 0x03) * 8 ) ) & 0xFF) >> (8 - 2)));  }  
}





 
static __inline void NVIC_SystemReset(void)
{
  __dsb(0xF);                                                     
 
  ((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->AIRCR  = ((0x5FA << 16)      |
                 (1UL << 2));
  __dsb(0xF);                                                      
  while(1);                                                     
}

 



 




 

















 
static __inline uint32_t SysTick_Config(uint32_t ticks)
{
  if ((ticks - 1) > (0xFFFFFFUL << 0))  return (1);       

  ((SysTick_Type *) ((0xE000E000UL) + 0x0010UL) )->LOAD  = ticks - 1;                                   
  NVIC_SetPriority (SysTick_IRQn, (1<<2) - 1);   
  ((SysTick_Type *) ((0xE000E000UL) + 0x0010UL) )->VAL   = 0;                                           
  ((SysTick_Type *) ((0xE000E000UL) + 0x0010UL) )->CTRL  = (1UL << 2) |
                   (1UL << 1)   |
                   (1UL << 0);                     
  return (0);                                                   
}



 








#line 120 "..\\..\\Include\\nrf51.h"
#line 1 "..\\..\\Include\\system_nrf51.h"




























 







#line 38 "..\\..\\Include\\system_nrf51.h"


extern uint32_t SystemCoreClock;     









 
extern void SystemInit (void);









 
extern void SystemCoreClockUpdate (void);





#line 121 "..\\..\\Include\\nrf51.h"


 
 
 




 


 

  #pragma push
  #pragma anon_unions
#line 148 "..\\..\\Include\\nrf51.h"


typedef struct {
  volatile uint32_t  CPU0;                               
  volatile uint32_t  SPIS1;                              
  volatile uint32_t  RADIO;                              
  volatile uint32_t  ECB;                                
  volatile uint32_t  CCM;                                
  volatile uint32_t  AAR;                                
} AMLI_RAMPRI_Type;

typedef struct {
  volatile  uint32_t  EN;                                 
  volatile  uint32_t  DIS;                                
} PPI_TASKS_CHG_Type;

typedef struct {
  volatile uint32_t  EEP;                                
  volatile uint32_t  TEP;                                
} PPI_CH_Type;


 
 
 




 

typedef struct {                                     
  volatile const  uint32_t  RESERVED0[30];
  volatile  uint32_t  TASKS_CONSTLAT;                     
  volatile  uint32_t  TASKS_LOWPWR;                       
  volatile const  uint32_t  RESERVED1[34];
  volatile uint32_t  EVENTS_POFWARN;                     
  volatile const  uint32_t  RESERVED2[126];
  volatile uint32_t  INTENSET;                           
  volatile uint32_t  INTENCLR;                           
  volatile const  uint32_t  RESERVED3[61];
  volatile uint32_t  RESETREAS;                          
  volatile const  uint32_t  RESERVED4[9];
  volatile const  uint32_t  RAMSTATUS;                          
  volatile const  uint32_t  RESERVED5[53];
  volatile  uint32_t  SYSTEMOFF;                          
  volatile const  uint32_t  RESERVED6[3];
  volatile uint32_t  POFCON;                             
  volatile const  uint32_t  RESERVED7[2];
  volatile uint32_t  GPREGRET;                          
 
  volatile const  uint32_t  RESERVED8;
  volatile uint32_t  RAMON;                              
  volatile const  uint32_t  RESERVED9[7];
  volatile uint32_t  RESET;                             
 
  volatile const  uint32_t  RESERVED10[3];
  volatile uint32_t  RAMONB;                             
  volatile const  uint32_t  RESERVED11[8];
  volatile uint32_t  DCDCEN;                             
  volatile const  uint32_t  RESERVED12[291];
  volatile uint32_t  DCDCFORCE;                          
} NRF_POWER_Type;


 
 
 




 

typedef struct {                                     
  volatile  uint32_t  TASKS_HFCLKSTART;                   
  volatile  uint32_t  TASKS_HFCLKSTOP;                    
  volatile  uint32_t  TASKS_LFCLKSTART;                   
  volatile  uint32_t  TASKS_LFCLKSTOP;                    
  volatile  uint32_t  TASKS_CAL;                          
  volatile  uint32_t  TASKS_CTSTART;                      
  volatile  uint32_t  TASKS_CTSTOP;                       
  volatile const  uint32_t  RESERVED0[57];
  volatile uint32_t  EVENTS_HFCLKSTARTED;                
  volatile uint32_t  EVENTS_LFCLKSTARTED;                
  volatile const  uint32_t  RESERVED1;
  volatile uint32_t  EVENTS_DONE;                        
  volatile uint32_t  EVENTS_CTTO;                        
  volatile const  uint32_t  RESERVED2[124];
  volatile uint32_t  INTENSET;                           
  volatile uint32_t  INTENCLR;                           
  volatile const  uint32_t  RESERVED3[63];
  volatile const  uint32_t  HFCLKRUN;                           
  volatile const  uint32_t  HFCLKSTAT;                          
  volatile const  uint32_t  RESERVED4;
  volatile const  uint32_t  LFCLKRUN;                           
  volatile const  uint32_t  LFCLKSTAT;                          
  volatile const  uint32_t  LFCLKSRCCOPY;                      
 
  volatile const  uint32_t  RESERVED5[62];
  volatile uint32_t  LFCLKSRC;                           
  volatile const  uint32_t  RESERVED6[7];
  volatile uint32_t  CTIV;                               
  volatile const  uint32_t  RESERVED7[5];
  volatile uint32_t  XTALFREQ;                           
} NRF_CLOCK_Type;


 
 
 




 

typedef struct {                                     
  volatile const  uint32_t  RESERVED0[330];
  volatile uint32_t  PERR0;                              
  volatile uint32_t  RLENR0;                             
  volatile const  uint32_t  RESERVED1[52];
  volatile uint32_t  PROTENSET0;                         
  volatile uint32_t  PROTENSET1;                         
  volatile uint32_t  DISABLEINDEBUG;                     
  volatile uint32_t  PROTBLOCKSIZE;                      
} NRF_MPU_Type;


 
 
 




 

typedef struct {                                     
  volatile const  uint32_t  RESERVED0[448];
  volatile uint32_t  REPLACEADDR[8];                     
  volatile const  uint32_t  RESERVED1[24];
  volatile uint32_t  PATCHADDR[8];                       
  volatile const  uint32_t  RESERVED2[24];
  volatile uint32_t  PATCHEN;                            
  volatile uint32_t  PATCHENSET;                         
  volatile uint32_t  PATCHENCLR;                         
} NRF_PU_Type;


 
 
 




 

typedef struct {                                     
  volatile const  uint32_t  RESERVED0[896];
  AMLI_RAMPRI_Type RAMPRI;                           
} NRF_AMLI_Type;


 
 
 




 

typedef struct {                                     
  volatile  uint32_t  TASKS_TXEN;                         
  volatile  uint32_t  TASKS_RXEN;                         
  volatile  uint32_t  TASKS_START;                        
  volatile  uint32_t  TASKS_STOP;                         
  volatile  uint32_t  TASKS_DISABLE;                      
  volatile  uint32_t  TASKS_RSSISTART;                    
  volatile  uint32_t  TASKS_RSSISTOP;                     
  volatile  uint32_t  TASKS_BCSTART;                      
  volatile  uint32_t  TASKS_BCSTOP;                       
  volatile const  uint32_t  RESERVED0[55];
  volatile uint32_t  EVENTS_READY;                       
  volatile uint32_t  EVENTS_ADDRESS;                     
  volatile uint32_t  EVENTS_PAYLOAD;                     
  volatile uint32_t  EVENTS_END;                         
  volatile uint32_t  EVENTS_DISABLED;                    
  volatile uint32_t  EVENTS_DEVMATCH;                    
  volatile uint32_t  EVENTS_DEVMISS;                     
  volatile uint32_t  EVENTS_RSSIEND;                    
 
  volatile const  uint32_t  RESERVED1[2];
  volatile uint32_t  EVENTS_BCMATCH;                     
  volatile const  uint32_t  RESERVED2[53];
  volatile uint32_t  SHORTS;                             
  volatile const  uint32_t  RESERVED3[64];
  volatile uint32_t  INTENSET;                           
  volatile uint32_t  INTENCLR;                           
  volatile const  uint32_t  RESERVED4[61];
  volatile const  uint32_t  CRCSTATUS;                          
  volatile const  uint32_t  CD;                                 
  volatile const  uint32_t  RXMATCH;                            
  volatile const  uint32_t  RXCRC;                              
  volatile const  uint32_t  DAI;                                
  volatile const  uint32_t  RESERVED5[60];
  volatile uint32_t  PACKETPTR;                          
  volatile uint32_t  FREQUENCY;                          
  volatile uint32_t  TXPOWER;                            
  volatile uint32_t  MODE;                               
  volatile uint32_t  PCNF0;                              
  volatile uint32_t  PCNF1;                              
  volatile uint32_t  BASE0;                              
  volatile uint32_t  BASE1;                              
  volatile uint32_t  PREFIX0;                            
  volatile uint32_t  PREFIX1;                            
  volatile uint32_t  TXADDRESS;                          
  volatile uint32_t  RXADDRESSES;                        
  volatile uint32_t  CRCCNF;                             
  volatile uint32_t  CRCPOLY;                            
  volatile uint32_t  CRCINIT;                            
  volatile uint32_t  TEST;                               
  volatile uint32_t  TIFS;                               
  volatile const  uint32_t  RSSISAMPLE;                         
  volatile const  uint32_t  RESERVED6;
  volatile const  uint32_t  STATE;                              
  volatile uint32_t  DATAWHITEIV;                        
  volatile const  uint32_t  RESERVED7[2];
  volatile uint32_t  BCC;                                
  volatile const  uint32_t  RESERVED8[39];
  volatile uint32_t  DAB[8];                             
  volatile uint32_t  DAP[8];                             
  volatile uint32_t  DACNF;                              
  volatile const  uint32_t  RESERVED9[56];
  volatile uint32_t  OVERRIDE0;                          
  volatile uint32_t  OVERRIDE1;                          
  volatile uint32_t  OVERRIDE2;                          
  volatile uint32_t  OVERRIDE3;                          
  volatile uint32_t  OVERRIDE4;                          
  volatile const  uint32_t  RESERVED10[561];
  volatile uint32_t  POWER;                              
} NRF_RADIO_Type;


 
 
 




 

typedef struct {                                     
  volatile  uint32_t  TASKS_STARTRX;                      
  volatile  uint32_t  TASKS_STOPRX;                       
  volatile  uint32_t  TASKS_STARTTX;                      
  volatile  uint32_t  TASKS_STOPTX;                       
  volatile const  uint32_t  RESERVED0[3];
  volatile  uint32_t  TASKS_SUSPEND;                      
  volatile const  uint32_t  RESERVED1[56];
  volatile uint32_t  EVENTS_CTS;                         
  volatile uint32_t  EVENTS_NCTS;                        
  volatile uint32_t  EVENTS_RXDRDY;                      
  volatile const  uint32_t  RESERVED2[4];
  volatile uint32_t  EVENTS_TXDRDY;                      
  volatile const  uint32_t  RESERVED3;
  volatile uint32_t  EVENTS_ERROR;                       
  volatile const  uint32_t  RESERVED4[7];
  volatile uint32_t  EVENTS_RXTO;                        
  volatile const  uint32_t  RESERVED5[46];
  volatile uint32_t  SHORTS;                             
  volatile const  uint32_t  RESERVED6[64];
  volatile uint32_t  INTENSET;                           
  volatile uint32_t  INTENCLR;                           
  volatile const  uint32_t  RESERVED7[93];
  volatile uint32_t  ERRORSRC;                           
  volatile const  uint32_t  RESERVED8[31];
  volatile uint32_t  ENABLE;                             
  volatile const  uint32_t  RESERVED9;
  volatile uint32_t  PSELRTS;                            
  volatile uint32_t  PSELTXD;                            
  volatile uint32_t  PSELCTS;                            
  volatile uint32_t  PSELRXD;                            
  volatile const  uint32_t  RXD;                               

 
  volatile  uint32_t  TXD;                                
  volatile const  uint32_t  RESERVED10;
  volatile uint32_t  BAUDRATE;                           
  volatile const  uint32_t  RESERVED11[17];
  volatile uint32_t  CONFIG;                             
  volatile const  uint32_t  RESERVED12[675];
  volatile uint32_t  POWER;                              
} NRF_UART_Type;


 
 
 




 

typedef struct {                                     
  volatile const  uint32_t  RESERVED0[66];
  volatile uint32_t  EVENTS_READY;                       
  volatile const  uint32_t  RESERVED1[126];
  volatile uint32_t  INTENSET;                           
  volatile uint32_t  INTENCLR;                           
  volatile const  uint32_t  RESERVED2[125];
  volatile uint32_t  ENABLE;                             
  volatile const  uint32_t  RESERVED3;
  volatile uint32_t  PSELSCK;                            
  volatile uint32_t  PSELMOSI;                           
  volatile uint32_t  PSELMISO;                           
  volatile const  uint32_t  RESERVED4;
  volatile const  uint32_t  RXD;                                
  volatile uint32_t  TXD;                                
  volatile const  uint32_t  RESERVED5;
  volatile uint32_t  FREQUENCY;                          
  volatile const  uint32_t  RESERVED6[11];
  volatile uint32_t  CONFIG;                             
  volatile const  uint32_t  RESERVED7[681];
  volatile uint32_t  POWER;                              
} NRF_SPI_Type;


 
 
 




 

typedef struct {                                     
  volatile  uint32_t  TASKS_STARTRX;                      
  volatile const  uint32_t  RESERVED0;
  volatile  uint32_t  TASKS_STARTTX;                      
  volatile const  uint32_t  RESERVED1[2];
  volatile  uint32_t  TASKS_STOP;                         
  volatile const  uint32_t  RESERVED2;
  volatile  uint32_t  TASKS_SUSPEND;                      
  volatile  uint32_t  TASKS_RESUME;                       
  volatile const  uint32_t  RESERVED3[56];
  volatile uint32_t  EVENTS_STOPPED;                     
  volatile uint32_t  EVENTS_RXDREADY;                    
  volatile const  uint32_t  RESERVED4[4];
  volatile uint32_t  EVENTS_TXDSENT;                     
  volatile const  uint32_t  RESERVED5;
  volatile uint32_t  EVENTS_ERROR;                       
  volatile const  uint32_t  RESERVED6[4];
  volatile uint32_t  EVENTS_BB;                          
  volatile const  uint32_t  RESERVED7[3];
  volatile uint32_t  EVENTS_SUSPENDED;                   
  volatile const  uint32_t  RESERVED8[45];
  volatile uint32_t  SHORTS;                             
  volatile const  uint32_t  RESERVED9[64];
  volatile uint32_t  INTENSET;                           
  volatile uint32_t  INTENCLR;                           
  volatile const  uint32_t  RESERVED10[110];
  volatile uint32_t  ERRORSRC;                           
  volatile const  uint32_t  RESERVED11[14];
  volatile uint32_t  ENABLE;                             
  volatile const  uint32_t  RESERVED12;
  volatile uint32_t  PSELSCL;                            
  volatile uint32_t  PSELSDA;                            
  volatile const  uint32_t  RESERVED13[2];
  volatile const  uint32_t  RXD;                                
  volatile uint32_t  TXD;                                
  volatile const  uint32_t  RESERVED14;
  volatile uint32_t  FREQUENCY;                          
  volatile const  uint32_t  RESERVED15[24];
  volatile uint32_t  ADDRESS;                            
  volatile const  uint32_t  RESERVED16[668];
  volatile uint32_t  POWER;                              
} NRF_TWI_Type;


 
 
 




 

typedef struct {                                     
  volatile const  uint32_t  RESERVED0[9];
  volatile  uint32_t  TASKS_ACQUIRE;                      
  volatile  uint32_t  TASKS_RELEASE;                      
  volatile const  uint32_t  RESERVED1[54];
  volatile uint32_t  EVENTS_END;                         
  volatile const  uint32_t  RESERVED2[8];
  volatile uint32_t  EVENTS_ACQUIRED;                    
  volatile const  uint32_t  RESERVED3[53];
  volatile uint32_t  SHORTS;                             
  volatile const  uint32_t  RESERVED4[64];
  volatile uint32_t  INTENSET;                           
  volatile uint32_t  INTENCLR;                           
  volatile const  uint32_t  RESERVED5[61];
  volatile const  uint32_t  SEMSTAT;                            
  volatile const  uint32_t  RESERVED6[15];
  volatile uint32_t  STATUS;                             
  volatile const  uint32_t  RESERVED7[47];
  volatile uint32_t  ENABLE;                             
  volatile const  uint32_t  RESERVED8;
  volatile uint32_t  PSELSCK;                            
  volatile uint32_t  PSELMISO;                           
  volatile uint32_t  PSELMOSI;                           
  volatile uint32_t  PSELCSN;                            
  volatile const  uint32_t  RESERVED9[7];
  volatile uint32_t  RXDPTR;                             
  volatile uint32_t  MAXRX;                              
  volatile const  uint32_t  AMOUNTRX;                           
  volatile const  uint32_t  RESERVED10;
  volatile uint32_t  TXDPTR;                             
  volatile uint32_t  MAXTX;                              
  volatile const  uint32_t  AMOUNTTX;                           
  volatile const  uint32_t  RESERVED11;
  volatile uint32_t  CONFIG;                             
  volatile const  uint32_t  RESERVED12;
  volatile uint32_t  DEF;                                
  volatile const  uint32_t  RESERVED13[24];
  volatile uint32_t  ORC;                                
  volatile const  uint32_t  RESERVED14[654];
  volatile uint32_t  POWER;                              
} NRF_SPIS_Type;


 
 
 




 

typedef struct {                                     
  volatile  uint32_t  TASKS_OUT[4];                       
  volatile const  uint32_t  RESERVED0[60];
  volatile uint32_t  EVENTS_IN[4];                       
  volatile const  uint32_t  RESERVED1[27];
  volatile uint32_t  EVENTS_PORT;                        
  volatile const  uint32_t  RESERVED2[97];
  volatile uint32_t  INTENSET;                           
  volatile uint32_t  INTENCLR;                           
  volatile const  uint32_t  RESERVED3[129];
  volatile uint32_t  CONFIG[4];                          
  volatile const  uint32_t  RESERVED4[695];
  volatile uint32_t  POWER;                              
} NRF_GPIOTE_Type;


 
 
 




 

typedef struct {                                     
  volatile  uint32_t  TASKS_START;                        
  volatile  uint32_t  TASKS_STOP;                         
  volatile const  uint32_t  RESERVED0[62];
  volatile uint32_t  EVENTS_END;                         
  volatile const  uint32_t  RESERVED1[128];
  volatile uint32_t  INTENSET;                           
  volatile uint32_t  INTENCLR;                           
  volatile const  uint32_t  RESERVED2[61];
  volatile const  uint32_t  BUSY;                               
  volatile const  uint32_t  RESERVED3[63];
  volatile uint32_t  ENABLE;                             
  volatile uint32_t  CONFIG;                             
  volatile const  uint32_t  RESULT;                             
  volatile const  uint32_t  RESERVED4[700];
  volatile uint32_t  POWER;                              
} NRF_ADC_Type;


 
 
 




 

typedef struct {                                     
  volatile  uint32_t  TASKS_START;                        
  volatile  uint32_t  TASKS_STOP;                         
  volatile  uint32_t  TASKS_COUNT;                        
  volatile  uint32_t  TASKS_CLEAR;                        
  volatile  uint32_t  TASKS_SHUTDOWN;                     
  volatile const  uint32_t  RESERVED0[11];
  volatile  uint32_t  TASKS_CAPTURE[4];                   
  volatile const  uint32_t  RESERVED1[60];
  volatile uint32_t  EVENTS_COMPARE[4];                  
  volatile const  uint32_t  RESERVED2[44];
  volatile uint32_t  SHORTS;                             
  volatile const  uint32_t  RESERVED3[64];
  volatile uint32_t  INTENSET;                           
  volatile uint32_t  INTENCLR;                           
  volatile const  uint32_t  RESERVED4[126];
  volatile uint32_t  MODE;                               
  volatile uint32_t  BITMODE;                            
  volatile const  uint32_t  RESERVED5;
  volatile uint32_t  PRESCALER;                         
 
  volatile const  uint32_t  RESERVED6[11];
  volatile uint32_t  CC[4];                              
  volatile const  uint32_t  RESERVED7[683];
  volatile uint32_t  POWER;                              
} NRF_TIMER_Type;


 
 
 




 

typedef struct {                                     
  volatile  uint32_t  TASKS_START;                        
  volatile  uint32_t  TASKS_STOP;                         
  volatile  uint32_t  TASKS_CLEAR;                        
  volatile  uint32_t  TASKS_TRIGOVRFLW;                   
  volatile const  uint32_t  RESERVED0[60];
  volatile uint32_t  EVENTS_TICK;                        
  volatile uint32_t  EVENTS_OVRFLW;                      
  volatile const  uint32_t  RESERVED1[14];
  volatile uint32_t  EVENTS_COMPARE[4];                  
  volatile const  uint32_t  RESERVED2[109];
  volatile uint32_t  INTENSET;                           
  volatile uint32_t  INTENCLR;                           
  volatile const  uint32_t  RESERVED3[13];
  volatile uint32_t  EVTEN;                              
  volatile uint32_t  EVTENSET;                          
 
  volatile uint32_t  EVTENCLR;                          
 
  volatile const  uint32_t  RESERVED4[110];
  volatile const  uint32_t  COUNTER;                            
  volatile uint32_t  PRESCALER;                         
 
  volatile const  uint32_t  RESERVED5[13];
  volatile uint32_t  CC[4];                              
  volatile const  uint32_t  RESERVED6[683];
  volatile uint32_t  POWER;                              
} NRF_RTC_Type;


 
 
 




 

typedef struct {                                     
  volatile  uint32_t  TASKS_START;                        
  volatile  uint32_t  TASKS_STOP;                         
  volatile const  uint32_t  RESERVED0[62];
  volatile uint32_t  EVENTS_DATARDY;                     
  volatile const  uint32_t  RESERVED1[128];
  volatile uint32_t  INTENSET;                           
  volatile uint32_t  INTENCLR;                           
  volatile const  uint32_t  RESERVED2[127];
  volatile const  int32_t   TEMP;                               
  volatile const  uint32_t  RESERVED3[700];
  volatile uint32_t  POWER;                              
} NRF_TEMP_Type;


 
 
 




 

typedef struct {                                     
  volatile  uint32_t  TASKS_START;                        
  volatile  uint32_t  TASKS_STOP;                         
  volatile const  uint32_t  RESERVED0[62];
  volatile uint32_t  EVENTS_VALRDY;                      
  volatile const  uint32_t  RESERVED1[63];
  volatile uint32_t  SHORTS;                             
  volatile const  uint32_t  RESERVED2[64];
  volatile uint32_t  INTENSET;                           
  volatile uint32_t  INTENCLR;                           
  volatile const  uint32_t  RESERVED3[126];
  volatile uint32_t  CONFIG;                             
  volatile const  uint32_t  VALUE;                              
  volatile const  uint32_t  RESERVED4[700];
  volatile uint32_t  POWER;                              
} NRF_RNG_Type;


 
 
 




 

typedef struct {                                     
  volatile  uint32_t  TASKS_STARTECB;                    

 
  volatile  uint32_t  TASKS_STOPECB;                     
 
  volatile const  uint32_t  RESERVED0[62];
  volatile uint32_t  EVENTS_ENDECB;                      
  volatile uint32_t  EVENTS_ERRORECB;                   
 
  volatile const  uint32_t  RESERVED1[127];
  volatile uint32_t  INTENSET;                           
  volatile uint32_t  INTENCLR;                           
  volatile const  uint32_t  RESERVED2[126];
  volatile uint32_t  ECBDATAPTR;                         
  volatile const  uint32_t  RESERVED3[701];
  volatile uint32_t  POWER;                              
} NRF_ECB_Type;


 
 
 




 

typedef struct {                                     
  volatile  uint32_t  TASKS_START;                       
 
  volatile const  uint32_t  RESERVED0;
  volatile  uint32_t  TASKS_STOP;                         
  volatile const  uint32_t  RESERVED1[61];
  volatile uint32_t  EVENTS_END;                         
  volatile uint32_t  EVENTS_RESOLVED;                    
  volatile uint32_t  EVENTS_NOTRESOLVED;                 
  volatile const  uint32_t  RESERVED2[126];
  volatile uint32_t  INTENSET;                           
  volatile uint32_t  INTENCLR;                           
  volatile const  uint32_t  RESERVED3[61];
  volatile const  uint32_t  STATUS;                             
  volatile const  uint32_t  RESERVED4[63];
  volatile uint32_t  ENABLE;                             
  volatile uint32_t  NIRK;                               
  volatile uint32_t  IRKPTR;                             
  volatile const  uint32_t  RESERVED5;
  volatile uint32_t  ADDRPTR;                            
  volatile uint32_t  SCRATCHPTR;                        
 
  volatile const  uint32_t  RESERVED6[697];
  volatile uint32_t  POWER;                              
} NRF_AAR_Type;


 
 
 




 

typedef struct {                                     
  volatile  uint32_t  TASKS_KSGEN;                       
 
  volatile  uint32_t  TASKS_CRYPT;                       
 
  volatile  uint32_t  TASKS_STOP;                         
  volatile const  uint32_t  RESERVED0[61];
  volatile uint32_t  EVENTS_ENDKSGEN;                    
  volatile uint32_t  EVENTS_ENDCRYPT;                    
  volatile uint32_t  EVENTS_ERROR;                       
  volatile const  uint32_t  RESERVED1[61];
  volatile uint32_t  SHORTS;                             
  volatile const  uint32_t  RESERVED2[64];
  volatile uint32_t  INTENSET;                           
  volatile uint32_t  INTENCLR;                           
  volatile const  uint32_t  RESERVED3[61];
  volatile const  uint32_t  MICSTATUS;                          
  volatile const  uint32_t  RESERVED4[63];
  volatile uint32_t  ENABLE;                             
  volatile uint32_t  MODE;                               
  volatile uint32_t  CNFPTR;                             
  volatile uint32_t  INPTR;                              
  volatile uint32_t  OUTPTR;                             
  volatile uint32_t  SCRATCHPTR;                        
 
  volatile const  uint32_t  RESERVED5[697];
  volatile uint32_t  POWER;                              
} NRF_CCM_Type;


 
 
 




 

typedef struct {                                     
  volatile  uint32_t  TASKS_START;                        
  volatile const  uint32_t  RESERVED0[63];
  volatile uint32_t  EVENTS_TIMEOUT;                     
  volatile const  uint32_t  RESERVED1[128];
  volatile uint32_t  INTENSET;                           
  volatile uint32_t  INTENCLR;                           
  volatile const  uint32_t  RESERVED2[61];
  volatile const  uint32_t  RUNSTATUS;                          
  volatile const  uint32_t  REQSTATUS;                          
  volatile const  uint32_t  RESERVED3[63];
  volatile uint32_t  CRV;                                
  volatile uint32_t  RREN;                               
  volatile uint32_t  CONFIG;                             
  volatile const  uint32_t  RESERVED4[60];
  volatile  uint32_t  RR[8];                              
  volatile const  uint32_t  RESERVED5[631];
  volatile uint32_t  POWER;                              
} NRF_WDT_Type;


 
 
 




 

typedef struct {                                     
  volatile  uint32_t  TASKS_START;                        
  volatile  uint32_t  TASKS_STOP;                         
  volatile  uint32_t  TASKS_READCLRACC;                  
 
  volatile const  uint32_t  RESERVED0[61];
  volatile uint32_t  EVENTS_SAMPLERDY;                   
  volatile uint32_t  EVENTS_REPORTRDY;                  
 
  volatile uint32_t  EVENTS_ACCOF;                       
  volatile const  uint32_t  RESERVED1[61];
  volatile uint32_t  SHORTS;                             
  volatile const  uint32_t  RESERVED2[64];
  volatile uint32_t  INTENSET;                           
  volatile uint32_t  INTENCLR;                           
  volatile const  uint32_t  RESERVED3[125];
  volatile uint32_t  ENABLE;                             
  volatile uint32_t  LEDPOL;                             
  volatile uint32_t  SAMPLEPER;                          
  volatile const  int32_t   SAMPLE;                             
  volatile uint32_t  REPORTPER;                          
  volatile const  int32_t   ACC;                                
  volatile const  int32_t   ACCREAD;                           
 
  volatile uint32_t  PSELLED;                            
  volatile uint32_t  PSELA;                              
  volatile uint32_t  PSELB;                              
  volatile uint32_t  DBFEN;                              
  volatile const  uint32_t  RESERVED4[5];
  volatile uint32_t  LEDPRE;                             
  volatile const  uint32_t  ACCDBL;                             
  volatile const  uint32_t  ACCDBLREAD;                        
 
  volatile const  uint32_t  RESERVED5[684];
  volatile uint32_t  POWER;                              
} NRF_QDEC_Type;


 
 
 




 

typedef struct {                                     
  volatile  uint32_t  TASKS_START;                        
  volatile  uint32_t  TASKS_STOP;                         
  volatile  uint32_t  TASKS_SAMPLE;                       
  volatile const  uint32_t  RESERVED0[61];
  volatile uint32_t  EVENTS_READY;                       
  volatile uint32_t  EVENTS_DOWN;                        
  volatile uint32_t  EVENTS_UP;                          
  volatile uint32_t  EVENTS_CROSS;                       
  volatile const  uint32_t  RESERVED1[60];
  volatile uint32_t  SHORTS;                             
  volatile const  uint32_t  RESERVED2[64];
  volatile uint32_t  INTENSET;                           
  volatile uint32_t  INTENCLR;                           
  volatile const  uint32_t  RESERVED3[61];
  volatile const  uint32_t  RESULT;                             
  volatile const  uint32_t  RESERVED4[63];
  volatile uint32_t  ENABLE;                             
  volatile uint32_t  PSEL;                               
  volatile uint32_t  REFSEL;                             
  volatile uint32_t  EXTREFSEL;                          
  volatile const  uint32_t  RESERVED5[4];
  volatile uint32_t  ANADETECT;                          
  volatile const  uint32_t  RESERVED6[694];
  volatile uint32_t  POWER;                              
} NRF_LPCOMP_Type;


 
 
 




 

typedef struct {                                     
  volatile const  uint32_t  UNUSED;                             
} NRF_SWI_Type;


 
 
 




 

typedef struct {                                     
  volatile const  uint32_t  RESERVED0[256];
  volatile const  uint32_t  READY;                              
  volatile const  uint32_t  RESERVED1[64];
  volatile uint32_t  CONFIG;                             
  volatile uint32_t  ERASEPAGE;                          
  volatile uint32_t  ERASEALL;                           
  volatile uint32_t  ERASEPROTECTEDPAGE;                 
  volatile uint32_t  ERASEUICR;                          
} NRF_NVMC_Type;


 
 
 




 

typedef struct {                                     
  PPI_TASKS_CHG_Type TASKS_CHG[4];                   
  volatile const  uint32_t  RESERVED0[312];
  volatile uint32_t  CHEN;                               
  volatile uint32_t  CHENSET;                            
  volatile uint32_t  CHENCLR;                            
  volatile const  uint32_t  RESERVED1;
  PPI_CH_Type CH[16];                                
  volatile const  uint32_t  RESERVED2[156];
  volatile uint32_t  CHG[4];                             
} NRF_PPI_Type;


 
 
 




 

typedef struct {                                     
  volatile const  uint32_t  RESERVED0[4];
  volatile const  uint32_t  CODEPAGESIZE;                       
  volatile const  uint32_t  CODESIZE;                           
  volatile const  uint32_t  RESERVED1[4];
  volatile const  uint32_t  CLENR0;                             
  volatile const  uint32_t  PPFC;                               
  volatile const  uint32_t  RESERVED2;
  volatile const  uint32_t  NUMRAMBLOCK;                        
  
  union {
    volatile const  uint32_t  SIZERAMBLOCK[4];                 

 
    volatile const  uint32_t  SIZERAMBLOCKS;                    
  };
  volatile const  uint32_t  RESERVED3[5];
  volatile const  uint32_t  CONFIGID;                           
  volatile const  uint32_t  DEVICEID[2];                        
  volatile const  uint32_t  RESERVED4[6];
  volatile const  uint32_t  ER[4];                              
  volatile const  uint32_t  IR[4];                              
  volatile const  uint32_t  DEVICEADDRTYPE;                     
  volatile const  uint32_t  DEVICEADDR[2];                      
  volatile const  uint32_t  OVERRIDEEN;                         
  volatile const  uint32_t  NRF_1MBIT[5];                      
 
  volatile const  uint32_t  RESERVED5[10];
  volatile const  uint32_t  BLE_1MBIT[5];                      
 
} NRF_FICR_Type;


 
 
 




 

typedef struct {                                     
  volatile uint32_t  CLENR0;                             
  volatile uint32_t  RBPCONF;                            
  volatile uint32_t  XTALFREQ;                           
  volatile const  uint32_t  RESERVED0;
  volatile const  uint32_t  FWID;                               
  volatile uint32_t  BOOTLOADERADDR;                     
} NRF_UICR_Type;


 
 
 




 

typedef struct {                                     
  volatile const  uint32_t  RESERVED0[321];
  volatile uint32_t  OUT;                                
  volatile uint32_t  OUTSET;                             
  volatile uint32_t  OUTCLR;                             
  volatile const  uint32_t  IN;                                 
  volatile uint32_t  DIR;                                
  volatile uint32_t  DIRSET;                             
  volatile uint32_t  DIRCLR;                             
  volatile const  uint32_t  RESERVED1[120];
  volatile uint32_t  PIN_CNF[32];                        
} NRF_GPIO_Type;


 

  #pragma pop
#line 1138 "..\\..\\Include\\nrf51.h"




 
 
 

#line 1179 "..\\..\\Include\\nrf51.h"


 
 
 

#line 1218 "..\\..\\Include\\nrf51.h"


   
   
   








#line 5 "..\\..\\Include\\nrf_gpio.h"
#line 1 "..\\..\\Include\\nrf51_bitfields.h"




























 



 

#line 1 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cm0.h"
 







 

























 










#line 151 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cm0.h"



#line 697 "C:\\Keil\\ARM\\CMSIS\\Include\\core_cm0.h"



#line 36 "..\\..\\Include\\nrf51_bitfields.h"

 
 

 
 

 






 






 






 
 

 






 






 






 
 

 



 
 

 





 
 

 



 
 

 






 
 

 
 

 






 
 

 






 
 

 





 
 

 





 
 

 






 
#line 184 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 192 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 201 "..\\..\\Include\\nrf51_bitfields.h"

 






 
 

 



 
 

 






 
 

 
 

 
#line 243 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 255 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 267 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 279 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 291 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 303 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 315 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 327 "..\\..\\Include\\nrf51_bitfields.h"

 
 

 
#line 342 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 354 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 366 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 378 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 390 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 402 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 414 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 426 "..\\..\\Include\\nrf51_bitfields.h"

 
 

 
#line 441 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 453 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 465 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 477 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 489 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 501 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 513 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 525 "..\\..\\Include\\nrf51_bitfields.h"

 
 

 
#line 540 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 552 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 564 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 576 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 588 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 600 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 612 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 624 "..\\..\\Include\\nrf51_bitfields.h"

 
 

 
#line 639 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 651 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 663 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 675 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 687 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 699 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 711 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 723 "..\\..\\Include\\nrf51_bitfields.h"

 
 

 
#line 738 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 750 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 762 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 774 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 786 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 798 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 810 "..\\..\\Include\\nrf51_bitfields.h"

 
#line 822 "..\\..\\Include\\nrf51_bitfields.h"

 
 

 
 

 





 
 

 






 






 






 
 

 






 






 






 
 

 





 
 

 





 
 

 





 
 

 






 
 

 
 

 






 






 






 






 
 

 






 






 






 






 
 

 





 
 

 





 





 
 

 





 
 

 





 






 
 

 






 
 

 






 
 

 



 
 

 






 
 

 
 

 






 






 
 

 






 






 
 

 






 
 

 
 

 





 
 

 



 



 
 

 





 
 

 





 






 
 

 
 

 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 
 

 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 
 

 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 
 

 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 
 

 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 
 

 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 
 

 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 
 

 






 
#line 2683 "..\\..\\Include\\nrf51_bitfields.h"

 






 





 






 
 

 
 

 






 






 






 






 






 
 

 






 






 






 






 






 
 

 





 






 



 






 
 

 






 
 

 
 

 





 





 





 





 





 
 

 






 






 






 






 
 

 






 






 






 






 
 

 





 
 

 





 
 

 
#line 2950 "..\\..\\Include\\nrf51_bitfields.h"

 
 

 
#line 2965 "..\\..\\Include\\nrf51_bitfields.h"

 
 

 





 
 

 






 
 

 






 
 

 
 

 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 
 

 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 
 

 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 
 

 





 
 

 





 
 

 
 

 





 
 

 






 
 

 





 
 

 






 
 

 
 

 






 
 

 






 
 

 



 



 



 



 



 



 



 
 

 





 





 





 





 
 

 




 
 

 
#line 3739 "..\\..\\Include\\nrf51_bitfields.h"

 





 
 

 



 
 

 





 





 





 





 
 

 





 
 

 





 





 





 





 
 

 





 
 

 





 






 
 

 
 

 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 
 

 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 
 

 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 






 
 

 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 





 






 
 

 
 

 



 
 

 





 





 





 





 





 





 





 





 
 

 






 






 






 






 






 






 






 






 
 

 






 






 






 






 






 






 






 







 
 

 
 

 





 





 
 

 






 






 






 
 

 






 






 






 
 

 





 
 

 





 
 

 
#line 4863 "..\\..\\Include\\nrf51_bitfields.h"

 
 

 



 
 

 
#line 4885 "..\\..\\Include\\nrf51_bitfields.h"

 
 

 





 
 

 



 
 

 



 
 

 



 
 

 






 
 

 
 

 





 





 





 





 





 





 





 





 
 

 






 






 






 






 






 






 






 






 






 
 

 






 






 






 






 






 






 






 






 






 
 

 





 
 

 



 
 

 



 
 

 



 
 

 



 
 

 



 
 

 
#line 5170 "..\\..\\Include\\nrf51_bitfields.h"

 
 

 
#line 5181 "..\\..\\Include\\nrf51_bitfields.h"

 
 

 



 



 



 
 

 





 





 



 



 



 
 

 



 



 



 



 
 

 



 



 



 



 
 

 



 
 

 





 





 





 





 





 





 





 





 
 

 





 
#line 5336 "..\\..\\Include\\nrf51_bitfields.h"

 
 

 



 
 

 



 
 

 





 





 
 

 



 
 

 



 
 

 
#line 5395 "..\\..\\Include\\nrf51_bitfields.h"

 
 

 



 
 

 



 
 

 



 



 



 



 



 



 



 



 





 





 





 





 





 





 





 





 
 

 



 
 

 



 
 

 



 
 

 



 
 

 





 



 
 

 






 
 

 
 

 





 
 

 






 
 

 






 
 

 





 
 

 



 
 

 






 
 

 
 

 






 






 






 






 






 






 
 

 






 






 






 






 






 






 
 

 





 





 





 





 





 





 
 

 






 






 






 






 






 






 
 

 






 






 






 






 






 






 
 

 



 
 

 



 
 

 



 
 

 






 
 

 
 

 






 
 

 






 
 

 





 
 

 



 
 

 



 
 

 
#line 5914 "..\\..\\Include\\nrf51_bitfields.h"

 
 

 





 





 





 
 

 






 
 

 
 

 





 
 

 






 






 
 

 






 






 
 

 
#line 6002 "..\\..\\Include\\nrf51_bitfields.h"

 
 

 






 






 
 

 





 
 

 



 
 

 



 
 

 



 
 

 



 
 

 





 





 





 
 

 



 
 

 



 
 

 






 
 

 
 

 






 
 

 






 
 

 






 
 

 
 

 





 





 





 





 





 





 





 





 
 

 






 






 






 






 
 

 






 






 






 






 
 

 





 
 

 
#line 6270 "..\\..\\Include\\nrf51_bitfields.h"

 
 

 



 
 

 






 
 

 
 

 





 





 
 

 






 






 






 






 






 






 
 

 






 






 






 






 






 






 
 

 






 






 
 

 





 
 

 



 
 

 



 
 

 






 
 

 



 
 

 






 
 

 
 

 





 





 
 

 






 






 






 






 






 






 
 

 






 






 






 






 






 






 
 

 






 






 






 






 
 

 





 
 

 



 
 

 



 
 

 
#line 6647 "..\\..\\Include\\nrf51_bitfields.h"

 
 

 





 





 
 

 






 
 

 
 

 





 





 
 

 





 
 

 




 
 

 
 

 






 
 

 






 
 

 





 
 

 





 





 





 





 





 





 





 





 
 

 





 





 





 





 





 





 





 





 
 

 





 





 
 

 




 
 

 






 
#line 6 "..\\..\\Include\\nrf_gpio.h"












 




 
typedef enum
{
    NRF_GPIO_PORT_DIR_OUTPUT,       
    NRF_GPIO_PORT_DIR_INPUT         
} nrf_gpio_port_dir_t;




 
typedef enum
{
    NRF_GPIO_PIN_DIR_INPUT,   
    NRF_GPIO_PIN_DIR_OUTPUT   
} nrf_gpio_pin_dir_t;




 
typedef enum
{
    NRF_GPIO_PORT_SELECT_PORT0 = 0,           
    NRF_GPIO_PORT_SELECT_PORT1,               
    NRF_GPIO_PORT_SELECT_PORT2,               
    NRF_GPIO_PORT_SELECT_PORT3,               
} nrf_gpio_port_select_t;




 
typedef enum
{
    NRF_GPIO_PIN_NOPULL   = (0x00UL),                 
    NRF_GPIO_PIN_PULLDOWN = (0x01UL),                 
    NRF_GPIO_PIN_PULLUP   = (0x03UL),                   
} nrf_gpio_pin_pull_t;




 
typedef enum
{
    NRF_GPIO_PIN_NOSENSE    = (0x00UL),              
    NRF_GPIO_PIN_SENSE_LOW  = (0x03UL),                   
    NRF_GPIO_PIN_SENSE_HIGH = (0x02UL),                  
} nrf_gpio_pin_sense_t;











 
static __inline void nrf_gpio_range_cfg_output(uint32_t pin_range_start, uint32_t pin_range_end)
{
     
    for (; pin_range_start <= pin_range_end; pin_range_start++)
    {
        ((NRF_GPIO_Type *) 0x50000000UL)->PIN_CNF[pin_range_start] = ((0x00UL) << (16UL))
                                        | ((0x00UL) << (8UL))
                                        | ((0x00UL) << (2UL))
                                        | ((1UL) << (1UL))
                                        | ((1UL) << (0UL));
    }
}













 
static __inline void nrf_gpio_range_cfg_input(uint32_t pin_range_start, uint32_t pin_range_end, nrf_gpio_pin_pull_t pull_config)
{
     
    for (; pin_range_start <= pin_range_end; pin_range_start++)
    {
        ((NRF_GPIO_Type *) 0x50000000UL)->PIN_CNF[pin_range_start] = ((0x00UL) << (16UL))
                                        | ((0x00UL) << (8UL))
                                        | (pull_config << (2UL))
                                        | ((0UL) << (1UL))
                                        | ((0UL) << (0UL));
    }
}








 
static __inline void nrf_gpio_cfg_output(uint32_t pin_number)
{
     
    ((NRF_GPIO_Type *) 0x50000000UL)->PIN_CNF[pin_number] = ((0x00UL) << (16UL))
                                            | ((0x00UL) << (8UL))
                                            | ((0x00UL) << (2UL))
                                            | ((1UL) << (1UL))
                                            | ((1UL) << (0UL));
}










 
static __inline void nrf_gpio_cfg_input(uint32_t pin_number, nrf_gpio_pin_pull_t pull_config)
{
     
    ((NRF_GPIO_Type *) 0x50000000UL)->PIN_CNF[pin_number] = ((0x00UL) << (16UL))
                                        | ((0x00UL) << (8UL))
                                        | (pull_config << (2UL))
                                        | ((0UL) << (1UL))
                                        | ((0UL) << (0UL));
}











 
static __inline void nrf_gpio_cfg_sense_input(uint32_t pin_number, nrf_gpio_pin_pull_t pull_config, nrf_gpio_pin_sense_t sense_config)
{
     
    ((NRF_GPIO_Type *) 0x50000000UL)->PIN_CNF[pin_number] = (sense_config << (16UL))
                                        | ((0x00UL) << (8UL))
                                        | (pull_config << (2UL))
                                        | ((0UL) << (1UL))
                                        | ((0UL) << (0UL));
}








 
static __inline void nrf_gpio_pin_dir_set(uint32_t pin_number, nrf_gpio_pin_dir_t direction)
{
    if(direction == NRF_GPIO_PIN_DIR_INPUT)
    {
        ((NRF_GPIO_Type *) 0x50000000UL)->PIN_CNF[pin_number] =
          ((0x00UL) << (16UL))
        | ((0x00UL) << (8UL))
        | ((0x00UL) << (2UL))
        | ((0UL) << (1UL))
        | ((0UL) << (0UL));
    }
    else
    {
        ((NRF_GPIO_Type *) 0x50000000UL)->DIRSET = (1UL << pin_number);
    }
}









 
static __inline void nrf_gpio_pin_set(uint32_t pin_number)
{
    ((NRF_GPIO_Type *) 0x50000000UL)->OUTSET = (1UL << pin_number);
}









 
static __inline void nrf_gpio_pin_clear(uint32_t pin_number)
{
    ((NRF_GPIO_Type *) 0x50000000UL)->OUTCLR = (1UL << pin_number);
}









 
static __inline void nrf_gpio_pin_toggle(uint32_t pin_number)
{
    const uint32_t pin_bit   = 1UL << pin_number;
    const uint32_t pin_state = ((((NRF_GPIO_Type *) 0x50000000UL)->OUT >> pin_number) & 1UL);
    
    if (pin_state == 0)
    {
        
        ((NRF_GPIO_Type *) 0x50000000UL)->OUTSET = pin_bit;        
    }
    else
    {
        
        ((NRF_GPIO_Type *) 0x50000000UL)->OUTCLR = pin_bit;       
    }
}













 
static __inline void nrf_gpio_pin_write(uint32_t pin_number, uint32_t value)
{
    if (value == 0)
    {
        nrf_gpio_pin_clear(pin_number);
    }
    else
    {
        nrf_gpio_pin_set(pin_number);
    }
}














 
static __inline uint32_t nrf_gpio_pin_read(uint32_t pin_number)
{
    return  ((((NRF_GPIO_Type *) 0x50000000UL)->IN >> pin_number) & 1UL);
}














 
static __inline void nrf_gpio_word_byte_write(volatile uint32_t * word_address, uint8_t byte_no, uint8_t value)
{
    *((volatile uint8_t*)(word_address) + byte_no) = value;
}













 
static __inline uint8_t nrf_gpio_word_byte_read(const volatile uint32_t* word_address, uint8_t byte_no)
{
    return (*((const volatile uint8_t*)(word_address) + byte_no));
}







 
static __inline void nrf_gpio_port_dir_set(nrf_gpio_port_select_t port, nrf_gpio_port_dir_t dir)
{
    if (dir == NRF_GPIO_PORT_DIR_OUTPUT)
    {
        nrf_gpio_word_byte_write(&((NRF_GPIO_Type *) 0x50000000UL)->DIRSET, port, 0xFF);
    }
    else
    {
        nrf_gpio_range_cfg_input(port*8, (port+1)*8-1, NRF_GPIO_PIN_NOPULL);
    }
}







 
static __inline uint8_t nrf_gpio_port_read(nrf_gpio_port_select_t port)
{
    return nrf_gpio_word_byte_read(&((NRF_GPIO_Type *) 0x50000000UL)->IN, port);
}









 
static __inline void nrf_gpio_port_write(nrf_gpio_port_select_t port, uint8_t value)
{
    nrf_gpio_word_byte_write(&((NRF_GPIO_Type *) 0x50000000UL)->OUT, port, value);
}











 
static __inline void nrf_gpio_port_set(nrf_gpio_port_select_t port, uint8_t set_mask)
{
    nrf_gpio_word_byte_write(&((NRF_GPIO_Type *) 0x50000000UL)->OUTSET, port, set_mask);
}











 
static __inline void nrf_gpio_port_clear(nrf_gpio_port_select_t port, uint8_t clr_mask)
{
    nrf_gpio_word_byte_write(&((NRF_GPIO_Type *) 0x50000000UL)->OUTCLR, port, clr_mask);
}

 

#line 11 "..\\..\\Include\\boards/ant_io_board_v_5.h"

#line 18 "..\\..\\Include\\boards/ant_io_board_v_5.h"









#line 34 "..\\..\\Include\\boards/ant_io_board_v_5.h"

















#line 57 "..\\..\\Include\\boards/ant_io_board_v_5.h"













#line 84 "..\\..\\Include\\boards/ant_io_board_v_5.h"




#line 99 "..\\..\\Include\\boards/ant_io_board_v_5.h"

#line 115 "..\\..\\Include\\boards/ant_io_board_v_5.h"




#line 31 "..\\..\\Include\\boards.h"




#line 204 "..\\Spansion_SLLD_v13.2.1\\src\\slld_targetspecific.h"
#line 205 "..\\Spansion_SLLD_v13.2.1\\src\\slld_targetspecific.h"
#line 1 "..\\..\\Include\\spi_master.h"










 











 




#line 29 "..\\..\\Include\\spi_master.h"
#line 1 "C:\\Keil\\ARM\\ARMCC\\bin\\..\\include\\stdlib.h"
 
 
 




 
 



 













  


 








#line 46 "C:\\Keil\\ARM\\ARMCC\\bin\\..\\include\\stdlib.h"


  
  typedef unsigned int size_t;










    



    typedef unsigned short wchar_t;  
#line 75 "C:\\Keil\\ARM\\ARMCC\\bin\\..\\include\\stdlib.h"

typedef struct div_t { int quot, rem; } div_t;
    
typedef struct ldiv_t { long int quot, rem; } ldiv_t;
    

typedef struct lldiv_t { __int64 quot, rem; } lldiv_t;
    


#line 96 "C:\\Keil\\ARM\\ARMCC\\bin\\..\\include\\stdlib.h"
   



 

   




 
#line 115 "C:\\Keil\\ARM\\ARMCC\\bin\\..\\include\\stdlib.h"
   


 
extern __declspec(__nothrow) int __aeabi_MB_CUR_MAX(void);

   




 

   




 




extern __declspec(__nothrow) double atof(const char *  ) __attribute__((__nonnull__(1)));
   



 
extern __declspec(__nothrow) int atoi(const char *  ) __attribute__((__nonnull__(1)));
   



 
extern __declspec(__nothrow) long int atol(const char *  ) __attribute__((__nonnull__(1)));
   



 

extern __declspec(__nothrow) __int64 atoll(const char *  ) __attribute__((__nonnull__(1)));
   



 


extern __declspec(__nothrow) double strtod(const char * __restrict  , char ** __restrict  ) __attribute__((__nonnull__(1)));
   

















 

extern __declspec(__nothrow) float strtof(const char * __restrict  , char ** __restrict  ) __attribute__((__nonnull__(1)));
extern __declspec(__nothrow) long double strtold(const char * __restrict  , char ** __restrict  ) __attribute__((__nonnull__(1)));
   

 

extern __declspec(__nothrow) long int strtol(const char * __restrict  ,
                        char ** __restrict  , int  ) __attribute__((__nonnull__(1)));
   



























 
extern __declspec(__nothrow) unsigned long int strtoul(const char * __restrict  ,
                                       char ** __restrict  , int  ) __attribute__((__nonnull__(1)));
   


























 

 
extern __declspec(__nothrow) __int64 strtoll(const char * __restrict  ,
                               char ** __restrict  , int  ) __attribute__((__nonnull__(1)));
   




 
extern __declspec(__nothrow) unsigned __int64 strtoull(const char * __restrict  ,
                                         char ** __restrict  , int  ) __attribute__((__nonnull__(1)));
   



 

extern __declspec(__nothrow) int rand(void);
   







 
extern __declspec(__nothrow) void srand(unsigned int  );
   






 

struct _rand_state { int __x[57]; };
extern __declspec(__nothrow) int _rand_r(struct _rand_state *);
extern __declspec(__nothrow) void _srand_r(struct _rand_state *, unsigned int);
struct _ANSI_rand_state { int __x[1]; };
extern __declspec(__nothrow) int _ANSI_rand_r(struct _ANSI_rand_state *);
extern __declspec(__nothrow) void _ANSI_srand_r(struct _ANSI_rand_state *, unsigned int);
   


 

extern __declspec(__nothrow) void *calloc(size_t  , size_t  );
   



 
extern __declspec(__nothrow) void free(void *  );
   





 
extern __declspec(__nothrow) void *malloc(size_t  );
   



 
extern __declspec(__nothrow) void *realloc(void *  , size_t  );
   













 

extern __declspec(__nothrow) int posix_memalign(void **  , size_t  , size_t  );
   









 

typedef int (*__heapprt)(void *, char const *, ...);
extern __declspec(__nothrow) void __heapstats(int (*  )(void *  ,
                                           char const *  , ...),
                        void *  ) __attribute__((__nonnull__(1)));
   










 
extern __declspec(__nothrow) int __heapvalid(int (*  )(void *  ,
                                           char const *  , ...),
                       void *  , int  ) __attribute__((__nonnull__(1)));
   














 
extern __declspec(__nothrow) __declspec(__noreturn) void abort(void);
   







 

extern __declspec(__nothrow) int atexit(void (*  )(void)) __attribute__((__nonnull__(1)));
   




 
#line 415 "C:\\Keil\\ARM\\ARMCC\\bin\\..\\include\\stdlib.h"


extern __declspec(__nothrow) __declspec(__noreturn) void exit(int  );
   












 

extern __declspec(__nothrow) __declspec(__noreturn) void _Exit(int  );
   







      

extern __declspec(__nothrow) char *getenv(const char *  ) __attribute__((__nonnull__(1)));
   









 

extern __declspec(__nothrow) int  system(const char *  );
   









 

extern  void *bsearch(const void *  , const void *  ,
              size_t  , size_t  ,
              int (*  )(const void *, const void *)) __attribute__((__nonnull__(1,2,5)));
   












 
#line 503 "C:\\Keil\\ARM\\ARMCC\\bin\\..\\include\\stdlib.h"


extern  void qsort(void *  , size_t  , size_t  ,
           int (*  )(const void *, const void *)) __attribute__((__nonnull__(1,4)));
   









 

#line 532 "C:\\Keil\\ARM\\ARMCC\\bin\\..\\include\\stdlib.h"

extern __declspec(__nothrow) __pure int abs(int  );
   



 

extern __declspec(__nothrow) __pure div_t div(int  , int  );
   









 
extern __declspec(__nothrow) __pure long int labs(long int  );
   



 




extern __declspec(__nothrow) __pure ldiv_t ldiv(long int  , long int  );
   











 







extern __declspec(__nothrow) __pure __int64 llabs(__int64  );
   



 




extern __declspec(__nothrow) __pure lldiv_t lldiv(__int64  , __int64  );
   











 
#line 613 "C:\\Keil\\ARM\\ARMCC\\bin\\..\\include\\stdlib.h"



 
typedef struct __sdiv32by16 { int quot, rem; } __sdiv32by16;
typedef struct __udiv32by16 { unsigned int quot, rem; } __udiv32by16;
    
typedef struct __sdiv64by32 { int rem, quot; } __sdiv64by32;

__value_in_regs extern __declspec(__nothrow) __pure __sdiv32by16 __rt_sdiv32by16(
     int  ,
     short int  );
   

 
__value_in_regs extern __declspec(__nothrow) __pure __udiv32by16 __rt_udiv32by16(
     unsigned int  ,
     unsigned short  );
   

 
__value_in_regs extern __declspec(__nothrow) __pure __sdiv64by32 __rt_sdiv64by32(
     int  , unsigned int  ,
     int  );
   

 



 
extern __declspec(__nothrow) unsigned int __fp_status(unsigned int  , unsigned int  );
   







 























 
extern __declspec(__nothrow) int mblen(const char *  , size_t  );
   












 
extern __declspec(__nothrow) int mbtowc(wchar_t * __restrict  ,
                   const char * __restrict  , size_t  );
   















 
extern __declspec(__nothrow) int wctomb(char *  , wchar_t  );
   













 





 
extern __declspec(__nothrow) size_t mbstowcs(wchar_t * __restrict  ,
                      const char * __restrict  , size_t  ) __attribute__((__nonnull__(2)));
   














 
extern __declspec(__nothrow) size_t wcstombs(char * __restrict  ,
                      const wchar_t * __restrict  , size_t  ) __attribute__((__nonnull__(2)));
   














 

extern __declspec(__nothrow) void __use_realtime_heap(void);
extern __declspec(__nothrow) void __use_realtime_division(void);
extern __declspec(__nothrow) void __use_two_region_memory(void);
extern __declspec(__nothrow) void __use_no_heap(void);
extern __declspec(__nothrow) void __use_no_heap_region(void);

extern __declspec(__nothrow) char const *__C_library_version_string(void);
extern __declspec(__nothrow) int __C_library_version_number(void);











#line 867 "C:\\Keil\\ARM\\ARMCC\\bin\\..\\include\\stdlib.h"


 

#line 30 "..\\..\\Include\\spi_master.h"





 
#line 49 "..\\..\\Include\\spi_master.h"

 
typedef struct
{
    uint32_t SPI_Freq;           
    uint32_t SPI_Pin_SCK;        
    uint32_t SPI_Pin_MISO;       
    uint32_t SPI_Pin_MOSI;       
    uint32_t SPI_Pin_SS;         
    uint32_t SPI_PriorityIRQ;    
    uint8_t SPI_CONFIG_ORDER;    
    uint8_t SPI_CONFIG_CPOL;     
    uint8_t SPI_CONFIG_CPHA;     
    uint8_t SPI_DisableAllIRQ;   
} spi_master_config_t;

 
typedef enum spi_master_evt_type_t
{
    SPI_MASTER_EVT_TRANSFER_STARTED = 0,     
    SPI_MASTER_EVT_TRANSFER_COMPLETED,       
    SPI_MASTER_EVT_TYPE_MAX                  
} spi_master_evt_type_t;

 
typedef struct
{
    spi_master_evt_type_t evt_type;  
    uint16_t data_count;             
} spi_master_evt_t;

 
typedef enum
{
    SPI_MASTER_STATE_DISABLED,   
    SPI_MASTER_STATE_BUSY,       
    SPI_MASTER_STATE_IDLE        
} spi_master_state_t;

 
typedef enum
{

    SPI_MASTER_0,    






    SPI_MASTER_HW_ENABLED_COUNT  
} spi_master_hw_instance_t;




 
typedef void (*spi_master_event_handler_t)(spi_master_evt_t spi_master_evt);

















 
uint32_t spi_master_open(const spi_master_hw_instance_t    spi_master_hw_instance,
                         spi_master_config_t const * const p_spi_master_config);








 
void spi_master_close(const spi_master_hw_instance_t spi_master_hw_instance);


















 
uint32_t spi_master_send_recv(const spi_master_hw_instance_t spi_master_hw_instance,
                              uint8_t * const p_tx_buf, const uint16_t tx_buf_len,
                              uint8_t * const p_rx_buf, const uint16_t rx_buf_len);










 
void spi_master_evt_handler_reg(const spi_master_hw_instance_t spi_master_hw_instance,
                                spi_master_event_handler_t     event_handler);











 
spi_master_state_t spi_master_get_state(const spi_master_hw_instance_t spi_master_hw_instance);


 
#line 206 "..\\Spansion_SLLD_v13.2.1\\src\\slld_targetspecific.h"










 



#line 43 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"














#line 114 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"

#line 174 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"

#line 246 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"

#line 303 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"

#line 366 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"



#line 381 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"

#line 432 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"










#line 450 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"


                                  










#line 483 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"


#line 560 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"


#line 650 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"





#line 753 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"

#line 778 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"






 



 
typedef unsigned short LLD_UINT16;   
typedef unsigned long  LLD_UINT32;   
typedef LLD_UINT32     ADDRESS;      

 
typedef uint8_t  BYTE;    
typedef uint16_t WORD;    
typedef uint32_t DWORD;   
typedef BYTE FLASHDATA;

 

typedef unsigned long BYTECOUNT;   

 
#line 810 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"

 
#line 819 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"





 










#line 845 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"











 
 
typedef enum {
    FLASH_SOFTWARE_UNPROTECTED = 0,
    FLASH_SOFTWARE_PROTECTED
} DEV_SOFTWARE_PROTECT_STATUS;


typedef enum {
    dev_status_unknown = 0,
    dev_not_busy,
    dev_program_error,
    dev_erase_error,
    dev_suspend,
    dev_busy
} DEVSTATUS;


typedef enum {
    SLLD_OK = 0x0,
    SLLD_E_DEVICE_SOFTWARE_PROTECTED,
    SLLD_E_HAL_ERROR = 0x200,
    SLLD_ERROR = 0xFFFF
} SLLD_STATUS;





extern BYTE modebit_char;




 
 

 

 

#line 982 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"



#line 993 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"


extern SLLD_STATUS slld_PPOp
(
        ADDRESS     sys_addr,                  
        BYTE       *data_buf,                  
        BYTECOUNT   len_in_bytes,              
        DEVSTATUS  *dev_status_ptr             
);




extern SLLD_STATUS slld_PP_4BOp
(
        ADDRESS     sys_addr,                  
        BYTE       *data_buf,                  
        BYTECOUNT   len_in_bytes,              
        DEVSTATUS  *dev_status_ptr             
);



extern SLLD_STATUS slld_BufferedProgramOp
(
        ADDRESS    sys_addr,                   
        BYTE      *data_buf,                   
        BYTECOUNT  len_in_bytes,               
        DEVSTATUS *dev_status_ptr              
);



extern SLLD_STATUS slld_BufferedProgram_4BOp
(
        ADDRESS     sys_addr,                  
        BYTE       *data_buf,                  
        BYTECOUNT   len_in_bytes,              
        DEVSTATUS  *dev_status_ptr             
);



extern SLLD_STATUS slld_SEOp
(
        ADDRESS     sys_addr,                  
        DEVSTATUS  *dev_status_ptr             
);



extern SLLD_STATUS slld_SE_4BOp
(
        ADDRESS     sys_addr,                  
        DEVSTATUS  *dev_status_ptr             
);



extern SLLD_STATUS slld_BEOp
(
        DEVSTATUS  *dev_status_ptr             
);

extern SLLD_STATUS slld_BE1Op
(
        DEVSTATUS  *dev_status_ptr             
);



extern SLLD_STATUS slld_WRSROp
(
        BYTE       *data_buf,                  
        DEVSTATUS  *dev_status_ptr             
);



extern SLLD_STATUS slld_BlockProtectOp
(
        BYTE        bpb_value,                 
        DEVSTATUS  *dev_status_ptr             
);


 

 


extern SLLD_STATUS slld_ReadCmd
(
        ADDRESS     sys_addr,                  
        BYTE       *target,                    
        BYTECOUNT   len_in_bytes               
);



extern SLLD_STATUS slld_Read_4BCmd
(
        ADDRESS     sys_addr,                  
        BYTE       *target,                    
        BYTECOUNT   len_in_bytes               
);



extern SLLD_STATUS slld_Fast_ReadCmd
(
        ADDRESS     sys_addr,                  
        BYTE       *target,                    
        BYTECOUNT   len_in_bytes               
);



extern SLLD_STATUS slld_Fast_Read_4BCmd
(
        ADDRESS     sys_addr,                  
        BYTE       *target,                    
        BYTECOUNT   len_in_bytes               
);



extern SLLD_STATUS slld_Read_IDCmd
(
        BYTE    *target                        
);



extern SLLD_STATUS slld_RDIDCmd
(
        BYTE      *target,                     
        BYTECOUNT  len_in_bytes                
);



SLLD_STATUS slld_Read_IdentificationCmd
(
        BYTE    *target,                       
        ADDRESS  addr                          
);


void slld_VersionCmd (void);


SLLD_STATUS slld_SRSTCmd (void);


 


extern SLLD_STATUS slld_RDSRCmd
(
        BYTE    *target                        
);



extern SLLD_STATUS slld_RASPCmd
(
        WORD    *target                        
);



extern SLLD_STATUS slld_BRRDCmd
(
        BYTE    *target                        
);



extern SLLD_STATUS slld_ABRDCmd
(
        DWORD    *target                        
);



extern SLLD_STATUS slld_ECCRDCmd
(
        ADDRESS  sys_addr,                     
        BYTE     *target                       
);



extern SLLD_STATUS slld_RPWDCmd
(
        BYTE    *target                        
);



extern SLLD_STATUS slld_WRENCmd (void);



extern SLLD_STATUS slld_WRDICmd (void);



extern SLLD_STATUS slld_WRSRCmd
(
        BYTE   *data_buf                       
);





extern SLLD_STATUS slld_ClearStatusRegisterCmd (void);



extern SLLD_STATUS slld_P4ECmd
(
        ADDRESS  sys_addr                      
);



extern SLLD_STATUS slld_P4EOp
(
        ADDRESS     sys_addr,                  
        DEVSTATUS  *dev_status_ptr             
);



SLLD_STATUS slld_WRRCmd
(
        BYTE    *status_val,                   
        BYTE    *config_val,                    
        BYTE    *status2_val                  
);



SLLD_STATUS slld_WRROp
(
        BYTE       *status_val,                
        BYTE       *config_val,                
        BYTE       *status2_val,               
        DEVSTATUS  *dev_status_ptr             
);



SLLD_STATUS slld_WASPCmd
(
        WORD    *asp_val                       
);



SLLD_STATUS slld_WASPOp
(
        WORD       *asp_val,                   
        DEVSTATUS  *dev_status_ptr             
);



SLLD_STATUS slld_BRWRCmd
(
        BYTE    *bnk_val                       
);



SLLD_STATUS slld_ABWRCmd
(
        DWORD    *abt_val                       
);



SLLD_STATUS slld_ABWROp
(
        DWORD       *abt_val,                   
        DEVSTATUS  *dev_status_ptr             
);



SLLD_STATUS slld_WPWDCmd
(
        BYTE     *target                       
);



SLLD_STATUS slld_WPWDOp
(
        BYTE       *target,                    
        DEVSTATUS  *dev_status_ptr             
);



SLLD_STATUS slld_RCRCmd
(
        BYTE    *target                        
);



SLLD_STATUS slld_OTPRCmd
(
        ADDRESS    sys_addr,                   
        BYTE      *target,                     
        BYTECOUNT  len_in_bytes                
);



SLLD_STATUS slld_OTPPCmd
(
        ADDRESS  sys_addr,                     
        BYTE    *data_buf                      
);



SLLD_STATUS slld_OTPPOp
(
        ADDRESS     sys_addr,                  
        BYTE       *data_buf,                  
        DEVSTATUS  *dev_status_ptr             
);


#line 1339 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"

#line 1346 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"

#line 1354 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"

#line 1362 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"


extern SLLD_STATUS slld_DualIOReadCmd
(
        ADDRESS    sys_addr,                   
        BYTE      *target,                     
        BYTECOUNT  len_in_bytes                
);



extern SLLD_STATUS slld_DualIORead_4BCmd
(
        ADDRESS    sys_addr,                   
        BYTE      *target,                     
        BYTECOUNT  len_in_bytes                
);



extern SLLD_STATUS slld_QuadIOReadCmd
(
        ADDRESS    sys_addr,                   
        BYTE      *target,                     
        BYTECOUNT  len_in_bytes                
);



extern SLLD_STATUS slld_QuadIORead_4BCmd
(
        ADDRESS    sys_addr,                   
        BYTE      *target,                     
        BYTECOUNT  len_in_bytes                
);



extern SLLD_STATUS slld_DualIOHPReadCmd
(
        ADDRESS                    sys_addr,     
        BYTE                    *target,         
        BYTE                    modebit,         
        BYTECOUNT          len_in_bytes          
);



extern SLLD_STATUS slld_DualIOHPRead_4BCmd
(
        ADDRESS                    sys_addr,       
        BYTE                    *target,           
        BYTE                    modebit,           
        BYTECOUNT          len_in_bytes            
);



extern SLLD_STATUS slld_QuadIOHPReadCmd
(
        ADDRESS                    sys_addr,        
        BYTE                    *target,            
        BYTE                    modebit,            
        BYTECOUNT          len_in_bytes             
);



extern SLLD_STATUS slld_QuadIOHPRead_4BCmd
(
        ADDRESS                    sys_addr,         
        BYTE                    *target,             
        BYTE                    modebit,             
        BYTECOUNT          len_in_bytes              
);




extern SLLD_STATUS slld_QPPCmd
(
        ADDRESS    sys_addr,                   
        BYTE      *data_buf,                   
        BYTECOUNT  len_in_bytes                
);



extern SLLD_STATUS slld_QPP_4BCmd
(
        ADDRESS    sys_addr,                   
        BYTE      *data_buf,                   
        BYTECOUNT  len_in_bytes                
);



extern SLLD_STATUS slld_QPPOp
(
        ADDRESS     sys_addr,                  
        BYTE       *data_buf,                  
        BYTECOUNT   len_in_bytes,              
        DEVSTATUS  *dev_status_ptr             
);
extern SLLD_STATUS slld_QuadWriteOp
(
        ADDRESS     sys_addr,                  
        BYTE       *data_buf,                  
        BYTECOUNT   len_in_bytes,              
        DEVSTATUS  *dev_status_ptr             
);



extern SLLD_STATUS slld_QPP_4BOp
(
        ADDRESS     sys_addr,                   
        BYTE       *data_buf,                   
        BYTECOUNT   len_in_bytes,               
        DEVSTATUS  *dev_status_ptr              
);








 


extern SLLD_STATUS slld_PPCmd
(
        ADDRESS      sys_addr,                 
        BYTE        *data_buf,                 
        BYTECOUNT    len_in_bytes              
);



extern SLLD_STATUS slld_PP_4BCmd
(
        ADDRESS      sys_addr,                 
        BYTE        *data_buf,                 
        BYTECOUNT    len_in_bytes              
);



extern SLLD_STATUS slld_PPB_PGCmd
(
        ADDRESS  sys_addr                      
);



extern SLLD_STATUS slld_PPB_PGOp
(
        ADDRESS     sys_addr,                  
        DEVSTATUS  *dev_status_ptr             
);



extern SLLD_STATUS slld_DYB_PGCmd
(
        ADDRESS   sys_addr,                    
        BYTE     *data_buf                     
);



extern SLLD_STATUS slld_DYB_PGOp
(
        ADDRESS     sys_addr,                  
        BYTE       *data_buf,                  
        DEVSTATUS  *dev_status_ptr             
);


 


extern SLLD_STATUS slld_SECmd
(
        ADDRESS  sys_addr                      
);



extern SLLD_STATUS slld_SE_4BCmd
(
        ADDRESS  sys_addr                      
);



extern SLLD_STATUS slld_ERS_SSPCmd (void);



extern SLLD_STATUS slld_ERS_RESCmd (void);



extern SLLD_STATUS slld_RCSPCmd (void);



extern SLLD_STATUS slld_RCRSCmd (void);



extern SLLD_STATUS slld_BECmd (void);
extern SLLD_STATUS slld_BE1Cmd (void);







extern SLLD_STATUS slld_RCVRCmd (void);



extern SLLD_STATUS slld_SPCmd (void);



 

extern SLLD_STATUS slld_DPCmd (void);



extern SLLD_STATUS slld_RESCmd (void);


 


extern SLLD_STATUS slld_Poll
(
        DEVSTATUS  *dev_status_ptr             
);



extern SLLD_STATUS slld_StatusGet
(
        DEVSTATUS  *dev_status_ptr             
);


#line 1627 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"




extern SLLD_STATUS slld_RDSR2Cmd
(
        BYTE    *target                         
);



extern SLLD_STATUS slld_P4E_4BCmd
(
        ADDRESS  sys_addr                           
);



extern SLLD_STATUS slld_QPP2Cmd
(
        ADDRESS    sys_addr,                   
        BYTE      *data_buf,                   
        BYTECOUNT  len_in_bytes                
);



extern SLLD_STATUS slld_DLPRDCmd
(
        BYTE      *data_buf,                   
        BYTECOUNT  len_in_bytes                
);



extern SLLD_STATUS slld_PNVDLRCmd
(
        BYTE      *data_buf,                   
        BYTECOUNT  len_in_bytes                
);



extern SLLD_STATUS slld_WVDLRCmd
(
        BYTE      *data_buf,                   
        BYTECOUNT  len_in_bytes                
);



extern SLLD_STATUS slld_PGSPCmd(void);



extern SLLD_STATUS slld_PGRSCmd(void);



extern SLLD_STATUS slld_PLBWRCmd(void);



extern SLLD_STATUS slld_PLBRDCmd
(
        BYTE      *data_buf,                   
        BYTECOUNT  len_in_bytes                
);



extern SLLD_STATUS slld_DYB_RDCmd
(
        ADDRESS   sys_addr,                       
        BYTE     *data_buf                        
);



extern SLLD_STATUS slld_PPB_RDCmd
(
        ADDRESS   sys_addr,                       
        BYTE     *data_buf                        
);



extern SLLD_STATUS slld_PPB_ERSCmd(void);



SLLD_STATUS slld_PASSRDCmd
(
        BYTE     *data_buf                        
);



SLLD_STATUS slld_PASSPCmd
(
        BYTE     *data_buf                        
);



SLLD_STATUS slld_PASSUCmd
(
        BYTE     *data_buf                        
);



extern SLLD_STATUS slld_P4E_4BOp
(
        ADDRESS     sys_addr,                  
        DEVSTATUS  *dev_status_ptr             
);
#line 1781 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"

#line 1801 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"

#line 1912 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"
extern SLLD_STATUS slld_ReadSFDPCmd
(
        ADDRESS    sys_addr,             
        BYTE      *read_buf               
);




extern SLLD_STATUS slld_ReadModeResetCmd(void);


#line 2003 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"


 
#line 2016 "..\\Spansion_SLLD_v13.2.1\\src\\slld.h"


typedef unsigned char   UCHAR;


typedef unsigned short  USHORT;


typedef unsigned int    UINT;


typedef unsigned long   ULONG;





extern SLLD_STATUS slld_ReadOp
(
        ADDRESS     sys_addr,                  
        BYTE       *target,                    
        BYTECOUNT   len_in_bytes               

);

extern SLLD_STATUS slld_WriteOp
(
        ADDRESS     sys_addr,                  
        BYTE       *data_buf,                  
        BYTECOUNT   len_in_bytes,              
        DEVSTATUS  *dev_status_ptr             
);
 
#line 39 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"
#line 1 "..\\Spansion_SLLD_v13.2.1\\Hals\\nRF51xxx\\slld_hal.h"
 
 






























 








#line 54 "..\\Spansion_SLLD_v13.2.1\\Hals\\nRF51xxx\\slld_hal.h"





 

#line 62 "..\\Spansion_SLLD_v13.2.1\\Hals\\nRF51xxx\\slld_hal.h"
	

SLLD_STATUS FLASH_READ
(
BYTE		command,																		 
ADDRESS sys_addr,																	  
BYTE	 *data_buffer,																 
int		 Number_Of_Read_Bytes												 
);


SLLD_STATUS FLASH_WRITE
(
BYTE		command,																		 
ADDRESS sys_addr,																	  
BYTE	 *data_buffer,																 
int		 Number_Of_Written_Bytes										  
);



void spi_EEPROM_init(void);






#line 40 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"










BYTE modebit_char;
BYTECOUNT g_PAGE_SIZE=256;
#line 400 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"










 
SLLD_STATUS slld_ReadCmd
(
        ADDRESS   sys_addr,           
        BYTE     *target,             
        BYTECOUNT len_in_bytes        
)
{
    SLLD_STATUS status = SLLD_OK;

#line 428 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_READ((0x03),sys_addr,target,len_in_bytes);

    return(status);
}












 
SLLD_STATUS slld_Read_4BCmd
(
        ADDRESS   sys_addr,            
        BYTE     *target,              
        BYTECOUNT len_in_bytes         
)
{
    SLLD_STATUS status = SLLD_OK;

#line 463 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_READ((0x13),sys_addr,target,len_in_bytes);

    return(status);
}











 
SLLD_STATUS slld_Fast_ReadCmd
(
        ADDRESS    sys_addr,               
        BYTE      *target,                 
        BYTECOUNT  len_in_bytes            
)
{
    SLLD_STATUS         status = SLLD_OK;

#line 497 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_READ((0x0B),sys_addr,target,len_in_bytes);

    return(status);
}











 
SLLD_STATUS slld_Fast_Read_4BCmd
(
        ADDRESS    sys_addr,               
        BYTE      *target,                 
        BYTECOUNT  len_in_bytes            
)
{
    SLLD_STATUS status = SLLD_OK;

#line 531 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_READ((0x0C),sys_addr,target,len_in_bytes);

    return(status);
}














 
SLLD_STATUS slld_Read_IDCmd
(
        BYTE    *target                       
)
{
    SLLD_STATUS status = SLLD_OK;

    status = FLASH_READ((0xAB),(ADDRESS)0xFFFFFFFF,target,1);





    return(status);
}













 
SLLD_STATUS slld_RDIDCmd
(
        BYTE      *target,               
        BYTECOUNT  len_in_bytes          
)
{
    SLLD_STATUS status = SLLD_OK;

#line 596 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_READ((0x9F),(ADDRESS)0xFFFFFFFF,target,len_in_bytes);

    return(status);
}











 
SLLD_STATUS slld_Read_IdentificationCmd
(
        BYTE    *target,                      
        ADDRESS  addr                         
)
{
    SLLD_STATUS status = SLLD_OK;
    ADDRESS    tmp_addr;

#line 630 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"
    if (addr < 0x2)
        tmp_addr = addr;
    else
        tmp_addr = (ADDRESS)0;

    status = FLASH_READ((0x90),tmp_addr,target,2);

    return(status);
}











 
SLLD_STATUS slld_RDSRCmd
(
        BYTE    *target                         
)
{
    SLLD_STATUS status = SLLD_OK;

#line 666 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"
    status = FLASH_READ((0x05),(ADDRESS)0xFFFFFFFF,target,1);

    return(status);
}











 
SLLD_STATUS slld_RASPCmd
(
        WORD    *target                         
)
{
    SLLD_STATUS status = SLLD_OK;

#line 697 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_READ((0x2B),(ADDRESS)0xFFFFFFFF,(BYTE *)target,2);

    return(status);
}











 
SLLD_STATUS slld_BRRDCmd
(
        BYTE    *target                         
)
{
    SLLD_STATUS status = SLLD_OK;

#line 729 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_READ((0x16),(ADDRESS)0xFFFFFFFF,target,1);

    return(status);
}











 
SLLD_STATUS slld_ABRDCmd
(
        DWORD    *target            

)
{
    SLLD_STATUS status = SLLD_OK;

#line 762 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_READ((0x14),(ADDRESS)0xFFFFFFFF,(BYTE *)target,4);

    return(status);
}











 
SLLD_STATUS slld_ECCRDCmd
(
        ADDRESS  sys_addr,                 
        BYTE    *target                    
)
{
    SLLD_STATUS status = SLLD_OK;

#line 795 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_READ((0x18),sys_addr,target,1);

    return(status);
}











 
SLLD_STATUS slld_RPWDCmd
(
        BYTE    *target                  
)
{
    SLLD_STATUS status = SLLD_OK;

#line 827 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_READ((0xE7),(ADDRESS)0xFFFFFFFF,target,8);

    return(status);
}











 
SLLD_STATUS slld_WRENCmd (void)
{
    SLLD_STATUS status = SLLD_OK;

#line 856 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0x06),(ADDRESS)0xFFFFFFFF,(BYTE*)0,0);

    return(status);
}











 
SLLD_STATUS slld_WRDICmd (void)
{
    SLLD_STATUS status = SLLD_OK;

#line 885 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0x04),(ADDRESS)0xFFFFFFFF,(BYTE*)0,0);

    return(status);
}











 
SLLD_STATUS slld_WRSRCmd
(
        BYTE   *data_buf              
)
{
    SLLD_STATUS status = SLLD_OK;

#line 917 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0x01),(ADDRESS)0xFFFFFFFF,data_buf,1);

    return(status);
}


void slld_VersionCmd(void)
{       
    printf("\n%s\n", "13.2.1");
}










 
SLLD_STATUS slld_SRSTCmd (void)
{
    SLLD_STATUS status = SLLD_OK;
    static unsigned int print_version= 0;

    if(!print_version) {
        slld_VersionCmd();
        print_version=1;
    }


#line 958 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0xF0),0xFFFFFFFF,(BYTE*)0,0);

    return(status);
}










 
SLLD_STATUS slld_PPCmd
(
        ADDRESS     sys_addr,                   
        BYTE       *data_buf,                   
        BYTECOUNT   len_in_bytes                
)
{
    SLLD_STATUS  status = SLLD_OK;

#line 991 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0x02),sys_addr,data_buf,len_in_bytes);

    return(status);
}











 
SLLD_STATUS slld_PP_4BCmd
(
        ADDRESS    sys_addr,                   
        BYTE      *data_buf,                   
        BYTECOUNT  len_in_bytes                
)
{
    SLLD_STATUS  status = SLLD_OK;

#line 1025 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0x12),sys_addr,data_buf,len_in_bytes);

    return(status);
}











 
SLLD_STATUS slld_PPB_PGCmd
(
        ADDRESS  sys_addr                   
)
{
    SLLD_STATUS  status = SLLD_OK;

#line 1057 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0xE3),sys_addr,(BYTE*)0,0);

    return(status);
}











 
SLLD_STATUS slld_PPB_PGOp
(
        ADDRESS     sys_addr,                        
        DEVSTATUS  *dev_status_ptr                   
)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if(status != SLLD_OK)
        return(status);

    status = slld_PPB_PGCmd(sys_addr);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd(); 

    return(status);
}











 
SLLD_STATUS slld_DYB_PGCmd
(
        ADDRESS   sys_addr,                       
        BYTE     *data_buf                        
)
{
    SLLD_STATUS status = SLLD_OK;

#line 1126 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0xE1),sys_addr,data_buf,1);

    return(status);
}











 
SLLD_STATUS slld_DYB_PGOp
(
        ADDRESS     sys_addr,                   
        BYTE       *data_buf,                   
        DEVSTATUS  *dev_status_ptr              
)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if(status != SLLD_OK)
        return(status);

    status = slld_DYB_PGCmd(sys_addr, data_buf);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd(); 

    return(status);
}












 
SLLD_STATUS slld_SECmd
(
        ADDRESS  sys_addr                      
)
{
    SLLD_STATUS status = SLLD_OK;

#line 1192 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0xD8),sys_addr,(BYTE*)0,0);

    return(status);
}











 
SLLD_STATUS slld_SE_4BCmd
(
        ADDRESS  sys_addr                
)
{
    SLLD_STATUS status = SLLD_OK;

#line 1224 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0xDC),sys_addr,(BYTE*)0,0);

    return(status);
}











 
SLLD_STATUS slld_ERS_SSPCmd (void)
{
    SLLD_STATUS status = SLLD_OK;

#line 1253 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0x75),0xFFFFFFFF,(BYTE*)0,0);

    return(status);
}











 
SLLD_STATUS slld_ERS_RESCmd (void)
{
    SLLD_STATUS status = SLLD_OK;

#line 1282 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0x7A),0xFFFFFFFF,(BYTE*)0,0);

    return(status);
}











 
SLLD_STATUS slld_RCSPCmd (void)
{
    SLLD_STATUS status = SLLD_OK;

#line 1311 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0x00),0xFFFFFFFF,(BYTE*)0,0);

    return(status);
}











 
SLLD_STATUS slld_RCRSCmd (void)
{
    SLLD_STATUS status = SLLD_OK;

#line 1340 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0x00),0xFFFFFFFF,(BYTE*)0,0);

    return(status);
}












 
SLLD_STATUS slld_BECmd (void)
{
    SLLD_STATUS status = SLLD_OK;

#line 1370 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0xC7),0xFFFFFFFF,(BYTE*)0,0);

    return(status);
}

SLLD_STATUS slld_BE1Cmd (void)
{
    SLLD_STATUS status = SLLD_OK;

#line 1388 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0x60),0xFFFFFFFF,(BYTE*)0,0);

    return(status);
}




#line 1427 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"












 
SLLD_STATUS slld_RCVRCmd (void)
{
    SLLD_STATUS status = SLLD_OK;

#line 1452 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0x00),0xFFFFFFFF,(BYTE*)0,0);

    return(status);
}












 
SLLD_STATUS slld_DPCmd (void)
{
    SLLD_STATUS status = SLLD_OK;

#line 1482 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0xB9),0xFFFFFFFF,(BYTE*)0,0);





    return(status);
}













 
SLLD_STATUS slld_SPCmd (void)
{
    SLLD_STATUS status = SLLD_OK;

#line 1517 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0xB9),0xFFFFFFFF,(BYTE*)0,0);





    return(status);
}













 
SLLD_STATUS slld_RESCmd (void)
{
    SLLD_STATUS status = SLLD_OK;

    status = FLASH_WRITE((0xAB),0xFFFFFFFF,(BYTE*)0,0);





    return(status);
}












 
SLLD_STATUS slld_ClearStatusRegisterCmd (void)
{
    SLLD_STATUS status = SLLD_OK;

#line 1577 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0x30),0xFFFFFFFF,(BYTE*)0,0);

    return(status);
}











 
SLLD_STATUS slld_WRRCmd
(
        BYTE  *status_val,          
        BYTE  *config_val,          
        BYTE  *status2_val          
)
{
    SLLD_STATUS status = SLLD_OK;
    BYTE Buffer[3];

#line 1612 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"
    if(!status_val)return SLLD_ERROR;
    Buffer[0] = *status_val;
    if(status2_val != 0)
    {
        if(!config_val || !status_val) return SLLD_ERROR;
        Buffer[1] = *config_val;
        Buffer[2] = *status2_val;
        status = FLASH_WRITE((0x01),0xFFFFFFFF,Buffer,3);
        return status;
    }
    if (config_val == 0)
    {
        status = FLASH_WRITE((0x01),0xFFFFFFFF,Buffer,1);
    } else {
        Buffer[1] = *config_val;
        status = FLASH_WRITE((0x01),0xFFFFFFFF,Buffer,2);
    }

    return(status);
}











 
SLLD_STATUS slld_WRROp
(
        BYTE       *status_val,           
        BYTE       *config_val,           
        BYTE       *status2_val,          
        DEVSTATUS  *dev_status_ptr        
)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if(status != SLLD_OK)
        return(status);

    status = slld_WRRCmd(status_val, config_val,status2_val);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd();        

    return(status);
}












 
SLLD_STATUS slld_WASPCmd
(
        WORD    *asp_val                  
)
{
    SLLD_STATUS status = SLLD_OK;

#line 1698 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0x2F),0xFFFFFFFF,(BYTE *)asp_val,2);

    return(status);
}











 
SLLD_STATUS slld_WASPOp
(
        WORD       *asp_val,              
        DEVSTATUS  *dev_status_ptr        
)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if(status != SLLD_OK)
        return(status);

    status = slld_WASPCmd(asp_val);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd();        

    return(status);
}











 
SLLD_STATUS slld_BRWRCmd
(
        BYTE    *bnk_val                  
)
{
    SLLD_STATUS status = SLLD_OK;

#line 1766 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0x17),0xFFFFFFFF,bnk_val,1);

    return(status);
}











 
SLLD_STATUS slld_ABWRCmd
(
        DWORD    *abt_val                  
)
{
    SLLD_STATUS status = SLLD_OK;

#line 1798 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0x15),0xFFFFFFFF,(BYTE*)abt_val,4);

    return(status);
}











 
SLLD_STATUS slld_ABWROp
(
        DWORD       *abt_val,              
        DEVSTATUS  *dev_status_ptr        
)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if(status != SLLD_OK)
        return(status);

    status = slld_ABWRCmd(abt_val);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd();           

    return(status);
}











 
SLLD_STATUS slld_WPWDCmd
(
        BYTE     *target                   
)
{
    SLLD_STATUS status = SLLD_OK;

#line 1866 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0xE8),0xFFFFFFFF,target,1);

    return(status);
}











 
SLLD_STATUS slld_WPWDOp
(
        BYTE       *target,               
        DEVSTATUS  *dev_status_ptr        
)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if(status != SLLD_OK)
        return(status);

    status = slld_WPWDCmd(target);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd();           

    return(status);
}











 
SLLD_STATUS slld_RCRCmd
(
        BYTE    *target                    
)
{
    SLLD_STATUS status = SLLD_OK;

#line 1934 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_READ((0x35),0xFFFFFFFF,target,1);

    return(status);
}











 
SLLD_STATUS slld_OTPRCmd
(
        ADDRESS    sys_addr,         
        BYTE      *target,           
        BYTECOUNT  len_in_bytes      
)
{
    SLLD_STATUS status = SLLD_OK;

#line 1968 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_READ((0x4B),sys_addr,target,len_in_bytes);

    return(status);
}











 
SLLD_STATUS slld_OTPPCmd
(
        ADDRESS  sys_addr,                        
        BYTE    *data_buf                         
)
{
    SLLD_STATUS status = SLLD_OK;

#line 2001 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0x42),sys_addr,data_buf,1);

    return(status);
}











 
SLLD_STATUS slld_P4ECmd
(
        ADDRESS  sys_addr                   
)
{
    SLLD_STATUS status = SLLD_OK;

#line 2033 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0x20),sys_addr,(BYTE*)0,0);

    return(status);
}


#line 2071 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

#line 2103 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"










 
SLLD_STATUS slld_DualIOReadCmd
(
        ADDRESS    sys_addr,         
        BYTE      *target,           
        BYTECOUNT  len_in_bytes      
)
{
    SLLD_STATUS status = SLLD_OK;

#line 2131 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_READ((0x3B),sys_addr,target,len_in_bytes);

    return(status);
}












 
SLLD_STATUS slld_DualIORead_4BCmd
(
        ADDRESS    sys_addr,         
        BYTE      *target,           
        BYTECOUNT  len_in_bytes      
)
{
    SLLD_STATUS status = SLLD_OK;

#line 2166 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_READ((0x3C),sys_addr,target,len_in_bytes);

    return(status);
}











 
SLLD_STATUS slld_QuadIOReadCmd
(
        ADDRESS    sys_addr,         
        BYTE      *target,           
        BYTECOUNT  len_in_bytes      
)
{
    SLLD_STATUS status = SLLD_OK;

#line 2200 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_READ((0x6B),sys_addr,target,len_in_bytes);

    return(status);
}












 
SLLD_STATUS slld_QuadIORead_4BCmd
(
        ADDRESS    sys_addr,         
        BYTE      *target,           
        BYTECOUNT  len_in_bytes      
)
{
    SLLD_STATUS status = SLLD_OK;

#line 2235 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_READ((0x6C),sys_addr,target,len_in_bytes);

    return(status);
}











 
SLLD_STATUS slld_DualIOHPReadCmd
(
        ADDRESS                sys_addr,      
        BYTE                *target,          
        BYTE                modebit,          
        BYTECOUNT        len_in_bytes         
)
{
    SLLD_STATUS status = SLLD_OK;

#line 2270 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    modebit_char = modebit;
    status = FLASH_READ((0xBB),sys_addr,target,len_in_bytes);

    return(status);
}












 
SLLD_STATUS slld_DualIOHPRead_4BCmd
(
        ADDRESS                sys_addr,      
        BYTE                *target,          
        BYTE                modebit,          
        BYTECOUNT        len_in_bytes         
)
{
    SLLD_STATUS status = SLLD_OK;

#line 2307 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    modebit_char = modebit;
    status = FLASH_READ((0xBC),sys_addr,target,len_in_bytes);

    return(status);
}











 
SLLD_STATUS slld_QuadIOHPReadCmd
(
        ADDRESS                sys_addr,     
        BYTE                *target,         
        BYTE                modebit,         
        BYTECOUNT        len_in_bytes        
)
{
    SLLD_STATUS status = SLLD_OK;

#line 2343 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    modebit_char = modebit;
    status = FLASH_READ((0xEB),sys_addr,target,len_in_bytes);

    return(status);
}












 
SLLD_STATUS slld_QuadIOHPRead_4BCmd
(
        ADDRESS                sys_addr,      
        BYTE                *target,          
        BYTE                modebit,          
        BYTECOUNT        len_in_bytes         
)
{
    SLLD_STATUS status = SLLD_OK;

#line 2380 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    modebit_char = modebit;
    status = FLASH_READ((0xEC),sys_addr,target,len_in_bytes);

    return(status);
}












 
SLLD_STATUS slld_QPPCmd
(
        ADDRESS    sys_addr,                   
        BYTE      *data_buf,                   
        BYTECOUNT  len_in_bytes                
)
{
    SLLD_STATUS         status = SLLD_OK;

#line 2416 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0x32),sys_addr,data_buf,len_in_bytes);

    return(status);
}











 
SLLD_STATUS slld_QPP_4BCmd
(
        ADDRESS     sys_addr,                   
        BYTE       *data_buf,                   
        BYTECOUNT   len_in_bytes                
)
{
    SLLD_STATUS         status = SLLD_OK;

#line 2450 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0x34),sys_addr,data_buf,len_in_bytes);

    return(status);
}



#line 2487 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"












 
SLLD_STATUS slld_Poll
(
        DEVSTATUS  *dev_status_ptr               
)
{
    SLLD_STATUS       status = SLLD_OK;

    *dev_status_ptr = dev_status_unknown;

#line 2517 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    do
    {
        status = slld_StatusGet(dev_status_ptr);
        if(status != SLLD_OK)
            return(status);
    }
    while(*dev_status_ptr == dev_busy);

    return(status);
}












 
SLLD_STATUS slld_StatusGet
(
        DEVSTATUS*  dev_status_ptr                   
)
{
    SLLD_STATUS  status = SLLD_OK;
    BYTE poll_data;

#line 2557 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = slld_RDSRCmd(&poll_data);
    if(status != SLLD_OK)
        return(status);

    
    if (poll_data & (0x01)) {                     
        
        if (((0x01 | 0x02) & 0x01) && (poll_data & (0x20)))
            *dev_status_ptr = dev_erase_error;
        
        else if (((0x01 | 0x02) & 0x02) && (poll_data & (0x40)))
            *dev_status_ptr = dev_program_error;
        else
            *dev_status_ptr = dev_busy;
    }
    else
    {
        
        *dev_status_ptr = dev_not_busy;
    }

    return (status);
}


#line 2607 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"












 
SLLD_STATUS slld_PPOp
(
        ADDRESS     sys_addr,                  
        BYTE       *data_buf,                  
        BYTECOUNT   len_in_bytes,              
        DEVSTATUS  *dev_status_ptr             
)
{
    SLLD_STATUS status = SLLD_OK;

    if(len_in_bytes > g_PAGE_SIZE)
    {
        status = SLLD_ERROR;   
        return(status);
    }

    status = slld_WRENCmd();
    if(status != SLLD_OK)
        return(status);

    status = slld_PPCmd(sys_addr, data_buf, len_in_bytes);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd();  

    return(status);
}















 
SLLD_STATUS slld_PP_4BOp
(
        ADDRESS     sys_addr,                  
        BYTE       *data_buf,                  
        BYTECOUNT   len_in_bytes,              
        DEVSTATUS  *dev_status_ptr             
)
{
    SLLD_STATUS status = SLLD_OK;

    if(len_in_bytes > g_PAGE_SIZE)
    {
        status = SLLD_ERROR;   
        return(status);
    }

    status = slld_WRENCmd();
    if(status != SLLD_OK)
        return(status);

    status = slld_PP_4BCmd(sys_addr, data_buf, len_in_bytes);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd(); 

    return(status);
}












 
SLLD_STATUS slld_SEOp
(
        ADDRESS     sys_addr,                  
        DEVSTATUS  *dev_status_ptr             
)
{
    SLLD_STATUS status = SLLD_OK;


    BYTE upper_addr;
    status = slld_DPCmd();             
    if(status != SLLD_OK)
        return(status);

    upper_addr = (BYTE)(sys_addr >> 24);
    status = slld_WRRCmd(&upper_addr,0,0);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRENCmd();
    if(status != SLLD_OK)
        return(status);




    status = slld_SECmd(sys_addr);

    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd();         

    return(status);
}












 
SLLD_STATUS slld_SE_4BOp
(
        ADDRESS     sys_addr,                  
        DEVSTATUS  *dev_status_ptr             
)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if(status != SLLD_OK)
        return(status);

    status = slld_SE_4BCmd(sys_addr);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd();         

    return(status);
}












 
SLLD_STATUS slld_BEOp
(
        DEVSTATUS  *dev_status_ptr             
)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if(status != SLLD_OK)
        return(status);

    status = slld_BECmd();
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd();        

    return(status);
}

SLLD_STATUS slld_BE1Op
(
        DEVSTATUS  *dev_status_ptr        
)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if(status != SLLD_OK)
        return(status);

    status = slld_BE1Cmd();
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd();        

    return(status);
}




#line 2892 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"













 
SLLD_STATUS slld_WRSROp
(
        BYTE       *data_buf,                  
        DEVSTATUS  *dev_status_ptr             
)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if(status != SLLD_OK)
        return(status);

    status = slld_WRSRCmd(data_buf);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd();           

    return(status);
}













 
SLLD_STATUS slld_BufferedProgramOp
(
        ADDRESS    sys_addr,                    
        BYTE      *data_buf,                    
        BYTECOUNT  len_in_bytes,                
        DEVSTATUS *dev_status_ptr               
)
{
    SLLD_STATUS status = SLLD_OK;
    ADDRESS     current_sys_addr;           
    BYTECOUNT   current_len_in_bytes;       
    BYTE       *current_data_ptr;           

     
    current_sys_addr = sys_addr;
    current_data_ptr = data_buf;

    current_len_in_bytes = g_PAGE_SIZE - (sys_addr & (g_PAGE_SIZE -1));
    if (current_len_in_bytes > len_in_bytes)
        current_len_in_bytes = len_in_bytes;

    do
    {
        status = slld_PPOp(current_sys_addr, current_data_ptr, current_len_in_bytes, dev_status_ptr);
        if(status != SLLD_OK)
            return(status);

        len_in_bytes -= current_len_in_bytes;

         
        current_sys_addr += current_len_in_bytes;
        current_data_ptr += current_len_in_bytes;

        if (len_in_bytes > g_PAGE_SIZE)
        {
            current_len_in_bytes = g_PAGE_SIZE;
        }
        else
        {
            current_len_in_bytes = len_in_bytes;
        }
    }
    while (len_in_bytes != 0);

    status = slld_WRDICmd();           

    return(status);
}













 
SLLD_STATUS slld_BufferedProgram_4BOp
(
        ADDRESS     sys_addr,                   
        BYTE       *data_buf,                   
        BYTECOUNT   len_in_bytes,               
        DEVSTATUS  *dev_status_ptr              
)
{
    SLLD_STATUS status = SLLD_OK;
    ADDRESS     current_sys_addr;           
    BYTECOUNT   current_len_in_bytes;       
    BYTE       *current_data_ptr;           

     
    current_sys_addr = sys_addr;
    current_data_ptr = data_buf;

    current_len_in_bytes = g_PAGE_SIZE - (sys_addr & (g_PAGE_SIZE -1));
    if (current_len_in_bytes > len_in_bytes)
        current_len_in_bytes = len_in_bytes;

    do
    {
        status = slld_PP_4BOp(current_sys_addr, current_data_ptr, current_len_in_bytes, dev_status_ptr);
        if(status != SLLD_OK)
            return(status);

        len_in_bytes -= current_len_in_bytes;

         
        current_sys_addr += current_len_in_bytes;
        current_data_ptr += current_len_in_bytes;

        if (len_in_bytes > g_PAGE_SIZE)
        {
            current_len_in_bytes = g_PAGE_SIZE;
        }
        else
        {
            current_len_in_bytes = len_in_bytes;
        }
    }
    while (len_in_bytes != 0);

    status = slld_WRDICmd();           

    return(status);
}
















 
SLLD_STATUS slld_BlockProtectOp
(
        BYTE        bpb_value,                  
        DEVSTATUS  *dev_status_ptr              
)
{
    SLLD_STATUS status = SLLD_OK;
    BYTE        current_status_register;           
    BYTE        new_status_register;               

    status = slld_RDSRCmd(&current_status_register);
    if(status != SLLD_OK)
        return(status);

    new_status_register = ((current_status_register & (~(0x1C))) | (bpb_value << 2) );

    status = slld_WRSROp(&new_status_register, dev_status_ptr);

    return(status);
}














 
SLLD_STATUS slld_QPPOp
(
        ADDRESS     sys_addr,                  
        BYTE       *data_buf,                  
        BYTECOUNT   len_in_bytes,              
        DEVSTATUS  *dev_status_ptr             
)
{
    SLLD_STATUS status = SLLD_OK;

    if(len_in_bytes > g_PAGE_SIZE)
    {
        status = SLLD_ERROR;   
        return(status);
    }

    status = slld_WRENCmd();
    if(status != SLLD_OK)
        return(status);

    status = slld_QPPCmd(sys_addr, data_buf, len_in_bytes);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd(); 

    return(status);
}

SLLD_STATUS slld_QuadWriteOp
(
        ADDRESS     sys_addr,                  
        BYTE       *data_buf,                  
        BYTECOUNT   len_in_bytes,              
        DEVSTATUS  *dev_status_ptr             
)
{
    SLLD_STATUS status = SLLD_OK;
    BYTECOUNT length;

    while(len_in_bytes)
    {
        if(len_in_bytes < g_PAGE_SIZE)
            length = len_in_bytes;
        else
            length = g_PAGE_SIZE;

        status = slld_QPPOp(sys_addr, data_buf, length, dev_status_ptr);
        if(status != SLLD_OK)
            return(status);

        len_in_bytes -= length;
        sys_addr += length;
        data_buf += length;
    }

    return(status);
}














 
SLLD_STATUS slld_QPP_4BOp
(
        ADDRESS     sys_addr,                  
        BYTE       *data_buf,                  
        BYTECOUNT   len_in_bytes,              
        DEVSTATUS  *dev_status_ptr             
)
{
    SLLD_STATUS status = SLLD_OK;

    if(len_in_bytes > g_PAGE_SIZE)
    {
        status = SLLD_ERROR;   
        return(status);
    }

    status = slld_WRENCmd();
    if(status != SLLD_OK)
        return(status);

    status = slld_QPP_4BCmd(sys_addr, data_buf, len_in_bytes);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd(); 

    return(status);
}













 
SLLD_STATUS slld_OTPPOp
(
        ADDRESS     sys_addr,                  
        BYTE       *data_buf,                  
        DEVSTATUS  *dev_status_ptr             
)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if(status != SLLD_OK)
        return(status);

    status = slld_OTPPCmd(sys_addr, data_buf);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd();   

    return(status);
}












 
SLLD_STATUS slld_P4EOp
(
        ADDRESS     sys_addr,                  
        DEVSTATUS  *dev_status_ptr             
)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if(status != SLLD_OK)
        return(status);

    status = slld_P4ECmd(sys_addr);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd();         

    return(status);
}


#line 3329 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

#line 3366 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"












 
SLLD_STATUS slld_P4E_4BCmd
(
        ADDRESS  sys_addr                           
)
{
    SLLD_STATUS status = SLLD_OK;

#line 3394 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0x21),sys_addr,(BYTE*)0,0);

    return(status);
}











 
SLLD_STATUS slld_P4E_4BOp
(
        ADDRESS     sys_addr,                  
        DEVSTATUS  *dev_status_ptr             
)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if(status != SLLD_OK)
        return(status);

    status = slld_P4E_4BCmd(sys_addr);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd();         

    return(status);
}












 
SLLD_STATUS slld_RDSR2Cmd
(
        BYTE    *target                         
)
{
    SLLD_STATUS status = SLLD_OK;

#line 3463 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_READ((0x07),(ADDRESS)0xFFFFFFFF,target,1);

    return(status);
}













 
SLLD_STATUS slld_QPP2Cmd
(
        ADDRESS    sys_addr,                   
        BYTE      *data_buf,                   
        BYTECOUNT  len_in_bytes                
)
{
    SLLD_STATUS         status = SLLD_OK;

#line 3499 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0x38),sys_addr,data_buf,len_in_bytes);

    return(status);
}











 
SLLD_STATUS slld_DLPRDCmd
(
        BYTE      *data_buf,                   
        BYTECOUNT  len_in_bytes                
)
{
    SLLD_STATUS         status = SLLD_OK;

#line 3532 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_READ((0x41),0xFFFFFFFF,data_buf,len_in_bytes);

    return(status);
}











 
SLLD_STATUS slld_PNVDLRCmd
(
        BYTE      *data_buf,                   
        BYTECOUNT  len_in_bytes                
)
{
    SLLD_STATUS         status = SLLD_OK;

#line 3565 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"
    status = slld_WRENCmd();
    if(status != SLLD_OK)
        return(status);
    status = FLASH_WRITE((0x43),0xFFFFFFFF,data_buf,len_in_bytes);

    return(status);
}











 
SLLD_STATUS slld_WVDLRCmd
(
        BYTE      *data_buf,                   
        BYTECOUNT  len_in_bytes                
)
{
    SLLD_STATUS         status = SLLD_OK;

#line 3600 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"
    status = slld_WRENCmd();
    if(status != SLLD_OK)
        return(status);

    status = FLASH_WRITE((0x4A),0xFFFFFFFF,data_buf,len_in_bytes);

    return(status);
}











 
SLLD_STATUS slld_PGSPCmd(void)
{
    SLLD_STATUS status = SLLD_OK;

#line 3632 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0x85),0xFFFFFFFF,(BYTE*)0,0);

    return(status);
}











 
SLLD_STATUS slld_PGRSCmd(void)
{
    SLLD_STATUS status = SLLD_OK;

#line 3661 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0x8A),0xFFFFFFFF,(BYTE*)0,0);

    return(status);
}











 
SLLD_STATUS slld_PLBWRCmd(void)
{
    SLLD_STATUS status = SLLD_OK;

#line 3690 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0xA6),0xFFFFFFFF,(BYTE*)0,0);

    return(status);
}











 
SLLD_STATUS slld_PLBRDCmd
(
        BYTE      *data_buf,                   
        BYTECOUNT  len_in_bytes                
)
{
    SLLD_STATUS         status = SLLD_OK;

#line 3723 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_READ((0xA7),0xFFFFFFFF,data_buf,len_in_bytes);

    return(status);
}












 
SLLD_STATUS slld_DYB_RDCmd
(
        ADDRESS   sys_addr,                       
        BYTE     *data_buf                        
)
{
    SLLD_STATUS status = SLLD_OK;

#line 3757 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_READ((0xE0),sys_addr,data_buf,1);

    return(status);
}











 
SLLD_STATUS slld_PPB_RDCmd
(
        ADDRESS   sys_addr,                       
        BYTE     *data_buf                        
)
{
    SLLD_STATUS status = SLLD_OK;

#line 3790 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_READ((0xE2),sys_addr,data_buf,1);

    return(status);
}











 
SLLD_STATUS slld_PPB_ERSCmd(void)
{
    SLLD_STATUS status = SLLD_OK;

#line 3819 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0xE4),0xFFFFFFFF,(BYTE*)0,0);

    return(status);
}












 
SLLD_STATUS slld_PASSRDCmd
(
        BYTE     *data_buf                        
)
{
    SLLD_STATUS status = SLLD_OK;

#line 3852 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_READ((0xE7),0xFFFFFFFF,data_buf,8);        

    return(status);
}











 
SLLD_STATUS slld_PASSPCmd
(
        BYTE     *data_buf                        
)
{
    SLLD_STATUS status = SLLD_OK;

#line 3884 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0xE8),0xFFFFFFFF,data_buf,8);        

    return(status);
}











 
SLLD_STATUS slld_PASSUCmd
(
        BYTE     *data_buf                        
)
{
    SLLD_STATUS status = SLLD_OK;

#line 3916 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    status = FLASH_WRITE((0xE9),0xFFFFFFFF,data_buf,8);        

    return(status);
}



#line 3959 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

#line 3995 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

#line 4031 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

#line 4068 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

#line 4104 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

#line 4141 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"



#line 4668 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"
SLLD_STATUS slld_ReadOp
(
        ADDRESS   sys_addr,           
        BYTE     *target,             
        BYTECOUNT len_in_bytes        
)
{
    SLLD_STATUS         status = SLLD_OK;
    unsigned long        length;

    BYTE upper_addr;
#line 4688 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    while (len_in_bytes)
    {
#line 4697 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"
        length = len_in_bytes;


        status = slld_DPCmd();             
        if(status != SLLD_OK)
            return(status);

        upper_addr = (BYTE)(sys_addr >> 24);
        status = slld_WRRCmd(&upper_addr,0,0);
        if(status != SLLD_OK)
            return(status);


#line 4728 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"
        status = FLASH_READ((0x03),sys_addr,target,length);



        len_in_bytes -= length;
        sys_addr += length;
        target += length;
    }

    return(status);
}


SLLD_STATUS slld_WriteOp
(
        ADDRESS     sys_addr,                  
        BYTE       *data_buf,                  
        BYTECOUNT   len_in_bytes,              
        DEVSTATUS  *dev_status_ptr             
)
{
    SLLD_STATUS status = SLLD_OK;
    unsigned long length;

    BYTE upper_addr;


    while(len_in_bytes)
    {

        status = slld_DPCmd();             
        if(status != SLLD_OK)
            return(status);

        upper_addr = (BYTE)(sys_addr >> 24);
        status = slld_WRRCmd(&upper_addr,0,0);
        if(status != SLLD_OK)
            return(status);


        if(len_in_bytes < g_PAGE_SIZE)
            length = len_in_bytes;
        else
            length = g_PAGE_SIZE;

        status = slld_WRENCmd();
        if(status != SLLD_OK)
            return(status);
#line 4786 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"
        status = slld_PPCmd(sys_addr, data_buf, length);



        if(status != SLLD_OK)
            return(status);

        status = slld_Poll(dev_status_ptr);
        if(status != SLLD_OK)
            return(status);

        status = slld_WRDICmd();
        if(status != SLLD_OK)
            return(status);

        len_in_bytes -= length;
        sys_addr += length;
        data_buf += length;
    }

    return(status);
}






 
SLLD_STATUS slld_ReadSFDPCmd
(
        ADDRESS    sys_addr,        
        BYTE      *read_buf         
)
{
    SLLD_STATUS status = SLLD_OK;

#line 4831 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    
    status = FLASH_READ((0x5A),sys_addr,read_buf,256);

    return(status);
}










 
SLLD_STATUS slld_ReadModeResetCmd (void)
{
    SLLD_STATUS status = SLLD_OK;
    BYTE Buffer[1];

#line 4861 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"

    Buffer[0] = 0xff;
    status = FLASH_WRITE((0xFF),0xFFFFFFFF,Buffer,1);

    return(status);
}
#line 4904 "..\\Spansion_SLLD_v13.2.1\\src\\slld.c"
 
