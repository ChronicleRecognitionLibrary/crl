/** ***********************************************************************************
 * \file ChronicleCut.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2013
 * \brief Sequence of two Chronicles with cut of the second member
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
#include "ChronicleCut.h"


// ----------------------------------------------------------------------------
// CLASS METHODS
// ----------------------------------------------------------------------------

namespace CRL 
{

  /** \param[in] opL Left member chronicle
  *   \param[in] opR Right member chronicle
  */  
  ChronicleCut::ChronicleCut(Chronicle* opL, Chronicle* opR) 
    : ChronicleBinaryOp(opL,opR) 
  {
    // if the chronicle is used, it is not "purgeable" anymore
    opL->setPurgeable(false);

    // Ce(C1 ! C2) = Cr(C1) U Cr(C2) \ {\bot}
    // Cr(C1 ! C2) = Ce(C1 C2) U {\bot}
    Context& opLCr = opL->getResultingContext();
    Context& opRCr = opR->getResultingContext();
    if ( Context::contextIntersection(opLCr, opRCr).isSingletonAnonymous() )
    {
      _evaluationContext = Context::contextUnion(opLCr, opRCr, true);
      _resultingContext  = _evaluationContext;
      _resultingContext.add(Context::ANONYMOUS());
    }
    else
      throw("ChronicleCut : inappropriate contexts");
  }


  /** \param[in] opL Left member chronicle
  *   \param[in] opR Right member chronicle
  */  
  ChronicleCut::ChronicleCut(Chronicle& opL, Chronicle& opR) 
    : ChronicleBinaryOp(&opL,&opR) 
  {
    // if the chronicle is used, it is not "purgeable" anymore
    opL.setPurgeable(false);

    // Ce(C1 ! C2) = Cr(C1) U Cr(C2) \ {\bot}
    // Cr(C1 ! C2) = Ce(C1 C2) U {\bot}
    Context& opLCr = opL.getResultingContext();
    Context& opRCr = opR.getResultingContext();
    if ( Context::contextIntersection(opLCr, opRCr).isSingletonAnonymous() )
    {
      _evaluationContext = Context::contextUnion(opLCr, opRCr, true);
      _resultingContext  = _evaluationContext;
      _resultingContext.add(Context::ANONYMOUS());
    }
    else
      throw("ChronicleCut : inappropriate contexts");
  }


  /** Display in the form : \code (C1 C2) \endcode
  *   \return string
  */
  std::string ChronicleCut::toString() const 
  {
    if (_name != "") return _name;
    return( "(" + _opLeft->toString() + " ! " + _opRight->toString() + ")" );
  }


  /** Updates the recognition set of the chronicle.
  *   \param[in] d date at which the evaluation is undertaken
  *   \param[in] e event to be evaluated
  */
  bool ChronicleCut::process(const DateType& d, CRL::Event* e) 
  {
    // If the chronicle has already been processed this turn
    if (_alreadyProcessed) 
      return _hasNewRecognitions;

    _hasNewRecognitions = false;
    _opLeft->process(d, e);

    // Transfers the new recognitions of Left into temporary
    Chronicle::RecoSet::iterator it;
    for (it= _opLeft->getNewRecognitions().begin();
         it!=  _opLeft->getNewRecognitions().end();
         it++)
      _tempRecogSet.insert(*it);


    if (_opRight->process(d, e))
    {
      Chronicle::RecoSet::iterator itL, itR, itLtmp;

      //the new recognitions of Right are merged with the recognitions of Left
      itL  = _tempRecogSet.begin();
      while (itL != _tempRecogSet.end())
      { 
        bool flag = false;
        for (itR  = _opRight->getNewRecognitions().begin();
          itR != _opRight->getNewRecognitions().end(); itR++)
        {
          if ((*itL)->getMaxOrder() < (*itR)->getMinOrder())
          {
            PropertyManager x1x2;  // Union of the properties, except anonymous
            x1x2.copyProperties(**itL, true, false);
            x1x2.copyProperties(**itR, true, false); 

            if ( applyPredicate(x1x2) )
            {
              flag=true;
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
          } // if
        } // for itR

        if (flag)
        {
          itLtmp = std::next(itL);
          _tempRecogSet.erase(itL);
          itL = itLtmp;
        }
        else
          itL++;
      }
    }

    _alreadyProcessed = true;
    return _hasNewRecognitions;
  }



} /* namespace CRL */
