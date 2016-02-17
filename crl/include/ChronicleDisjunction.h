/** ***********************************************************************************
 * \file ChronicleDisjunction.h
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Disjunction Chronicles
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

#ifndef CHRONICLE_DISJUNCTION_H_
#define CHRONICLE_DISJUNCTION_H_

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include "ChronicleBinaryOp.h"
#include "RecoTreeCouple.h"


// ----------------------------------------------------------------------------
// CLASS DESCRIPTION
// ----------------------------------------------------------------------------

namespace CRL {

  class ChronicleDisjunction: public CRL::ChronicleBinaryOp 
  {
  public:

    //! Constructor
    ChronicleDisjunction(Chronicle* opL, Chronicle* opR);

    //! Constructor
    ChronicleDisjunction(Chronicle& opL, Chronicle& opR);

    //! Main event processing function
    bool process(const DateType& d, CRL::Event* e = NULL);

    //! Display function for unit tests
    std::string toString() const;

  protected:

    //! Destructor protected (to prevent stack allocation)
    ~ChronicleDisjunction() { /* empty */ }

  }; // class ChronicleDisjunction

} /* namespace CRL */

#endif /* CHRONICLE_DISJUNCTION_H_ */
