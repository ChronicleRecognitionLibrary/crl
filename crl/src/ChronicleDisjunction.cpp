/** ***********************************************************************************
 * \file ChronicleDisjunction.cpp
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

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include "RecoTreeCouple.h"
#include "ChronicleDisjunction.h"


// ----------------------------------------------------------------------------
// CLASS METHODS
// ----------------------------------------------------------------------------

namespace CRL 
{

  /** \param[in] opL Left member chronicle
  *   \param[in] opR Right member chronicle
  */
  ChronicleDisjunction::ChronicleDisjunction(Chronicle* opL, Chronicle* opR) 
    : ChronicleBinaryOp(opL,opR) 
  {
    // Ce(C1||C2) = Cr(C1) \inter Cr(C2) \ {\bot}
    // Cr(c1||C2) = Ce(C1||C2) U {\bot}
    Context& opLCr = opL->getResultingContext();
    Context& opRCr = opR->getResultingContext();
    _evaluationContext = Context::contextIntersection(opLCr, opRCr, true);
    _resultingContext  = _evaluationContext;
    _resultingContext.add(Context::ANONYMOUS());
  }


  /** \param[in] opL Left member chronicle
  *   \param[in] opR Right member chronicle
  */
  ChronicleDisjunction::ChronicleDisjunction(Chronicle& opL, Chronicle& opR) 
    : ChronicleBinaryOp(&opL,&opR) 
  {
    // Ce(C1||C2) = Cr(C1) \inter Cr(C2) \ {\bot}
    // Cr(c1||C2) = Ce(C1||C2) U {\bot}
    Context& opLCr = opL.getResultingContext();
    Context& opRCr = opR.getResultingContext();
    _evaluationContext = Context::contextIntersection(opLCr, opRCr, true);
    _resultingContext  = _evaluationContext;
    _resultingContext.add(Context::ANONYMOUS());
  }


  /** Display in the form : \code (a || b) \endcode
  *   \return string
  */
  std::string ChronicleDisjunction::toString() const 
  {
    if (_name != "") return _name;
    return( "(" + _opLeft->toString() + "||" + _opRight->toString() + ")" );
  }


  /** Updates the recognition set of the chronicle.
  *   \param[in] d date at which the evaluation is undertaken
  *   \param[in] e event to be evaluated
  */
  bool ChronicleDisjunction::process(const DateType& d, CRL::Event* e) 
  {
    // If the chronicle has already been processed this turn
    if (_alreadyProcessed) 
      return _hasNewRecognitions;

    bool flagLeft  = _opLeft->process(d, e);
    bool flagRight = _opRight->process(d, e);

    if (flagLeft||flagRight)
    {
      Chronicle::RecoSet::iterator it;

      //Adds the new recognitions of Left
      for (it  = _opLeft->getNewRecognitions().begin();
           it != _opLeft->getNewRecognitions().end(); it++)
      {
        PropertyManager x1;
        x1.copyProperties(**it, true, false); // Untransfer ownership

        if ( applyPredicate(x1) )
        {
          RecoTree* tmp = new RecoTreeCouple((*it), NULL);
          tmp->copyDateAndOrder(**it);
          tmp->copyProperties(x1, false, false); // Untransfer ownership
          if ( hasOutputFunction() )
          {
            PropertyManager pm;
            applyOutputFunction(x1, pm);
            tmp->upgradeProperties(pm, true, true); // Transfer ownership
          }
          applyActionFunction(tmp);
        }

      }

      //Adds the new recognitions of Right
      for (it  = _opRight->getNewRecognitions().begin();
           it != _opRight->getNewRecognitions().end(); it++)
      {
        PropertyManager x1;
        x1.copyProperties(**it, true, false); // Untransfer ownership

        if ( applyPredicate(x1) )
        {
          RecoTree* tmp = new RecoTreeCouple(NULL, (*it));
          tmp->copyDateAndOrder(**it);
          tmp->copyProperties(x1, true, false); // Untransfer ownership
          if ( hasOutputFunction() )
          {
            PropertyManager pm;
            applyOutputFunction(x1, pm);
            tmp->upgradeProperties(pm, true, true); // Transfer ownership
          }
          applyActionFunction(tmp);
        }
      }
      _alreadyProcessed = true;
      return _hasNewRecognitions;
    }
    _alreadyProcessed = true;
    return (_hasNewRecognitions = false);
  }


} /* namespace CRL */
