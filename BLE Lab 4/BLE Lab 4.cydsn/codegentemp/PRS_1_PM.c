/*******************************************************************************
* File Name: PRS_1_PM.c
* Version 2.20
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality of the PrISM component
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "PRS_1.h"


/***************************************
* Local data allocation
***************************************/
static PRS_1_BACKUP_STRUCT  PRS_1_backup = 
{
    /* enableState */
    0u,
    /* cr */
    #if(!PRS_1_PULSE_TYPE_HARDCODED)
       (((PRS_1_GREATERTHAN_OR_EQUAL == PRS_1_COMPARE0) ? \
                                                PRS_1_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL : 0u) |
        ((PRS_1_GREATERTHAN_OR_EQUAL == PRS_1_COMPARE1) ? \
                                                PRS_1_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL : 0u)),
    #endif /* End PRS_1_PULSE_TYPE_HARDCODED */
    /* seed */    
    PRS_1_SEED,
    /* seed_copy */    
    PRS_1_SEED,
    /* polynom */
    PRS_1_POLYNOM,
    #if(CY_UDB_V0)
        /* density0 */
        PRS_1_DENSITY0,
        /* density1 */
        PRS_1_DENSITY1,
    #endif /*End CY_UDB_V0*/
};


/*******************************************************************************
* Function Name: PRS_1_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*  
* Parameters:  
*  None.
*
* Return: 
*  None.
*
* Global Variables:
*  PRS_1_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PRS_1_SaveConfig(void) 
{
    #if (CY_UDB_V0)
        #if(!PRS_1_PULSE_TYPE_HARDCODED)
            PRS_1_backup.cr = PRS_1_CONTROL_REG;
        #endif /* End PRS_1_PULSE_TYPE_HARDCODED */
        PRS_1_backup.seed = PRS_1_ReadSeed();
        PRS_1_backup.seed_copy = CY_GET_REG8(PRS_1_SEED_COPY_PTR);
        PRS_1_backup.polynom = PRS_1_ReadPolynomial();
        PRS_1_backup.density0 = PRS_1_ReadPulse0();
        PRS_1_backup.density1 = PRS_1_ReadPulse1();
    #else /* CY_UDB_V1 */
        #if(!PRS_1_PULSE_TYPE_HARDCODED)
            PRS_1_backup.cr = PRS_1_CONTROL_REG;
        #endif /* End PRS_1_PULSE_TYPE_HARDCODED */
        PRS_1_backup.seed = PRS_1_ReadSeed();
        PRS_1_backup.seed_copy = CY_GET_REG8(PRS_1_SEED_COPY_PTR);
        PRS_1_backup.polynom = PRS_1_ReadPolynomial();
    #endif  /* CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PRS_1_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  None.
*
* Return: 
*  None.
*
* Global Variables:
*  PRS_1_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PRS_1_RestoreConfig(void) 
{
    #if (CY_UDB_V0)
    
        uint8 enableInterrupts;
        
        #if(!PRS_1_PULSE_TYPE_HARDCODED)
            PRS_1_CONTROL_REG = PRS_1_backup.cr;
        #endif /* End PRS_1_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PRS_1_SEED_COPY_PTR, PRS_1_backup.seed_copy);
        CY_SET_REG8(PRS_1_SEED_PTR, PRS_1_backup.seed);
        PRS_1_WritePolynomial(PRS_1_backup.polynom);
        PRS_1_WritePulse0(PRS_1_backup.density0);
        PRS_1_WritePulse1(PRS_1_backup.density1);
        
        enableInterrupts = CyEnterCriticalSection();
        /* Set FIFO0_CLR bit to use FIFO0 as a simple one-byte buffer*/
        CY_SET_REG8(PRS_1_AUX_CONTROL_PTR, 
                        CY_GET_REG8(PRS_1_AUX_CONTROL_PTR) | PRS_1_FIFO0_CLR);
        CyExitCriticalSection(enableInterrupts);

    #else   /* CY_UDB_V1 */

        #if(!PRS_1_PULSE_TYPE_HARDCODED)
            PRS_1_CONTROL_REG = PRS_1_backup.cr;
        #endif /* End PRS_1_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PRS_1_SEED_COPY_PTR, PRS_1_backup.seed_copy);
        CY_SET_REG8(PRS_1_SEED_PTR, PRS_1_backup.seed);
        PRS_1_WritePolynomial(PRS_1_backup.polynom);
    
    #endif  /* End CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PRS_1_Sleep
********************************************************************************
*
* Summary:
*  Stops and saves the user configuration
*  
* Parameters:  
*  None.
*
* Return: 
*  None.
*
* Global Variables:
*  PRS_1_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PRS_1_Sleep(void) 
{
    #if(!PRS_1_PULSE_TYPE_HARDCODED)
        if((PRS_1_CONTROL_REG & PRS_1_CTRL_ENABLE) != 0u) 
        {
            PRS_1_backup.enableState = 1u;
        }
        else
        {
            PRS_1_backup.enableState = 0u;
        }
    #endif /* End PRS_1_PULSE_TYPE_HARDCODED */
    PRS_1_Stop();
    PRS_1_SaveConfig();
}


/*******************************************************************************
* Function Name: PRS_1_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  None.
*
* Return: 
*  None.
*
* Global Variables:
*  PRS_1_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PRS_1_Wakeup(void) 
{
    PRS_1_RestoreConfig();
    if(PRS_1_backup.enableState != 0u)
    {
        PRS_1_Enable();
    } 
}


/* [] END OF FILE */
