// -*-Mode: C++;-*-

/* ---------------------------------------------------------------------- *//*!
 *
 *  @file     DataFragment.cc
 *  @brief    Access methods for the RCE data fragments
 *            decoding a binary test file.
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
   2018.08.30 jjr Added isTpcEmpty   method
   2018.03.23 jjr Added isTpcDamaged method
   2017.10.27 jjr Corrected getHeader (buf) routine. The way it was written
                  it called itself. 
   2017.08.29 jjr Created
  
\* ---------------------------------------------------------------------- */


#include "dunepdlegacy/rce/dam/DataFragmentUnpack.hh"
#include "dunepdlegacy/rce/dam/access/Identifier.hh"
#include "dunepdlegacy/rce/dam/access/Originator.hh"
#include "dunepdlegacy/rce/dam/access/Data.hh"
#include "dunepdlegacy/rce/dam/records/DataFragment.hh"

#include <cinttypes>
#include <cstdio>


/* ---------------------------------------------------------------------- *//*!

  \brief Constructor to populate pointers to the various subrecords of a 
         data fragment

  \param[in]  buf The ProtoDune data buffer.  This must have been 
                  verified as a ProtoDune data.
                                                                          */
/* ---------------------------------------------------------------------- */
DataFragmentUnpack::DataFragmentUnpack (uint64_t const *buf) :
   m_df (buf)
{
   return;
}
/* ---------------------------------------------------------------------- */


/* ---------------------------------------------------------------------- *//*!

  \brief   Return a pointer to the DataFragment header.
  \return  A pointer to the DataFragment header.
                                                                          */
/* ---------------------------------------------------------------------- */
pdd::record::DataFragmentHeader  const *
            DataFragmentUnpack::getHeader () const
{
   auto const *hdr = m_df.getHeader ();
   return      hdr;
}
/* ---------------------------------------------------------------------- */



/* ---------------------------------------------------------------------- *//*!

  \brief  Test if this DataFragment is a normal, \i.e. error free TPC
          fragment.
  \retval true,   if this DataFragment is a normal TPC fragment
  \retval false,  if this DataFragment is not a normal TPC fragment
                                                                          */
/* ---------------------------------------------------------------------- */
bool DataFragmentUnpack::isTpcNormal () const
{
   bool   isTpcNormal = m_df.isTpcNormal ();
   return isTpcNormal;
}
/* ---------------------------------------------------------------------- */



/* ---------------------------------------------------------------------- *//*!

  \brief  Test if this DataFragment is a damaged, \i.e. a TPC fragment 
          with an error in one or more of its streams.

  \retval true,   if this DataFragment is a damaged TPC fragment
  \retval false,  if this DataFragment is not a damaged TPC fragment
                                                                          */
/* ---------------------------------------------------------------------- */
bool DataFragmentUnpack::isTpcDamaged () const
{
   bool   isTpcDamaged = m_df.isTpcDamaged ();
   return isTpcDamaged;
}
/* ---------------------------------------------------------------------- */



/* ---------------------------------------------------------------------- *//*!

  \brief  Test if this DataFragment is a empty \i.e. there are no TPC
          streams in this fragment

  \retval true,   if this DataFragment is an empty TPC fragment
  \retval false,  if this DataFragment is not an empty TPC fragment
                                                                          */
/* ---------------------------------------------------------------------- */
bool DataFragmentUnpack::isTpcEmpty () const
{
   bool   isTpcEmpty = m_df.isTpcEmpty ();
   return isTpcEmpty;
}
/* ---------------------------------------------------------------------- */



/* ---------------------------------------------------------------------- *//*!

  \brief  Get the length of this DataFragment, in units of 64-bit words
  \return The length of this DataFragment, in units of 64-bit words
                                                                          */
/* ---------------------------------------------------------------------- */
uint32_t  DataFragmentUnpack::getN64() const
{
   uint32_t n64 = m_df.getN64 ();
   return   n64;
}
/* ---------------------------------------------------------------------- */



/* ---------------------------------------------------------------------- *//*!

  \brief  Return a pointer to the Identifier subrecord.
  \return A pointer to the Identifier subrecord.
                                                                          */
