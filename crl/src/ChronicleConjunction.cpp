/** ***********************************************************************************
 * \file ChronicleConjunction.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Conjunction Chronicles
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
#include "ChronicleConjunction.h"


// ----------------------------------------------------------------------------
// CLASS METHODS
// ----------------------------------------------------------------------------

namespace CRL 
{

  /** \param[in] opL Left member chronicle
  *   \param[in] opR Right member chronicle
  */
  ChronicleConjunction::ChronicleConjunction(Chronicle* opL, Chronicle* opR) 
    : ChronicleBinaryOp(opL,opR) 
  {
    // If chronicles are used in an AND, they are not "purgeable" anymore
    opL->setPurgeable(false);
    opR->setPurgeable(false);

    // Ce(C1&C2) = Cr(C1) U Cr(C2) \ {\bot}
    // Cr(c1&C2) = Ce(C1&C2) U {\bot}
    Context& opLCr = opL->getResultingContext();
    Context& opRCr = opR->getResultingContext();
    if ( Context::contextIntersection(opLCr, opRCr).isSingletonAnonymous() )
    {
      _evaluationContext = Context::contextUnion(opLCr, opRCr, true);
      _resultingContext  = _evaluationContext;
      _resultingContext.add(Context::ANONYMOUS());
    }
    else
      throw("ChronicleConjunction : inappropriate contexts");
  }


  /** \param[in] opL Left member chronicle
  *   \param[in] opR Right member chronicle
  */
  ChronicleConjunction::ChronicleConjunction(Chronicle& opL, Chronicle& opR) 
    : ChronicleBinaryOp(&opL,&opR) 
  {
    // If chronicles are used in an AND, they are not "purgeable" anymore
    opL.setPurgeable(false);
    opR.setPurgeable(false);

    // Ce(C1&C2) = Cr(C1) U Cr(C2) \ {\bot}
    // Cr(c1&C2) = Ce(C1&C2) U {\bot}
    Context& opLCr = opL.getResultingContext();
    Context& opRCr = opR.getResultingContext();
    if ( Context::contextIntersection(opLCr, opRCr).isSingletonAnonymous() )
    {
      _evaluationContext = Context::contextUnion(opLCr, opRCr, true);
      _resultingContext  = _evaluationContext;
      _resultingContext.add(Context::ANONYMOUS());
    }
    else
      throw("ChronicleConjunction : inappropriate contexts");
  }


  /** Display in the form : \code (a & b) \endcode
  *   \return string
  */
  std::string ChronicleConjunction::toString() const 
  {
    if (_name != "") return _name;
    return( "(" + _opLeft->toString() + "&&" + _opRight->toString() + ")" );
  }


  /** Updates the recognition set of the chronicle.
  *   \param[in] d date at which the evaluation is undertaken
  *   \param[in] e event to be evaluated
  */
  bool ChronicleConjunction::process(const DateType& d, CRL::Event* e) 
  {
    // If the chronicle has already been processed this turn
    if (_alreadyProcessed) 
      return _hasNewRecognitions;

    _hasNewRecognitions = false;
    bool flagLeft       = _opLeft->process(d, e);
    bool flagRight      = _opRight->process(d, e);

    if (flagLeft || flagRight)
    {
      Chronicle::RecoSet::iterator it, itL, itR;

      //the new recognitions of Left are merged with the recognitions of Right
      for (itL  = _opLeft->getNewRecognitions().begin();
           itL != _opLeft->getNewRecognitions().end(); itL++)
      {
        for (itR  = _opRight->getRecognitionSet().begin();
             itR != _opRight->getRecognitionSet().end(); itR++)
        {
          PropertyManager x1x2;  // Union of the properties, except anonymous
          x1x2.copyProperties(**itL, true, false); // no transfer of ownership, thus
          x1x2.copyProperties(**itR, true, false); // instance x1x2 is safely deletable 

          if ( applyPredicate(x1x2) )
          {
            RecoTree* tmp = new RecoTreeCouple(*itL, *itR);
            tmp->copyDateAndOrder(**itL, **itR);
            tmp->copyProperties(x1x2, false, false); // Untransfer ownership
            if ( hasOutputFunction() )
            {
              PropertyManager pm;
              applyOutputFunction(x1x2, pm);
              tmp->upgradeProperties(pm, true, true); // Transfer ownership
            }
            applyActionFunction(tmp);
          }
        }
      }

      //the new recognitions of Right are merged with the recognitions of Left
      for (itR  = _opRight->getNewRecognitions().begin();
           itR != _opRight->getNewRecognitions().end(); itR++)
      { 
        for (itL  = _opLeft->getRecognitionSet().begin();
             itL != _opLeft->getRecognitionSet().end(); itL++)
        {
          // This test eliminates the couples (new reco left, new reco right)
          // already treated with the previous double loop
          if ((*itL)->getMaxOrder() == e->getOrder())
            continue;

          PropertyManager x1x2;  // Union of the properties, except anonymous
          x1x2.copyProperties(**itL, true, false);
          x1x2.copyProperties(**itR, true, false);  

          if ( applyPredicate(x1x2) )
          {
            RecoTree* tmp = new RecoTreeCouple(*itL, *itR);
            tmp->copyDateAndOrder(**itL, **itR);
            tmp->copyProperties(x1x2, false, false); // Untransfer ownership
            // OLD' FASHION TO ELIMINATE DOUBLE RECOGNITIONS
            // Do not use because of quadratic cost !
            //if (isIn(*tmp, this->_newRecognitions))
            //{
            //  delete tmp;
            //  continue;
            //}
            if ( hasOutputFunction() )
            {
              PropertyManager pm;
              applyOutputFunction(x1x2, pm);
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
