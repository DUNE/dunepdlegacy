// -*-Mode: C++;-*-

#ifndef RECORDS_TPCSTREAM_HH
#define RECORDS_TPCSTREAM_HH

/* ---------------------------------------------------------------------- *//*!
 *
 *  @file     records/TpcStream.hh
 *  @brief    Raw RCE Tpc Stream data record definitions
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
                    enumeration of the data record types
                    getRecType  ()
                    isTpcNormal ()
                    isTpcDamaged()
                    isTpcEmpty  ()
   2017.10.12 jjr Created
  
\* ---------------------------------------------------------------------- */

#include "dunepdlegacy/rce/dam/access/Headers.hh"
#include "dunepdlegacy/rce/dam/records/Data.hh"
#include <cstdint>



namespace pdd    {
namespace record {

/* ====================================================================== */
/* CLASS DEFINITIONS                                                      */
/* ---------------------------------------------------------------------- *//*!

  \class TpcStreamHeader
  \brief Specializes a standard header for Tpc Data usage
                                                                          */
/* ---------------------------------------------------------------------- */
class TpcStreamHeader : public pdd::record::DataHeader
{
public:
   TpcStreamHeader () = delete;


public:
   enum class RecType
   {
      Reserved_0   = 0,  /*!< Reserved for future use                     */
      Originator   = 1,  /*!< Originator record type                      */
      TpcNormal    = 2,  /*!< Normal  TPC data, \e i.e. no errors         */
      TpcDamaged   = 3,  /*!< Damaged TPC data, \e i.e. has errors        */
      TpcEmpty     = 4   /*!< No      TPC data,                           */
   };



public:
   // ----------------------------------------------------
   // Convenience methods to get at the bridge word fields
   // ----------------------------------------------------
   int     getFormat () const { return Bridge::getFormat (getBridge ()); }
   int     getLeft   () const { return Bridge::getLeft   (getBridge ()); }
   int     getCsf    () const { return Bridge::getCsf    (getBridge ()); }
   int     getStatus () const { return Bridge::getStatus (getBridge ()); }
   RecType getRecType() const;
      

public:
   bool       isTpcNormal   () const  { return isTpcNormal  (getRecType ()); }
   bool       isTpcDamaged  () const  { return isTpcDamaged (getRecType ()); }

   static bool isTpcNormal  (RecType recType);
   static bool isTpcDamaged (RecType recType);
   static bool isTpcEmpty   (RecType recType);


   // -----------------------------------------
   // Primitive TPC stream header print methods
   // -----------------------------------------
   static void print (DataHeader const *dh);
   void        print () const;


   /* ------------------------------------------------------------------- *//*!

     \class Bridge
     \brief Accesses the fields of the TPC stream header's ebridge word
                                                                          */
   /* ------------------------------------------------------------------- */
   class Bridge
   {
   public:
      explicit Bridge () { return; }

   public:
      int             getFormat () const { return getFormat (m_w32); }
      int             getLeft   () const { return getLeft   (m_w32); }
      int             getCsf    () const { return getCsf    (m_w32); }
      uint32_t        getStatus () const { return getStatus (m_w32); }

      static int      getFormat (uint32_t bridge);
      static int      getLeft   (uint32_t bridge);
      static uint32_t getCsf    (uint32_t bridge);
      static uint32_t getStatus (uint32_t bridge);

   private:
      uint32_t m_w32;  /*!< Storage for the bridge word                   */
   };
   /* ------------------------------------------------------------------- */

} __attribute__ ((packed));
/* ---------------------------------------------------------------------- */
} /* END: namespace record                                                */
/* ---------------------------------------------------------------------- */
} /* END: namespace pdd                                                   */
/* ====================================================================== */




/* ====================================================================== */
/* IMPLEMENTATION: TpcStreamHeader                                     */
/* ---------------------------------------------------------------------- */
namespace pdd    {
namespace record {

/* ---------------------------------------------------------------------- */
inline pdd::record::TpcStreamHeader::RecType 
                 TpcStreamHeader::getRecType () const
{
   return static_cast<TpcStreamHeader::RecType>(getType ());
}
/* ---------------------------------------------------------------------- */



/* ---------------------------------------------------------------------- */
inline bool         TpcStreamHeader::
       isTpcNormal (TpcStreamHeader::RecType recType)
{
   return (recType == TpcStreamHeader::RecType::TpcNormal);
}

inline bool          TpcStreamHeader::
       isTpcDamaged (TpcStreamHeader::RecType recType)
{
   return (recType == TpcStreamHeader::RecType::TpcDamaged);
}

inline bool          TpcStreamHeader::
       isTpcEmpty   (TpcStreamHeader::RecType recType)
{
   return (recType == TpcStreamHeader::RecType::TpcEmpty);
}
/* ---------------------------------------------------------------------- */
/*   END: TpcStreamHeader                                                 */
} /* END: namespace record                                                */
} /* END: namespace pdd                                                   */
/* ====================================================================== */

#endif
