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
/*  F i l e               &F: eep_dia.c                                 :F&  */
/*                                                                           */
/*  V e r s i o n         &V: PnIODDevkits_P04.07.01.01_00.01.00.18     :V&  */
/*                                                                           */
/*  D a t e  (YYYY-MM-DD) &D: 2020-11-09                                :D&  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  Functions that use an EEPROM,                                            */
/*  used in peripheral modules for the ET200S                                */
/*                                                                           */
/*                                                                           */
/*  Funktionen: (*** -> not implemented yet)                                 */
/*                                                                           */
/*  - EEP_Write()                                                            */
/*  - EEP_Read()                                                             */
/*                                                                           */
/*****************************************************************************/


#include "compiler.h"
#if (PNIOD_PLATFORM & PNIOD_PLATFORM_ECOS_DKSW_ATMEL)
    #include "eep_inc.h"

    #include "eep_dat.h"          /* EEPROM constants and defines */


    #define EEP_MODULE EEP_ERR_MODULE_DIA


    /*****************************************************************************/

    void eep_init( void)
    {
        UnsignedOpt i;

        eep_data.write_error   = EEPROM_ERROR_NO_ERROR;
        eep_data.retry_counter = EEPROM_MAX_RETRY * 2; /* twice as much for the first start */
        eep_data.counter       = 0;

        eep_data.rd_req.kind  = EEP_REQ_KIND_IDLE;
        eep_data.rd_req.count = 0;
        eep_data.wr_req.kind  = EEP_REQ_KIND_IDLE;
        eep_data.wr_req.count = 0;

        for ( i = 0; i < EEPR_ID_MAX ; i++ )
        {
            eep_data.rd_teilliste[i].valid = FALSE;
        }
    }


    /*****************************************************************************/

    void eep_open(void)
    {
    #if 1
        EEP_Read(EEPR_ID_TEILLISTE1, &eep_data.rd_teilliste[eep_data.rd_req.id].data[0], 0, PAGE_SIZE);
        eep_data.rd_teilliste[eep_data.rd_req.id].valid = TRUE;
    #else
        if (OK == EEP_Read(EEPR_ID_TEILLISTE1, &eep_data.rd_teilliste[eep_data.rd_req.id].data[0], 0, PAGE_SIZE))
        {
            eep_data.rd_teilliste[eep_data.rd_req.id].valid = TRUE;
        }
        else
        {
            eep_data.rd_teilliste[eep_data.rd_req.id].valid = FALSE;
        }
    #endif
    }


    /*****************************************************************************/

    UnsignedOpt eep_write_req( UBYTE id, UBYTE P_DAT_DIA_ATTR *src_ptr, UBYTE len)
    {
        UnsignedOpt     ret_value;
        Unsigned8       tmp_len;

        if ( EEP_REQ_KIND_IDLE == eep_data.wr_req.kind )
        {
            if (   (EEPR_ID_MAX       >  id)
                && (PAGE_SIZE         >= len)
               )
            {
                for (tmp_len = 0; tmp_len < len; tmp_len++)
                {
                    eep_data.wr_req.data[tmp_len] = *src_ptr++;
                }

                eep_data.wr_req.kind     = EEP_REQ_KIND_WRITE;
                eep_data.wr_req.id       = id;
                eep_data.wr_req.len      = len;

                eep_data.wr_req.count++;

                ret_value = EEP_OK;  // order accepted

                EEP_TRIGGER_PFS();
            }
            else
            {
                ret_value = EEP_ERR_PARAMETER; // Order rejected
            }
        }
        else
        {
            ret_value = EEP_ERR_RESOURCE; // Auftrag abgelehnt
        }

        return (ret_value);
    }


    /*****************************************************************************/

    Boolean eep_read_req( UBYTE id, void (*done_fct_ptr) (Unsigned8  resp))
    {
        Boolean ret_value;

        if (   (EEP_REQ_KIND_IDLE == eep_data.rd_req.kind )
            && (EEPR_ID_MAX       >  id)
           )
        {
            eep_data.rd_req.kind         = EEP_REQ_KIND_READ;
            eep_data.rd_req.id           = id;
            eep_data.rd_req.data_ptr     = &eep_data.rd_teilliste[id].data[0];
            eep_data.rd_req.len          = PAGE_SIZE;
            eep_data.rd_req.done_fct_ptr = done_fct_ptr;


            eep_data.rd_req.count++;

            ret_value = TRUE;  // order accepted

            EEP_TRIGGER_PFS();
        }
        else
        {
            ret_value = FALSE; // Order rejected
        }

        return (ret_value);
    }


    /*****************************************************************************/

    Boolean eep_read_sync( UBYTE id, UBYTE P_DAT_DIA_ATTR *dst_ptr, UBYTE *len)
    {
        Boolean      ret_value;
        UnsignedOpt  tmp_count;
        Unsigned8    list_len;

        ret_value = TRUE;  // Data ok

        if (EEPR_ID_MAX > id)
        {
            if ( TRUE == eep_data.rd_teilliste[id].valid)
            {
                switch (id)
                {
                    case EEPR_ID_TEILLISTE1:
                    {
                        list_len                = EEPR_ID_TEILLISTE1_LENGTH + sizeof(IUA_KOPF_INFO_STRUC);
                        break;
                    }
                    case EEPR_ID_TEILLISTE2:
                    {
                        list_len                = EEPR_ID_TEILLISTE2_LENGTH + sizeof(IUA_KOPF_INFO_STRUC);
                        break;
                    }
                    case EEPR_ID_TEILLISTE3:
                    {
                        list_len                = EEPR_ID_TEILLISTE3_LENGTH + sizeof(IUA_KOPF_INFO_STRUC);
                        break;
                    }
                    case EEPR_ID_TEILLISTE4:
                    {
                        list_len                = EEPR_ID_TEILLISTE4_LENGTH + sizeof(IUA_KOPF_INFO_STRUC);
                        break;
                    }
                    default:
                    {
                        ret_value = FALSE;  // Data not available
                    }
                }

                if ( TRUE == ret_value)
                {
                    if ( list_len < *len )
                    {
                        *len = list_len;
                    }

                    for ( tmp_count = 0; tmp_count < *len; tmp_count++)
                    {
                        *dst_ptr++ = eep_data.rd_teilliste[id].data[tmp_count];
                    }

                    ret_value = TRUE;  // Data ok
                }
            }
            else
            {
                ret_value = FALSE;  // Data not available
            }
        }
        else
        {
            eep_in_fatal_error(EEP_MODULE, __LINE__, id);
        }

        return (ret_value);
    }


    /*****************************************************************************/

    /* Write a SZL-DS synchronously in the EEPROM.
    ------------------------------------------------------------------------- */
    UBYTE EEP_Write( UBYTE id, UBYTE P_DAT_DIA_ATTR *src_ptr, UBYTE offset, UBYTE len)
    {
      UBYTE page;
      UBYTE written;

      written = !OK;

      /* Area test:
      ----------------------------------------------------------------------- */
      switch ( id )
      {
      #ifdef DS248_IM_EEPROM
        case EEPR_ID_INHALTSVERZEICHNIS:
          page = DS248_BUF0_PAGE;
          break;
      #endif

      #ifdef DS239_IM_EEPROM
        case EEPR_ID_OBJEKTE_LISTE1:
          page = DS239_BUF0_PAGE;
          break;
      #endif

        case EEPR_ID_TEILLISTE1:
          page = DS231_BUF0_PAGE;
          break;

        case EEPR_ID_TEILLISTE2:
          page = DS232_BUF0_PAGE;
          break;

        case EEPR_ID_TEILLISTE3:
          page = DS233_BUF0_PAGE;
          break;

        case EEPR_ID_TEILLISTE4:
          page = DS234_BUF0_PAGE;
          break;

    /* ### 09.11.2007 MaSi: new for IOL master - beginning ### */

        #ifdef DIA_SZL_F111_INDEX_17_TEILLISTE1
          case EEPR_ID_TEILLISTE17:
            page = TL17_BUF0_PAGE;
            break;
        #endif

        #ifdef DIA_SZL_F111_INDEX_18_TEILLISTE1
          case EEPR_ID_TEILLISTE18:
            page = TL18_BUF0_PAGE;
            break;
        #endif

        #ifdef DIA_SZL_F111_INDEX_19_TEILLISTE1
          case EEPR_ID_TEILLISTE19:
            page = TL19_BUF0_PAGE;
            break;
        #endif

        #ifdef DIA_SZL_F111_INDEX_20_TEILLISTE1
          case EEPR_ID_TEILLISTE20:
            page = TL20_BUF0_PAGE;
            break;
        #endif

    /* ### 09.11.2007 MaSi: new for IOL master - end   ### */
      }

      /* Test whether what is to be written is already in the EEPROM.
         If not, write.
      ----------------------------------------------------------------------- */
      eep_data.retry_counter = EEPROM_MAX_RETRY;

      if ( FALSE == EEPROM_compareBlock( page, offset, len, src_ptr ) )
      {
        while ( eep_data.retry_counter )
        {
          eep_data.retry_counter--;

          EEPROM_writeBlock( page, offset, len, src_ptr );

          eep_data.counter = 0;

          START_EEPROM_WRITE();

          while ( GET_EEPROM_BUSY() )
          {
            eep_data.counter++;
          }
          /* Test whether what should be written is now in the EEPROM.
             If so, cancel the loop, otherwise repeat. 
			 If the counter has reached 0, enter an error.
          --------------------------------------------------------------------- */
          if ( TRUE == EEPROM_compareBlock( page, offset, len, src_ptr ) )
          {
            written = OK;
            break;
          }
          else
          {
              eep_in_fatal_error( EEP_MODULE, __LINE__, 0);
          }
        }
      }

      return( written );
    }


    /*****************************************************************************/

    /* Read a SZL-DS from the EEPROM.
    ------------------------------------------------------------------------- */

    UBYTE EEP_Read( UBYTE id, UBYTE P_DAT_DIA_ATTR *dest_ptr, UBYTE offset, UBYTE len)
    {
      UBYTE page;
      UBYTE first_byte;

      /* Area test:
      ----------------------------------------------------------------------- */
      switch ( id )
      {
      #ifdef DS248_IM_EEPROM
        case EEPR_ID_INHALTSVERZEICHNIS:
          first_byte = 0x00;
          page = DS248_BUF0_PAGE;
          break;
      #endif

      #ifdef DS239_IM_EEPROM
        case EEPR_ID_OBJEKTE_LISTE1:
          first_byte = 0xf1;
          page = DS239_BUF0_PAGE;
          break;
      #endif

        case EEPR_ID_TEILLISTE1:
          first_byte = 0xf1;
          page = DS231_BUF0_PAGE;
          break;

        case EEPR_ID_TEILLISTE2:
          first_byte = 0xf1;
          page = DS232_BUF0_PAGE;
          break;

        case EEPR_ID_TEILLISTE3:
          first_byte = 0xf1;
          page = DS233_BUF0_PAGE;
          break;

        case EEPR_ID_TEILLISTE4:
          first_byte = 0xf1;
          page = DS234_BUF0_PAGE;
          break;

    /* ### 09.11.2007 MaSi: new for IOL master - beginning ### */

        #ifdef DIA_SZL_F111_INDEX_17_TEILLISTE1
          case EEPR_ID_TEILLISTE17:
            first_byte = (UBYTE)szl_teilliste_17.teilliste_iol_kopf.szl_id; //0xf1;
            page = TL17_BUF0_PAGE;
            break;
        #endif

        #ifdef DIA_SZL_F111_INDEX_18_TEILLISTE1
          case EEPR_ID_TEILLISTE18:
            first_byte = (UBYTE)szl_teilliste_18.teilliste_iol_kopf.szl_id; //0xf1;
            page = TL18_BUF0_PAGE;
            break;
        #endif

        #ifdef DIA_SZL_F111_INDEX_19_TEILLISTE1
          case EEPR_ID_TEILLISTE19:
            first_byte = (UBYTE)szl_teilliste_19.teilliste_iol_kopf.szl_id; //0xf1;
            page = TL19_BUF0_PAGE;
            break;
        #endif

        #ifdef DIA_SZL_F111_INDEX_20_TEILLISTE1
          case EEPR_ID_TEILLISTE20:
            first_byte = (UBYTE)szl_teilliste_20.teilliste_iol_kopf.szl_id; //0xf1;
            page = TL20_BUF0_PAGE;
            break;
        #endif

    /* ### 09.11.2007 MaSi: new for IOL master - end   ### */
      }

    #if 0 /* 09.07.2007 MaSi */

      EEPROM_readBlock( page, offset, len, dest_ptr );

      if ( first_byte == *dest_ptr )
        return( OK );
      else
        return( !OK );

    #else

    //  EEPROM_readBlock( page, 0, 1, dest_ptr );

      /* 1. Read the byte and compare it with the expected value.
         If it is not in it, do not read the rest and !OK deliver.
         If reading the first byte did not work, there is no point in reading further.
      ----------------------------------------------------------------------- */
      if ( TRUE != EEPROM_readBlock( page, 0, 1, dest_ptr ) )
        return( !OK );

      if ( first_byte != *dest_ptr )
        return( !OK );

    //  EEPROM_readBlock( page, offset, len, dest_ptr );
    //  return( OK );

      /* EEPROM_readBlock() returns TRUE in the good case and FALSE in the event of an error.
         EEP_Read() delivers OK in the good and! OK in the event of an error.
         Since OK == 0 and TRUE == 1, negation is necessary here.
      ----------------------------------------------------------------------- */
      return ( !EEPROM_readBlock( page, offset, len, dest_ptr) );

    #endif
    }

    /*****************************************************************************/

    void eep_perform_services( void)
    {
        UnsignedOpt tmp_count;

        if (EEP_REQ_KIND_WRITE == eep_data.wr_req.kind)
        {
            // write takes precedence over read

            EEP_Write(eep_data.wr_req.id, &eep_data.wr_req.data[0], 0, eep_data.wr_req.len);

            eep_data.wr_req.kind = EEP_REQ_KIND_IDLE;

            EEP_REQ_WR_DONE();
        }
        else
        {
            if (EEP_REQ_KIND_READ == eep_data.rd_req.kind)
            {
    #if 1 

                EEP_Read(eep_data.rd_req.id, eep_data.rd_req.data_ptr, 0, eep_data.rd_req.len);

                for ( tmp_count = 0; tmp_count < eep_data.rd_req.len; tmp_count++)
                {
                    eep_data.rd_teilliste[eep_data.rd_req.id].data[tmp_count] = *eep_data.rd_req.data_ptr++;
                }

                eep_data.rd_teilliste[eep_data.rd_req.id].valid = TRUE;

    #else
                if (OK == EEP_Read(eep_data.rd_req.id, eep_data.rd_req.data_ptr, 0, eep_data.rd_req.len))
                {
                    for ( tmp_count = 0; tmp_count < eep_data.rd_req.len; tmp_count++)
                    {
                        eep_data.rd_teilliste[eep_data.rd_req.id].data[tmp_count] = *eep_data.rd_req.data_ptr++;
                    }

                    eep_data.rd_teilliste[eep_data.rd_req.id].valid = TRUE;
                }
                else
                {
                    eep_data.rd_teilliste[eep_data.rd_req.id].valid = FALSE;
                }
    #endif

                eep_data.rd_req.kind = EEP_REQ_KIND_IDLE;

                eep_data.rd_req.done_fct_ptr( eep_data.rd_teilliste[eep_data.rd_req.id].valid);
            }
        }
    }


    /*****************************************************************************/

    void eep_in_fatal_error( Unsigned8 error_module, Unsigned16 error_line, Unsigned16 error_code)
    {
        eep_errcb.lsa_component_id = EEP_LSA_COMPONENT_ID;

        eep_errcb.module_id        = error_module;
        eep_errcb.line             = error_line;

        eep_errcb.error_code[ 0]   = error_code;
        eep_errcb.error_code[ 1]   =
        eep_errcb.error_code[ 2]   =
        eep_errcb.error_code[ 3]   = 0;

        eep_errcb.error_data_length = sizeof( eep_data);
        eep_errcb.error_data_ptr    =        &eep_data;

        EEP_FATAL_ERROR( &eep_errcb);
    }

#endif

/*****************************************************************************/
/*  Copyright (C) 2020 Siemens Aktiengesellschaft. All rights reserved.      */
/*****************************************************************************/
