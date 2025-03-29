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
/*  F i l e               &F: pnpb_rec8028_8029.c                       :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  This module contains an example code how to handle pnio I&M (identifica- */
/*  tion and maintanance) functions. Support of I&M0..I&M4 is mandatory for  */
/*  the DAP. All other I&M is optional.                                      */
/*                                                                           */
/*  THIS MODULE HAS TO BE MODIFIED BY THE PNIO USER                          */
/*                                                                           */
/*****************************************************************************/

#include "compiler.h"

#if IOD_INCLUDE_REC8028_8029

    #include "pniousrd.h"
    #include "trc_if.h"

    #include "pndv_inc.h"
    #include "pnpb_api.h"
    #include "pnpb_sub_real.h"
    #include "pnpb_sub_exp.h"
    #include "pnpb_trc.h"
    #include "pnpb_rec8028_8029.h"
    #include "pnpb.h"
    #include "pnpb_io.h"
    #include "pnpb_com.h"

    #define LTRC_ACT_MODUL_ID   216
    #define PNPB_MODULE_ID      216

    // *---------------------------------------------------
    // *  structure of IN data (see PNIO specification):
    // *
    // *   REC_IO_INPUT_STRUCT
    // *---------------------------------------------------
    //lint -e(578) Declaration of symbol 'Symbol' hides symbol 'Symbol' (Location)
    typedef ATTR_PNIO_PACKED_PRE struct
    {
       REC_IO_BLOCKHDR      BlockHeader;                // BlockHeader
       PNIO_UINT8           LengthIOCS;                 // LengthIOCS
       PNIO_UINT8           IOCS;                       // iocs
       PNIO_UINT8           LengthIOPS;                 // LengthIOPS
       PNIO_UINT8           IOPS;                       // iops
       PNIO_UINT16          LengthData;                 // LengthData
    }  ATTR_PNIO_PACKED REC_IO_INPUT_STRUCT; // more details see pnio spec


    // *---------------------------------------------------
    // *  structure of OUT data (see PNIO specification):
    // *
    // *   REC_IO_OUTPUT_STRUCT         (with BlockHeader type = 0x0016 = output)
    // *   iocs                         (iocs value)
    // *   OutputData                   (normal output data)
    // *   iops                         (iops value)
    // *   REC_IO_SUBSTVAL_STRUCT       (with BlockHeader type = 0x0014 = substituted)
    // *   iocs                         (iocs value)
    // *   SubstituteData               (substitute output data)
    // *   Substitute data valid        (coded as iops and iocs)
    // *---------------------------------------------------
    typedef ATTR_PNIO_PACKED_PRE struct
    {
       REC_IO_BLOCKHDR      BlockHeader;                // BlockHeader
       PNIO_UINT16          SubstActiveFlag;            // SubstituteActiveFlag:  0=operation, 1=substitute
       PNIO_UINT8           LengthIOCS;                 // LengthIOCS
       PNIO_UINT8           LengthIOPS;                 // LengthIOPS
       PNIO_UINT16          LengthData;                 // LengthData
    }  ATTR_PNIO_PACKED REC_IO_OUTPUT_STRUCT; // more details see pnio spec

    typedef ATTR_PNIO_PACKED_PRE struct
    {
       REC_IO_BLOCKHDR      BlockHeader;                // BlockHeader
       PNIO_UINT16          SubstMode;                  // SubstitutionMode: 0=ZERO or inactive, 1:LastValue, 2:Replacement value
    }  ATTR_PNIO_PACKED REC_IO_SUBSTVAL_STRUCT;// more details see pnio spec



    // *----------------------------------------------------------------*
    // *  pnpb_rec8028_Handler  (....)
    // *----------------------------------------------------------------*
    // *  this function handles a record request on RecordIndex 0x8028,
    // *  which is specified as a "read input data" request.
    // *  If an error occures, the error structure PNIO_ERR_STAT is filled.
    // *  See PNIO specification for more details, how to fill the error
    // *  structure.
    // *----------------------------------------------------------------*
    PNIO_UINT32  pnpb_rec8028_Handler
		    (
	            PNIO_UINT32         ArIndExp,       // expected AR-index from the record read request index 0x8029
                PNIO_UINT32         Api,			// api number
			    PNIO_DEV_ADDR		*pAddr, 		// geographical or logical address
			    PNIO_UINT32			*pBufLen,		// [in, out] in: length to read, out: length, read by user
			    PNIO_UINT8			*pBuffer,		// [in] buffer pointer
			    PNIO_ERR_STAT		*pPnioState		// [out] 4 byte PNIOStatus (ErrCode, ErrDecode, ErrCode1, ErrCode2), see IEC61158-6
		    )
    {
        REC_IO_INPUT_STRUCT*     pIo = (REC_IO_INPUT_STRUCT*)    pBuffer;
        PNIO_UINT32              ExpectedBufLen;
        PNIO_UINT16              RecIoStrucSize = sizeof (REC_IO_INPUT_STRUCT);
        PNIO_EXP_SUB*            pExpSub;
        PNIO_UINT32              ArInd;
        PNIO_UINT32              Status;

        // *** clear error structure ***
        OsMemSet (pPnioState, 0, sizeof (PNIO_ERR_STAT));

        // *** get pointer to expected submodule ***
        pExpSub = pnpb_sub_exp_allAr_getp   (&ArInd, Api, pAddr->Geo.Slot, pAddr->Geo.Subslot);

        // **** plausibility check: module is present, an output module with datalength > 0 and correct AR-index ?? ***
         if (    (pExpSub                  == NULL)
              || ((pExpSub->In.data_length  == 0) && (pAddr->Geo.Slot != IOD_CFG_DAP_SLOT_NUMBER))  // we have no io data inside the DAP subslots
              || (pExpSub->IsWrongSubmod   == PNIO_TRUE)
              || (pExpSub->OwnSessionKey   == 0)             // AR is not owner of the submodule
             )
         { // **** error "invalid index" ****
             pPnioState->ErrCode   = 0xde;  // IODReadRes with ErrorDecode = PNIORW
             pPnioState->ErrDecode = 0x80;  // PNIORW
             pPnioState->ErrCode1  = 0xb0;  // example: Error Class 11 , ErrorNr  = ""
             return (PNIO_NOT_OK);
         }

        // *** get buffer length, buffer consists of REC_IO_INPUT_STRUCT + io-data length  ***
        ExpectedBufLen = RecIoStrucSize + pExpSub->In.data_length;

        // *** plausibility check buffer length, set total data length ***
        if (*pBufLen <  ExpectedBufLen)        // plausibility check
        { // **** error ****
            pPnioState->ErrCode   = 0xde;  // IODReadRes with ErrorDecode = PNIORW
            pPnioState->ErrDecode = 0x80;  // PNIORW
            pPnioState->ErrCode1  = 0xc0;  // example: Error Class 12 = resource, ErrorNr  = "write constrain conflict"
            return (PNIO_NOT_OK);
        }

        // *** return value = real blocklength ***
        *pBufLen = ExpectedBufLen;

        // *** set blockheader ***
        pIo->BlockHeader.Type      = OsHtons (0x0015);              // must be 0x15 for RecordInputDataObjectElement
        pIo->BlockHeader.Len       = OsHtons ((PNIO_UINT16)(ExpectedBufLen - sizeof (REC_IO_BLOCKHDR) + 2)); // blocklength, header excluded
        pIo->BlockHeader.Version   = OsHtons (0x0100);                      // must be 0x0100 (see PNIO specification)

        // *** set length and values of IOCS and IOPS value ***
        pIo->LengthData            = OsHtons ((PNIO_UINT16)pExpSub->In.data_length);   // input data: length includes data without iops,iocs
        pIo->LengthIOCS            = 1;
        pIo->LengthIOPS            = 1;

        Status = pnpb_get_input_data   (ArInd,                          // AR-index = AR number - 1
                                        pExpSub,                        // pointer to expected submodule
                                        & (*(pBuffer + RecIoStrucSize)),// pointer to input data buffer (destination)
                                        &(pIo->IOPS),                   // pointer to IOPS (destination)
                                        &(pIo->IOCS));                  // pointer to IOCS (destination)
        return (Status);
    }


    // *----------------------------------------------------------------*
    // *  pnpb_rec8029_Handler  (....)
    // *----------------------------------------------------------------*
    // *  this function handles a record request on RecordIndex 0x8029,
    // *  which is specified as a "read output data" request.
    // *  If an error occures, the error structure PNIO_ERR_STAT is filled.
    // *  See PNIO specification for more details, how to fill the error
    // *  structure.
    // *----------------------------------------------------------------*
    PNIO_UINT32  pnpb_rec8029_Handler
            (
                PNIO_UINT32         ArIndExp,       // expected AR-index from the record read request index 0x8029
                PNIO_UINT32         Api,            // api number
                PNIO_DEV_ADDR       *pAddr,         // geographical or logical address
                PNIO_UINT32         *pBufLen,       // [in, out] in: length to read, out: length, read by user
                PNIO_UINT8          *pBuffer,       // [in] buffer pointer
                PNIO_ERR_STAT       *pPnioState     // [out] 4 byte PNIOStatus (ErrCode, ErrDecode, ErrCode1, ErrCode2), see IEC61158-6
            )
    {

        REC_IO_OUTPUT_STRUCT*    pIo = (REC_IO_OUTPUT_STRUCT*) pBuffer;
        PNIO_UINT32              ExpectedBufLen;
        PNIO_UINT16              RecIoStrucSize = sizeof (REC_IO_OUTPUT_STRUCT);
        PNIO_UINT32              BufOffset = 0;
        PNIO_EXP_SUB*            pExpSub;
        PNIO_UINT32              ArInd;
        PNIO_UINT32              Status;
        PNIO_UINT8               RemIops;
        PNIO_UINT8               LocIocs;
        PNIO_UINT8               SubstDatValid;

        // *** clear error structure ***
        OsMemSet (pPnioState, 0, sizeof (PNIO_ERR_STAT));

        // *** get pointer to expected submodule ***
        pExpSub = pnpb_sub_exp_allAr_getp(&ArInd, Api, pAddr->Geo.Slot, pAddr->Geo.Subslot);

       // **** plausibility check: module is present, an output module with datalength > 0 and correct AR-index ?? ***
        if (    (pExpSub                  == NULL)
             || (pExpSub->Out.data_length == 0)             // we have no io data inside the DAP subslots
             || (pExpSub->IsWrongSubmod   == PNIO_TRUE)
             || (pExpSub->OwnSessionKey   == 0)             // AR is not owner of the submodule
            )
        { // **** error "invalid index" ****
            pPnioState->ErrCode   = 0xde;  // IODReadRes with ErrorDecode = PNIORW
            pPnioState->ErrDecode = 0x80;  // PNIORW
            pPnioState->ErrCode1  = 0xb0;  // example: Error Class 11 , ErrorNr  = ""
            return (PNIO_NOT_OK);
        }

        // *** get buffer length, buffer consists of REC_IO_OUTPUT_STRUCT + io-data length  ***
        ExpectedBufLen = RecIoStrucSize                       // size of the output header structure (iocs is included)
                          + 1                                 // IOCS
                          + pExpSub->Out.data_length          // size of normal io data
                          + 1                                 // IOPS-len
                          + sizeof (REC_IO_SUBSTVAL_STRUCT)   // size of substituted value struct.
                          + 1                                 // IOCS-len of substituted data
                          + pExpSub->Out.data_length          // size of substituted data (== size of normal io data)
                          + 1;                                // size of SubstituteDataValid

        // *** plausibility check buffer length, set total data length ***
        if (*pBufLen <  ExpectedBufLen)        // plausibility check
        { // **** error ****
            pPnioState->ErrCode   = 0xde;  // IODReadRes with ErrorDecode = PNIORW
            pPnioState->ErrDecode = 0x80;  // PNIORW
            pPnioState->ErrCode1  = 0xb0;  // example: Error Class 11 = resource, ErrorNr  = ""
            return (PNIO_NOT_OK);
        }

        // *** return value = real blocklength ***
        *pBufLen = ExpectedBufLen;

        // *** set blockheader ***
        pIo->BlockHeader.Type      = OsHtons (0x0016);              // must be 0x16 for RecordOutputDataObjectElement
        pIo->BlockHeader.Len       = OsHtons ((PNIO_UINT16)(ExpectedBufLen - sizeof (REC_IO_BLOCKHDR) + 2)); // blocklength, iops and header excluded
        pIo->BlockHeader.Version   = OsHtons (0x0100);              // must be 0x01 (see PNIO specification)

        // *** set length and values of IOCS and IOPS value ***
        pIo->LengthData            = OsHtons ((PNIO_UINT16)pExpSub->Out.data_length);   // input data: length includes data without iops,iocs
        pIo->LengthIOCS            = 1;
        pIo->LengthIOPS            = 1;

        Status = pnpb_get_output_data  (ArInd,                              // AR-index = AR number - 1
                                        pExpSub,                            // pointer to expected submodule
                                        & (*(pBuffer + RecIoStrucSize + 1)),// IOCS is  located between REC_IO_BLOCKHDR and data block
                                        &RemIops,                           // pointer to IOPS (destination)
                                        &LocIocs);                          // pointer to IOCS (destination)



        // *** set IOCS, located between REC_IO_BLOCKHDR and data block ***
        BufOffset = RecIoStrucSize;  // set buffer offset behind REC_IO_BLOCKHDR
        *(pBuffer + BufOffset) = LocIocs;

        //*** set buffer offset behind output data (consider 1 byte IOCS)***
        BufOffset = BufOffset + pExpSub->Out.data_length + 1;

        // *** set IOPS behind IO data block ***
#if(IOD_INCLUDE_S2_REDUNDANCY == 1)
        if((PNIO_AR_TYPE_SINGLE_SYSRED == PnpbExp[ArInd].ArType) && (pnpb_data.io.s2.primary_backup_state[ArInd] == PNPB_IO_DATA_STATE_P_B_BACKUP))
        {
            // In case of Backup AR we need to discard IOCs IOPS. Some IOCs (including certification tool ART) send IOPS as good in Backup AR as well.
            *(pBuffer + BufOffset) = PNIO_S_BAD;
        }
        else      
#endif
        {
            *(pBuffer + BufOffset) = RemIops;
        }

        // *** set new BufOffset behind iops ***
        BufOffset += 1;


        // *** copy substitute data behind IOPS                       ***
        // *** we assume in this example: substitue value = 0x00      ***
        // *** note: if necessary every io byte could have its own    ***
        // *** substitue value                                        ***
        {
            REC_IO_SUBSTVAL_STRUCT*  pSubst       = (REC_IO_SUBSTVAL_STRUCT*) (pBuffer + BufOffset);
            PNIO_UINT32              SubstDataLen =   sizeof (REC_IO_SUBSTVAL_STRUCT) // size of the output header structure
                                                     + pExpSub->Out.data_length       // size of normal io data
                                                     + 2;                             // iocs-len and SubstitudeDataValid-len

            pSubst->BlockHeader.Type      = OsHtons (0x0014);              // must be 0x14 for SubstituteValue
            pSubst->BlockHeader.Len       = OsHtons ((PNIO_UINT16)(SubstDataLen - sizeof (REC_IO_BLOCKHDR) + 2)); // blocklength, header excluded
            pSubst->BlockHeader.Version   = OsHtons (0x0100);              // must be 0x01 (see PNIO specification)

            // *** set iocs
            BufOffset += sizeof (REC_IO_SUBSTVAL_STRUCT);
            *(pBuffer + BufOffset) = LocIocs;  ;        // set iocs for substitude data (we assume, value is always GOOD)

            // *** set substitue data to replacement value (here: EXAMPLE_SUBSTITUTE_VALUE)
            BufOffset += 1;                      // set buffer offset to start of substitude data

            // *** preset value: substituted value mode is "ZERO", here machine endian format ***
            pSubst->SubstMode = 0x0000;     // preset  SubstitutionMode: 0=ZERO, 1:LastValue, 2:Replacement value

            SubstDatValid = PNIO_cbf_substval_out_read  (
                                PNIO_SINGLE_DEVICE_HNDL,    // [in] Handle for Multidevice
                                pAddr,                      // [in] geographical or logical address
                                pExpSub->Out.data_length,   // [in] length of the submodule output substitute data
                                pBuffer + BufOffset,        // [in] Ptr to submodule output substitute data
                                &(pSubst->SubstMode),       // [in, out] SubstitutionMode: can be overwritten by application
                                &(pIo->SubstActiveFlag));   // [in, out] SubstituteActiveFlag: can be overwritten by application

            // *** make shure, buffer is set to 0 in case of SubstMode = 0
            if (pSubst->SubstMode == 0x0000)     // MODE = ZERO ?
            {
                OsMemSet (pBuffer + BufOffset,       // output data destination
                          0,                         // substitute values = 0
                          pExpSub->Out.data_length); // output data length
            }

            // ***  set OPERATION/SUBSTITUE Mode, here machine endian format    ***
            // ***  preset value: depends on IOPS and IOPS                      ***
            // ***  if both are GOOD; set to OPERATION,                         ***
            // ***  else to SUBSTITUTE                                          ***
            if ((LocIocs & PNIO_S_GOOD) && (RemIops & PNIO_S_GOOD))
            {
                pIo->SubstActiveFlag = REC8029_SUBSTSTATE_OPERATION;  // 0x0000 == operation, 0x0001 = substitute, see pnio spec.
                #if 1
                    // preset substitude values to 0
                    SubstDatValid = 0x00;           // force SubstituteDataValid to bad and copy the substitute data set
                #else
                    SubstDatValid = 0x80;           // if SubstituteDataValid is good, output values from IOC have to b� copied.
                    OsMemCpy (pBuffer + BufOffset,                  // output data destination
                              & (*(pBuffer + RecIoStrucSize + 1)),  // copy valid data from IOC  to substitute values
                              pExpSub->Out.data_length);            // output data length

                    // *** set SubstitudeDataValid (coded as iops, iocs,  we assume: all substvalues = 0, state GOOD)
                    SubstDatValid = 0x80;           // set Substitute data valid (we assume, value is always GOOD)
                #endif
            }
            else
            {
                pIo->SubstActiveFlag = REC8029_SUBSTSTATE_SUBSTITUTE;  // 0x0000 == operation, 0x0001 = substitute, see pnio spec.

            }


            // ** change 16 bit values to BIG ENDIAN MODE **
            pSubst->SubstMode       = OsHtons (pSubst->SubstMode);      // change to BIG endian format
            pIo->SubstActiveFlag    = OsHtons (pIo->SubstActiveFlag);   // change to BIG endian format

             BufOffset += pExpSub->Out.data_length;   // set buffer offset behind substitude data
             *(pBuffer + BufOffset) = SubstDatValid;
        }

        return (Status);
    }

#endif //IOD_INCLUDE_REC8028_8029


/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
