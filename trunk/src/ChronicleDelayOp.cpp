/** ***********************************************************************************
 * \file ChronicleDelayOp.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Base virtual class of Chronicles with delay
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

#include "ChronicleDelayOp.h"
#include <sstream>


// ----------------------------------------------------------------------------
// CLASS METHODS
// ----------------------------------------------------------------------------

namespace CRL 
{

  /** \param[in] opL desired chronicle
  *   \param[in] d  duration associated to the chronicle
  */ 
  ChronicleDelayOp::ChronicleDelayOp(Chronicle* opL, const DurationType& d) 
    : _opLeft(opL),_delay(d) 
  {
  }


  /** Empties the temporary set of the last recognitions.
  *   The argument \a daughtersOnly is here to deal with the special case
  *   of certain chronicles which temporary set must not be emptied
  *   (only those of the sub-chronicles will be emptied recursively).
  *   \param[in] daughtersOnly indicates to empty only the sub-chronicles
  */
  void ChronicleDelayOp::purgeNewRecognitions(bool daughtersOnly)
  {
    Chronicle::purgeNewRecognitions(daughtersOnly);
    _opLeft->purgeNewRecognitions();
  }


  //! Empties the recognition set (for optimisation ends)
  void ChronicleDelayOp::purgeRecognitionsIfPurgeable()
  {
    Chronicle::purgeRecognitionsIfPurgeable();
    _opLeft->purgeRecognitionsIfPurgeable();
  }

  /** Sets peremption duration to \a duration,
  * recursively if \a recursive.
  * \param[in] duration indicates the peremption duration
  * \param[in] recursive indicates whether the setting of the peremption duration should be recursive
  */
    void ChronicleDelayOp::setPeremptionDuration(double duration, bool recursive)
  {
    Chronicle::setPeremptionDuration(duration, recursive);
    if (recursive)
    {
    _opLeft->setPeremptionDuration(duration, recursive);
    }
  }


   //! Deletes too old recognitions recursively.
   void ChronicleDelayOp::purgeOldRecognitions()
   {
    Chronicle::purgeOldRecognitions();
    _opLeft->purgeOldRecognitions();
   }



  /** Displays the chronicle as a string : the definition of  
  *   the chronicle in a condensed form (ex : \code (a & b) + c \endcode),
  *   and the content of its recognition sets.
  *   \return string
  */
  std::string ChronicleDelayOp::prettyPrint() const
  {
    std::stringstream ss;
    ss << _delay;
    return this->Chronicle::prettyPrint() + std::string("\n")
      + this->_opLeft->prettyPrint() + std::string("\n")
      + ss.str() + std::string("\n");
  }



} /* namespace CRL */
