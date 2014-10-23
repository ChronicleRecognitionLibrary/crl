/** ***********************************************************************************
 * \file RecoTreeCouple.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Class of Recognition Nodes ("complex" chronicles)
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


// ----------------------------------------------------------------------------
// CLASS METHODS
// ----------------------------------------------------------------------------

namespace CRL 
{

  /** Overload of the eponymous method of class RecoTree.
  *   \return a new instance of class RecoTreeCouple, copy of \a this
  */
  /*
  RecoTree* RecoTreeCouple::clone() const 
  {
    return new RecoTreeCouple(_leftMember->clone(), _rightMember->clone());
  }
  */


  /** \param[in] e event at this node of the tree
  */
  RecoTreeCouple::RecoTreeCouple(const RecoTree* left, const RecoTree* right, bool rightToDelete)
    : RecoTree(2), _leftMember(left), _rightMember(right), _rightToDelete(rightToDelete)
  {
  }


  /** \return NULL
  */
  const Event* RecoTreeCouple::getEvent() const
  { 
    return NULL;
  }


  /** \return the sub-tree, left member.
  */
  const RecoTree* RecoTreeCouple::getLeftMember() const
  {
    return _leftMember;
  }


  /** \return the sub-tree, right member.
  */
  const RecoTree* RecoTreeCouple::getRightMember() const
  {
    return _rightMember;
  }


  /** Recursively examines the sub-trees and compares them.
  *   \param[in] t recognition tree to be compared
  *   \return true sif both trees are indentical
  */
  bool RecoTreeCouple::equal(const RecoTree* t) const 
  {
    if ((t == NULL) || (t->getArity() != 2)) return false;

    const RecoTree* t_leftMember  = t->getLeftMember();
    const RecoTree* t_rightMember = t->getRightMember();

    if ((_leftMember  == NULL) && (t_leftMember  != NULL)) return false;
    if ((_rightMember == NULL) && (t_rightMember != NULL)) return false;

    if (_leftMember  == NULL) return _rightMember->equal(t_rightMember);
    if (_rightMember == NULL) return _leftMember->equal(t_leftMember);

    return ( _leftMember->equal(t_leftMember) && _rightMember->equal(t_rightMember) );
  }


  /** Through a descending recursive path, returns the most recent event (leaf) 
  *   of a tree.
  *   \return event
  */
  const Event* RecoTreeCouple::lastEvent() const 
  {
    if (_leftMember == NULL) return _rightMember->lastEvent();
    if (_rightMember == NULL) return _leftMember->lastEvent();

    long o1 = _leftMember->getMaxOrder();
    long o2 = _rightMember->getMaxOrder();
    return (o1 < o2 ? _rightMember->lastEvent() : _leftMember->lastEvent());
  }




  /** Displays the content of a leaf using the name and date of its event.
  *   Displays symbol "bottom" for anonymous events.
  *   Otherwise, displays the contained son node.
  *   \param[in] ntab tabulations to be respected for the tree view display
  *   \param[in,out] os flux d'affichage
  */
  void RecoTreeCouple::prettyPrint(std::ostream& os, int ntab) const
  {
    for(int n=0; n<ntab; n++) os << ' '; //'\t';
    os << "< tmin=" << _minDate << " tmax=" << _maxDate << std::endl;
    if (_leftMember != NULL)
      _leftMember->prettyPrint(os, ntab+1);
    else
    {
      for(int n=0; n<ntab+1; n++) os << ' '; //'\t';
      os << "NULL" << std::endl;
    }
    if (_rightMember != NULL)
      _rightMember->prettyPrint(os, ntab+1);
    else
    {
      for(int n=0; n<ntab+1; n++) os << ' '; //'\t';
      os << "NULL" << std::endl;
    }
    for(int n=0; n<ntab; n++) os << ' '; //'\t';
    os << '>' << std::endl;
  }






} /* namespace CRL */
