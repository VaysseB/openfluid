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
 \file HomeModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "HomeModel.hpp"

#include <glibmm/i18n.h>
#include <openfluid/config.hpp>
#include <openfluid/guicommon/PreferencesManager.hpp>

// =====================================================================
// =====================================================================


HomeModelImpl::HomeModelImpl()
{
  m_OFVersionTxt
      = "OpenFLUID v" + openfluid::config::FULL_VERSION;
  m_OFWebSite = "http://www.umr-lisah.fr/openfluid/";
}

// =====================================================================
// =====================================================================


sigc::signal<void, std::string> HomeModelImpl::signal_OpenProjectAsked()
{
  return m_signal_OpenProjectAsked;
}

// =====================================================================
// =====================================================================


std::string HomeModelImpl::getOFVersionTxt()
{
  return m_OFVersionTxt;
}

// =====================================================================
// =====================================================================


std::string HomeModelImpl::getOFWebSite()
{
  return m_OFWebSite;
}

// =====================================================================
// =====================================================================


std::vector<std::pair<std::string, std::string> > HomeModelImpl::getRecentProjects()
{
  std::vector<std::pair<std::string, std::string> > Recents;

  int RecentsMax =
      openfluid::guicommon::PreferencesManager::getInstance()->getRecentMax();

  if (RecentsMax < 1)
    return Recents;

  Recents
      = openfluid::guicommon::PreferencesManager::getInstance()->getRecentProjects();

  if ((int)Recents.size() > RecentsMax)
    Recents.resize(RecentsMax);

  return Recents;
}

// =====================================================================
// =====================================================================


void HomeModelImpl::openProjectAsked(std::string ProjectPath)
{
  m_signal_OpenProjectAsked.emit(ProjectPath);
}
