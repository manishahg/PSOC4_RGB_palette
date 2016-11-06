/*******************************************************************************
* File Name: PRS_2.c
* Version 2.20
*
* Description:
*  This file provides the source code of the API for the PrISM Component.
*
* Note:
*  The PRiSM Component consists of a 8, 16, 24, 32 - bit PrISM, it
*  depends on length Polynomial value and user selected Seed Value. 
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

uint8 PRS_2_initVar = 0u;


/*******************************************************************************
* FUNCTION NAME:   PRS_2_Start
********************************************************************************
*
* Summary:
*  The start function sets Polynomial, Seed and Pulse Density registers provided
*  by customizer. PrISM computation starts on rising edge of input clock.
*
* Parameters:  
*  None.
*
* Return:  
*  None.
*
* Global variables:
*  The PRS_2_initVar variable is used to indicate initial
*  configuration of this component.  The variable is initialized to zero and
*  set to 1 the first time PRS_2_Start() is called. This allows for
*  component initialization without re-initialization in all subsequent calls
*  to the PRS_2_Start() routine. 

* Reentrant:
*  No.
*
*******************************************************************************/
void PRS_2_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(PRS_2_initVar == 0u)
    {
        PRS_2_Init();
        PRS_2_initVar = 1u;
    }
    PRS_2_Enable();
}


/*******************************************************************************
* Function Name: PRS_2_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  PRS_2_Start().
*
* Parameters:  
*  None.
*
* Return: 
*  None.
*
*******************************************************************************/
void PRS_2_Init(void) 
{
    uint8 enableInterrupts;
    
    /* Writes Seed value, polynom value and density provided by customizer */
    PRS_2_WriteSeed(PRS_2_SEED);
    PRS_2_WritePolynomial(PRS_2_POLYNOM);
    PRS_2_WritePulse0(PRS_2_DENSITY0);
    PRS_2_WritePulse1(PRS_2_DENSITY1);
    
    enableInterrupts = CyEnterCriticalSection();
    /* Set FIFO0_CLR bit to use FIFO0 as a simple one-byte buffer*/
    CY_SET_REG8(PRS_2_AUX_CONTROL_PTR, 
                        CY_GET_REG8(PRS_2_AUX_CONTROL_PTR) | PRS_2_FIFO0_CLR);
    CyExitCriticalSection(enableInterrupts);
    
    #if(!PRS_2_PULSE_TYPE_HARDCODED)
        /* Writes density type provided by customizer */
        #if(PRS_2_GREATERTHAN_OR_EQUAL == PRS_2_COMPARE0)
            PRS_2_CONTROL_REG |= PRS_2_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL;
        #else
            PRS_2_CONTROL_REG &= (uint8)~PRS_2_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL;
        #endif /* End PRS_2_COMPARE0 */    
    
        #if(PRS_2_GREATERTHAN_OR_EQUAL == PRS_2_COMPARE1)
            PRS_2_CONTROL_REG |= PRS_2_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL;
        #else
            PRS_2_CONTROL_REG &= (uint8)~PRS_2_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL;
        #endif /* End PRS_2_COMPARE1 */    
    #endif /* End PRS_2_PULSE_TYPE_HARDCODED */
}


/*******************************************************************************
* Function Name: PRS_2_Enable
********************************************************************************
*  
* Summary: 
*  Enables the PrISM block operation
*
* Parameters:  
*  None.
*
* Return: 
*  None.
*
*******************************************************************************/
void PRS_2_Enable(void) 
{
    #if(!PRS_2_PULSE_TYPE_HARDCODED)
        /* Enable the PrISM computation */
        PRS_2_CONTROL_REG |= PRS_2_CTRL_ENABLE;
    #endif /* End PRS_2_PULSE_TYPE_HARDCODED */
}


/*******************************************************************************
* FUNCTION NAME:   PRS_2_Stop
********************************************************************************
*
* Summary:
*  Stops PrISM computation. Outputs remain constant.
*
* Parameters:  
*  None.
*
* Return: 
*  None.
*
*******************************************************************************/
void PRS_2_Stop(void) 
{
    #if(!PRS_2_PULSE_TYPE_HARDCODED)
        PRS_2_CONTROL_REG &= (uint8)~PRS_2_CTRL_ENABLE;
    #else
        /* PulseTypeHardoded option enabled - to stop PrISM use enable input */
    #endif /* End $INSTANCE_NAME`_PULSE_TYPE_HARDCODED */
}

