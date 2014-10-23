/** ***********************************************************************************
 * \file ChronicleDelayAtLeast.h
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Delay At Least Chronicles
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

#ifndef CHRONICLE_DELAY_AT_LEAST_H_
#define CHRONICLE_DELAY_AT_LEAST_H_

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include "ChronicleDelayOp.h"


// ----------------------------------------------------------------------------
// CLASS DESCRIPTION
// ----------------------------------------------------------------------------

namespace CRL {

  class ChronicleDelayAtLeast: public CRL::ChronicleDelayOp
  {
  public:

    //! Constructor
    ChronicleDelayAtLeast(Chronicle* opL, const DurationType& d);

    //! Constructor
    ChronicleDelayAtLeast(Chronicle& opL, const DurationType& d);

    //! Main event processing function
    bool process(const DateType& d, CRL::Event* e = NULL);

    //! Display function for unit tests
    std::string toString() const;

  }; // class ChronicleDelayAtLeast

} /* namespace CRL */

#endif /* CHRONICLE_DELAY_AT_LEAST_H_ */
