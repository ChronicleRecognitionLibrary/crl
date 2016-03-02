/** ***********************************************************************************
 * \file ChronicleMeets.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Meeting of two Chronicles
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
#include "ChronicleMeets.h"


// ----------------------------------------------------------------------------
// CLASS METHODS
// ----------------------------------------------------------------------------

namespace CRL 
{

  /** \param[in] opL Left member chronicle
  *   \param[in] opR Right member chronicle
  */  
  ChronicleMeets::ChronicleMeets(Chronicle* opL, Chronicle* opR) 
    : ChronicleBinaryOp(opL,opR)
  {
    // If the chronicle is used, it is not "purgeable" anymore
    opL->setPurgeable(false);    

    // Ce(C1 equals C2) = Cr(C1) U Cr(C2) \ {\bot}
    // Cr(C1 equals C2) = Ce(C1 equals C2) U {\bot}
    Context& opLCr = opL->getResultingContext();
    Context& opRCr = opR->getResultingContext();
    if ( Context::contextIntersection(opLCr, opRCr).isSingletonAnonymous() )
    {
      _evaluationContext = Context::contextUnion(opLCr, opRCr, true);
      _resultingContext  = _evaluationContext;
      _resultingContext.add(Context::ANONYMOUS());
    }
    else
      throw("ChronicleMeets : inappropriate contexts");
  }


  /** \param[in] opL Left member chronicle
  *   \param[in] opR Right member chronicle
  */  
  ChronicleMeets::ChronicleMeets(Chronicle& opL, Chronicle& opR) 
    : ChronicleBinaryOp(&opL,&opR)
  {
    // If the chronicle is used, it is not "purgeable" anymore
    opL.setPurgeable(false);    

    // Ce(C1 equals C2) = Cr(C1) U Cr(C2) \ {\bot}
    // Cr(C1 equals C2) = Ce(C1 equals C2) U {\bot}
    Context& opLCr = opL.getResultingContext();
    Context& opRCr = opR.getResultingContext();
    if ( Context::contextIntersection(opLCr, opRCr).isSingletonAnonymous() )
    {
      _evaluationContext = Context::contextUnion(opLCr, opRCr, true);
      _resultingContext  = _evaluationContext;
      _resultingContext.add(Context::ANONYMOUS());
    }
    else
      throw("ChronicleMeets : inappropriate contexts");
  }


  /** Display in the form : \code (C1 /= C2) \endcode
  *   \return string
  */
  std::string ChronicleMeets::toString() const
  {
    if (_name != "") return _name;
    return( "(" + _opLeft->toString() + " meets " + _opRight->toString() + ")" );
  }


  /** Updates the recognition set of the chronicle.
  *   \param[in] d date at which the evaluation is undertaken
  *   \param[in] e event to be evaluated
  */
  bool ChronicleMeets::process(const DateType& d, CRL::Event* e)
  {
    // If the chronicle has already been processed this turn
    if (_alreadyProcessed) 
      return _hasNewRecognitions;

    _hasNewRecognitions = false;
    _opLeft->process(d, e);

    if (_opRight->process(d, e))
    {
      Chronicle::RecoSet::iterator itL, itR;

      //the new recognitions of Right are merged with the recognitions of Left
      for (itR  = _opRight->getNewRecognitions().begin();
        itR != _opRight->getNewRecognitions().end(); itR++)
      {
        for (itL  = _opLeft->getRecognitionSet().begin();
          itL != _opLeft->getRecognitionSet().end(); itL++)
        {
          if ((*itL)->getMaxOrder() == (*itR)->getMinOrder())
          {
            PropertyManager x1x2;   // Union of the properties, except anonymous
            x1x2.copyProperties(**itL, true, false);
            x1x2.copyProperties(**itR, true, false); 

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
      }
    }
    _alreadyProcessed = true;
    return _hasNewRecognitions;
  }

} /* namespace CRL */
