/** ***********************************************************************************
 * \file RecoTreeCouple.h
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

#ifndef RECO_TREE_COUPLE_H_
#define RECO_TREE_COUPLE_H_

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include <iostream>
#include "RecoTree.h"


// ----------------------------------------------------------------------------
// CLASS DESCRIPTION
// ----------------------------------------------------------------------------

namespace CRL {

  class RecoTreeCouple : public RecoTree
  {
  private:

    //! Left member
    const RecoTree* _leftMember;

    //! Right member
    const RecoTree* _rightMember;

    //! Indicates whether the right member must be destroyed
    bool _rightToDelete;

  public:

    //! Destructor
    ~RecoTreeCouple() { if (_rightToDelete) delete _rightMember; }

    //! Constructor
    RecoTreeCouple(const RecoTree* left, const RecoTree* right, bool rightToDelete = false);

    //! Accessor (overload of RecoTree)
    const Event* getEvent() const;

    //! Accessor (overload of RecoTree)
    const RecoTree* getLeftMember() const;

    //! Accessor (overload of RecoTree)
    const RecoTree* getRightMember() const;

    //! Returns true if the input tree is the same as "this"
    bool equal(const RecoTree* t) const;

    //! Returns the most recent (leaf) event of a tree
    const Event* lastEvent() const;

    //! Display function
    void prettyPrint(std::ostream& os, int ntab) const;

  }; // class RecoTreeCouple

} /* namespace CRL */

#endif /* RECO_TREE_COUPLE_H_ */