; DOSDRV16.INC - DOS Stub Device Driver Includes

CSEG  segment word public 'CODE'
CSEG  ends	;header segment

STAT_OK      equ  0000h     ;ok
STAT_DONE    equ  0100h     ;function complete
STAT_GF      equ  800Ch     ;general failure

RH  EQU  ES:[BX]  ;request header

;**------ Common Request Header Structure ------**
RHC     struc     ;common to all commands
        db  ?     ;length of request header
        db  ?     ;unit code of device
RHC_CMD db  ?     ;command code
RHC_STA dw  ?     ;completion status, 16-bits
        dq  ?     ;reserved for DOS
        db  ?     ;this field varies with command
RHC_OFF dw  ?     ;offset of data
RHC_SEG dw  ?     ;segment of data
RHC_CNT dw  ?     ;byte count (length) of data
RHC     ends      ;end of common portion

; end of file
