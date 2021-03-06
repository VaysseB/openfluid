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
  \file MarketBinPackage.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#include <openfluid/market/MarketBinPackage.hpp>

#include <cstdlib>

#include <glibmm/error.h>
#include <glibmm/spawn.h>

namespace openfluid { namespace market {


MarketBinPackage::MarketBinPackage(openfluid::ware::WareID_t ID, std::string PackageURL)
                : MarketPackage(ID,PackageURL)
{

}


// =====================================================================
// =====================================================================


void MarketBinPackage::process()
{
  if (!m_Initialized)
    throw openfluid::base::OFException("OpenFLUID framework","MarketBinPackage::download()","package "+m_PackageFilename+" not initialized");


  if (!m_Downloaded)
    throw openfluid::base::OFException("OpenFLUID framework","MarketBinPackage::process()","package "+m_PackageFilename+" cannot be processed before download");


  if (m_CMakeCommand.empty())
    throw openfluid::base::OFException("OpenFLUID framework","MarketBinPackage::process()","CMake command not defined");


  std::string StrOut;
  std::string StrErr;
  int RetValue;

  std::string ProcessCommand = "\"" + m_CMakeCommand +"\" -E chdir \"" + m_MarketBagBinDir+ "\" \"" + m_CMakeCommand + "\" -E tar xfz \"" + m_PackageDest + "\"";

  try
  {
    appendToLogFile(m_PackageFilename,"processing binaries",ProcessCommand);

    StrOut.clear();
    StrErr.clear();
    RetValue = 0;
    Glib::spawn_command_line_sync(ProcessCommand,&StrOut,&StrErr,&RetValue);

    appendToLogFile(StrOut);

    if (RetValue != 0)
    {
      appendToLogFile(StrErr);
      throw openfluid::base::OFException("OpenFLUID framework","MarketBinPackage::process()","Error uncompressing package using CMake");

    }

  }
  catch (Glib::Error& E)
  {
    throw openfluid::base::OFException("OpenFLUID framework","MarketBinPackage::process()","Glib error uncompressing package using CMake");
  }

}


} } // namespaces



