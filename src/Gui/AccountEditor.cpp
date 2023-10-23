#line 2 "src/Gui/AccountEditor.cpp"

#include "AccountEditor.h"

GCW::Gui::AccountEditor::
AccountEditor( const std::string & _accountGuid )
: m_accountGuid( _accountGuid )
{

  m_tabWidget = addNew< Wt::WTabWidget >();

  auto t1 = std::make_unique< Wt::WTemplateFormView >( TR( "gcw.accounteditor.form.tab1" ) );
  m_tab1 = t1.get();

  auto twt1 =
    tabWidget()->
    addTab
    (
     std::move( t1 ),
     "General"
    );


  auto t2 = std::make_unique< Wt::WTemplateFormView >( TR( "gcw.accounteditor.form.tab2" ) );
  m_tab2 = t2.get();

  auto twt2 =
    tabWidget()->
    addTab
    (
     std::move( t2 ),
     "More Properties"
    );

  auto model = std::make_shared< Wt::WFormModel >();
  model-> addField( GCW::Dbo::Account::Field::name, "Account Name" );

  m_tab1-> updateView( model.get() );
  m_tab2-> updateView( model.get() );

} // endGCW::AccountEditor::AccountEditor( const std::string & _accountGuid )





GCW::Gui::AccountEditorDialog::
AccountEditorDialog( const std::string & _accountGuid )
: Wt::WDialog( "Edit Account" )
{
  rejectWhenEscapePressed( true );
  setResizable( true );
  setClosable( true );
  setMinimumSize( "800px", "600px" );

  contents()-> addNew< GCW::Gui::AccountEditor >( _accountGuid );

}; // endclass GCW::Gui::AccountEditorDialog::AccountEditorDialog( const std::string & _accountGuid )



