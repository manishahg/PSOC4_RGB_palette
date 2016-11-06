/*******************************************************************************
* File Name: PRS_Clock.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_PRS_Clock_H)
#define CY_CLOCK_PRS_Clock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void PRS_Clock_StartEx(uint32 alignClkDiv);
#define PRS_Clock_Start() \
    PRS_Clock_StartEx(PRS_Clock__PA_DIV_ID)

#else

void PRS_Clock_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void PRS_Clock_Stop(void);

void PRS_Clock_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 PRS_Clock_GetDividerRegister(void);
uint8  PRS_Clock_GetFractionalDividerRegister(void);

#define PRS_Clock_Enable()                         PRS_Clock_Start()
#define PRS_Clock_Disable()                        PRS_Clock_Stop()
#define PRS_Clock_SetDividerRegister(clkDivider, reset)  \
    PRS_Clock_SetFractionalDividerRegister((clkDivider), 0u)
#define PRS_Clock_SetDivider(clkDivider)           PRS_Clock_SetDividerRegister((clkDivider), 1u)
#define PRS_Clock_SetDividerValue(clkDivider)      PRS_Clock_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define PRS_Clock_DIV_ID     PRS_Clock__DIV_ID

#define PRS_Clock_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define PRS_Clock_CTRL_REG   (*(reg32 *)PRS_Clock__CTRL_REGISTER)
#define PRS_Clock_DIV_REG    (*(reg32 *)PRS_Clock__DIV_REGISTER)

#define PRS_Clock_CMD_DIV_SHIFT          (0u)
#define PRS_Clock_CMD_PA_DIV_SHIFT       (8u)
#define PRS_Clock_CMD_DISABLE_SHIFT      (30u)
#define PRS_Clock_CMD_ENABLE_SHIFT       (31u)

#define PRS_Clock_CMD_DISABLE_MASK       ((uint32)((uint32)1u << PRS_Clock_CMD_DISABLE_SHIFT))
#define PRS_Clock_CMD_ENABLE_MASK        ((uint32)((uint32)1u << PRS_Clock_CMD_ENABLE_SHIFT))

#define PRS_Clock_DIV_FRAC_MASK  (0x000000F8u)
#define PRS_Clock_DIV_FRAC_SHIFT (3u)
#define PRS_Clock_DIV_INT_MASK   (0xFFFFFF00u)
#define PRS_Clock_DIV_INT_SHIFT  (8u)

#else 

#define PRS_Clock_DIV_REG        (*(reg32 *)PRS_Clock__REGISTER)
#define PRS_Clock_ENABLE_REG     PRS_Clock_DIV_REG
#define PRS_Clock_DIV_FRAC_MASK  PRS_Clock__FRAC_MASK
#define PRS_Clock_DIV_FRAC_SHIFT (16u)
#define PRS_Clock_DIV_INT_MASK   PRS_Clock__DIVIDER_MASK
#define PRS_Clock_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_PRS_Clock_H) */

/* [] END OF FILE */
