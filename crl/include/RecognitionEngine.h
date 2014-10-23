/** ***********************************************************************************
 * \file RecognitionEngine.h
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Manage a buffer of incoming events and a set of chronicles to recognize
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

#ifndef RECOGNITION_ENGINE_H_
#define RECOGNITION_ENGINE_H_

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include <list>
#include <iostream>

#include "Event.h"
#include "Chronicle.h"


// ----------------------------------------------------------------------------
// CLASS DESCRIPTION
// ----------------------------------------------------------------------------

namespace CRL {

  class RecognitionEngine 
  {
  public:

    //! Internal type : level of verbosity in the output flow (log)
    enum VerbosityLevel { SILENT=0, WARNING=1, VERBOSE=2, DETAILED=3 };

  protected:

    //! Chronicles to be recognised
    std::list<CRL::Chronicle*> _rootChronicles;

    //! Input event buffer
    std::list<CRL::Event*> _eventBuffer;

    //! Current time
    DateType _currentTime;

    //! Current order
    long _currentOrder;

    //! Buffer insertion policy for non dated events
    enum { LAST_EVENT, CURRENT_TIME } _insertionPolicy;
    
    //! Current recognition engine verbosity level 
    VerbosityLevel _verbosityLevel;

    //! Output flows (logs)
    std::ostream* _outputLog;

    //! Indicates whether too old recognitions have to be purged
    bool _purgeOldRecognitions;

  public:

    //! Default constructor
    RecognitionEngine();

    //! Constructor with an output flow for logs
    RecognitionEngine(std::ostream* out, VerbosityLevel lvl = WARNING);

    //! Adds a chronicle in the list of the chronicles to be recognised (#_rootChronicles)
    void addChronicle(CRL::Chronicle *cr);

    //! Adds a chronicle in the list of the chronicles to be recognised  (#_rootChronicles)
    void addChronicle(CRL::Chronicle &cr);

    //! Empties the list of the chronicles to be recognised
    void clearChronicleList();

    //! Adds an event to the buffer, handles its date and order
    void addEvent(CRL::Event* e);

    //! Adds an event to the buffer, handles its date and order
    void addEvent(CRL::Event& e);

    //! Adds an event named \a name to the input buffer
    RecognitionEngine& operator<<(const char* name);

    //! Adds an event named \a name to the input buffer
    RecognitionEngine& operator<<(const std::string& name);

    //! Adds an event to the input buffer
    RecognitionEngine& operator<<(CRL::Event &e);

    //! Adds an event to the input buffer
    RecognitionEngine& operator<<(CRL::Event *e);

    //! Adds a delay to the input buffer which will increase #_currentTime
    RecognitionEngine& operator<<(const double& d);

    //! Empties the input buffer of all the events
    void clearEventBuffer();

    //! Updates all the recognition sets until instant d
    int process(const DateType& date = INFTY_DATE);

    //! Inserts an event and updates the recognition sets
    int process(CRL::Event *e);

    //! Inserts an event and updates the recognition sets
    int process(CRL::Event &e);

    //! Returns the minimal date up to which the engine can go forward
    DateType lookAhead() const;

    //! Accessor, returns the list of chronicles to be recognised
    const std::list<CRL::Chronicle*>& getRootChronicles() const { return _rootChronicles; }

    //! Accessor, returns the event input buffer
    const std::list<CRL::Event*>& getEventBuffer() const { return _eventBuffer; }

    //! Accessor, returns the order the last event processed
    long getCurrentOrder() const { return _currentOrder; }

    //! Accessor
    void setCurrentOrder(long  currentOrder) { _currentOrder = currentOrder; }

    //! Accessor, returns the current time of the recognition engine
    DateType getCurrentTime() const { return _currentTime; }

    //! Accessor
    void setCurrentTime(const DateType& currentTime) { _currentTime = currentTime; }

    //! Accessor
    bool isInsertionPolicyLastEvent() const { return (_insertionPolicy==LAST_EVENT); }

    //! Accessor
    bool isInsertionPolicyCurrentTime() const { return (_insertionPolicy==CURRENT_TIME); }

    //! Accessor, returns the attribute as a string
    std::string insertionPolicyToString() const;

    //! Applies the #LAST_EVENT events insertion policy
    void setPolicyLastEvent();

    //! Applies the #LAST_EVENT events insertion policy
    void setPolicyCurrentTime();

    //! Activates the deleting policy of too old recognitions
    void activateForget(double d);

    //! Accessor
    void setVerbosityLevel(VerbosityLevel v) { _verbosityLevel = v; }

    //! Accessor, returns the output flow (log)
    std::ostream* getOutputLog() { return _outputLog; }

    //! Modifies the output flow (log)
    void setOutputLog(std::ostream* log) { _outputLog = log; }

    //! Accessor, returns true if old recognitions have to be purged
    bool getPurgeOldRecognitions() const { return _purgeOldRecognitions; }

    //! Accessor
    void setPurgeOldRecognitions(bool  purgeOldRecognitions) { _purgeOldRecognitions = purgeOldRecognitions; }

    //! Displays a list of events as a string
    static std::string eventListToString(const std::list<Event*> &s);

    //! Displays the contents of the recognition engine input buffer
    std::string eventBufferToString() const;

    //! Sends events to the flow
    friend std::istream& operator>>(std::istream& is, RecognitionEngine& engine);

    //! Applies a method (flush, clear) to the recognition engine
    friend RecognitionEngine& operator<<(RecognitionEngine& engine, 
                                         void (*f)(RecognitionEngine&));

  protected:

    //! Processes an event at a given time, and updates the recognition sets
    void processEvent(const DateType& date, CRL::Event *e = NULL);

    //! Empties list Chronicle::_newRecognitions of all the chronicles
    void purgeNewRecognitions();

    //! Deletes too old recognitions
    void purgeOldRecognitions();

  }; // class RecognitionEngine

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // (Global) functions that may be used on the RecognitionEngine
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  //! Updates the recognition sets until \a lookAhead
  void flush(RecognitionEngine& engine);

  //! Empties the input buffer
  void clear(RecognitionEngine& engine);

  //! Output log macro conditioned by the verbosity level
  #define CRL_LOG(V) if ((V <= _verbosityLevel)&&(_outputLog!=NULL)) *_outputLog


} /* namespace CRL */

#endif /* RECOGNITION_ENGINE_H_ */
