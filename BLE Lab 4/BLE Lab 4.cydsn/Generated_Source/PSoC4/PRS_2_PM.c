/*******************************************************************************
* File Name: PRS_2_PM.c
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

#include "PRS_2.h"


/***************************************
* Local data allocation
***************************************/
static PRS_2_BACKUP_STRUCT  PRS_2_backup = 
{
    /* enableState */
    0u,
    /* cr */
    #if(!PRS_2_PULSE_TYPE_HARDCODED)
       (((PRS_2_GREATERTHAN_OR_EQUAL == PRS_2_COMPARE0) ? \
                                                PRS_2_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL : 0u) |
        ((PRS_2_GREATERTHAN_OR_EQUAL == PRS_2_COMPARE1) ? \
                                                PRS_2_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL : 0u)),
    #endif /* End PRS_2_PULSE_TYPE_HARDCODED */
    /* seed */    
    PRS_2_SEED,
    /* seed_copy */    
    PRS_2_SEED,
    /* polynom */
    PRS_2_POLYNOM,
    #if(CY_UDB_V0)
        /* density0 */
        PRS_2_DENSITY0,
        /* density1 */
        PRS_2_DENSITY1,
    #endif /*End CY_UDB_V0*/
};


/*******************************************************************************
* Function Name: PRS_2_SaveConfig
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
*  PRS_2_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PRS_2_SaveConfig(void) 
{
    #if (CY_UDB_V0)
        #if(!PRS_2_PULSE_TYPE_HARDCODED)
            PRS_2_backup.cr = PRS_2_CONTROL_REG;
        #endif /* End PRS_2_PULSE_TYPE_HARDCODED */
        PRS_2_backup.seed = PRS_2_ReadSeed();
        PRS_2_backup.seed_copy = CY_GET_REG8(PRS_2_SEED_COPY_PTR);
        PRS_2_backup.polynom = PRS_2_ReadPolynomial();
        PRS_2_backup.density0 = PRS_2_ReadPulse0();
        PRS_2_backup.density1 = PRS_2_ReadPulse1();
    #else /* CY_UDB_V1 */
        #if(!PRS_2_PULSE_TYPE_HARDCODED)
            PRS_2_backup.cr = PRS_2_CONTROL_REG;
        #endif /* End PRS_2_PULSE_TYPE_HARDCODED */
        PRS_2_backup.seed = PRS_2_ReadSeed();
        PRS_2_backup.seed_copy = CY_GET_REG8(PRS_2_SEED_COPY_PTR);
        PRS_2_backup.polynom = PRS_2_ReadPolynomial();
    #endif  /* CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PRS_2_RestoreConfig
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
*  PRS_2_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PRS_2_RestoreConfig(void) 
{
    #if (CY_UDB_V0)
    
        uint8 enableInterrupts;
        
        #if(!PRS_2_PULSE_TYPE_HARDCODED)
            PRS_2_CONTROL_REG = PRS_2_backup.cr;
        #endif /* End PRS_2_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PRS_2_SEED_COPY_PTR, PRS_2_backup.seed_copy);
        CY_SET_REG8(PRS_2_SEED_PTR, PRS_2_backup.seed);
        PRS_2_WritePolynomial(PRS_2_backup.polynom);
        PRS_2_WritePulse0(PRS_2_backup.density0);
        PRS_2_WritePulse1(PRS_2_backup.density1);
        
        enableInterrupts = CyEnterCriticalSection();
        /* Set FIFO0_CLR bit to use FIFO0 as a simple one-byte buffer*/
        CY_SET_REG8(PRS_2_AUX_CONTROL_PTR, 
                        CY_GET_REG8(PRS_2_AUX_CONTROL_PTR) | PRS_2_FIFO0_CLR);
        CyExitCriticalSection(enableInterrupts);

    #else   /* CY_UDB_V1 */

        #if(!PRS_2_PULSE_TYPE_HARDCODED)
            PRS_2_CONTROL_REG = PRS_2_backup.cr;
        #endif /* End PRS_2_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PRS_2_SEED_COPY_PTR, PRS_2_backup.seed_copy);
        CY_SET_REG8(PRS_2_SEED_PTR, PRS_2_backup.seed);
        PRS_2_WritePolynomial(PRS_2_backup.polynom);
    
    #endif  /* End CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PRS_2_Sleep
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
*  PRS_2_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PRS_2_Sleep(void) 
{
    #if(!PRS_2_PULSE_TYPE_HARDCODED)
        if((PRS_2_CONTROL_REG & PRS_2_CTRL_ENABLE) != 0u) 
        {
            PRS_2_backup.enableState = 1u;
        }
        else
        {
            PRS_2_backup.enableState = 0u;
        }
    #endif /* End PRS_2_PULSE_TYPE_HARDCODED */
    PRS_2_Stop();
    PRS_2_SaveConfig();
}


/*******************************************************************************
* Function Name: PRS_2_Wakeup
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
*  PRS_2_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PRS_2_Wakeup(void) 
{
    PRS_2_RestoreConfig();
    if(PRS_2_backup.enableState != 0u)
    {
        PRS_2_Enable();
    } 
}


/* [] END OF FILE */
