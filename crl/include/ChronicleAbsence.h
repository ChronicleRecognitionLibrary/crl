/** ***********************************************************************************
 * \file ChronicleAbsence.h
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Recognition of a chronicle, without recognition of another
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

#ifndef CHRONICLE_ABSENCE_H_
#define CHRONICLE_ABSENCE_H_

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include "ChronicleBinaryOp.h"


// ----------------------------------------------------------------------------
// CLASS DESCRIPTION
// ----------------------------------------------------------------------------

namespace CRL {

  class ChronicleAbsence : public CRL::ChronicleBinaryOp 
  {
	protected:

    //! Indicates the bounds to take into account for the absence
		bool _exclInf, _exclSup;

  public:

    //! Constructor C = opL - opR, by default exluding the upper bound but not the lower bound
    ChronicleAbsence(Chronicle* opL, Chronicle* opR);

    //! Constructor C = opL - opR, by default exluding the upper bound but not the lower bound
    ChronicleAbsence(Chronicle& opL, Chronicle& opR);

    //! Accessor
    bool isExclInf() const { return _exclInf; }

    //! Accessor
    void setExclInf(bool b) { _exclInf = b; }

    //! Accessor
    bool isExclSup() const { return _exclSup; }

    //! Accessor
    void setExclSup(bool b) { _exclSup = b; }

    //! Main event processing function
    bool process(const DateType& d, CRL::Event* e = NULL);

    //! Display function for unit tests
    std::string toString() const;

  protected:

    //! Destructor protected (to prevent stack allocation)
    ~ChronicleAbsence() { /* empty */ }

  }; // class ChronicleAbsence

} /* namespace CRL */

#endif /* CHRONICLE_ABSENCE_H_ */
