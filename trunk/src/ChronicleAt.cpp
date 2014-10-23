/** ***********************************************************************************
 * \file ChronicleAt.h
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Class of chronicles filtering terminal events
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

#include <sstream>

#include "ChronicleAt.h"
#include "RecoTreeSingle.h"


// ----------------------------------------------------------------------------
// CLASS METHODS
// ----------------------------------------------------------------------------

namespace CRL 
{

  /** \param[in] chr Chronicle to be recognised
  */
  ChronicleAt::ChronicleAt(Chronicle* chr)
    : _myChronicle(chr)
  {
    // Ce(@C) = Cr(C) \ {\bot}
    _evaluationContext = chr->getResultingContext();
    _evaluationContext.remove(Context::ANONYMOUS());

    // Cr(@C) = Ce(@C) U {\bot}
    _resultingContext = _evaluationContext;
    _resultingContext.add(Context::ANONYMOUS());
  }


  /** \param[in] chr Chronicle to be recognised
  */
  ChronicleAt::ChronicleAt(Chronicle& chr)
    : _myChronicle(&chr)
  {
    // Ce(@C) = Cr(C) \ {\bot}
    _evaluationContext = chr.getResultingContext();
    _evaluationContext.remove(Context::ANONYMOUS());

    // Cr(@C) = Ce(@C) U {\bot}
    _resultingContext = _evaluationContext;
    _resultingContext.add(Context::ANONYMOUS());
  }


  /** Display in the form : \code @C \endcode
  *   \return string
  */
  std::string ChronicleAt::toString() const
  {
    if (_name != "") return _name;
    std::stringstream ss;
    ss<< "@" << _myChronicle->toString() << "";
    return ss.str();
  }


  /** Updates the recognition set of the chronicle.
  *   \param[in] d date at which the evaluation is undertaken
  *   \param[in] e event to be evaluated
  */
  bool ChronicleAt::process(const DateType& d, CRL::Event* e)
  {
    // If the chronicle has already been processed this turn
    if (_alreadyProcessed) 
      return _hasNewRecognitions;

    _hasNewRecognitions = false;

    if (_myChronicle->process(d, e))
    {
      Chronicle::RecoSet::iterator it;
      for (it  = _myChronicle->getNewRecognitions().begin();
        it != _myChronicle->getNewRecognitions().end(); it++)
      {
        if ( applyPredicate(**it) )
        {
          RecoTreeSingle* r = new RecoTreeSingle(e);
          r->copyDateAndOrder(*e);
          // The attributes of r are the ones of the recognition *it
          r->copyProperties(**it);
          if ( hasOutputFunction() )
          {
            PropertyManager pm;
            applyOutputFunction(**it, pm);
            r->shiftProperties(pm, true);
          }
          applyActionFunction(r);
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
  void ChronicleAt::purgeNewRecognitions(bool daughtersOnly){
    Chronicle::purgeNewRecognitions(daughtersOnly);
    _myChronicle->purgeNewRecognitions();
  }


  //! Empties the recognition set (for optimisation ends)
  void ChronicleAt::purgeRecognitionsIfPurgeable()
  {
    Chronicle::purgeRecognitionsIfPurgeable();
    _myChronicle->purgeRecognitionsIfPurgeable();
  }

  /** Sets peremption duration to \a duration,
  * recursively if \a recursive.
  * \param[in] duration indicates the peremption duration
  * \param[in] recursive indicates whether the setting of the peremption duration should be recursive
  */
    void ChronicleAt::setPeremptionDuration(double duration, bool recursive)
  {
    Chronicle::setPeremptionDuration(duration, recursive);
    if (recursive)
    {
    _myChronicle->setPeremptionDuration(duration, recursive);
    }
  }


  //! Deletes too old recognitions recursively.
  void ChronicleAt::purgeOldRecognitions()
  {
    Chronicle::purgeOldRecognitions();
    _myChronicle->purgeOldRecognitions();
  }


  /** The lookahead of chronicle @C is that of \c C.
  *   \param[in] tcurr current date
  *   \return future date 
  */
  DateType ChronicleAt::lookAhead(const DateType& tcurr) const
  {
    return _myChronicle->lookAhead(tcurr);
  }



} /* namespace CRL */
