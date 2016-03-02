/** ***********************************************************************************
 * \file Chronicle.h
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Base virtual class of Chronicles
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


#ifndef CHRONICLE_H_
#define CHRONICLE_H_

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include <string>
#include <set>
#include <iostream>

#include "Event.h"
#include "Context.h"
#include "RecoTree.h"
#include "Property.h"


// ----------------------------------------------------------------------------
// CLASS DESCRIPTION
// ----------------------------------------------------------------------------

namespace CRL {

  class RecognitionEngine;

  class Chronicle 
  {
  public:

    //! Data type : set of recognition trees
    typedef std::set<RecoTree*> RecoSet;

  protected:

    //! Name of the chronicle (default = "")
    std::string _name;

    //! Recognition set of the chronicle
    RecoSet _recognitionSet;

    //! Set of the new recognitions of the chronicle (during #process)
    RecoSet _newRecognitions;

    //! Evaluation context of the (possible) predicate of the chronicle
    Context _evaluationContext;

    //! Resulting context (evaluation context possibly modified)
    Context _resultingContext;

    //! Indicates whether de recognitions may be emptied (Indique si on peut vider les reconnaissances (optimisation purpose)
    bool _purgeable;

    //! Indicates whether the chronicle has already been treated, for the current event
    bool _alreadyProcessed;

    //! Indicates whether the chronicle has had new recognitions, for the current event
    bool _hasNewRecognitions;

    //! To be put to true in a sub-class with overwriting of outputPropertiesMethod
    bool _hasOutputPropertiesMethod;

    //! Link to the engine in which is the chronicle
    RecognitionEngine* _myEngine;

    //! Pointer to the USER function of the predicate
    bool (*_predicateFunction)(const PropertyManager& properties);

    //! Pointer to the USER function calculating new properties
    void (*_outputFunction)(const PropertyManager& inProps,
                            PropertyManager& outProps);

    //! Pointer to the USER function of actions at a recognition
    void (*_actionFunction)(RecoTree& rc);

    //! Peremption duration
    DurationType _peremptionDuration;

  public:

    //! Constructor, by default purgeable
    Chronicle()
      : _name(""), _purgeable(true), 
        _alreadyProcessed(false), _hasNewRecognitions(false), _hasOutputPropertiesMethod(false),
        _myEngine(NULL), _predicateFunction(NULL), _outputFunction(NULL), _actionFunction(NULL), _peremptionDuration(-1.0) { }

  protected:

    //! Destructor, virtual since abstract class, protected to prevent heap allocation
    virtual ~Chronicle();

  public:

    //! Calls the destructor
    void destroy() { delete this; }

    //! Deletes sub-chronicles (if any), and calls the destructor
    virtual void deepDestroy() { delete this; }

    //! Function to be re-defined in the sub-classes
    virtual std::string toString() const = 0;

    //! Display function for unit tests
    virtual std::string prettyPrint() const;

    //! Accessor
    void setName(const std::string& s) { _name = s;}

    //! Accessor
    const std::string& getName() const { return _name; }

    //! Main event processing function
    virtual bool process(const DateType& d, CRL::Event* e = NULL) = 0;

    //! Accessor
    bool isPurgeable() const { return _purgeable; }

    //! Accessor
    void setPurgeable(bool b) { _purgeable = b; }

    //! Accessor (const version)
    const RecoSet& getNewRecognitions() const { return _newRecognitions; }

    //! Accessor (non const version)
    RecoSet& getNewRecognitions() { return _newRecognitions; }

    //! Accessor (const version)
    const RecoSet& getRecognitionSet() const { return _recognitionSet; }

    //! Accessor (non const version)
    RecoSet& getRecognitionSet() { return _recognitionSet; }

    //! Accessor (const version)
    const Context& getEvaluationContext() const { return _evaluationContext; }

    //! Accessor (non const version)
    Context& getEvaluationContext() { return _evaluationContext; }

    //! Accessor
    void setEvaluationContext(const Context& c) { _evaluationContext = c; }

    //! Accessor (const version)
    const Context& getResultingContext() const { return _resultingContext; }

    //! Accessor (non const version)
    Context& getResultingContext() { return _resultingContext; }

    //! Accessor
    void setResultingContext(const Context& subc) { _resultingContext = subc; }

    //! Accessor
    bool isAlreadyProcessed() const { return _alreadyProcessed; }

    //! Accessor
    void setAlreadyProcessed(bool b) { _alreadyProcessed = b; }

    //! Accessor
    void setPredicateFunction(bool (*p)(const PropertyManager&)){
      _predicateFunction=p; }

    //! Accessor
    void setOutputPropertiesFunction(void (*p)(const PropertyManager&, PropertyManager&)){
      _outputFunction=p; }

    //! Accessor
    void setActionFunction(void (*p)(RecoTree&)) {
      _actionFunction = p;
    }

    //! Accessor, links to the recognition engine
    virtual void setMyEngine(RecognitionEngine* e) = 0;

    //! Accessor
    RecognitionEngine* getMyEngine() { return _myEngine; }

    //! Empties the new recognition set
    virtual void purgeNewRecognitions(bool daughtersOnly = false);

    //! Empties the recognition set (optimisation purpose)
    virtual void purgeRecognitionsIfPurgeable();

    //! Accessor
    DurationType getPeremptionDuration() { return _peremptionDuration; }

    //! Accessor
    virtual void setPeremptionDuration(double duration, bool recursive){ _peremptionDuration = duration; }

    //! Empties the recognition sets of too old recognitions (optimisation purpose)
    virtual void purgeOldRecognitions();

    //! Determines whether a recognition tree belongs to a given set
    static bool isIn(const RecoTree& elmt, const RecoSet& rset);

    //! Returns the date in the future at which the recognitions set of the chronicle must be re-assessed
    virtual DateType lookAhead(const DateType& tcurr) const = 0;

    //! Calls the predicate function or method
    bool applyPredicate(const PropertyManager& pm);

    //! Calls the output properties calculating function or method
    void applyOutputFunction(const PropertyManager& in, PropertyManager& out);

    //! Method called during a new recognition (version 1)
    void applyActionFunction(RecoTree& rc);

    //! Method called during a new recognition (version 2)
    void applyActionFunction(RecoTree* rc);

    //! Tests during recognitions whether an action has been provided by the user 
    bool hasOutputFunction() const;

    //! Output flow for tests
    friend std::ostream& operator<<(std::ostream& os, const Chronicle& cr);

    //! Return sub-chronicle if exist (left member for binary chronicles)
    virtual Chronicle* getChild1() = 0;

    //! Return sub-chronicle if exist (right member for binary chronicles)
    virtual Chronicle* getChild2() = 0;

    //! Return the (first) sub-chronicle
    Chronicle* getChild() { return getChild1(); }

    //! Returns the left part of binary chrronicles
    Chronicle* getChildLeft() { return getChild1(); }

    //! Return the right part of binary chronicles
    Chronicle* getChildRight() { return getChild2(); }


  protected:


    //! USER method defining a predicate
    virtual bool predicateMethod(const PropertyManager&) {
      return true; /* Default implementation */
    }

    //! USER method calculating output properties
    virtual void outputPropertiesMethod(const PropertyManager& inProps,
                                        PropertyManager& outProps) {
      // Default : no implementation
      // Beware : if a subclass of Chronicle overloads this method
      // you must set the boolean _hasOutputPropertiesMethod to 'true' for
      // instances of this subclass.
    }

    //! USER method called after a new recognition
    virtual void actionMethod(const RecoTree&) { 
      // Default : no implementation
    }

  }; // class Chronicle

} /* namespace CRL */

#endif /* CHRONICLE_H_ */