#if(!PRS_2_PULSE_TYPE_HARDCODED)


    /***************************************************************************
    * FUNCTION NAME:     PRS_2_SetPulse0Mode
    ****************************************************************************
    *
    * Summary:
    *  Sets the pulse density type for Density0. Less than or Equal(<=) or 
    *  Greater than or Equal(>=).
    *
    * Parameters:
    *  pulse0Type: Selected pulse density type.
    *
    * Return:
    *  None.
    *
    ***************************************************************************/
    void PRS_2_SetPulse0Mode(uint8 pulse0Type) 
    {
        if(pulse0Type == PRS_2_GREATERTHAN_OR_EQUAL)
        {
            PRS_2_CONTROL_REG |= PRS_2_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL;
        }
        else
        {
            PRS_2_CONTROL_REG &= (uint8)~PRS_2_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL;
        }
    }
    
    
    /***************************************************************************
    * FUNCTION NAME:   PRS_2_SetPulse1Mode
    ****************************************************************************
    *
    * Summary:
    *  Sets the pulse density type for Density1. Less than or Equal(<=) or 
    *  Greater than or Equal(>=).
    *
    * Parameters:  
    *  pulse1Type: Selected pulse density type.
    *
    * Return:
    *  None.
    *
    ***************************************************************************/
    void PRS_2_SetPulse1Mode(uint8 pulse1Type) 
    {
        if(pulse1Type == PRS_2_GREATERTHAN_OR_EQUAL)
        {
            PRS_2_CONTROL_REG |= PRS_2_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL;
        }
        else
        {
            PRS_2_CONTROL_REG &= (uint8)~PRS_2_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL;
        }
    }

#endif /* End PRS_2_PULSE_TYPE_HARDCODED == 0 */


/*******************************************************************************
* FUNCTION NAME:   PRS_2_ReadSeed
********************************************************************************
*
* Summary:
*  Reads the PrISM Seed register.
*
* Parameters:
*  None.
*
* Return:
*  Current Period register value.
*
*******************************************************************************/
uint8 PRS_2_ReadSeed(void) 
{
    return( CY_GET_REG8(PRS_2_SEED_PTR) );
}


/*******************************************************************************
* FUNCTION NAME:   PRS_2_WriteSeed
********************************************************************************
*
* Summary:
*  Writes the PrISM Seed register with the start value.
*
* Parameters:
*  seed: Seed register value.
*
* Return:
*  None.
*
*******************************************************************************/
void PRS_2_WriteSeed(uint8 seed) 
{
    if(seed != 0u)
    {
        CY_SET_REG8(PRS_2_SEED_COPY_PTR, seed);
        CY_SET_REG8(PRS_2_SEED_PTR, seed);
    }
}


/*******************************************************************************
* FUNCTION NAME:   PRS_2_ReadPolynomial
********************************************************************************
*
* Summary:
*  Reads the PrISM polynomial.
*
* Parameters:
*  None.
*
* Return:
*  PrISM polynomial.
*
*******************************************************************************/
uint8 PRS_2_ReadPolynomial(void) 
{
    return( CY_GET_REG8(PRS_2_POLYNOM_PTR) );
}


/*******************************************************************************
* FUNCTION NAME:   PRS_2_WritePolynomial
********************************************************************************
*
* Summary:
*  Writes the PrISM polynomial.
*
* Parameters:
*  polynomial: PrISM polynomial.
*
* Return:
*  None.
*
*******************************************************************************/
void PRS_2_WritePolynomial(uint8 polynomial) \
                                                                
{
    CY_SET_REG8(PRS_2_POLYNOM_PTR, polynomial);
    
}


/*******************************************************************************
* FUNCTION NAME:   PRS_2_ReadPulse0
********************************************************************************
*
* Summary:
*  Reads the PrISM Pulse Density0 register.
*
* Parameters:
*  None.
*
* Return:
*  Pulse Density0 register value.
*
*******************************************************************************/
uint8 PRS_2_ReadPulse0(void) 
{
    return( CY_GET_REG8(PRS_2_DENSITY0_PTR) );
}


/*******************************************************************************
* FUNCTION NAME:   PRS_2_WritePulse0
********************************************************************************
*
* Summary:
*  Writes the PrISM Pulse Density0 register with the Pulse Density value.
*
* Parameters:
*  pulseDensity0: Pulse Density value.
*
* Return:
*  None.
*
*******************************************************************************/
void PRS_2_WritePulse0(uint8 pulseDensity0) \
                                                                
{
    CY_SET_REG8(PRS_2_DENSITY0_PTR, pulseDensity0);
}


/*******************************************************************************
* FUNCTION NAME:   PRS_2_ReadPulse1
********************************************************************************
*
* Summary:
*  Reads the PrISM Pulse Density1 register.
*
* Parameters:
*  None.
*
* Return:
*  Pulse Density1 register value.
*
*******************************************************************************/
uint8 PRS_2_ReadPulse1(void) 
{
    return( CY_GET_REG8(PRS_2_DENSITY1_PTR) );
}


/*******************************************************************************
* FUNCTION NAME:   PRS_2_WritePulse1
********************************************************************************
*
* Summary:
*  Writes the PrISM Pulse Density1 register with the Pulse Density value.
*
* Parameters:
*  pulseDensity1: Pulse Density value.
*
* Return:
*  None.
*
*******************************************************************************/
void PRS_2_WritePulse1(uint8 pulseDensity1) \
                                                                    
{
    CY_SET_REG8(PRS_2_DENSITY1_PTR, pulseDensity1);
}


/* [] END OF FILE */
