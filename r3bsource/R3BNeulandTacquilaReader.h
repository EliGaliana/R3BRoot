
#ifndef R3BNEULANDTACQUILAREADER_H
#define R3BNEULANDTACQUILAREADER_H

#include "R3BReader.h"

class TClonesArray;

class FairLogger;

struct EXT_STR_h101_raw_nnp_t;
typedef struct EXT_STR_h101_raw_nnp_t EXT_STR_h101_raw_nnp;
class ext_data_struct_info;

/**
 * A reader of NeuLAND Tacquila data with UCESB.
 * Receives mapped raw data and converts it to
 * R3BRoot objects.
 * @author D. Kresan
 * @since December 8, 2015
 */
class R3BNeulandTacquilaReader : public R3BReader
{
  public:
    /**
     * Standard constructor.
     * Creates instance of the reader. To be called
     * in the steering macro.
     * @param Pointer to a full C structure generated by
     * the Ucesb unpacker.
     */
    R3BNeulandTacquilaReader(EXT_STR_h101_raw_nnp*, UInt_t);

    /**
     * Destructor.
     */
    ~R3BNeulandTacquilaReader();

    /**
     * Method for reader initialization.
     * Called by the R3BUcesbSource before the unpacking starts.
     * @param info structure.
     */
    Bool_t Init(ext_data_struct_info*);

    /**
     * Method for reading the data.
     * Called by the R3BUcesbSource once per sub-event.
     * Makes data conversion.
     */
    Bool_t Read();

    /**
     * Method for clearing the output array.
     */
    void Reset();

  private:
    UInt_t fNEvent;       /**< An event counter. */
    EXT_STR_h101_raw_nnp* fData;  /**< Reader specific data structure from ucesb. */
    UInt_t fOffset;
    FairLogger* fLogger;  /**< FairLogger. */
    TClonesArray* fArray; /**< Output array. */

  public:
    ClassDef(R3BNeulandTacquilaReader, 0);
};

#endif
