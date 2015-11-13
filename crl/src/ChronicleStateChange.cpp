/** ***********************************************************************************
 * \file ChronicleStateChange.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2013
 * \brief Sequence of two Chronicles with cut of both members
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
#include "ChronicleStateChange.h"


// ----------------------------------------------------------------------------
// CLASS METHODS
// ----------------------------------------------------------------------------

namespace CRL 
{

  /** \param[in] opL Left member chronicle
  *   \param[in] opR Right member chronicle
  */  
  ChronicleStateChange::ChronicleStateChange(Chronicle* opL, Chronicle* opR) 
    : ChronicleBinaryOp(opL,opR)
  {
    // If the chronicle is used, it is not "purgeable" anymore
    opL->setPurgeable(false);

    // Ce(C1 !! C2) = Cr(C1) U Cr(C2) \ {\bot}
    // Cr(C1 !! C2) = Ce(C1 !! C2) U {\bot}
    Context& opLCr = opL->getResultingContext();
    Context& opRCr = opR->getResultingContext();
    if ( Context::contextIntersection(opLCr, opRCr).isSingletonAnonymous() )
    {
      _evaluationContext = Context::contextUnion(opLCr, opRCr, true);
      _resultingContext  = _evaluationContext;
      _resultingContext.add(Context::ANONYMOUS());
    }
    else
      throw("ChronicleStateChange : inappropriate contexts");
  }
  

  /** \param[in] opL Left member chronicle
  *   \param[in] opR Right member chronicle
  */  
  ChronicleStateChange::ChronicleStateChange(Chronicle& opL, Chronicle& opR) 
    : ChronicleBinaryOp(&opL,&opR)
  {
    // If the chronicle is used, it is not "purgeable" anymore
    opL.setPurgeable(false);

    // Ce(C1 !! C2) = Cr(C1) U Cr(C2) \ {\bot}
    // Cr(C1 !! C2) = Ce(C1 !! C2) U {\bot}
    Context& opLCr = opL.getResultingContext();
    Context& opRCr = opR.getResultingContext();
    if ( Context::contextIntersection(opLCr, opRCr).isSingletonAnonymous() )
    {
      _evaluationContext = Context::contextUnion(opLCr, opRCr, true);
      _resultingContext  = _evaluationContext;
      _resultingContext.add(Context::ANONYMOUS());
    }
    else
      throw("ChronicleStateChange : inappropriate contexts");
  }
  

  /** Display in the form : \code (C1 C2) \endcode
  *   \return string
  */
  std::string ChronicleStateChange::toString() const 
  {
    if (_name != "") return _name;
    return( "(" + _opLeft->toString() + " != " + _opRight->toString() + ")" );
  }


  /** Updates the recognition set of the chronicle.
  *   \param[in] d date at which the evaluation is undertaken
  *   \param[in] e event to be evaluated
  */
  bool ChronicleStateChange::process(const DateType& d, CRL::Event* e) 
  {
    // If the chronicle has already been processed this turn
    if (_alreadyProcessed) 
      return _hasNewRecognitions;

    _hasNewRecognitions = false;

    // If there is a new recognition of C2
    // I scan the new recognitions of C2 :
    //   list tmpNewReco
    //   list recoC1toDelete
    //   var tmpTmaxC1 = -infini
    //   I scan the temporary recognitions of C1 (tmpRecogSet) :
    //     If (min/max order ok) :
    //       I create RecoTree tmp
    //         I evaluate the predicate
    //         If the predicate is verified on tmp :
    //           if (Tmax(C1) > tmpTmaxC1)
    //             tmpTmaxC1 = TMax(C1)
    //             trasnfers tmpNewReco into recoC1toDelete
    //             puts tmp into tmpNewReco
    //           otherwise if (Tmax(C1) == tmpTmaxC1)
    //             puts Met tmp into tmpNewReco
    //           otherwise (<)
    //             puts left into recoC1toDelete
    //   I transfer tmpNewReco into newReco and into reco, bool _hasNewRecognitions=true
    //   In tmpRecogSet, I delete what is in recoC1toDelete
    // the recognitions of C1 are missing

    //std::set<RecoTree*> recoLeftToDeleteEnd;

    Chronicle::RecoSet::iterator it;

    if (_opRight->process(d, e))
    {
      Chronicle::RecoSet::iterator itL, itR;
      std::set<RecoTree*> recoLeftToDelete;
      std::set<RecoTree*> tmpNewReco;

      for (itR  = _opRight->getNewRecognitions().begin();
        itR != _opRight->getNewRecognitions().end(); itR++)
      {
        long maxLeftMaxOrder = -1L;

        for (itL = _tempRecogSet.begin(); itL != _tempRecogSet.end(); itL++)
        {
          long leftMaxOrder;
          if ((leftMaxOrder=(*itL)->getMaxOrder()) < (*itR)->getMinOrder())
          {
            PropertyManager x1x2;  // Union of the properties, except anonymous
            x1x2.copyProperties(**itL, true);
            x1x2.copyProperties(**itR, true); 

            if ( applyPredicate(x1x2) )
            {
              RecoTree* tmp = new RecoTreeCouple(*itL, *itR);
              tmp->copyDateAndOrder(**itL, **itR);
              tmp->copyProperties(x1x2);

              // Case 1 : first recognition at a given instant, which empties the recognitions of prior instants
              if (leftMaxOrder > maxLeftMaxOrder)
              {
                maxLeftMaxOrder = leftMaxOrder;

                for (it = tmpNewReco.begin(); it != tmpNewReco.end(); it++)
                  recoLeftToDelete.insert(const_cast<RecoTree*>((*it)->getLeftMember()));
                tmpNewReco.clear();

                if (hasOutputFunction() == true)
                {
                  PropertyManager pm;
                  applyOutputFunction(x1x2, pm);
                  tmp->shiftProperties(pm, true);
                }
                tmpNewReco.insert(tmp);
              }
              // Case 2 : i^th recognition at a given instant, which is added to the other recognitions of the same instant
              else if (leftMaxOrder == maxLeftMaxOrder)
              {
                if (hasOutputFunction() == true)
                {
                  PropertyManager pm;
                  applyOutputFunction(x1x2, pm);
                  tmp->shiftProperties(pm, true);
                }
                tmpNewReco.insert(tmp);
              }
              else // (leftMaxOrder < maxLeftMaxOrder)
              {
                recoLeftToDelete.insert(*itL);
                delete tmp;
              }
            }
          } // if ((leftMaxOrder=(*itL) ...
        } // for (itL = _tempRecogSet.begin();  ...

        for (it = tmpNewReco.begin(); it != tmpNewReco.end(); it++)
          applyActionFunction(*it);
        tmpNewReco.clear(); 

      } // for (itR  = _opRight->getNewRecognitions().begin() ...

      for (it = _newRecognitions.begin(); it != _newRecognitions.end(); it++)
        _tempRecogSet.erase(const_cast<RecoTree*>((*it)->getLeftMember()));

      for (it = recoLeftToDelete.begin(); it != recoLeftToDelete.end(); it++)
        _tempRecogSet.erase(*it);

    } // if (_opRight->process(d, e))

    // Copies the new recognitions of Left into temporary
    if (_opLeft->process(d, e))
    {
      Chronicle::RecoSet::iterator it;
      for (it= _opLeft->getNewRecognitions().begin();
        it!=  _opLeft->getNewRecognitions().end(); it++)
        _tempRecogSet.insert(*it);
    }
 
    _alreadyProcessed = true;
    return _hasNewRecognitions;
  }


} /* namespace CRL */