/* ---------------------------------------------------------------------- */
pdd::record::Identifier const *
             DataFragmentUnpack::getIdentifier () const
{
   auto const *id = m_df.getIdentifier ();
   return      id;
}
/* ---------------------------------------------------------------------- */



/* ---------------------------------------------------------------------- *//*!

  \brief  Return a pointer to the Originator subrecord.
  \return A pointer to the Originator subrecord.
                                                                          */
/* ---------------------------------------------------------------------- */
pdd::record::Originator const *
             DataFragmentUnpack::getOriginator () const
{
   auto const *org = m_df.getOriginator ();
   return      org;
}
/* ---------------------------------------------------------------------- */



/* ---------------------------------------------------------------------- *//*!

  \brief  Return a pointer to the Data subrecord.
  \return A pointer to the Data subrecord.

  \par
   The Data subrecord contains not only the actual TPC data, but since
   this data can occur in a various formats, additional subrecords to 
   help locate and access this data.
                                                                          */
/* ---------------------------------------------------------------------- */
pdd::record::Data const *
             DataFragmentUnpack::getData () const
{
   auto const *data = m_df.getData ();
   return data;
}
/* ---------------------------------------------------------------------- */



/* ---------------------------------------------------------------------- *//*!

  \brief  Return a pointer to the Trailer subrecord.
  \return A pointer to the Trailer subrecord.
                                                                          */
/* ---------------------------------------------------------------------- */
pdd::Trailer const *DataFragmentUnpack::getTrailer () const
{
   auto const *tlr = m_df.getTrailer ();
   return      tlr;
}
/* ---------------------------------------------------------------------- */



/* ---------------------------------------------------------------------- *//*!

  \brief   Return a pointer to the DataFragment header.
  \return  A pointer to the DataFragment header.

  \param[in]  buf The ProtoDune data buffer.  This must have been 
                  verified as a ProtoDune data.
                                                                          */
/* ---------------------------------------------------------------------- */
pdd::record::DataFragmentHeader  const *
             DataFragmentUnpack::getHeader (uint64_t const *buf)
{   

  auto const *hdr =
        reinterpret_cast<pdd::record::DataFragmentHeader const *>(buf);
  return      hdr;
}
/* ---------------------------------------------------------------------- */



/* ---------------------------------------------------------------------- *//*!

  \brief  Return a pointer to the Identifier subrecord.
  \return A pointer to the Identifier subrecord.
                                                                          */
/* ---------------------------------------------------------------------- */
pdd::record::Identifier const *
             DataFragmentUnpack::getIdentifier (uint64_t const *buf)
{
   auto const *identifier = pdd::access::DataFragment::getIdentifier (buf);
   return identifier;
}
/* ---------------------------------------------------------------------- */



/* ---------------------------------------------------------------------- *//*!

  \brief  Return a pointer to the Originator subrecord.
  \return A pointer to the Originator subrecord.

  \param[in]  buf The ProtoDune data buffer.  This must have been 
                  verified as a ProtoDune data.
                                                                          */
/* ---------------------------------------------------------------------- */
pdd::record::Originator const *
               DataFragmentUnpack::getOriginator (uint64_t const *buf)
{
   auto const *originator = pdd::access::DataFragment::getOriginator (buf);
   return originator;
}
/* ---------------------------------------------------------------------- */



/* ---------------------------------------------------------------------- *//*!

  \brief  Return a pointer to the Data subrecord.
  \return A pointer to the Data subrecord.

  \param[in]  buf The ProtoDune data buffer.  This must have been 
                  verified as a ProtoDune data.

  \par
   The Data subrecord contains not only the actual TPC data, but since
   this data can occur in a various formats, additional subrecords to 
   help locate and access this data.
                                                                          */
/* ---------------------------------------------------------------------- */
pdd::record::Data const *
             DataFragmentUnpack::getData (uint64_t const *buf)
{
   pdd::record::Data  const *data = pdd::access::DataFragment::getData (buf);
   return data;
}
/* ---------------------------------------------------------------------- */
 


