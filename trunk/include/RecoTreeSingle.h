/** ***********************************************************************************
 * \file RecoTreeSingle.h
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

#ifndef RECO_TREE_SINGLE_H_
#define RECO_TREE_SINGLE_H_

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include <iostream>
#include "Event.h"
#include "RecoTree.h"
#include "Property.h"


// ----------------------------------------------------------------------------
// CLASS DESCRIPTION
// ----------------------------------------------------------------------------

namespace CRL {

  class RecoTreeSingle : public RecoTree
  {
  private:

    //! (1) Event linked to the recognition
    const Event* _event;

    //! (2) OR lower node of the tree linked to the recognition
    const RecoTree* _tree;

    //! Indicates whether the event has to be destroyed
    bool _eventToDelete;

    //! Indicates whether the underlying recognition has to be destroyed
    bool _treeToDelete;

  public:

    //! Constructor (default)
    RecoTreeSingle()
      : RecoTree(0), _event(NULL), _tree(NULL), _eventToDelete(false), _treeToDelete(false)  { }

    //! Constructor (1)
    RecoTreeSingle(const Event* e, bool eventToDelete = false);

    //! Constructor (2)
    RecoTreeSingle(const RecoTree* r, bool treeToDelete = false);

    //! Destructor
    ~RecoTreeSingle();

    //! Accessor (overload of RecoTree)
    const Event* getEvent() const;

    //! Accessor (overload of RecoTree)
    const RecoTree* getLeftMember() const;

    //! Accessor (overload of RecoTree)
    const RecoTree* getRightMember() const;

    //! Returns true if the input tree is the same as "this"
    bool equal(const RecoTree* t) const;

    //! Returns the name of the associated event
    const std::string& getName() const;

    //! Returns the most recent (leaf) event of a tree
    const Event* lastEvent() const;

    //! Display function
    void prettyPrint(std::ostream& os, int ntab) const;

  }; // class RecoTreeSingle

} /* namespace CRL */

#endif /* RECO_TREE_SINGLE_H_ */
