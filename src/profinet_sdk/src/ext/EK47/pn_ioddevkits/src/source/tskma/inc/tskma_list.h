/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
/*  This program is protected by German copyright law and international      */
/*  treaties. The use of this software including but not limited to its      */
/*  Source Code is subject to restrictions as agreed in the license          */
/*  agreement between you and Siemens.                                       */
/*  Copying or distribution is not allowed unless expressly permitted        */
/*  according to your license agreement with Siemens.                        */
/*****************************************************************************/
/*                                                                           */
/*  P r o j e c t         &P: PROFINET IO Runtime Software              :P&  */
/*                                                                           */
/*  P a c k a g e         &W: PROFINET IO Runtime Software              :W&  */
/*                                                                           */
/*  C o m p o n e n t     &C: PnIODDevkits                              :C&  */
/*                                                                           */
/*  F i l e               &F: tskma_list.h                              :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*   TSKMA double pointered lists                                            */
/*                                                                           */
/*****************************************************************************/


#ifndef TSKMA_LIST_H
#define TSKMA_LIST_H

/*****************************************************************************/
/*                                 STRUCTURES                                */
/*****************************************************************************/
struct  tskma_list_cb
{
	struct  tskma_list_cb  *next_blk_ptr;
	struct  tskma_list_cb  *prev_blk_ptr;
};

/*****************************************************************************/
/*                                   CASTs                                   */
/*****************************************************************************/
#define TSKMA_CAST_LIST_CB_PTR                (struct tskma_list_cb *)
#define TSKMA_CAST_LIST_CB_PTR_PTR            (struct tskma_list_cb * *)

/*****************************************************************************/
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  double pointered list                                                    */
/*****************************************************************************/
/*                                                                           */
/*     +-------------------------------------------------------+             */
/*     |                                                       |             */
/*     |    list_cb                                            |             */
/*     |    +------+     +------+     +------+     +------+    |             */
/*     +--> | next | --> | next | --> | next | ... | next | ---+             */
/*     +--- | prev | <-- | prev | <-- | prev | ... | prev | <--+             */
/*     |    +------+     | ...  |     | ...  |     | ...  |    |             */
/*     |                 +------+     +------+     +------+    |             */
/*     |                                                       |             */
/*     +-------------------------------------------------------+             */
/*                                                                           */
/*****************************************************************************/


/*****************************************************************************/
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  initialize desired double pointered list                                 */
/*****************************************************************************/
#define tskma_init_list__(list_ptr)                                                             \
{                                                                                               \
	(TSKMA_CAST_LIST_CB_PTR (list_ptr)) -> next_blk_ptr =                                    	\
	(TSKMA_CAST_LIST_CB_PTR (list_ptr)) -> prev_blk_ptr = TSKMA_CAST_LIST_CB_PTR (list_ptr);  	\
}                                                                                               \


/*****************************************************************************/
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  check if desired double pointered list empty                             */
/*****************************************************************************/
#define tskma_list_empty__(list_ptr)                                                            \
																								\
	(TSKMA_CAST_LIST_CB_PTR (list_ptr)) -> next_blk_ptr == TSKMA_CAST_LIST_CB_PTR (list_ptr)  	\


/*****************************************************************************/
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  check if desired double pointered list not empty                         */
/*****************************************************************************/
#define tskma_list_not_empty__(list_ptr)                                                        \
																								\
	(TSKMA_CAST_LIST_CB_PTR (list_ptr)) -> next_blk_ptr != TSKMA_CAST_LIST_CB_PTR (list_ptr)  	\


/*****************************************************************************/
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  put block to begin of double pointered list                              */
/*****************************************************************************/
#define tskma_put_blk_to_begin_of_list__(list_ptr, blk_ptr)                                                 	\
{                                                                                                               \
	/* blk.next = list.next */                                                                              	\
	(TSKMA_CAST_LIST_CB_PTR (blk_ptr)) -> next_blk_ptr = (TSKMA_CAST_LIST_CB_PTR (list_ptr)) -> next_blk_ptr; 	\
																												\
	/* blk.prev = &list */                                                                                  	\
	(TSKMA_CAST_LIST_CB_PTR (blk_ptr)) -> prev_blk_ptr =  TSKMA_CAST_LIST_CB_PTR (list_ptr);                	\
																												\
	/* list.next -> prev = &blk */                                                                          	\
	(TSKMA_CAST_LIST_CB_PTR (list_ptr)) -> next_blk_ptr -> prev_blk_ptr = TSKMA_CAST_LIST_CB_PTR (blk_ptr); 	\
																												\
	/* list.next = &blk */                                                                                  	\
	(TSKMA_CAST_LIST_CB_PTR (list_ptr)) -> next_blk_ptr = TSKMA_CAST_LIST_CB_PTR (blk_ptr);                 	\
																												\
}                                                                                                       		\


