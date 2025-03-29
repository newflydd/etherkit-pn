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
/*  F i l e               &F: pnpb_list.h                               :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  List macros                                                              */
/*                                                                           */
/*****************************************************************************/
#ifndef PNPB_LIST_H
#define PNPB_LIST_H

#ifdef __cplusplus                       /* If C++ - compiler: Use C linkage */
extern "C"
{
#endif


/*-----------------------------------------------------------------------------
    build the structures in the following way (example TESTSTRUC):
    
    typedef struct _TESTSTRUC
    {
        struct _TESTSTRUC*  Flink;      // always first  element in structure !!
        struct _TESTSTRUC*  Blink;      // always second element in structure !!

        unsigned int        userDat1;   // user defined data 
        unsigned int        userDat2;   // user defined data 
    } TESTSTRUC;

-----------------------------------------------------------------------------*/



// ***** some basic definitions ******
#define PNPB_BOOL    unsigned int
#define PNPB_NULL    0

#define PNPB_PTR_EQUAL(ptr1_,  ptr2_) \
   ( (void*)(ptr1_) == (void*)(ptr2_) )

// ***** memalloc/memfree *****
#define LST_ALLOC(pptr_,size_)  {*pptr=memalloc(size_);}
#define LST_FREE(ptr_)          {memfree(ptr_);}

// *------------------------------------------------------------------------------
// *   circularly linked list macros
// *------------------------------------------------------------------------------
struct pnpb_list_entry_tag {
	struct pnpb_list_entry_tag  * Flink; /* forward link */
	struct pnpb_list_entry_tag  * Blink; /* backward link */
};

typedef struct pnpb_list_entry_tag  PNPB_LIST_ENTRY_TYPE;
typedef struct pnpb_list_entry_tag   *PNPB_LIST_ENTRY_PTR_TYPE;


// * ----------------------------
// *  LIST INIT  
// * ----------------------------

// ***** initialize list *****
#define PnpbListInitialize(ListHead_) {\
	(ListHead_)->Flink = (ListHead_)->Blink = (ListHead_);\
	}


// * ----------------------------
// *  remove entry from list
// * ----------------------------

// ***** remove specified entry *****
#define PnpbListRemoveEntry(Entry_) {\
	Entry_->Flink->Blink = Entry_->Blink;\
	Entry_->Blink->Flink = Entry_->Flink;\
	}

// ***** remove first entry  *****
#define PnpbListRemoveHead(ListHead_, Entry_) {\
	(Entry_) = ((ListHead_)->Flink);\
	PnpbListRemoveEntry((ListHead_)->Flink);\
	}

// * ----------------------------
// *  insert new entry
// * ----------------------------

// ***** insert new element to head *****
#define PnpbListInsertHead(ListHead_, Entry_) {\
	(Entry_)->Flink = (ListHead_)->Flink;\
	(Entry_)->Blink = (ListHead_);\
	(ListHead_)->Flink->Blink = (Entry_);\
	(ListHead_)->Flink = (Entry_);\
	}

// ***** insert new element to tail *****
#define PnpbListInsertTail(ListHead_, Entry_) {\
	(Entry_)->Flink = (ListHead_);\
	(Entry_)->Blink = (ListHead_)->Blink;\
	(ListHead_)->Blink->Flink = (Entry_);\
	(ListHead_)->Blink = (Entry_);\
	}

// ***** insert new element before specified element *****
#define PnpbListInsertBefore(Existing_, Entry_) {\
	PnpbListInsertTail (Existing_, Entry_);\
	}

// * ----------------------------
// *  Fill state
// * ----------------------------

// ***** check if list is empty *****
#define PnpbListIsEmpty(ListHead_) (\
	PNPB_PTR_EQUAL((ListHead_)->Flink, (ListHead_))\
	)

// ***** evaluate list length *****
#define PnpbListLen(ListHead_, LenPtr_) {\
	PNPB_LIST_ENTRY_PTR_TYPE el = (PNPB_LIST_ENTRY_PTR_TYPE)PnpbListFirst (ListHead_);\
	*(LenPtr_) = 0;\
	while (! PNPB_PTR_EQUAL(el,PNPB_NULL)) {\
		*(LenPtr_) += 1;\
		el = PnpbListNext (ListHead_, el);\
	}}


// * ----------------------------
// *  navigation
// * ----------------------------
// ***** read first list element *****
#define PnpbListFirst(ListHead_) (\
	(PnpbListIsEmpty(ListHead_) ? PNPB_NULL : (ListHead_)->Flink)\
	)

// ***** read next list element (NULL, if last element)  *****
#define PnpbListNext(ListHead_, Entry_) (\
	(PNPB_PTR_EQUAL((Entry_)->Flink, (ListHead_)) ? PNPB_NULL : (Entry_)->Flink)\
	)

// ***** read previous element (returns header, if list is empty) *****
#define PnpbListPrevOrHead(Entry_, Type_) (\
	(Type_)((Entry_)->Blink)\
	)


/* usage: for( PnpbListEach(...) ) { ... } -- variant 2 */
#define PnpbListEach(Var_, ListHead_) \
	(Var_) = PnpbListFirst(ListHead_); \
	! PNPB_PTR_EQUAL((Var_), PNPB_NULL); \
	(Var_) = PnpbListNext(ListHead_, (Var_))





#ifdef __cplusplus  /* If C++ - compiler: End of C linkage */
}
#endif


#endif  /* of PNPB_LIST_H */
 
/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
