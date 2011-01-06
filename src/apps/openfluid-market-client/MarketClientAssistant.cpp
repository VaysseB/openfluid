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
  \file MarketClientAssistant.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#include "MarketClientAssistant.hpp"

#include <iostream>

#include <openfluid/base.hpp>

MarketClientAssistant::MarketClientAssistant()
: m_SelectionPageBox(false,0),
  m_LicensesPageBox(false,0),
  m_InstallPageBox(false,0)
{
  set_title("OpenFLUID Market client");
  set_border_width(20);
  set_default_size(700, 500);
  set_position(Gtk::WIN_POS_CENTER);


  setupSelectionPage();
  setupLicensesPage();
  setupDownloadPage();


  append_page(m_SelectionPageBox);
  append_page(m_LicensesPageBox);
  append_page(m_InstallPageBox);


  set_page_title(*get_nth_page(0), "Packages selection");
  set_page_title(*get_nth_page(1), "Licenses");
  set_page_title(*get_nth_page(2), "Download and install");


  set_page_complete(m_SelectionPageBox, false);
  set_page_complete(m_LicensesPageBox, false);
  set_page_complete(m_InstallPageBox, true);


  set_page_type(m_SelectionPageBox, Gtk::ASSISTANT_PAGE_INTRO);
  set_page_type(m_LicensesPageBox, Gtk::ASSISTANT_PAGE_CONTENT);
  set_page_type(m_InstallPageBox, Gtk::ASSISTANT_PAGE_SUMMARY);


  signal_apply().connect(sigc::mem_fun(*this,
      &MarketClientAssistant::onApply));

  signal_cancel().connect(sigc::mem_fun(*this,
      &MarketClientAssistant::onCancel));

  signal_close().connect(sigc::mem_fun(*this,
      &MarketClientAssistant::onClose));

  signal_prepare().connect(sigc::mem_fun(*this,
      &MarketClientAssistant::onPrepare));

  show_all_children();
}


// =====================================================================
// =====================================================================


MarketClientAssistant::~MarketClientAssistant()
{
}



// =====================================================================
// =====================================================================


void MarketClientAssistant::setupSelectionPage()
{

  m_URLLabel.set_text("Marketplace: ");

  m_RefURLComboBoxModel = Gtk::ListStore::create(m_URLColumns);
  m_URLCombo.set_model(m_RefURLComboBoxModel);


  Gtk::TreeModel::Row TmpURLRow = *(m_RefURLComboBoxModel->append());
  TmpURLRow[m_URLColumns.m_Name] = "--- No marketplace selected ---";
  TmpURLRow[m_URLColumns.m_URL] = "";


  Glib::KeyFile MarketConf;

  if (MarketConf.load_from_file(openfluid::base::RuntimeEnvironment::getInstance()->getDefaultConfigFile()))
  {
    if (MarketConf.has_group("openfluid.market.marketplaces"))
    {
      std::vector<std::string> PlacesKeys = MarketConf.get_keys("openfluid.market.marketplaces");

      for (unsigned int i=0; i< PlacesKeys.size();i++)
      {
        std::vector<std::string> PlaceInfo = MarketConf.get_string_list("openfluid.market.marketplaces",PlacesKeys[i]);
        if (PlaceInfo.size() == 2 )
        {
          TmpURLRow = *(m_RefURLComboBoxModel->append());
          TmpURLRow[m_URLColumns.m_Name] = PlaceInfo[0];
          TmpURLRow[m_URLColumns.m_URL] = PlaceInfo[1];
        }
      }
    }
  }



  m_URLCombo.pack_start(m_URLColumns.m_Name);
  m_URLCombo.pack_start(m_URLColumns.m_URL);

  m_URLCombo.set_active(0);

  m_URLBox.pack_start(m_URLLabel,Gtk::PACK_SHRINK);
  m_URLBox.pack_start(m_URLCombo,Gtk::PACK_EXPAND_WIDGET);


  m_RefAvailPacksModel = Gtk::ListStore::create(m_AvailPacksColumns);
  m_AvailPacksTreeview.set_model(m_RefAvailPacksModel);

  m_AvailPacksTreeview.append_column_editable("",m_AvailPacksColumns.m_Selected);
  m_AvailPacksTreeview.append_column("ID",m_AvailPacksColumns.m_ID);
  m_AvailPacksTreeview.append_column("Format",m_AvailPacksColumns.m_Format);
  m_AvailPacksTreeview.append_column("Name",m_AvailPacksColumns.m_Name);


  m_AvailPacksLabel.set_text("Available packages:");
  m_AvailPacksLabel.set_justify(Gtk::JUSTIFY_LEFT);
  m_AvailPacksLabel.set_alignment(0, 0);


  m_AvailPacksBox.pack_start(m_AvailPacksLabel,Gtk::PACK_SHRINK,0);
  m_AvailPacksBox.pack_start(m_AvailPacksTreeview,Gtk::PACK_EXPAND_WIDGET);

  m_SelectionPageBox.set_border_width(12);
  m_SelectionPageBox.pack_start(m_URLBox,Gtk::PACK_SHRINK,12);
  m_SelectionPageBox.pack_start(m_AvailPacksBox,Gtk::PACK_EXPAND_WIDGET);



  m_URLCombo.signal_changed().connect(
    sigc::mem_fun(*this, &MarketClientAssistant::onURLComboChanged)
  );


}


// =====================================================================
// =====================================================================


