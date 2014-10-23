/** ***********************************************************************************
 * \file RecognitionEngine.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Recognition Engine
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

#include <cstdlib>
#include <sstream>
#include <iostream>
#include <typeinfo>

#include "RecognitionEngine.h"


// ----------------------------------------------------------------------------
// CLASS METHODS
// ----------------------------------------------------------------------------

namespace CRL 
{
  /** By default, the insertion policy is #LAST_EVENT, 
  *   current date is zero, and old recognitions are not purged.
  */
  RecognitionEngine::RecognitionEngine() 
    : _currentTime(NO_DATE), _currentOrder(0),
      _insertionPolicy(LAST_EVENT), _verbosityLevel(SILENT), 
      _outputLog(NULL), _purgeOldRecognitions(false)
  {
  }


  /** By default, the insertion policy is #LAST_EVENT, 
  *    current date is zero, and old recognitions are not purged.
  */
  RecognitionEngine::RecognitionEngine(std::ostream* out, 
                                       VerbosityLevel lvl) 
    : _currentTime(NO_DATE), _currentOrder(0),
      _insertionPolicy(LAST_EVENT), _verbosityLevel(lvl), 
      _outputLog(out), _purgeOldRecognitions(false)
  {
    CRL_LOG(VERBOSE) << "Engine created  : "
                     << "t = " << _currentTime
                     << ", n = " << _currentOrder
                     << ", policy = " << insertionPolicyToString()
                     << std::endl
                     << std::flush;
  }


  /** \param[in] cr pointer to the chronicle which is to be detected
  */
  void RecognitionEngine::addChronicle(CRL::Chronicle* cr)
  {
    if (cr->getMyEngine() != NULL)
    {
      CRL_LOG(WARNING) << "Chronicle : " << cr->toString() << " is already"
                       << " in a recognition engine" << std::endl;
    }
    else
    {
      cr->setPurgeable(false);
      _rootChronicles.push_back(cr);
      cr->setMyEngine(this);
      CRL_LOG(VERBOSE) << "Added chronicle : " << cr->toString() << std::endl
                       << std::flush;
    }               
  }


  /** \param[in] cr chronicle which is to be detected
  */
  void RecognitionEngine::addChronicle(CRL::Chronicle& cr)
  {
    addChronicle(&cr);
  }

  /** Deletes all the pointers to the chronicles, in list #_rootChronicles,
  *   but does not call the destructors of the chronicles.
  */
  void RecognitionEngine::clearChronicleList()
  {
    std::list<Chronicle*>::iterator it;
    for(it=_rootChronicles.begin(); it!=_rootChronicles.end(); it++)
      (*it)->setMyEngine(NULL);

    _rootChronicles.clear();
  }


  /** Method which adds an event to the input buffer (flow) of the recognition engine
  *   It is \b important to know that the engine does not try to
  *   use the event to recognise a chronicle (this is done through
  *   method #process).
  *
  *   The event is places in the input buffer at its place:
  *    - between two events: a <= e < b
  *    - at the end if it is not dated (case of #LAST_EVENT)
  *    - at current date if it is not dated (case of #CURRENT_TIME)
  *
  *  If the event is not dated, it will be dated by the engine, following the 
  *  insertion policy.
  *
  *   \param[in] e pointer to the event to be inserted in the flow.
  */
  void RecognitionEngine::addEvent(CRL::Event* e)
  {
    CRL_LOG(DETAILED) << "Evts buffer ==> : " << eventBufferToString() << std::endl << std::flush;

    // If the buffer is empty
    if (_eventBuffer.empty())
    {
      // If the event is not dated, it is dated at current time
      if (e->getDate() == NO_DATE)
        e->setDate(this->_currentTime);

      e->setOrder(_currentOrder); _currentOrder++;
      _eventBuffer.push_back(e);
    }
    else  // otherwise, the buffer contains elements
    {
      // Iterator
      std::list<CRL::Event*>::iterator it;

      // If the event is not dated, it is inserted, either at the end by dating it 
      // at the date of the event preceding it, either at the current date 
      // of the engine.
      if ((e->getDate() == NO_DATE) && (_insertionPolicy == LAST_EVENT))
      {
        it = _eventBuffer.end();              // "After" the last element
        it--;                                 // Last element
        e->setDate((*it)->getDate());
        e->setOrder(_currentOrder); _currentOrder++;
        _eventBuffer.push_back(e);
      }
      else
      {
        if ((e->getDate() == NO_DATE) && (_insertionPolicy == CURRENT_TIME))
          e->setDate(this->_currentTime);

        // Error case : an event prior to _currentTime is rejected
        if (e->getDate() < this->_currentTime)
          throw("Evenement de date anterieure a currentTime");

        // Its place is sought for, and it is inserted
        it = _eventBuffer.begin();
        while ( ( it != _eventBuffer.end() ) && ( (*it)->getDate() <= e->getDate() ) )
          it++;

        if (it == _eventBuffer.end()) {
          it--;
          e->setOrder((*it)->getOrder()+1);
          it++;
        }
        else {
          e->setOrder((*it)->getOrder());
          std::list<CRL::Event*>::iterator it2;
          for (it2=it; it2!=_eventBuffer.end(); it2++)
            (*it2)->setOrder((*it2)->getOrder()+1);
        }
        _currentOrder++;
        _eventBuffer.insert(it, e);
      }
    }
    CRL_LOG(VERBOSE) << "Added Event     : " << e->getName()
                     << "\t t = " << e->getDate()
                     << ", n = " << e->getOrder()
                     << std::endl
                     << std::flush;
    CRL_LOG(DETAILED) << "Evts buffer <== : " << eventBufferToString() << std::endl << std::flush;
  }


  /** \param[in] e pointer to the event to be inserted in the flow.
  */
  void RecognitionEngine::addEvent(CRL::Event& e)
  {
    addEvent(&e);
  }



  /** \param[in] name name of the event to be inserted in the input buffer
  */
  RecognitionEngine& RecognitionEngine::operator<<(const char* name)
  {
    this->addEvent(new Event(std::string(name)));
    return *this;
  }


  /** \param[in] name name of the event to be inserted in the input buffer
  */
  RecognitionEngine& RecognitionEngine::operator<<(const std::string& name) 
  {
    this->addEvent(new Event(name));
    return *this;
  }


  /** \param[in] e event to be added to the input buffer
  */
  RecognitionEngine& RecognitionEngine::operator<<(CRL::Event &e) 
  {
    this->addEvent(&e);
    return *this;
  }


  /** \param[in] e pointer to the event to be added to the input buffer
  */
  RecognitionEngine& RecognitionEngine::operator<<(CRL::Event *e) 
  {
    this->addEvent(e);
    return *this;
  }


  /** \param[in] d date to be inserted in the flow
  */
  RecognitionEngine& RecognitionEngine::operator<<(const double& d)
  {
	  DateType dd = d;
    this->addEvent(new Event(dd));
    return *this;
  }



  /** Deletes all the pointers to the events, in the input buffer
  *   #_eventBuffer, before their recognition. Does not call the
  *   destructors of the events.
  */
  void RecognitionEngine::clearEventBuffer()
  {
    _eventBuffer.clear();
    CRL_LOG(DETAILED) << "Evts buffer <== : " << eventBufferToString() << std::endl << std::flush;
  }


  /** Processes the events of the input buffer, and updates the recognition sets
  *   of all the chronicles present in the engine.
  *   The processed events are then removed from the input buffer, gradually
  *   as they are processed.
  *   \param[in] date date until which we want to go (by default : infinity)
  *   \return number of processed events
  */
  int RecognitionEngine::process(const DateType& date)
  {
    int count = 0;
    std::list<CRL::Event*>::iterator it = _eventBuffer.begin();

    // As long as the end of the buffer has not been reached and as long as the date of the events
    // to be processed is prior or equal to "d"
    while ( (it != _eventBuffer.end()) && ((*it)->getDate() <= date) )
    {
    	DateType look=this->lookAhead();
    	if (look < (*it)->getDate())
    	{
        Event* e = new Event(look);
        process(e);
        _eventBuffer.remove(e);
        this->_currentTime = look;
    	}
    	else
    	{
        this->_currentTime = (*it)->getDate();
        processEvent((*it)->getDate(), *it);
        count++;
        it=_eventBuffer.erase(it);
    	}
    }

    // One processing step, without any event
    if  (date != INFTY_DATE)
    {
      while (this->_currentTime < date)
      {
        DateType look=this->lookAhead();
        if (look < date)
        {
          Event* e = new Event(look);
          process(e);
          _eventBuffer.remove(e);
          this->_currentTime = look;
        }
        else
        {
          Event* e = new Event(date);
          process(e);
          _eventBuffer.remove(e);
          this->_currentTime = date;
        }
      }
    }
    CRL_LOG(VERBOSE) << "Processed evts  : " << count 
                     << "\t t = " << _currentTime << std::endl
                     << std::flush;
    return count;
  }


  /** Adds event \e e to the input buffer, and then processes it by 
  *   updating the recognition sets of all the chronicles.
  *   \param[in] e pointer to the event to be taken into account
  */
  int RecognitionEngine::process(CRL::Event *e)
  {
    addEvent(e);
    processEvent(e->getDate(), e);
    return 1;
  }


  /** Adds event \e e to the input buffer, and then processes it by 
  *   updating the recognition sets of all the chronicles.
  *   \param[in] e event to be taken into account
  */
  int RecognitionEngine::process(CRL::Event &e)
  {
    return process(&e);
  }


  /** The function returns the minimum of the lookahead of all the active chronicles.
  *   In other words, the date until which the recognition engine
  *   may go forward to (throug method #process), while being sure that no new
  *   recognition will occur until this date.
  *   \return the minimum of the lookahead of all the active chronicles
  */
  DateType RecognitionEngine::lookAhead() const 
  {
    DateType tmpDate, dateMin = INFTY_DATE;
    std::list<CRL::Chronicle*>::const_iterator it;
    for (it=_rootChronicles.begin(); it!=_rootChronicles.end();it++)
    {  
      tmpDate = (*it)->lookAhead(_currentTime);
      if (tmpDate < dateMin) 
        dateMin = tmpDate;
    }

    CRL_LOG(DETAILED) << "Lookahead       : " << dateMin << std::endl << std::flush;
    return dateMin;
  }


  /** Internal class method. Processes event \a e by placing itself
  *   at time \a d and calling the methods Chronicle::process() of all
  *   the chronicles to be recognised.
  *   \param[in] d date to be considered for the recognition
  *   \param[in] e pointer to the event to be processed
  */
  void RecognitionEngine::processEvent(const DateType& d, CRL::Event *e)
  {
    if (_purgeOldRecognitions) purgeOldRecognitions();
    bool flag;
    std::list<CRL::Chronicle*>::iterator it;
    for (it=_rootChronicles.begin(); it!=_rootChronicles.end();it++)
    {
      flag = (*it)->process(d, e);
      if (flag) {
        CRL_LOG(VERBOSE) << "Chronicle       : " << (*it)->toString() << " recognition at" 
                         << " (" << d << ")" << std::endl << std::flush;
        CRL_LOG(DETAILED) << "                  " << (*it)->prettyPrint() << std::endl << std::flush;
      }
    }
    purgeNewRecognitions();
  }


  /** Internal class method. Calls method Chronicle::purgeNewRecognitions()
  *   on all the chronicles to be recognised. This method empties the eponymous list.
  */
  void RecognitionEngine::purgeNewRecognitions()
  {
    std::list<CRL::Chronicle*>::iterator it;
    for (it=_rootChronicles.begin(); it!=_rootChronicles.end();it++)
    {
      (*it)->purgeNewRecognitions();
      (*it)->purgeRecognitionsIfPurgeable();
    }
  }

  /** Internal class method. Calls method Chronicle::purgeOldRecognitions()
   * on all chronicles to be purged because too old.
   */
  void RecognitionEngine::purgeOldRecognitions()
  {
    std::list<CRL::Chronicle*>::iterator it;
    for (it=_rootChronicles.begin(); it!=_rootChronicles.end();it++)
    {
      (*it)->purgeOldRecognitions();
    }
  }



  /** \return string with the #_insertionPolicy name
  */
  std::string RecognitionEngine::insertionPolicyToString() const
  {
    if (_insertionPolicy == LAST_EVENT)
      return std::string("LAST_EVENT");
    else if (_insertionPolicy == CURRENT_TIME)
      return std::string("CURRENT_TIME");
    else
      return std::string("UnknownPolicy");
  }


  /** The insertion policy consists in inserting the non dated events
  *   at the end of the input buffer.
  */
  void RecognitionEngine::setPolicyLastEvent()
  {
    _insertionPolicy = LAST_EVENT;
    CRL_LOG(VERBOSE) << "Insert. policy  : LAST_EVENT" << std::endl << std::flush;
  }


  /** The insertion policy consists in inserting the non dated events
  *   at date #_currentTime of the recognition engine.
  */
  void RecognitionEngine::setPolicyCurrentTime()
  {
    _insertionPolicy = CURRENT_TIME;
    CRL_LOG(VERBOSE) << "Insert. policy  : CURRENT_TIME" << std::endl << std::flush;
  }

  /** Activates the deleting policy of too old recognitions
   *  recursively and for all chronicles, setting the peremption
   *  duration to \a d.
   *  \param[in] d is the value of the peremption duration
   */
  void RecognitionEngine::activateForget(double d)
   {
    _purgeOldRecognitions=true;
    std::list<CRL::Chronicle*>::iterator it;
    for (it=_rootChronicles.begin(); it!=_rootChronicles.end();it++)
    {
      (*it)->setPeremptionDuration(d, true);
    }
   }


  /** Displays the content of a list of events (for example the input buffer of
  *   an engine) in the form of a string.
  *   \param[in] s the list of events
  *   \return string "{ (e1,t1), (e2,t2), ... }"
  */
  std::string RecognitionEngine::eventListToString(const std::list<Event*> &s)
  {
    std::stringstream ss;
    ss << "{";
    std::list<Event*>::const_iterator it;
    for (it=s.begin(); it!=s.end();it++)
    {
      ss << "(" << (*it)->getName() << "," << (*it)->getDate() << ")" ;
    }
    ss << "}";
    return ss.str();
  }


  /** Uses method #eventListToString().
  *   \return string "{ (e1,t1), (e2,t2), ... }"
  */
  std::string RecognitionEngine::eventBufferToString() const 
  {
    return eventListToString(_eventBuffer);
  }


  /** Takes as input a text flow in the form "(e1,t1), (e2,t2), ..."
  *   and adds the corresponding events to the recognition engine.
  *   \param[in,out] is text flow
  *   \param[in] engine recognition engine
  *   \return flow \a is
  */
  std::istream& operator>>(std::istream& is, RecognitionEngine& engine)
  {
    char c;
    std::string tmp;
    while (   ( ((c=is.peek())!='\n') && (c!='\r') )
      && (!std::getline(is, tmp, '(').eof())   )
    {
      std::string name, date;
      std::getline(is, name, ',');
      std::getline(is, date, ')');
      engine.addEvent(new Event(name, strtod(date.c_str(),NULL)));
    }
    return is;
  }

  /** Applies a function to an instance of RecognitionEngine.
  *   \param[in] engine instance
  *   \param[in] f pointer to the function which is to be applied
  *   \return modified instance of engine
  */
  RecognitionEngine& operator<<(RecognitionEngine& engine, 
                                void (*f)(RecognitionEngine&))
  {
    (*f)(engine);
    return engine;
  }

  
  /** Equivalent to \code process(INFINI) \endcode.
  *   \param[in] engine instance
  */
  void flush(RecognitionEngine& engine)
  {
    engine.process(INFTY_DATE);
  }

  /** Equivalent to \code clearEventBuffer() \endcode.
  *   \param[in] engine instance
  */
  void clear(RecognitionEngine& engine)
  {
    engine.clearEventBuffer();
  }


} /* namespace CRL */
