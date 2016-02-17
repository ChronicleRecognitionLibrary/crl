/** ***********************************************************************************
 * \file ChronicleNamed.h
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2013
 * \brief Class of chronicles whise recognitions are named
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

#ifndef CHRONICLE_NAMED_H_
#define CHRONICLE_NAMED_H_

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include <string>

#include "Chronicle.h"

// ----------------------------------------------------------------------------
// CLASS DESCRIPTION
// ----------------------------------------------------------------------------

namespace CRL {

  class ChronicleNamed: public CRL::Chronicle 
  {
  protected:

    //! Pointer to the chronicle which is named
    Chronicle* _myChronicle;

    //! Alias of the chronicle
    std::string _alias;

  public:

    //! Constructor
    ChronicleNamed(Chronicle* chr, const std::string& name);

    //! Constructor
    ChronicleNamed(Chronicle& chr, const std::string& name);

    //! Calls destructor of sub-chronicle, and destructor of this
    void deepDestroy() {
      _myChronicle->deepDestroy();
      delete this;
    }

    //! Accessor, links to the recognition engine
    void setMyEngine(RecognitionEngine* e) {
      _myEngine = e;
      _myChronicle->setMyEngine(e);
    }

    //! Main event processing function
    bool process(const DateType& d, CRL::Event* e = NULL);

    //! Display function for unit tests
    std::string toString() const;

    //! Empties the new recognitions set
    void purgeNewRecognitions(bool daughtersOnly = false);

    //! Empties the recognitions set (optimisation purpose)
    void purgeRecognitionsIfPurgeable();

    //! Sets peremption duration to \a duration, recursively if \a recursive.
    void setPeremptionDuration(double duration, bool recursive);

    //! Deletes too old recognitions recursively.
    void purgeOldRecognitions();

    //! Returns the date in the future at which the recognitions set of the chronicle must be re-assessed
    DateType lookAhead(const DateType& tcurr) const ;

  protected:

    //! Destructor protected (to prevent stack allocation)
    ~ChronicleNamed() { /* empty */ }

  }; // class ChronicleNamed

} /* namespace CRL */

#endif /* CHRONICLE_NAMED_H_ */
