/** ***********************************************************************************
 * \file ChronicleSingleEvent.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Class of single event chronicles (leafs of chronicles tree)
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
#include "RecoTreeSingle.h"
#include "ChronicleSingleEvent.h"


// ----------------------------------------------------------------------------
// CLASS METHODS
// ----------------------------------------------------------------------------

namespace CRL 
{
  /** \param[in] code event type
  */
  ChronicleSingleEvent::ChronicleSingleEvent(const std::string& code)
    :_code(code)
  {
    _evaluationContext.add(Context::ANONYMOUS());
    _resultingContext.add(Context::ANONYMOUS());
  }


  /** Updates the recognition set of the chronicle.
  *   \param[in] d date at which the evaluation is undertaken
  *   \param[in] e event to be evaluated
  */
  bool ChronicleSingleEvent::process(const DateType& d, CRL::Event* e) 
  {
    // If the chronicle has already been processed this turn
    if (_alreadyProcessed) 
      return _hasNewRecognitions;

    if ( (e != NULL) && (e->getName()==_code) && (e->getDate() <= d) ) 
    {
      PropertyManager pm;

      // Note that when no predicate function nor predicate method are defined, the following
      // call is useless. But, for the moment, no way found to know if a predicate exists or not
      pm.upgradeProperties(*e, true, false);

      if ( applyPredicate(pm) ) // if the predicate is verified or if there is no predicate
      {
        RecoTree* tmp = new RecoTreeSingle(e);
        tmp->copyDateAndOrder(*e);

        PropertyManager pm2;
        pm2.copyProperties(*e, true, false); // Untransfer ownership
        pm2["CRL ID"]=e->getOrder();
        if ( hasOutputFunction() )
          applyOutputFunction(pm, pm2);
        tmp->upgradeProperties(pm2, true, true); // Transfer ownership
                                                 // thus transfers only "CRL ID"

        applyActionFunction(tmp);
      }
    }

    _alreadyProcessed = true;
    return _hasNewRecognitions;
  }



} /* namespace CRL */
