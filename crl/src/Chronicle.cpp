/** ***********************************************************************************
 * \file Chronicle.cpp
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

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include "Chronicle.h"
#include "RecognitionEngine.h"
#include <limits>
#include <algorithm>
#include <sstream>


// --------------------------------------------------------------------
// PAGE DE DOCUMENTATION PRINCIPALE DOXYGEN
// --------------------------------------------------------------------

/** \mainpage CRL Documentation
 *  \section sec_intro Introduction
 *  The library CRL (Chronicle Recognition Library) has been developed
 *  by the Department for the Conception and evaluation of Sytem 
 *  Perfromances (DCPS) of the French Aerospace Lab (Office
 *  National d'Etudes et Recherches Aérospatiales -- ONERA). This
 *  C++ library includes 
 *
 *  \subsection sec_recup Où trouver la bibliothèque ?
 *  La bibliothèque FMT peut être récupérée de deux façons différentes :
 * 
 *  - une version stable se trouve disponible en permanence sur le serveur
 *  documentaire Knowledge Tree de DPRS (serveur «&nbsp;lala&nbsp;»)
 *  
 *  - les utilisateurs désireux de voir les dernières évolutions pourront 
 *  se procurer une version «&nbsp;bêta&nbsp;» via CVS sur le serveur
 *  "lili" de DPRS. Sous Windows, l'outil WinCVS offre une interface
 *  conviviale au dessus de CVS. 
 *
 *  Il est aussi possible d'obtenir la bibliothèque ou des éléments de la 
 *  bibliothèque en s'adressant aux membres de l'unité TCS (Techniques pour 
 *  la Conception et la Simulation de systèmes). Contacts :
 *  - Jean Bourrely   : Jean.Bourrely@onera.fr
 *  - Robert Cordeau  : Robert.Cordeau@onera.fr
 *  - Romain Kervarc  : Romain.Kervarc@onera.fr
 *  - Thierry Marchon : Thierry.Marchon@onera.fr
 *  - Claire Quillien : Claire.Quillien@onera.fr
 *
 *  \section sec_intall Installation
 *  Pour utiliser la bibliothèque, il suffit de disposer des fichiers
 *  "include" et des librairies compilées "lib". On peut aussi travailler
 *  directement sur les sources (fichiers ".cpp"). 
 *
 *  \subsection sec_debug La directive de compilation FMT_DEBUG
 *  Cette directive est définie pour les versions de Debug de la bibliothèque
 *  FMT. 
 *
 *  Pour définir une directive de compilation :
 *  - sous Visual C++, dans les "Settings" du projet, onglets "Preprocessor"
 *  et "Preprocessor directives", indiquer <tt>FMT_DEBUG</tt> dans la liste ;
 *  - sous GNU g++, utiliser l'option <tt>-DFMT_DEBUG</tt>.
 *
 *  \warning Un programme peut avoir un comportement différent en mode
 *  Debug ou Release. Un calcul peut par exemple lever une exception en mode 
 *  Debug, ce qui ne sera pas le cas en mode Release (on continue le calcul  
 *  avec des NAN, des +/-INF, ...). On risque alors d'avoir l'impression
 *  que le programme «&nbsp;marche&nbsp;»  en mode Release, et pas en Debug, MAIS CE N'EST
 *  PAS LE CAS ! <em>Il faut absolument résoudre tous les problèmes en mode Debug
 *  avant de passer à une version Release.</em>
 *
 *  \section sec_modules Les modules de la bibliothèque
 *  La bibliothèque FMT est séparée en plusieurs modules. D'un point de vue
 *  informatique, les modules n'ont pas d'existence concrète, si ce n'est pour
 *  leurs fichiers sources "cpp" qui se trouvent dans des répertoires séparés.
 *
 *  \todo Ajout d'un module de géométrie euclidienne.
 *
 *  Liste détaillée des modules:
 *  - \subpage General "Module général" : Il définit des macros, des méthodes 
 *    d'allocation, un mode de gestion des erreurs, un logger, des utilitaires 
 *    pour les tests unitaires.
 *  - \subpage Matrix_Vector "Module matrices-vecteurs" : Il définit deux classes 
 *    pour manipuler les nombres réels : FMT::DVector et FMT::DMatrix.
 *  - \subpage Probability "Module lois de probabilités" : Il contient
 *    la définition de plusieurs lois de probabilités : distributions
 *    \link FMT::UniformDistribution uniforme \endlink,
 *    \link FMT::NormalDistribution gaussienne \endlink,
 *    \link FMT::GammaDistribution loi gamma \endlink; ainsi que des
 *    générateurs de nombres aléatoires.
 */