// ----------------- PREVIOUS VERSION
    //  //the new recognitions of Right are merged with the recognition sof Left
    //  itL  = _tempRecogSet.begin();
    //  while (itL != _tempRecogSet.end())
    //  { 
    //    bool flag = false;
    //    for (itR  = _opRight->getNewRecognitions().begin();
    //      itR != _opRight->getNewRecognitions().end(); itR++)
    //    {
    //      if ((*itL)->maxOrder() < (*itR)->minOrder())
    //      {
    //        RecoTree* tmp = new RecoTreeCouple(*itL, *itR);

    //        PropertyManager props;
    //        std::set<std::string>::const_iterator ctxt;
    //        for (ctxt=_predicateContext.getData().begin(); ctxt!=_predicateContext.getData().end(); ctxt++)
    //          this->eval(*ctxt, *tmp, props);

    //         if ( (*_predicateFunction)(props) )
    //         {
    //            flag=true;
    //            _newRecognitions.insert(tmp);
    //            _recognitionSet.insert(tmp);
    //            _hasNewRecognitions = true;
    //         }
    //         else
    //           delete tmp;
    //      } // if
    //    } // for itR

    //    if (flag)
    //      itL = _tempRecogSet.erase(itL);
    //    else
    //      itL++;
    //  }
    //}

    // // The new recognitions of Left are transfered into temporary
    //if (_opLeft->process(d, e)){
    //_tempRecogSet.clear();
    //Chronicle::RecoSet::iterator it;
    //for (it= _opLeft->getNewRecognitions().begin();
    //  it!=  _opLeft->getNewRecognitions().end();
    //  it++)
    //  _tempRecogSet.insert(*it);
    //}

    //_alreadyProcessed = true;
    //return _hasNewRecognitions;
