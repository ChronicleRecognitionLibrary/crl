/** ***********************************************************************************
 * \file ChronicleDelayThen.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Delay Then Chronicles
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
#include "RecoTreeSingle.h"
#include "ChronicleDelayThen.h"
#include "RecognitionEngine.h"
#include <sstream>


// ----------------------------------------------------------------------------
// CLASS METHODS
// ----------------------------------------------------------------------------

namespace CRL 
{

  /** \param[in] opL Desired chronicle
  *   \param[in] d Waiting duration 
  */ 
  ChronicleDelayThen::ChronicleDelayThen(Chronicle* opL, const DurationType& d) 
    : ChronicleDelayOp(opL,d) 
  {
    // If the chronicle is used, it is not "purgeable" anymore
    opL->setPurgeable(false);

    // Ce(C1 then d) = Cr(C1) \ {\bot}
    // Cr(C1 then d) = Cr(C1) U {\bot}
    _evaluationContext = opL->getResultingContext();
    _evaluationContext.remove(Context::ANONYMOUS());
    _resultingContext  = _evaluationContext;
    _resultingContext.add(Context::ANONYMOUS());
  }


  /** \param[in] opL Desired chronicle
  *   \param[in] d Waiting duration  
  */ 
  ChronicleDelayThen::ChronicleDelayThen(Chronicle& opL, const DurationType& d) 
    : ChronicleDelayOp(&opL,d) 
  {
    // If the chronicle is used, it is not "purgeable" anymore
    opL.setPurgeable(false);

    // Ce(C1 then d) = Cr(C1) \ {\bot}
    // Cr(C1 then d) = Cr(C1) U {\bot}
    _evaluationContext = opL.getResultingContext();
    _evaluationContext.remove(Context::ANONYMOUS());
    _resultingContext  = _evaluationContext;
    _resultingContext.add(Context::ANONYMOUS());
  }


  /** Display in the form : \code (a + 4.0) \endcode
  *   \return string
  */
  std::string ChronicleDelayThen::toString() const
  {
    if (_name != "") return _name;
    std::stringstream ss;
    ss<< "(" << _opLeft->toString() << "+" << _delay << ")";
    return ss.str();
  }


  /** Empties the temporary set of the last recognitions.
  *   The argument \a daughtersOnly is here to deal with the special case
  *   of certain chronicles which temporary set must not be emptied
  *   (only those of the sub-chronicles will be emptied recursively).
  *   \param[in] daughtersOnly indicates to empty only the sub-chronicles
  */
  void ChronicleDelayThen::purgeNewRecognitions(bool daughtersOnly)
  {
    Chronicle::purgeNewRecognitions(daughtersOnly);
    _opLeft->purgeNewRecognitions(true);

  }



  /** Updates the recognition set of the chronicle.
  *   \param[in] d date at which the evaluation is undertaken
  *   \param[in] e event to be evaluated
  */
  bool ChronicleDelayThen::process(const DateType& d, CRL::Event* e)
  {
    // If the chronicle has already been processed this turn
    if (_alreadyProcessed) 
      return _hasNewRecognitions;

    _hasNewRecognitions =  false;

    _opLeft->process(d, e);

    Chronicle::RecoSet::iterator itL= _opLeft->getNewRecognitions().begin();
    Chronicle::RecoSet::iterator itLtmp;

    while (itL != _opLeft->getNewRecognitions().end())
    {
      if ( ((*itL)->getMaxDate() + _delay) == d )
      {
        PropertyManager xL;
        xL.copyProperties(**itL, true, false); // except \bot
        if ( applyPredicate(xL) )
        { 
          RecoTree* tmpR;
          if ( e->getName() == Event::getTimeEventName() )
            tmpR = new RecoTreeSingle(e, false);
          else 
            tmpR = new RecoTreeSingle(new Event(d), true);

          // The order and date of event e are adopted, no matter which case
          tmpR->copyDateAndOrder(*e); 

          RecoTree* tmp = new RecoTreeCouple(*itL, tmpR, true);
          tmp->copyDateAndOrder(**itL, *tmpR);
          tmp->copyProperties(**itL, true, false); // Untransfer ownership

          if ( hasOutputFunction() )
          {
            PropertyManager pm;
            applyOutputFunction(xL, pm);
            tmp->upgradeProperties(pm, true, true); // Transfer ownership
          }
          applyActionFunction(tmp);

          // Removes element itL processed in the list, while keeping
          // an iterator in a correct state for the following element
          itLtmp=itL;
          itLtmp++;
          _opLeft->getNewRecognitions().erase(itL);
          itL=itLtmp;
        }
        else itL++; //if ( applyPredicate(xL) )
      }
      else itL++;//if ( ((*itL)->getMaxDate() + _delay) == d )
    }
    _alreadyProcessed = true;
    return _hasNewRecognitions;
  }


  DateType ChronicleDelayThen::lookAhead(const DateType& tcurr) const
  {
    
    DateType tmp, tmpmin=INFTY_DATE;
    Chronicle::RecoSet::const_iterator itL= _opLeft->getRecognitionSet().begin();
    for (;itL !=_opLeft->getRecognitionSet().end();itL++)
    {
      tmp = (*itL)->getMaxDate();
      if ((tmp + _delay)>tcurr)
        if (tmpmin > tmp + _delay) tmpmin=tmp + _delay;
    }
    DateType minL = _opLeft->lookAhead(tcurr);
    return (tmpmin <= minL ? tmpmin : minL);
  }


} /* namespace CRL */