/** \page Geometry Module Géométrie
 *  <CENTER><B>À VENIR</B></CENTER>
 */
 
/** \page General Module general
 *  Module commun à toute la bibliothèque. Il définit :
 *  - des macros : #NULL, #CONDITION(TST, MSG), 
 *    #DELETE_AND_NULLIFY(x), #DELETE_AND_NULLIFY_TAB(x)
 * 
 */




// ----------------------------------------------------------------------------
// CLASS METHODS
// ----------------------------------------------------------------------------

namespace CRL 
{

  /** Destructor, deletes the recognition sets
  */
  Chronicle::~Chronicle()
  {
    std::set<RecoTree*>::iterator it;
    for(it=_newRecognitions.begin(); it!=_newRecognitions.end(); it++)
    {
      if (_recognitionSet.find(*it) == _recognitionSet.end())
       delete (*it);
    }
    for(it=_recognitionSet.begin(); it!=_recognitionSet.end(); it++)
      delete (*it);
  }

  /** Displays the chronicle as a string: the definition 
  *   of the chronicle in a condensed form (ex: \code (a & b) + c \endcode),
  *   and the contents of its recognition sets.
  *   \return string
  */
  std::string Chronicle::prettyPrint() const
  {
    std::ostringstream os;
    os << "Chronicle       : " << this->toString() << std::endl;
    Chronicle::RecoSet::const_iterator it;
    os << "                  Reco Set = { ";
    for (it=_recognitionSet.begin();it!=_recognitionSet.end();it++)
      os << (**it);
    os << " } " << std::endl;;
   
    return os.str();
  }


  /** Empties the temporary set of the last recognitions.
  *   The argument \a daughtersOnly is here to deal with the special case
  *   of certain chronicles which temporary set must not be emptied
  *   (only those of the sub-chronicles will be emptied recursively).
  *   \param[in] daughtersOnly indicates to empty only the sub-chronicles
  */
  void Chronicle::purgeNewRecognitions(bool daughtersOnly) 
  {
    // Resets the flag which avoids double processing
    _alreadyProcessed = false;
    _hasNewRecognitions = false;
    
    if (!daughtersOnly)
      _newRecognitions.clear();
  }


  /** Deletes recognitions which are considered
   *  obsolete: the recognitions which have been created
   *  since more than _peremptionDuration.
   */
  void Chronicle::purgeOldRecognitions() 
  {
    if ( _peremptionDuration != NULL )
    {
      Chronicle::RecoSet::iterator itTmp, it=_recognitionSet.begin();

      DateType limitDate = _myEngine->getCurrentTime() - _peremptionDuration;
      while (it!=_recognitionSet.end())
      {
        if ((*it)->getMaxDate() < limitDate)
        {
          itTmp=it;
          itTmp++;
          _recognitionSet.erase(it);
          it=itTmp;
        }
        else ++it;
      }
      // Usefull when recursively inside a ''then''
      Chronicle::RecoSet::iterator itNew=_newRecognitions.begin();
      while (itNew!=_newRecognitions.end())
      {
        if ( (*itNew)->getMaxDate() < limitDate )
        {
          itTmp=itNew;
          itTmp++;
          _newRecognitions.erase(itNew);
          itNew=itTmp;
        }
        else ++itNew;
      }
    }
  }



