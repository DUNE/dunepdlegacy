// -*-Mode: C++;-*-

#ifndef ACCESS_TPCSTREAM_HH
#define ACCESS_TPCSTREAM_HH

/* ---------------------------------------------------------------------- *//*!
 *
 *  @file     access/TpcStream.hh
 *  @brief    Access methods for the raw RCE Tpc Stream data records
 *
 *  @verbatim
 *                               Copyright 2017
 *                                    by
 *
 *                       The Board of Trustees of the
 *                    Leland Stanford Junior University.
 *                           All rights reserved.
 *
 *  @endverbatim
 *
\* ---------------------------------------------------------------------- */



/* ---------------------------------------------------------------------- *\
   
   HISTORY
   -------
  
   DATE       WHO WHAT
   ---------- --- ---------------------------------------------------------
   2018.09.11 jjr Added
                    isTpcNormal  ()
                    isTpcDamaged ()
   2017.12.19 jjr Removed the need for specifying a class member as unused
                  by somewhat artificially using it.
   2017.11.02 jjr Modified the unused in __attribute__ ((unused) to be
                  CLASS_MEMBER_UNUSED.  clang flags unused class members
                  as errors, but standard gcc does not and furthermore
                  does not accept __attribute__ ((unused)) on class
                  members.  The symbol CLASS_MEMBER_UNUSED is 
                  perferentially defined on the compile command to be 
                  either 'unused' or a blank string.
   2017.10.27 jjr Added __attribute ((unused)) to m_rsvd fields.  gcc on
                  the MAC complains about unused class members.
   2017.10.23 jjr Padded the data section with 4 reserved words to be used
                  for future use. 
   2017.10.12 jjr Moved the record definition -> records/TpcStream.hh
   2017.08.29 jjr Created
  
\* ---------------------------------------------------------------------- */

#include "dunepdlegacy/rce/dam/access/Headers.hh"
#include <cstdint>


/* ====================================================================== */
/* FORWARD REFERENCES                                                     */
/* ---------------------------------------------------------------------- */
namespace pdd    {
namespace record {

   class TpcStreamHeader;
   class       TpcStream;
   class       TpcRanges;
   class          TpcToc;
   class       TpcPacket;
}}
/* ====================================================================== */



/* ====================================================================== */
/* DEFINITION: TpcStream                                                  */
/* ---------------------------------------------------------------------- */
namespace pdd    {
namespace access {


/* ---------------------------------------------------------------------- *//*!

  \brief Class to access the Tpc Stream Record and its subrecords
                                                                          */
/* ---------------------------------------------------------------------- */ 
class TpcStream 
{
public:
   explicit TpcStream () { return; }
   TpcStream (pdd::record::TpcStream const *stream);

public:
   void construct (pdd::record::TpcStream const *stream);

public:
   pdd::record::TpcStream       const *getRecord () const;
   pdd::record::TpcStreamHeader const *getHeader () const;
   pdd::record::TpcRanges       const *getRanges () const;
   pdd::record::TpcToc          const *getToc    () const;
   pdd::record::TpcPacket       const *getPacket () const;
   int                                 getLeft   () const;
   uint32_t                            getCsf    () const;
   uint32_t                            getStatus () const;

public:
   bool       isTpcNormal   () const;
   bool       isTpcDamaged  () const;

private:
   void                         const *getReserved (int idx) const;


private:
   pdd::record::TpcStream       const   *m_stream; /*!< The raw TPC stream*/
   pdd::record::TpcRanges       const   *m_ranges; /*!< Time/Packet Ranges*/
   pdd::record::TpcToc          const      *m_toc; /*!< Table of Contents */
   pdd::record::TpcPacket       const   *m_packet; /*!< The data packets  */
   void                         const  *m_rsvd[4]; /*!< Future use        */
};
/* ---------------------------------------------------------------------- */
} /* END: namespace access                                                */
/* ---------------------------------------------------------------------- */
} /* END: namespace pdd                                                   */
/* ====================================================================== */







/* ====================================================================== */
/* IMPLEMENTATION: TpcStream                                              */
/* ---------------------------------------------------------------------- */
namespace pdd    {
namespace access {
/* ---------------------------------------------------------------------- */
inline pdd::record::TpcStream       const *TpcStream::getRecord () const 
{ return  m_stream; }

inline pdd::record::TpcStreamHeader const *TpcStream::getHeader () const 
{ return  reinterpret_cast<pdd::record::TpcStreamHeader const *>(m_stream); }

inline pdd::record::TpcRanges       const *TpcStream::getRanges () const 
{ return m_ranges;  }

inline pdd::record::TpcToc          const *TpcStream::getToc    () const 
{ return     m_toc; }

inline pdd::record::TpcPacket       const *TpcStream::getPacket () const
 { return  m_packet; }

inline void                      const  *TpcStream::getReserved (int idx) const
{ return m_rsvd[idx]; }
/* ---------------------------------------------------------------------- */
/*   END: TpcStream                                                       */
/* ---------------------------------------------------------------------- */
} /* END: namespace access                                                */
/* ---------------------------------------------------------------------- */
} /* END: namespace pdd                                                   */
/* ====================================================================== */


#endif
