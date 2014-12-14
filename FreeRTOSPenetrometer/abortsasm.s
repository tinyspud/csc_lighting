;
;	SafeRTOS Copyright (C) Wittenstein High Integrity Systems.
;
;	See projdefs.h for version number information.
;
;	SafeRTOS is distributed exclusively by Wittenstein High Integrity Systems,
;	and is subject to the terms of the License granted to your organization,
;	including its warranties and limitations on distribution.  It cannot be
;	copied or reproduced in any way except as permitted by the License.
;
;	Licenses are issued for each concurrent user working on a specified product
;	line.
;
;	WITTENSTEIN high integrity systems is a trading name of WITTENSTEIN
;	aerospace & simulation ltd, Registered Office: Brown's Court, Long Ashton
;	Business Park, Yanley Lane, Long Ashton, Bristol, BS41 9LB, UK.
;	Tel: +44 (0) 1275 395 600, fax: +44 (0) 1275 393 630.
;	E-mail: info@HighIntegritySystems.com
;	Registered in England No. 3711047; VAT No. GB 729 1583 15
;
;	http://www.SafeRTOS.com
;
;-------------------------------------------------------------------------------

	.text
	.state32

;-------------------------------------------------------------------------------
; Public Functions
;-------------------------------------------------------------------------------

	.global ulGetDataFaultStatusRegister
	.global ulGetDataFaultAddressRegister
	.global ulGetInstructionFaultStatusRegister
	.global ulGetInstructionFaultAddressRegister

;-------------------------------------------------------------------------------
; portUInt32Type ulGetDataFaultStatusRegister( void )
;-------------------------------------------------------------------------------
ulGetDataFaultStatusRegister:
		MRC 	p15, #0, r0, c5, c0, #0
		BX 		LR

;-------------------------------------------------------------------------------
; portUInt32Type ulGetDataFaultAddressRegister( void )
;-------------------------------------------------------------------------------
ulGetDataFaultAddressRegister:
		MRC		p15, #0, r0, c6, c0, #0
		BX		LR

;-------------------------------------------------------------------------------
; portUInt32Type ulGetInstructionFaultStatusRegister( void )
;-------------------------------------------------------------------------------
ulGetInstructionFaultStatusRegister:
		MRC		p15, #0, r0, c5, c0, #1
		BX		LR

;-------------------------------------------------------------------------------
; portUInt32Type ulGetInstructionFaultAddressRegister( void )
;-------------------------------------------------------------------------------
ulGetInstructionFaultAddressRegister:
		MRC		p15, #0, r0, c6, c0, #2
		BX		LR

;-------------------------------------------------------------------------------
