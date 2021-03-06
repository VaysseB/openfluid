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
 \file PreferencesDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "PreferencesDialog.hpp"

#include <glibmm/i18n.h>
#include <gtkmm/treeselection.h>

#include <openfluid/guicommon/PreferencesManager.hpp>
#include <openfluid/guicommon/PreferencesPanel.hpp>
#include <openfluid/base/RuntimeEnv.hpp>

#include "PreferencesPanelImpl.hpp"
#include "BuilderExtensionsManager.hpp"
#include <openfluid/guicommon/FunctionSignatureRegistry.hpp>

// =====================================================================
// =====================================================================


PreferencesDialog::PreferencesDialog() :
  mp_CurrentPanel(0)
{
  mref_GroupsTreeModel = Gtk::TreeStore::create(m_GroupsColumns);
  Gtk::TreeRow Row;

  Row = *mref_GroupsTreeModel->append();
  Row[m_GroupsColumns.m_Id] = "interface";
  Row[m_GroupsColumns.m_Name] = _("Interface");

  Row = *mref_GroupsTreeModel->append();
  Row[m_GroupsColumns.m_Id] = "paths";
  Row[m_GroupsColumns.m_Name] = _("Paths");

  Row = *mref_GroupsTreeModel->append();
  Row[m_GroupsColumns.m_Id] = "simulation";
  Row[m_GroupsColumns.m_Name] = _("Simulations");

  Row = *mref_GroupsTreeModel->append();
  Row[m_GroupsColumns.m_Id] = "market";
  Row[m_GroupsColumns.m_Name] = _("Market");

  mp_GroupsTreeView = Gtk::manage(new Gtk::TreeView(mref_GroupsTreeModel));
  mp_GroupsTreeView->append_column("", m_GroupsColumns.m_Name);
  mp_GroupsTreeView->set_headers_visible(false);
  mp_GroupsTreeView->set_border_width(5);
  mp_GroupsTreeView->get_selection()->signal_changed().connect(sigc::mem_fun(
      *this, &PreferencesDialog::onGroupSelectionChanged));

  mp_GroupsSWindow = Gtk::manage(new Gtk::ScrolledWindow());
  mp_GroupsSWindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_GroupsSWindow->set_visible(true);
  mp_GroupsSWindow->set_size_request(170, -1);
  mp_GroupsSWindow->add(*mp_GroupsTreeView);
  mp_GroupsSWindow->set_shadow_type(Gtk::SHADOW_ETCHED_IN);

  m_GroupPanels["interface"] = new PreferencesInterfacePanel();
  m_GroupPanels["paths"] = new PreferencesPathsPanel();
  m_GroupPanels["simulation"] = new PreferencesSimPanel();
  m_GroupPanels["market"] = new PreferencesMarketPanel();

  mp_MainBox = Gtk::manage(new Gtk::HBox());
  mp_MainBox->pack_start(*mp_GroupsSWindow, Gtk::PACK_SHRINK, 6);

  mp_Dialog = new Gtk::Dialog();
  mp_Dialog->get_vbox()->pack_start(*mp_MainBox, Gtk::PACK_EXPAND_WIDGET, 6);
  mp_Dialog->set_default_size(950, 500);
  mp_Dialog->set_border_width(6);
  mp_Dialog->set_title(_("Preferences"));
  mp_Dialog->show_all_children();

  mp_Dialog->add_button(Gtk::Stock::CLOSE, Gtk::RESPONSE_CLOSE);

  //select first group
  mp_GroupsTreeView->get_selection()->select(
      mref_GroupsTreeModel->children().begin());
}

// =====================================================================
// =====================================================================


PreferencesDialog::~PreferencesDialog()
{
  BuilderExtensionsManager::getInstance()->deleteRegisteredExtensionPreferences();
}

// =====================================================================
// =====================================================================


void PreferencesDialog::show()
{
  openfluid::guicommon::PreferencesManager* PrefMgr =
      openfluid::guicommon::PreferencesManager::getInstance();

  m_PlugPathsHaveChanged = false;
  m_RecentsHaveChanged = false;

  int OldRecentMax = PrefMgr->getRecentMax();
  int OldRecentCount = PrefMgr->getRecentProjects().size();

  BuilderExtensionsManager* BEM = BuilderExtensionsManager::getInstance();

  if (!BEM->isPreferencesInstantiationDone())
  {
    Gtk::TreeRow Row = *mref_GroupsTreeModel->append();
    Row[m_GroupsColumns.m_Id] = "extensions";
    Row[m_GroupsColumns.m_Name] = _("Extensions");

    BEM->instantiateRegisteredExtensionPreferences();

    ExtensionContainerMap_t* Exts = BEM->getRegisteredExtensionPreferences();

    for (ExtensionContainerMap_t::iterator it = Exts->begin(); it
        != Exts->end(); ++it)
    {
      Gtk::TreeRow ExtRow = *mref_GroupsTreeModel->append(Row.children());
      ExtRow[m_GroupsColumns.m_Id] = it->second.Infos.ID;
      ExtRow[m_GroupsColumns.m_Name] = it->second.Infos.ShortName;
      m_GroupPanels[it->second.Infos.ID] = it->second.Preferences;
    }

  }

  for (std::map<Glib::ustring, openfluid::guicommon::PreferencesPanel*>::iterator
      it = m_GroupPanels.begin(); it != m_GroupPanels.end(); ++it)
    it->second->init();

  //select first group
  mp_GroupsTreeView->get_selection()->select(
      mref_GroupsTreeModel->children().begin());

  if (mp_Dialog->run())
  {
    openfluid::base::RuntimeEnvironment* RunEnv =
        openfluid::base::RuntimeEnvironment::getInstance();

    PrefMgr->save();

    if (PrefMgr->getRecentMax() != OldRecentMax
        || PrefMgr->getRecentProjects().size() != OldRecentCount)
      m_RecentsHaveChanged = true;

    // update plugs paths

    std::vector<std::string> ExistingPlugPaths = RunEnv->getExtraFunctionsPluginsPaths();
    std::vector<std::string> PrefPlugPaths = PrefMgr->getExtraPlugPaths();

    if (!(ExistingPlugPaths.size() == PrefPlugPaths.size() && std::equal(
        ExistingPlugPaths.begin(), ExistingPlugPaths.end(),
        PrefPlugPaths.begin())))
    {
      m_PlugPathsHaveChanged = true;

      RunEnv->resetExtraFunctionsPluginsPaths();

      for (int i = PrefPlugPaths.size() - 1; i >= 0; i--)
        RunEnv->addExtraFunctionsPluginsPaths(PrefPlugPaths[i]);

      openfluid::guicommon::FunctionSignatureRegistry::getInstance()->updatePluggableSignatures();
    }

    mp_Dialog->hide();
  }
}

// =====================================================================
// =====================================================================


void PreferencesDialog::onGroupSelectionChanged()
{
  Gtk::TreeIter SelectedIter =
      mp_GroupsTreeView->get_selection()->get_selected();

  if (!SelectedIter)
    return;

  std::string SelectedGroup = SelectedIter->get_value(m_GroupsColumns.m_Id);

  if (m_GroupPanels.find(SelectedGroup) == m_GroupPanels.end())
    return;

  if (mp_CurrentPanel)
    mp_MainBox->remove(*mp_CurrentPanel);

  mp_CurrentPanel = m_GroupPanels[SelectedGroup]->asWidget();

  mp_MainBox->pack_start(*mp_CurrentPanel, Gtk::PACK_EXPAND_WIDGET, 6);

}

// =====================================================================
// =====================================================================

