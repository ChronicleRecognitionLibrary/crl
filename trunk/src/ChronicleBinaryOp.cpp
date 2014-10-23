/** ***********************************************************************************
 * \file ChronicleBinaryOp.cpp
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

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include "RecoTreeCouple.h"
#include "ChronicleBinaryOp.h"


// ----------------------------------------------------------------------------
// CLASS METHODS
// ----------------------------------------------------------------------------

namespace CRL 
{

  /** \param[in] opL Left member chronicle
  *   \param[in] opR Right member chronicle
  */
  ChronicleBinaryOp::ChronicleBinaryOp(Chronicle* opL, Chronicle* opR) 
    : _opLeft(opL),_opRight(opR)
  {
  }


  /** Empties the temporary set of the last recognitions of the
  *   sub-chronicles; and its own if \a daughtersOnly is not \a true.
  *   \param[in] daughtersOnly indicates to only empty the sub-chronicles
  */
  void ChronicleBinaryOp::purgeNewRecognitions(bool daughtersOnly)
  {
    Chronicle::purgeNewRecognitions(daughtersOnly);
    _opLeft->purgeNewRecognitions();
    _opRight->purgeNewRecognitions();
  }


 //! Empties the recognition set (for optimisation ends)
  void ChronicleBinaryOp::purgeRecognitionsIfPurgeable()
  {
    Chronicle::purgeRecognitionsIfPurgeable();
    _opLeft->purgeRecognitionsIfPurgeable();
    _opRight->purgeRecognitionsIfPurgeable();
  }

  /** Sets peremption duration to \a duration,
  * recursively if \a recursive.
  * \param[in] duration indicates the peremption duration
  * \param[in] recursive indicates whether the setting of the peremption duration should be recursive
  */
    void ChronicleBinaryOp::setPeremptionDuration(double duration, bool recursive)
  {
    Chronicle::setPeremptionDuration(duration, recursive);
    if (recursive)
    {
    _opLeft->setPeremptionDuration(duration, recursive);
    _opRight->setPeremptionDuration(duration, recursive);
    }
  }


 //! Deletes too old recognitions recursively.
    void ChronicleBinaryOp::purgeOldRecognitions()
  {
    Chronicle::purgeOldRecognitions();
    _opLeft->purgeOldRecognitions();
    _opRight->purgeOldRecognitions();
  }



  /** The lookahead of chronicle @C is that of \c C.
  *   \param[in] tcurr current date
  *   \return future date 
  */
  DateType ChronicleBinaryOp::lookAhead(const DateType& tcurr) const 
  {
    DateType minL = _opLeft->lookAhead(tcurr);
    DateType minR = _opRight->lookAhead(tcurr);
    return (minR <= minL ? minR : minL);
  }


  /** Displays the chronicle as a string.
  *   \return string
  */
  std::string ChronicleBinaryOp::prettyPrint() const
  {
    std::string result = Chronicle::prettyPrint();
    if (_opLeft != NULL) 
      result += _opLeft->prettyPrint();
    if (_opRight != NULL) 
      result += _opRight->prettyPrint();
    return result;
  }


} /* namespace CRL */
