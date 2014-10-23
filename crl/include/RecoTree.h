/** ***********************************************************************************
 * \file RecoTree.h
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Base virtual class of Recognition Trees
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

#ifndef RECO_TREE_H_
#define RECO_TREE_H_

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include <iostream>
#include "Event.h"
#include "PropertyManager.h"

// ----------------------------------------------------------------------------
// CLASS DESCRIPTION
// ----------------------------------------------------------------------------

namespace CRL {

  class Chronicle;
  class RecognitionEngine;

  class RecoTree : public PropertyManager
  {
  protected:

    //! Indicates whether single (1) or couple (2)
    int _arity;

    //! Min order of the leaf events
    long _minOrder;

    //! Max order of the leaf events
    long _maxOrder;

    //! Date min of the leaf events
    DateType _minDate;

    //! Date max of the leaf events
    DateType _maxDate;

    //! Link to the chronicle which has created the recognition
    Chronicle* _myChronicle;

  protected:

    //! Protected constructor (abstract class)
    RecoTree(int n) : _arity(n), _minOrder(0), _maxOrder(0), 
                      _minDate(0.0), _maxDate(0.0), _myChronicle(NULL) { }

  public:

    //! Virtual destructor
    virtual ~RecoTree() { }

    //! Accessor
    int getArity() const { return _arity; }

    //! Accessor
    long getMinOrder() const { return _minOrder; }

    //! Accessor
    void setMinOrder(long n) { _minOrder=n; }

    //! Accessor
    long getMaxOrder() const { return _maxOrder; }

    //! Accessor
    void setMaxOrder(long n) { _maxOrder=n; }

    //! Accessor
    const DateType& getMinDate() const { return _minDate; }

    //! Accessor
    void setMinDate(const DateType& d) { _minDate=d; }

    //! Accessor
    const DateType& getMaxDate() const { return _maxDate; }

    //! Accessor
    void setMaxDate(const DateType& d) { _maxDate=d; }

    //! Accessor
    void setMyChronicle(Chronicle* c) { _myChronicle = c; }

    //! Accessor
    Chronicle* getMyChronicle() { return _myChronicle; }

    //! Accessor, via the chronicle
    RecognitionEngine* getMyEngine();

    //! Copies the dates and orders
    void copyDateAndOrder(const Event& e);

    //! Copies the dates and orders
    void copyDateAndOrder(const RecoTree& r);

    //! Copies the dates and orders
    void copyDateAndOrder(const RecoTree& r1, const RecoTree& r2);

    //! Copies the dates and orders
    void copyDateAndOrder(const RecoTree& r1, const Event& r2);

    //! Accessor (valid if #_arity is 1);
    virtual const Event* getEvent() const = 0;

    //! Accessor (valid if #_arity is 2)
    virtual const RecoTree* getLeftMember() const = 0;

    //! Accessor (valid if #_arity is 2)
    virtual const RecoTree* getRightMember() const = 0;

    //! Returns true if the input tree is the same as "this"
    virtual bool equal(const RecoTree* t) const = 0;

    //! Returns the most recent (leaf) event of a tree
    virtual const Event* lastEvent() const = 0;

    //! Tree display function
    virtual void prettyPrint(std::ostream& os, int ntab) const = 0;

    //! Output flow for tests
    friend std::ostream& operator<<(std::ostream& os, const RecoTree& t) {
      t.prettyPrint(os, 0);
      return os;
    }

  }; // class RecoTree

} /* namespace CRL */

#endif /* RECO_TREE_H_ */
