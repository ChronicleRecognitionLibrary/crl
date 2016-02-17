/** ***********************************************************************************
 * \file ChronicleDelayOp.h
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Base virtual class of Chronicles with delay
 **************************************************************************************/

/*  Copyright (C) 2012, 2013, 2014  ONERA – http://www.onera.fr
    This file is part of CRL : Chronicle Recognition Library.

    CRL is free software: you can redistribute it and/or modify it under
    the terms of the Lesser GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CRL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    Lesser GNU General Public License for more details.

    You should have received a copy of the Lesser GNU General Public License
    along with CRL.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CHRONICLE_DELAY_OP_H_
#define CHRONICLE_DELAY_OP_H_

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include "Chronicle.h"
#include "Event.h"

// ----------------------------------------------------------------------------
// CLASS DESCRIPTION
// ----------------------------------------------------------------------------

namespace CRL {

  class ChronicleDelayOp: public CRL::Chronicle
  {
  protected:

    //! Pointer to the left member
    Chronicle* _opLeft;

    //! Right member (delay)
    DurationType _delay;

  public:

    //! Constructor
    ChronicleDelayOp(Chronicle* opL, const DurationType& d);

    //! Calls destructor of sub-chronicle, and destructor of this
    void deepDestroy() {
      _opLeft->deepDestroy();
      delete this;
    }

    //! Accessor, links to the recognition engine
    void setMyEngine(RecognitionEngine* e) {
      _myEngine = e;
      _opLeft->setMyEngine(e);
    }

    //! Display function for unit tests
    std::string prettyPrint() const;

    //! Accessor (const)
    const Chronicle* getOpLeft() const { return _opLeft; }

    //! Accessor (const)
    const DurationType& getDelay() const { return _delay; }

    //! Accessor (non const)
    Chronicle* getOpLeft() { return _opLeft; }

    //! Accessor
    void setOpLeft(Chronicle* opLeft) { _opLeft = opLeft; }

    //! Accesseur
    void setDelay ( const DurationType& d) { _delay = d; }

    //! Empties the new recognitions set
    void purgeNewRecognitions(bool daughtersOnly = false);

    //! Empties the recognitions set (optimisation purpose)
    void purgeRecognitionsIfPurgeable();

    //! Sets peremption duration to \a duration, recursively if \a recursive.
    void setPeremptionDuration(double duration, bool recursive);

    //! Deletes too old recognitions recursively.
    void purgeOldRecognitions();

    //! Returns the date in the future at which the recognitions set of the chronicle must be re-assessed
    DateType lookAhead(const DateType& tcurr) const {
      return _opLeft->lookAhead(tcurr);
    }

  protected:

    //! Destructor protected (to prevent stack allocation)
    ~ChronicleDelayOp() { /* empty */ }

  }; // class ChronicleDelayOp

} /* namespace CRL */

#endif /* CHRONICLE_DELAY_OP_H_ */