void MarketClientAssistant::setupLicensesPage()
{

  m_LicensesLabel.set_text("Licenses:");
  m_LicensesLabel.set_alignment(0,0);

  m_LicensesSWindow.add(m_LicensesTextview);
  m_LicensesSWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

  m_LicensesReviewBox.set_spacing(12);
  m_LicensesReviewBox.pack_start(m_LicensesTreeview,Gtk::PACK_EXPAND_WIDGET);
  m_LicensesReviewBox.pack_start(m_LicensesSWindow,Gtk::PACK_EXPAND_WIDGET);


  m_LicensesAcceptRadio.set_label("Accept packages licenses");
  m_LicensesDoNotRadio.set_label("Do not accept packages licenses");
  Gtk::RadioButton::Group Group = m_LicensesAcceptRadio.get_group();
  m_LicensesDoNotRadio.set_group(Group);
  m_LicensesDoNotRadio.set_active(true);

  m_LicensesPageBox.set_border_width(12);
  m_LicensesPageBox.pack_start(m_LicensesLabel,Gtk::PACK_SHRINK);
  m_LicensesPageBox.pack_start(m_LicensesReviewBox,Gtk::PACK_EXPAND_WIDGET);
  m_LicensesPageBox.pack_start(m_LicensesAcceptRadio,Gtk::PACK_SHRINK);
  m_LicensesPageBox.pack_start(m_LicensesDoNotRadio,Gtk::PACK_SHRINK);


  m_LicensesAcceptRadio.signal_clicked().connect(
      sigc::mem_fun(*this,&MarketClientAssistant::onLicenseRadioClicked)
  );
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::setupDownloadPage()
{
  m_InstallSWindow.add(m_InstallTextview);
  m_InstallSWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

  m_InstallPageBox.set_border_width(12);
  m_InstallPageBox.pack_start(m_InstallProgressBar,Gtk::PACK_SHRINK);
  m_InstallPageBox.pack_start(m_InstallSWindow,Gtk::PACK_EXPAND_WIDGET,12);

}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onApply()
{
  std::cout << "Apply was clicked on page " << get_page_title(*get_nth_page(get_current_page()));
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onCancel()
{
  std::cout << "Cancel was clicked on page " << get_page_title(*get_nth_page(get_current_page()))  << std::endl;
  hide();
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onClose()
{
  std::cout << "Assistant was closed" << std::endl;
  hide();
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onPrepare(Gtk::Widget* widget)
{
    set_title(Glib::ustring::compose("OpenFLUID Market client (Step %1 of %2)",
      get_current_page() + 1, get_n_pages()));

  std::cout << "onprepare on page " << get_page_title(*widget) << std::endl;


  if (get_current_page() == 1)
  {
/*    Gtk::MessageDialog dialog(*this, "This is an INFO MessageDialog");
    dialog.set_secondary_text(
            "And this is the secondary text that explains things.");

    dialog.run();

    set_current_page(0);
    show();
*/
  }

//  Gtk::MessageDialog dialog(*this, "OnPrepare()");
//  dialog.run();


}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onLicenseRadioClicked()
{
  if (m_LicensesAcceptRadio.get_active())
    set_page_complete(m_LicensesPageBox, true);
  else
    set_page_complete(m_LicensesPageBox, false);
}


// =====================================================================
// =====================================================================



void MarketClientAssistant::onURLComboChanged()
{
  std::cout << "onURLComboChanged() " << m_URLCombo.get_active() << std::endl;
  m_MarketClient.disconnect();

  if (m_URLCombo.get_active_row_number() == 0)
  {
    set_page_complete(m_SelectionPageBox,false);
  }
  else
  {
    Gtk::TreeModel::iterator TmpIter = m_URLCombo.get_active();
    if (TmpIter)
    {
      Gtk::TreeModel::Row TmpRow = *TmpIter;
      if (TmpRow)
      {
        Glib::ustring TmpURL = TmpRow[m_URLColumns.m_URL];
        m_MarketClient.connect(TmpURL);
        set_page_complete(m_SelectionPageBox,true);
      }
    }
  }
  updateAvailPacksTreeview();
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::updateAvailPacksTreeview()
{
  openfluid::market::MetaPackagesCatalog_t Catalog;
  openfluid::market::MetaPackagesCatalog_t::const_iterator CIter;

  m_RefAvailPacksModel->clear();
  Catalog = m_MarketClient.getMetaPackagesCatalog();

  for (CIter=Catalog.begin();CIter!=Catalog.end();++CIter)
  {

    std::map<openfluid::market::MetaPackageInfo::SelectionType,openfluid::market::PackageInfo>::const_iterator PIter;

    for (PIter = CIter->second.AvailablePackages.begin();PIter!=CIter->second.AvailablePackages.end();++PIter)
    {
      Gtk::TreeModel::Row TmpAvailPacksRow = *(m_RefAvailPacksModel->append());
      TmpAvailPacksRow[m_AvailPacksColumns.m_Selected] = false;
      TmpAvailPacksRow[m_AvailPacksColumns.m_ID] = CIter->first;
      if (PIter->first == openfluid::market::MetaPackageInfo::BIN) TmpAvailPacksRow[m_AvailPacksColumns.m_Format] = "binary";
      else TmpAvailPacksRow[m_AvailPacksColumns.m_Format] = "source";
      TmpAvailPacksRow[m_AvailPacksColumns.m_Name] = PIter->second.Name;
    }
  }
}
