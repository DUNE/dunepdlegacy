#ifndef dune_artdaq_Overlays_CTBFragment_hh
#define dune_artdaq_Overlays_CTBFragment_hh

#include "artdaq-core/Data/Fragment.hh"

#include "dunepdlegacy/Overlays/CTB_content.h"

#include <ostream>
#include <vector>




/* 
-------------------------------------
Brief guide to CTB fragments
-------------------------------------

   The CTB keeps sending information to the DAQ at all times. 
   Its fragments are just random collections of words
   from the trigger board. 
   In this context "random" means that the wrapping is not particularly significant 
   and it is determined by pure convenience during runtime. 

   Hence the structure of fragments is quite simple. 
   It is just an array of trigger words. 
   The trigger words are defined in the content.h file, see include. 

   This overlay classes is just providing the proper readouts and 
   casts to the proper word type.

*/

namespace dune {


class CTBFragment {
  public:

  // The constructor simply sets its const private member "artdaq_Fragment_"
  // to refer to the artdaq::Fragment object

  CTBFragment( artdaq::Fragment const & f ) ;


  // const getter functions for the data in the header
  unsigned int NWords() const noexcept { return _n_words ; } 

  // array-like access to the words, index from 0 to NWords()-1 
  // this pointer should always be valid, if not there is a problem
  // tipically i is too big
  const ptb::content::word::word_t * operator [] ( unsigned int i ) const { return Word(i) ; }

  // casted words depending on their type for easy deconding
  // An null pointer as a return means the requested cast is not correct

  // access to the generic word structure.
  // This simply allows access to word type, TS
  // and the payload is not formatted
  // this pointer should always be valid, if not there is a problem
  const ptb::content::word::word_t* Word( unsigned int i ) const ;  
  
  // These are words that report errors from the board
  const ptb::content::word::feedback_t* Feedback( unsigned int i ) const ;

  // Channel status reports
  const ptb::content::word::ch_status_t* ChStatus( unsigned int i ) const ;

  // simple mask to make explicit that this was a TS word
  const ptb::content::word::timestamp_t* Timestamp( unsigned int i ) const ;

  // Trigger words
  // There are a number of trigger words, the complete list being
  // - High level trigger (HLT)
  // - Low  level trigger (LLT) 
  // yet the payload is organized in the same way and so the casting 
  // function is the same
  const ptb::content::word::trigger_t * Trigger( unsigned int i ) const ;

  //static methods for casting
  static const ptb::content::word::feedback_t * Feedback ( const ptb::content::word::word_t & w ) ;
  static const ptb::content::word::ch_status_t* ChStatus ( const ptb::content::word::word_t & w ) ;
  static const ptb::content::word::timestamp_t* Timestamp( const ptb::content::word::word_t & w ) ;
  static const ptb::content::word::trigger_t*   Trigger  ( const ptb::content::word::word_t & w ) ;

  static constexpr unsigned int WordSize() { return sizeof( ptb::content::word::word_t ) ; } 

  friend std::ostream & operator << (std::ostream &, CTBFragment const & ) ;
  
protected:

  //maybe let's put some utilities here

private:

  const unsigned int _n_words ;

  artdaq::Fragment const & artdaq_Fragment_;

};


}  // dune namespace 

#endif /* dune_artdaq_Overlays_CTBFragment_hh */
