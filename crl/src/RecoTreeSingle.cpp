/** ***********************************************************************************
 * \file RecoTreeSingle.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Class of Recognition Leafs (single events)
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


// ----------------------------------------------------------------------------
// CLASS METHODS
// ----------------------------------------------------------------------------

namespace CRL 
{

  /** Overloads the eponymous method of class RecoTree.
  *   \return a new instance of the class RecoTreeSingle, copy of \a this
  */
  /*
  RecoTree* RecoTreeSingle::clone() const 
  {
    return new RecoTreeSingle(_event);
  }
  */

  

  /** \param[in] e event at this node of the tree
  */
  RecoTreeSingle::RecoTreeSingle(const Event* e, bool eventToDelete)
    : RecoTree(1), _event(e), _tree(NULL), _eventToDelete(eventToDelete), _treeToDelete(false)
  { 
  }


  /** \param[in] r inferior node
  */
  RecoTreeSingle::RecoTreeSingle(const RecoTree* r, bool treeToDelete)
    : RecoTree(1), _event(NULL), _tree(r),_eventToDelete(false), _treeToDelete(treeToDelete)
  { 
  }

  /** Returns a pointer to the event at this node of the tree.
  *   \return event at this node of the tree
  */
  const Event* RecoTreeSingle::getEvent() const
  { 
    return _event;
  }


  /** \return NULL for a simple node, without sons.
  */
  const RecoTree* RecoTreeSingle::getLeftMember() const
  {
    return _tree;
  }


  /** \return NULL for a simple node, without sons.
  */
  const RecoTree* RecoTreeSingle::getRightMember() const
  {
    return _tree;
  }


  /** Possibly calls the destructor of the contained object
  */
  RecoTreeSingle::~RecoTreeSingle()
  {
    if (_eventToDelete)
      delete _event;
    if (_treeToDelete)
      delete _tree;
  }


  /** Examins the event at node \a t and at node \a this. If both are
  *   the same event, returns \a true.
  *   \param[in] t recognition tree to be compared
  *   \return true if the two trees are identical
  */
  bool RecoTreeSingle::equal(const RecoTree* t) const 
  {
    if ((t == NULL) || (t->getArity() != _arity)) return false;

    const RecoTree* t_tree  = t->getLeftMember();

    if ((_tree  == NULL) && (t_tree  != NULL)) return false;
    if ((_tree  == NULL) && (t_tree  == NULL)) 
      return (t->getEvent() == this->_event);

    return _tree->equal(t_tree);
  }


  /** \return the event of the leaf
  */
  const Event* RecoTreeSingle::lastEvent() const 
  {
    return _event;
  }


  /** Displays the content of the node by recursively calling the display of the sons.
  *   \param[in] ntab tabulations to be respected for the tree view display
  *   \param[in,out] os display flow
  */
  void RecoTreeSingle::prettyPrint(std::ostream& os, int ntab) const
  {
    if (_event != NULL)
    {
      for(int n=0; n<ntab; n++) os << ' '; //'\t';
      if (_event->getName() != Event::getTimeEventName())
        os << "<(" << _event->getName() << "," << _event->getDate() << ")>" << std::endl;
      else
        os << "<(\193," << _event->getDate() << ")>" << std::endl;
    }
    else if (_tree != NULL)
    { 
      _tree->prettyPrint(os, ntab+1);
    }
    else
      throw("RecoTreeSingle::prettyPrint : incoherent node");
  }

} /* namespace CRL */
