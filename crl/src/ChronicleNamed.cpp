/** ***********************************************************************************
 * \file ChronicleNamed.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2013
 * \brief Class of chronicles which recognitions are named
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

#include "Context.h"
#include "Property.h"
#include "RecoTreeSingle.h"
#include "ChronicleNamed.h"


// ----------------------------------------------------------------------------
// CLASS METHODS
// ----------------------------------------------------------------------------

namespace CRL
{

  /** \param[in] chr Included chronicle
  *   \param[in] name Given name
  */
  ChronicleNamed::ChronicleNamed(Chronicle* chr, const std::string& name) 
    : _myChronicle(chr), _alias(name)
  {
    // Ce(C->x) = Cr(C)
    _evaluationContext = chr->getResultingContext();

    // Cr(C->x) = {x} U {\bot}
    _resultingContext.add(Context::ANONYMOUS());
    _resultingContext.add(_alias);
  }

  /** \param[in] chr Included chronicle
  *   \param[in] name Given name
  */
  ChronicleNamed::ChronicleNamed(Chronicle& chr, const std::string& name) 
    : _myChronicle(&chr), _alias(name) 
  {
    // Ce(C->x) = Cr(C)
    _evaluationContext = chr.getResultingContext();

    // Cr(C->) = {x} U {\bot}
    _resultingContext.add(Context::ANONYMOUS());
    _resultingContext.add(_alias);
  }

  
  /** Display in the form : \code (C->x) \endcode
  *   \return string
  */
  std::string ChronicleNamed::toString() const
  {
    if (_name != "") return _name;
    return( "(" + _myChronicle->toString() + "->" + _alias + ")" );
  }



  /** Updates the recognition set of the chronicle.
  *   \param[in] d date at which the evaluation is undertaken
  *   \param[in] e event to be evaluated
  */
  bool ChronicleNamed::process(const DateType& d, CRL::Event* e) 
  {
    // If the chronicle has already been processed this turn
    if (_alreadyProcessed) 
      return _hasNewRecognitions;

    if (_myChronicle->process(d, e))
    {
      Chronicle::RecoSet::iterator it;

      for (it  = _myChronicle->getNewRecognitions().begin();
           it != _myChronicle->getNewRecognitions().end(); it++)
      {
        if( applyPredicate(**it) ) // If the predicate is verified or if there is no predicate
        {
          RecoTree* tmp = new RecoTreeSingle(*it);
          tmp->copyDateAndOrder(**it);
          if ( hasOutputFunction() )
          {
            PropertyManager pm;
            applyOutputFunction(**it, pm);
            tmp->shiftProperties(pm, true);
          }
          // Finds the anonymous property bottom (if it exists)
          // and renames it "_alias = code"
          Property* tmpProp = (*it)->findProperty(Context::ANONYMOUS());
          if ( tmpProp != NULL )
            tmp->insertProperty(_alias, tmpProp);

          applyActionFunction(tmp);
        }
      }
    }
    _alreadyProcessed = true;
    return _hasNewRecognitions;
  }




 /** Empties the temporary set of the last recognitions of the
  *   sub-chronicles; and its own if \a daughtersOnly is not \a true.
  *   \param[in] daughtersOnly indicates to only empty the sub-chronicles
  */
  void ChronicleNamed::purgeNewRecognitions(bool daughtersOnly)
  {
    Chronicle::purgeNewRecognitions(daughtersOnly);
    _myChronicle->purgeNewRecognitions();
  }


  //! Empties the recognition set (for optimisation ends)
  void ChronicleNamed::purgeRecognitionsIfPurgeable()
  {
    Chronicle::purgeRecognitionsIfPurgeable();
    _myChronicle->purgeRecognitionsIfPurgeable();
  }

  /** Sets peremption duration to \a duration,
  * recursively if \a recursive.
  * \param[in] duration indicates the peremption duration
  * \param[in] recursive indicates whether the setting of the peremption duration should be recursive
  */
    void ChronicleNamed::setPeremptionDuration(double duration, bool recursive)
  {
    Chronicle::setPeremptionDuration(duration, recursive);
    if (recursive)
    {
    _myChronicle->setPeremptionDuration(duration, recursive);
    }
  }


  //! Deletes too old recognitions recursively.
  void ChronicleNamed::purgeOldRecognitions()
  {
    Chronicle::purgeOldRecognitions();
    _myChronicle->purgeOldRecognitions();
  }


  /** The lookahead of the chronicle is the minimum of those of the sub-chronicles.
  *   \param[in] tcurr current date
  *   \return future date 
  */
  DateType ChronicleNamed::lookAhead(const DateType& tcurr) const 
  {
    return _myChronicle->lookAhead(tcurr);
  }




} /* namespace CRL */
