/** ***********************************************************************************
 * \file ChronicleCut.h
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2013
 * \brief Sequence of two Chronicles with cut of the second member
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

#ifndef CHRONICLE_CUT_H_
#define CHRONICLE_CUT_H_

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include "ChronicleBinaryOp.h"


// ----------------------------------------------------------------------------
// CLASS DESCRIPTION
// ----------------------------------------------------------------------------

namespace CRL {

  class ChronicleCut: public CRL::ChronicleBinaryOp
  {
  private:
    //! Recognitions set awaiting the left member of the chronicle
    Chronicle::RecoSet _tempRecogSet;

  public:

    //! Constructor
    ChronicleCut(Chronicle* opL, Chronicle* opR);

    //! Constructor
    ChronicleCut(Chronicle& opL, Chronicle& opR);

    //! Main event processing function
    bool process(const DateType& d, CRL::Event* e = NULL);

    //! Display function for unit tests
    std::string toString() const;

  }; // class ChronicleCut

} /* namespace CRL */

#endif /* CHRONICLE_CUT_H_ */
