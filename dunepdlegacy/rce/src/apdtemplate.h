/* ---------------------------------------------------------------------- *//*!

   \file  apdtemplate.h
   \brief Arithmetic Byte Decoder, template implementation file
   \author JJRussell - russell@slac.stanford.edu

   \par
    This file acts as a poor man's C++ template. It contains the real
    meat of the code. By defining a couple of C macros, (notably
    the stream access size and the method of loading data from the input
    stream), one can use this template to generate routines that process

       -# byte streams access as bytes or 32-bit integers. The access
          style is not of external user concern and is chosen by the
          implementer strictly on the basis of performance. It turns
          out on the sun that the byte access method is 20% faster.
          These are the APD_ routines
       -# 32-bit byte-swapped streams. These are the APD32_ routines
                                                                          */
/* ---------------------------------------------------------------------- */




/* ---------------------------------------------------------------------- *\
 *
 * HISTORY
 * -------
 *
 * DATE     WHO WHAT
 * -------- --- ---------------------------------------------------------
 * 08.20.18 jjr Remove attempt to realign the input buffer to 64-bits.
 *              This cannot be done.  The data is stored as a big-endian
 *              bit stream place in uint64_t's.  If it was little-endian
 *              this realignment would work, but not for big-endian
 *              streams.  It is most natural to store the data stream
 *              a big-endian since the first bit out is the most 
 *              significant bit.
 * 08.18.18 jjr Eliminated use of register qualfier, deprecated under 
 *              C++17
 * 08.18.18 jjr Changed name of macro load -> apd_load.  There was a 
 *              conflict with a compiler.
 * 01.18.05 jjr Separated basic code (here) from platform related stuff
 * 09.21.05 jjr Eliminated used macro store
 *
\* ---------------------------------------------------------------------- */

#ifndef  AP_DECODE
#define  AP_DECODE 1
#endif

#include "AP-Common.h"
#include <stdint.h>
#include <iostream>
#include <iomanip>

/* ---------------------------------------------------------------------- */
#ifndef CMX_DOXYGEN
/* ---------------------------------------------------------------------- */


/* INPUT A BIT. */
#define add_input_bit(_v, _in, _buffer, _bits_to_go)                      \
do                                                                        \
{   _bits_to_go -= 1;                                                     \
    if (_bits_to_go < 0)                                                  \
    {                                                                     \
        _buffer =  apd_load (_in);                                        \
        _in     = _in + sizeof (APD_iobuf_t);                             \
                                                                          \
        /* printf ("Buffer = %8.8x\n", _buffer); */                       \
        _bits_to_go = APD_K_IOBUF_BITS - 1;                               \
                                                                          \
    }                                                                     \
    _v <<= 1;                                                             \
    _v  |= (_buffer >> _bits_to_go) & 1;                                  \
                                                                          \
   /*std::cout << "Get bit " << std::hex << ((_buffer >> _bits_to_go) & 1)              \
             << " value " << std::hex << _v << std::endl;                  */           \
                                                                          \
   _v &= APC_M_CV_ALL;                                                    \
} while (0)
/* ---------------------------------------------------------------------- */





/* ---------------------------------------------------------------------- */
extern void apd_start (APD_dtx      *dtx,
                       void const   *src,
                       unsigned int boff)
{
    int                i;
    const uint8_t    *in;
    APD_iobuf_t   buffer;

    uint32_t      value = 0;
    int      bits_to_go = 0;


    /* Move to the nearest buffer boundary */
    in        = (uint8_t const *)src;
    in       += (boff >> APD_K_IOBUF_SHIFT) << 3;  // Force alignment to buffer width, convert to byte offset
    boff     &= APD_M_IOBUF_MASK;


    /* Pick up the shard, this can be from 1 to 8 bits */
    dtx->beg   =  in;
    buffer     =  apd_load (in);
    in         =  in + sizeof (APD_iobuf_t);
    bits_to_go =  APD_K_IOBUF_BITS - boff;
    dtx->bbeg  =  boff;

    //printf ("Input buffer = %8.8x\n", buffer);

    /* Input bits to fill the code value word */
    for (i = 1; i <= APC_K_NBITS; i++)
    {
        add_input_bit (value, in, buffer, bits_to_go);
    }


    dtx->lo     = 0;
    dtx->hi     = APC_K_HI;
    dtx->togo   = bits_to_go;
    dtx->value  = value;
    dtx->buffer = buffer;
    dtx->cur    = in;

    return;
}
/* ---------------------------------------------------------------------- */