/* ---------------------------------------------------------------------- *//*!

  \brief  Return a pointer to the Trailer subrecord.
  \return A pointer to the Trailer subrecord.

  \param[in]  buf The ProtoDune data buffer.  This must have been 
                  verified as a ProtoDune data.
                                                                          */
/* ---------------------------------------------------------------------- */
pdd::Trailer const *DataFragmentUnpack::getTrailer (uint64_t const *buf)
{
   auto const *tlr = pdd::access::DataFragment::getTrailer (buf);
   return tlr;
}
/* ---------------------------------------------------------------------- */


/* ---------------------------------------------------------------------- *//*!

  \brief Prints all or portions of the Data Fragment's subrecords
                                                                          */
/* ---------------------------------------------------------------------- */
void DataFragmentUnpack::print () const
{
   printHeader      ();
   printIdentifier  ();
   printOriginator  ();
   printData        ();
   return;
}
/* ---------------------------------------------------------------------- */



/* ---------------------------------------------------------------------- *//*!

  \brief Print the Data Fragment Header
                                                                          */
/* ---------------------------------------------------------------------- */
void DataFragmentUnpack::printHeader () const
{
   auto const hdr = getHeader ();
   print (hdr);
   return;
}
/* ---------------------------------------------------------------------- */



/* ---------------------------------------------------------------------- *//*!

  \brief  Print's a generic Fragment Header interpretted as a 
          Data Fragment Header.

  \param[in] header The generic Fragment Header.
                                                                          */
/* ---------------------------------------------------------------------- */
void DataFragmentUnpack::print (pdd::Header0 const *header)
{
   printf ("Header: %16.16" PRIx64 "\n", header->retrieve());
      
   unsigned int  format = header->getFormat  ();
   unsigned int    type = header->getType    ();
   unsigned int     n64 = header->getN64     ();
   unsigned int  naux64 = header->getNaux64  ();
   unsigned int subtype = header->getSubtype ();
   unsigned int  bridge = header->getBridge  ();

   printf ("Header      Type.Format = %1.1x.%1.1x "
           "N64:%6.6x:%1.1x SubType=%1.1x Bridge=%6.6x\n",
           type,format,
           n64, naux64,
           subtype, bridge);
}
/* ---------------------------------------------------------------------- */



/* ---------------------------------------------------------------------- *//*!

  \brief Print the Identifier record
                                                                          */
/* ---------------------------------------------------------------------- */
void DataFragmentUnpack::printIdentifier () const
{
   pdd::access::Identifier const id ( getIdentifier ());
   id.print ();
   return;
}
/* ---------------------------------------------------------------------- */



/* ---------------------------------------------------------------------- *//*!

  \brief Print the Originator record
                                                                          */
/* ---------------------------------------------------------------------- */
void DataFragmentUnpack::printOriginator () const
{
   pdd::access::Originator const org (getOriginator ());
   org.print ();
   return;
}
/* ---------------------------------------------------------------------- */



/* ---------------------------------------------------------------------- *//*!

  \brief Print some portion of Data record

  \par
   Since the Data record is potentially very large, only the meta-data
   and some limited piece of the actual TPC data is printed.  There 
   really is no way to provide a generic print routine that will satisfy
   the needs in all or even most circumstances, so this is meant more as
   a first level, get off-the-ground method.
                                                                          */
/* ---------------------------------------------------------------------- */
void DataFragmentUnpack::printData () const
{
   pdd::access::Data const data(getData ());
   data.print ();
}
/* ---------------------------------------------------------------------- */



/* ---------------------------------------------------------------------- *//*!

  \brief Print the Trailer record
                                                                          */
/* ---------------------------------------------------------------------- */
void DataFragmentUnpack::printTrailer () const
{
   pdd::Trailer const *tlr = getTrailer ();
   print (tlr);
   return;
}
/* ---------------------------------------------------------------------- */



/* ---------------------------------------------------------------------- *//*!

  \brief Print the Trailer record

  \param[in] trailer Pointer to the trailer record.  
                                                                          */
/* ---------------------------------------------------------------------- */
void DataFragmentUnpack::print (pdd::Trailer const *trailer)
{
   printf ("Trailer = %16.16" PRIx64 "\n", trailer->retrieve ());
   return;
}
/* ====================================================================== */