  /** Empties the recognition set.
  *   This is a mechanism allowing, to a certain extent, an optimisation
  *   of memory management by deleting useless elements. Only
  *   certain chronicles are "purgeable", the most striking example is 
  *   chronicle OR.
  */
  void Chronicle::purgeRecognitionsIfPurgeable() 
  {
    if (_purgeable)
    {
      // The set of pointers may be emptied, but the destructors may not be called
      // since the objects are maybe used in a recognition set
      // above.
      _recognitionSet.clear();
    }
  }


  /** Scans a set of recognition trees, and returns \a true
  *   if one is found equal to \a elmt
  *   \param[in] elmt sought after recognition tree
  *   \param[in] rset recognition set
  *   \return true if belongs
  */
  bool Chronicle::isIn(const RecoTree& elmt, const RecoSet& rset)
  {
    Chronicle::RecoSet::const_iterator it;
    for (it=rset.begin();it!=rset.end();it++)
      if (elmt.equal(*it)) return true;
    return false;
  }


  /** \param[in] os output flow
  *   \param[in] cr chronicle to display
  *   \return modified output flow
  */
  std::ostream& operator<<(std::ostream& os, const Chronicle& cr)
  {
    os << cr.prettyPrint();
    return os;
  }


  /** Tests the (possible) predicate and returns true or false :
   *  - if there is no predicate function (NULL) : calls predicate method
   *  - if the predicate crashes (function or method) : returns false
   *  - otherwise, returns what the predicate function returns
   *  \param[in] pm property sets on which the predicate is applied
   *  \return result of the predicate
   */
  bool Chronicle::applyPredicate(const PropertyManager& pm)
  {
    try{
      if (_predicateFunction == NULL)
        return predicateMethod(pm);
      else
        return (*_predicateFunction)(pm);
    }
    catch(...){
      return false;
    }
  }


  /** Allows to compute new properties during a recognition. 
   *  If there exists a function (C), it is called, otherwise it is the class (or sub-class) method (C++)
   *  which is called.
   *  \param[in] in property set as argument of the function
   *  \param[out] out property set as output of the function
   */
  void Chronicle::applyOutputFunction(const PropertyManager& in, PropertyManager& out)
  {
    if (_outputFunction == NULL)
      outputPropertiesMethod(in, out);
    else
      (*_outputFunction)(in, out);
  }


  /** The method saves the new recognition in the lists
   *  \a _newRecognitions and \a _recognitionSet.
   *  It allows to carry out actions during a recognition. 
   *  If a function (C) exists, it is called, otherwise it is the class (or sub-class) method (C++)
   *  which is called.
   *  \param[in] rc new recognition triggering the action
   */
  void Chronicle::applyActionFunction(RecoTree& rc)
  {
    // 1) Saves the new recognition
    _newRecognitions.insert(&rc);
    _recognitionSet.insert(&rc);
    rc.setMyChronicle(this);
    _hasNewRecognitions = true;

    // 2) Applies the possible action method provided by the user
    if (_actionFunction == NULL)
      actionMethod(rc);
    else
      (*_actionFunction)(rc);
  }


  /** The method saves the new recognition in the lists
   *  \a _newRecognitions and \a _recognitionSet.
   *  It allows to carry out actions during a recognition. 
   *  If a function (C) exists, it is called, otherwise it is the class (or sub-class) method (C++)
   *  which is called.
   *  \param[in] rc new recognition triggering the action
   */
  void Chronicle::applyActionFunction(RecoTree* rc)
  {
    applyActionFunction(*rc);
  }


  /** Returns \a true if there is a function (C) computing new
   *  attributes, or a method provided by the user.
   *  \return user method indicator
   */
  bool Chronicle::hasOutputFunction() const
  {
    return ( (_outputFunction != NULL) || (_hasOutputPropertiesMethod) );
  }


} /* namespace CRL */