/* ---------------------------------------------------------------------- */
int  apd_bdecompress (uint8_t             *out,
                      int                  cnt,
                      void const          *src,
                      unsigned int        boff,
                      APD_table_t const *table)
{
    APD_dtx dtx;

    apd_start (&dtx, src, boff);

    while (--cnt >= 0)
    {
      *out++ = apd_decode (&dtx, table);
    }

    return APD_finish (&dtx);
}
/* ---------------------------------------------------------------------- */




/* ---------------------------------------------------------------------- */
extern unsigned int apd_decode (APD_dtx             *dtx,
                                APD_table_t const *table)
{
    uint32_t          cum;
    int            symbol;
    int             range;

    APD_cv_t             lo = dtx->lo;
    APD_cv_t             hi = dtx->hi;
    int          bits_to_go = dtx->togo;
    APD_cv_t          value = dtx->value;
    APD_iobuf_t      buffer = dtx->buffer;
    uint8_t const       *in = dtx->cur;
    unsigned int        cnt = table[0] - 1;


    range  = (hi - lo) + 1;
    cum    = scale_m1 (value - lo + 1, range);
    table  = table + 1;
    symbol = lookup_bot (cum, table, cnt);



    /*
     |  Narrow the code region  to that alloted to this symbol
     |  This must be done independent of whether the input stream
     |  is exhausted, so that the bit count can be correctly
     |  calculated in APD_finish
    */
    hi = scale_hi (lo, range, table[symbol+1]);
    lo = scale_lo (lo, range, table[symbol  ]);

    APD_dumpStatement (int value_save = value;)
    while (1)
    {
        /* Loop to get rid of bits. */

        // printf ("Val = %x Lo:Hi = [%8.8x,%8.8x)\n", value, lo, hi);

        if      (hi <  APC_K_HALF)
        {
            /* Expand low half.         */
            /* nothing */
        }
        else if (lo >= APC_K_HALF)
        {
            /* Expand high half, subtract offset to top.*/
            value -= APC_K_HALF;
            lo    -= APC_K_HALF;
            hi    -= APC_K_HALF;
        }
        else if (lo >= APC_K_Q1 && hi < APC_K_Q3)
        {
            /* Expand middle half, subtract offset to middle*/
            value -= APC_K_Q1;
            lo    -= APC_K_Q1;
            hi    -= APC_K_Q1;
        }
        else
        {
            /* Otherwise exit loop.     */
            break;
        }

        /* Scale up code range.     */
        lo <<= 1;
        hi <<= 1;
        hi  |= 1;

        lo &= APC_M_CV_ALL;
        hi &= APC_M_CV_ALL;

        /* Move in next input bit.  */
        add_input_bit(value, in, buffer, bits_to_go);
    }

    APD_dumpStatement
    (
    std::cout << "Decoded symbol:" << std::hex << symbol << " cum:" << cum << " value:"
              << std::hex << std::setw (5) << value_save << " => " << value
              << std::hex << std::setw (5) << " cv " << dtx->lo << ':' << dtx->hi << "=> " << lo << ':' << hi
              << " extracted " << nbits << ':' << (value_save >> (12 - nbits)) << std::endl;
    )

    dtx->lo     = lo;
    dtx->hi     = hi;
    dtx->togo   = bits_to_go;
    dtx->value  = value;
    dtx->buffer = buffer;
    dtx->cur    = in;

    return symbol;
}
/* ---------------------------------------------------------------------- */




/* ---------------------------------------------------------------------- */
#ifdef apd_finish     /* This routine is the same in both cases           */
/* ---------------------------------------------------------------------- */
int APD_finish (APD_dtx *dtx)
{
    return 8 * (dtx->cur - dtx->beg) /* This is the number of bits 'read'*/
         - dtx->togo              /* Subtract off those not yet consumed */
         - APC_K_NBITS            /* Subtract off the initial read       */
         - dtx->bbeg              /* # of unused bits first byte/word    */
         + 1                      /* Last bit still in code word         */
         + 1;                     /* this is the 'follow bit'            */

}
/* ---------------------------------------------------------------------- */
#endif                                                   /* apd_finish    */
/* ---------------------------------------------------------------------- */


/* ---------------------------------------------------------------------- */
#endif                                                   /* CMX_DOXYGEN   */
/* ---------------------------------------------------------------------- */

