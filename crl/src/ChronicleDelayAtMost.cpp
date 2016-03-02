/** ***********************************************************************************
 * \file ChronicleDelayAtMost.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Delay At Most Chronicles
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

#include "RecoTreeSingle.h"
#include "ChronicleDelayAtMost.h"
#include <sstream>


// ----------------------------------------------------------------------------
// CLASS METHODS
// ----------------------------------------------------------------------------

namespace CRL 
{

  /** \param[in] opL desired chronicle
  *   \param[in] d maximum duration of the chronicle
  */ 
  ChronicleDelayAtMost::ChronicleDelayAtMost(Chronicle* opL, const DurationType& d)
    : ChronicleDelayOp(opL,d) 
  {
    // Ce(C1 at most d) = Cr(C1) \ {\bot}
    // Cr(C1 at most d) = Cr(C1) U {\bot}
    _evaluationContext = opL->getResultingContext();
    _evaluationContext.remove(Context::ANONYMOUS());
    _resultingContext  = _evaluationContext;
    _resultingContext.add(Context::ANONYMOUS());
  }


  /** \param[in] opL desired chronicle
  *   \param[in] d maximum duration of the chronicle
  */ 
  ChronicleDelayAtMost::ChronicleDelayAtMost(Chronicle& opL, const DurationType& d)
    : ChronicleDelayOp(&opL,d) 
  {
    // Ce(C1 at most d) = Cr(C1) \ {\bot}
    // Cr(C1 at most d) = Cr(C1) U {\bot}
    _evaluationContext = opL.getResultingContext();
    _evaluationContext.remove(Context::ANONYMOUS());
    _resultingContext  = _evaluationContext;
    _resultingContext.add(Context::ANONYMOUS());
  }



  /** Display in the form : \code (a << 4.0) \endcode
  *   \return string
  */
  std::string ChronicleDelayAtMost::toString() const
  {
    if (_name != "") return _name;
    std::stringstream ss;
    ss<< "(" << _opLeft->toString() << "<<" << _delay << ")";
    return ss.str();
  }


  /** Updates the recognition set of the chronicle.
  *   \param[in] d date at which the evaluation is undertaken
  *   \param[in] e event to be evaluated
  */
  bool ChronicleDelayAtMost::process(const DateType& d, CRL::Event* e)
  {
    // If the chronicle has already been processed this turn
    if (_alreadyProcessed) 
      return _hasNewRecognitions;

    _hasNewRecognitions = false;

    if (_opLeft->process(d,e))
    {
      Chronicle::RecoSet::iterator itL;
      for (itL  = _opLeft->getNewRecognitions().begin();
        itL != _opLeft->getNewRecognitions().end(); itL++)
      {
        if ( ((*itL)->getMaxDate() - (*itL)->getMinDate()) < _delay )
        {
          PropertyManager xL;
          xL.copyProperties(**itL, true, false); // except \bot
          if ( applyPredicate(xL) )
          { 
            RecoTree* tmp = new RecoTreeSingle(*itL);
            tmp->copyDateAndOrder(**itL);
            tmp->copyProperties(**itL, false, false); // Untransfer ownership
            if ( hasOutputFunction() )
            {
              PropertyManager pm;
              applyOutputFunction(**itL, pm);
              tmp->upgradeProperties(pm, true, true); // Transfer ownership
            }
            applyActionFunction(tmp);
          }
        }
      }
    }
    _alreadyProcessed = true;
    return _hasNewRecognitions;
  }

} /* namespace CRL */
