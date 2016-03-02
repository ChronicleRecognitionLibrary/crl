/** ***********************************************************************************
 * \file ChronicleAbsence.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Recognition of a chronicle, without recognition of another
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
#include "ChronicleAbsence.h"


// ----------------------------------------------------------------------------
// CLASS METHODS
// ----------------------------------------------------------------------------

namespace CRL 
{

  /** \param[in] opL Desired chronicle
  *   \param[in] opR Undesired chronicle
  */
  ChronicleAbsence::ChronicleAbsence(Chronicle* opL, Chronicle* opR) 
    : ChronicleBinaryOp(opL,opR), _exclInf(false), _exclSup(true)
  {
     // If the chronicle is used in an ABS, it is not "purgeable" anymore
    opR->setPurgeable(false);

    // Ce((C1)-[C2]) = Cr(C1) U Cr(C2) \ {\bot}
    // Cr((C1)-[C2]) = Cr(C1) U {\bot}
    Context& opLCr = opL->getResultingContext();
    Context& opRCr = opR->getResultingContext();
    if ( Context::contextIntersection(opLCr, opRCr).isSingletonAnonymous() )
    {
      _evaluationContext = Context::contextUnion(opLCr, opRCr, true);
      _resultingContext  = opLCr;
      _resultingContext.add(Context::ANONYMOUS(), true);
    }
    else
      throw("ChronicleAbsence : inappropriate contexts");
  }


  /** \param[in] opL Desired chronicle
  *   \param[in] opR Undesired chronicle
  */
  ChronicleAbsence::ChronicleAbsence(Chronicle& opL, Chronicle& opR) 
    : ChronicleBinaryOp(&opL,&opR), _exclInf(false), _exclSup(true)
  {
     // If the chronnicle is used in an ABS, it is not "purgeable" anymore
    opR.setPurgeable(false);

    // Ce((C1)-[C2]) = Cr(C1) U Cr(C2) \ {\bot}
    // Cr((C1)-[C2]) = Cr(C1) U {\bot}
    Context& opLCr = opL.getResultingContext();
    Context& opRCr = opR.getResultingContext();
    if ( Context::contextIntersection(opLCr, opRCr).isSingletonAnonymous() )
    {
      _evaluationContext = Context::contextUnion(opLCr, opRCr, true);
      _resultingContext  = opLCr;
      _resultingContext.add(Context::ANONYMOUS(), true);
    }
    else
      throw("ChronicleAbsence : inappropriate contexts");
  }


  /** Display in the form: \code (a b) - [c] \endcode
  *   \return string
  */
  std::string ChronicleAbsence::toString() const 
  { 
    std::string lftbr = (_exclInf? "]" : "[");
    std::string rghtbr = (_exclSup? "[" : "]");
    if (_name != "") return _name;
    std::string s = _opLeft->toString();
    size_t br1 = s.find_first_of('(');
    size_t br2 = s.find_last_of(')');
    if ( (br1!=std::string::npos) && (br2!=std::string::npos) )
      return( "(" + lftbr + _opLeft->toString().substr(br1+1, br2-br1-1) 
      + rghtbr + "-" + _opRight->toString() + ")" );
    else
      return( "(" + lftbr + _opLeft->toString() + rghtbr + "-" + _opRight->toString() + ")" );
  }


  /** Updates the recognition set of the chronicle.
  *   \param[in] d date at which the evaluation is undertaken
  *   \param[in] e event to be evaluated
  */
  bool ChronicleAbsence::process(const DateType& d, CRL::Event* e) 
  {
    // If the chronicle has already been processed this turn
    if (_alreadyProcessed) 
      return _hasNewRecognitions;

    _hasNewRecognitions = false;
    _opRight->process(d, e);

    if (_opLeft->process(d, e))
    {
      Chronicle::RecoSet::const_iterator itL, itR;

      //adds the new recognitions of Left
      for (itL  = _opLeft->getNewRecognitions().begin();
           itL != _opLeft->getNewRecognitions().end(); itL++)
      {

        // 1) The presence of a tree on the right is tested
        bool flag = true;

        for (itR  = _opRight->getRecognitionSet().begin(); 
             itR != _opRight->getRecognitionSet().end(); itR++)
        {
          // If a recognition r2 is found during r1, according to 
          // the specified boundaries, it is tested with the predicate
          // and r1 is possibly invalidated
		    if ((_exclInf ? (*itL)->getMinOrder() < (*itR)->getMinOrder() : (*itL)->getMinOrder() <= (*itR)->getMinOrder())
           && (_exclSup ? (*itR)->getMaxOrder() < (*itL)->getMaxOrder() : (*itR)->getMaxOrder() <= (*itL)->getMaxOrder()))
          {
            {
              PropertyManager x1x2;  // Union of the properties, except anonymous
              x1x2.copyProperties(**itL, true, false); // no transfer of ownership, thus
              x1x2.copyProperties(**itR, true, false); // instance x1x2 is safely deletable
              try
              {
               if ( applyPredicate(x1x2) )
                 {
                    flag = false;
                    break;
                 }
              }
              catch(...) { }
            }
          }
        }
        // 2) Otherwise, r1 is recognised and inserted
        if (flag)
        {
          RecoTree* tree = new RecoTreeSingle(*itL);
          tree->copyDateAndOrder(**itL);
          tree->copyProperties(**itL, false, false); // Untransfer ownership
          if ( hasOutputFunction() )
          {
            try 
            {
              PropertyManager pm;
              applyOutputFunction(**itL, pm);
              tree->upgradeProperties(pm, true, true); // Transfer ownership
            }
            catch(...){
              // IMPLEMENTATION CHOICE: nothing in case of crash
            }
          }
          applyActionFunction(tree);
        } // if (flag)

      }
    }
    _alreadyProcessed = true;
    return _hasNewRecognitions;
  }


} /* namespace CRL */
