/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  @file
  @brief implements distributed event definition class

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include <openfluid/core/Event.hpp>

#include <openfluid/tools/SwissTools.hpp>

namespace openfluid { namespace core {

Event::Event():
  InstantiationInfo(openfluid::core::InstantiationInfo::UNKNOWN),
  m_Date(DateTime())
{

}


// =====================================================================
// =====================================================================


Event::Event(DateTime Date):
  InstantiationInfo(openfluid::core::InstantiationInfo::UNKNOWN),
  m_Date(Date)
{

}


// =====================================================================
// =====================================================================


Event::~Event()
{

}


// =====================================================================
// =====================================================================


bool Event::isInfoExist(const std::string Key) const
{
  return m_Infos.count(Key);
}


// =====================================================================
// =====================================================================


bool Event::isInfoEqual(const std::string Key, const std::string Value) const
{
  return (isInfoExist(Key) && m_Infos.at(Key).get() == Value);
}


// =====================================================================
// =====================================================================


bool Event::isInfoEqual(const std::string Key, const long Value) const
{
  long TmpValue;

  return (getInfoAsLong(Key,TmpValue) && TmpValue == Value);
}


// =====================================================================
// =====================================================================


bool Event::isInfoEqual(const std::string Key, const double Value) const
{
  double TmpValue;

  return (getInfoAsDouble(Key,TmpValue) && openfluid::tools::IsCloseEnough(TmpValue,Value,0.00001));
}


// =====================================================================
// =====================================================================


bool Event::isInfoEqual(const std::string Key, const double* Value) const
{
  return isInfoEqual(Key,*Value);
}


// =====================================================================
// =====================================================================


bool Event::isInfoEqual(const std::string Key, const DoubleValue& Value) const
{
  DoubleValue TmpValue;

  return (getInfoAsDoubleValue(Key,TmpValue) && openfluid::tools::IsCloseEnough(TmpValue.get(),Value.get(),0.00001));
}


// =====================================================================
// =====================================================================


bool Event::getInfoAsString(const std::string Key, std::string *Info) const
{
  return getInfoAsString(Key,*Info);
}


// =====================================================================
// =====================================================================


bool Event::getInfoAsString(const std::string Key, std::string& Info) const
{
  if(isInfoExist(Key))
  {
    Info = m_Infos.at(Key).get();

    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


bool Event::getInfoAsLong(const std::string Key, long *Info) const
{
  return getInfoAsLong(Key,*Info);
}


// =====================================================================
// =====================================================================


bool Event::getInfoAsLong(const std::string Key, long& Info) const
{
  return (isInfoExist(Key) && m_Infos.at(Key).toInteger(Info));
}


// =====================================================================
// =====================================================================


bool Event::getInfoAsDouble(const std::string Key, double *Info) const
{
  return getInfoAsDouble(Key,*Info);
}


// =====================================================================
// =====================================================================


bool Event::getInfoAsDouble(const std::string Key, double& Info) const
{
  return (isInfoExist(Key) && m_Infos.at(Key).toDouble(Info));
}


// =====================================================================
// =====================================================================

bool Event::getInfoAsScalarValue(const std::string Key, double* Info) const
{
  return getInfoAsDouble(Key,*Info);
}

// =====================================================================
// =====================================================================


bool Event::getInfoAsDoubleValue(const std::string Key, DoubleValue& Info) const
{
  return (isInfoExist(Key) && m_Infos.at(Key).toDoubleValue(Info));
}


// =====================================================================
// =====================================================================


bool Event::addInfo(const std::string Key, const std::string Info)
{
  if (!isInfoExist(Key))
  {
    m_Infos[Key] = openfluid::core::StringValue(Info);

    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


void Event::println() const
{
  std::cout << "Date : " << m_Date.getAsISOString() << std::endl;
  std::cout << "Infos : " << std::endl;

  EventInfosMap_t::const_iterator it;
  for (it = m_Infos.begin();it != m_Infos.end();++it)
  {
    std::cout << " - " << it->first << " = " << it->second.get() << std::endl;
  }
}



} }
