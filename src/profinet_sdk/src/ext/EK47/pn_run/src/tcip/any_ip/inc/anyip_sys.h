#ifndef __ANYIP_SYS_H__
#define __ANYIP_SYS_H__

/**************************************************************************//**
@file		anyip_sys.h
@brief		Contains functions for managing the IP stack.																			
******************************************************************************/

/**************************************************************************//**
@ingroup	anyip_system
@brief		Contains all IP-stack specific actions that must be taken when
			a TCIP channel is opened.
																			
@param[in]	channel:	TCIP channel which is opened
******************************************************************************/
void anyip_init_channel(void *channel);

/**************************************************************************//**
@ingroup	anyip_system
@brief		Contains all IP-stack specific actions that must be taken when
			a TCIP channel is closed.
																			
@param[in]	channel:	TCIP channel which is closed
******************************************************************************/
void anyip_deinit_channel(void *channel);

/**************************************************************************//**
@ingroup	anyip_system
@brief		Initializes the IP stack

@note		The function is called during tcip_init()
																			
@return		0: in case of success
@return		-1: in case of an error
******************************************************************************/
int anyip_init(void);

/**************************************************************************//**
@ingroup	anyip_system
@brief		Is called every 100 millisecond and should be used to update the 
			IP-stack specific timer.

@note		The function is called by TCIP every 100 milliseconds.
******************************************************************************/
void anyip_ctick(void);

/**************************************************************************//**
@ingroup	anyip_system
@brief		De-initializes the IP stack

@note		The function is called during tcip_undo_init()
******************************************************************************/
void anyip_deinit();

#endif