/*****************************************************************************/
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  put block to end of double pointered list                                */
/*****************************************************************************/
#define tskma_put_blk_to_list__(list_ptr, blk_ptr)                                                              \
{                                                                                                               \
	/* blk.next = &list */                                                                                  	\
	(TSKMA_CAST_LIST_CB_PTR (blk_ptr)) -> next_blk_ptr = TSKMA_CAST_LIST_CB_PTR (list_ptr);                   	\
																												\
	/* list.prev -> next = blk */                                                                           	\
	(TSKMA_CAST_LIST_CB_PTR (list_ptr)) -> prev_blk_ptr -> next_blk_ptr = TSKMA_CAST_LIST_CB_PTR (blk_ptr);   	\
																												\
	/* blk.prev = list.prev */                                                                              	\
	(TSKMA_CAST_LIST_CB_PTR (blk_ptr)) -> prev_blk_ptr = (TSKMA_CAST_LIST_CB_PTR (list_ptr)) -> prev_blk_ptr; 	\
																												\
	/* list.prev = blk */                                                                                   	\
	(TSKMA_CAST_LIST_CB_PTR (list_ptr)) -> prev_blk_ptr = TSKMA_CAST_LIST_CB_PTR (blk_ptr);                   	\
}                                                                                                               \


/*****************************************************************************/
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  get and remove block from start of desired double pointered list         */
/*****************************************************************************/
#define tskma_get_remove_blk_from_list__(list_ptr, adr_blk_ptr)      		\
{                                                                         	\
		tskma_get_blk_from_list__((list_ptr), (adr_blk_ptr));              	\
																			\
		tskma_remove_blk_from_list__(list_ptr);                            	\
}                                                                        	\


/*****************************************************************************/
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  remove block from anywhere of double pointered list                      */
/*****************************************************************************/
#define tskma_remove_blk__(blk_ptr)                                                                                             \
{                                                                                                                               \
	/* blk.prev -> next = blk.next */                                                                                       	\
	(TSKMA_CAST_LIST_CB_PTR (blk_ptr)) -> prev_blk_ptr -> next_blk_ptr = (TSKMA_CAST_LIST_CB_PTR (blk_ptr)) -> next_blk_ptr; 	\
																																\
	/* blk.next -> prev = blk.prev */                                                                                       	\
	(TSKMA_CAST_LIST_CB_PTR (blk_ptr)) -> next_blk_ptr -> prev_blk_ptr = (TSKMA_CAST_LIST_CB_PTR (blk_ptr)) -> prev_blk_ptr; 	\
}                                                                                                                               \


/*****************************************************************************/
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  get block from start of desired double pointered list                    */
/*****************************************************************************/
#define tskma_get_blk_from_list__(list_ptr, adr_blk_ptr)                                                    \
{                                                                                                           \
	/* blk = list.next */                                                                               	\
	*(TSKMA_CAST_LIST_CB_PTR_PTR (adr_blk_ptr)) = (TSKMA_CAST_LIST_CB_PTR (list_ptr)) -> next_blk_ptr;    	\
}                                                                                                           \


/*****************************************************************************/
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  remove block from start of desired double pointered list                 */
/*****************************************************************************/
#define tskma_remove_blk_from_list__(list_ptr)                                                                                  \
{                                                                                                                               \
	/* list.next = list.next -> next */                                                                                     	\
	(TSKMA_CAST_LIST_CB_PTR (list_ptr)) -> next_blk_ptr = (TSKMA_CAST_LIST_CB_PTR (list_ptr)) -> next_blk_ptr -> next_blk_ptr;	\
																																\
	/* list.next -> prev = &list */                                                                                         	\
	(TSKMA_CAST_LIST_CB_PTR (list_ptr)) -> next_blk_ptr -> prev_blk_ptr = TSKMA_CAST_LIST_CB_PTR (list_ptr);                  	\
}                                                                                                                               \


#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
