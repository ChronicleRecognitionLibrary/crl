/** ***********************************************************************************
 * \file ChronicleBinaryOp.h
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Base virtual class of Binary Chronicles
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

#ifndef CHRONICLE_BINARY_OP_H_
#define CHRONICLE_BINARY_OP_H_

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include "Chronicle.h"

// ----------------------------------------------------------------------------
// CLASS DESCRIPTION
// ----------------------------------------------------------------------------

namespace CRL {

  class ChronicleBinaryOp : public CRL::Chronicle 
  {
  protected:

    //! Pointor to the left member
    Chronicle* _opLeft;

    //! Pointor to the right member
    Chronicle* _opRight;

  public:

    //! Constructor
    ChronicleBinaryOp(Chronicle* opL, Chronicle* opR);

    //! Calls destructors of sub-chronicles
    void deepDelete() {
      _opLeft->deepDelete();
      _opRight->deepDelete();
      delete _opLeft;
      delete _opRight;
    }

    //! Accessor, links to the recognition engine
    void setMyEngine(RecognitionEngine* e) {
      _myEngine = e;
      _opLeft->setMyEngine(e);
      _opRight->setMyEngine(e);
    }

    //! Display function for unit tests
    std::string prettyPrint() const;

    //! Accessor (const)
    const Chronicle* getOpLeft() const { return _opLeft; }

    //! Accessor (const)
    const Chronicle* getOpRight() const { return _opRight; }

    //! Accessor (non const)
    Chronicle* getOpLeft() { return _opLeft; }

    //! Accessor (non const)
    Chronicle* getOpRight() { return _opRight; }

    //! Accessor
    void setOpLeft(Chronicle* opLeft) { _opLeft = opLeft; }

    //! Accessor
    void setOpRight(Chronicle* opRight) { _opRight = opRight; }

    //! Empties the new recognitions set
    void purgeNewRecognitions(bool daughtersOnly = false);

    //! Empties the recognitions set (optimisation purpose)
    void purgeRecognitionsIfPurgeable();

    //! Sets peremption duration to \a duration, recursively if \a recursive.
    void setPeremptionDuration(double duration, bool recursive);

    //! Deletes too old recognitions recursively.
    void purgeOldRecognitions();

    //! Returns the date in the future at which the recognitions set of the chronicle must be re-assessed
    DateType lookAhead(const DateType& tcurr) const;


  }; // class ChronicleBinaryOp

} /* namespace CRL */

#endif /* CHRONICLE_BINARY_OP_H_ */
