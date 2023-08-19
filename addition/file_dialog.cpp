// addition/file_dialog.cpp

#include "tt_file_dialog.h"
#include "tt_folder_browser_dialog.h"

#include "tt_squirrel_virtual_machine.h"

using namespace TtSquirrel;


namespace Tag {
#define DEFINE_PARAMETER_NAME_STRING( name ) static const std::string name( #name )
  DEFINE_PARAMETER_NAME_STRING( FileDialog );
  DEFINE_PARAMETER_NAME_STRING( Filter );
  DEFINE_PARAMETER_NAME_STRING( constructor );
  DEFINE_PARAMETER_NAME_STRING( description );
  DEFINE_PARAMETER_NAME_STRING( extensions );
  DEFINE_PARAMETER_NAME_STRING( OpenFileDialog );
  DEFINE_PARAMETER_NAME_STRING( SaveFileDialog );
  DEFINE_PARAMETER_NAME_STRING( show_dialog );
  DEFINE_PARAMETER_NAME_STRING( multiselect );
  DEFINE_PARAMETER_NAME_STRING( filters );
  DEFINE_PARAMETER_NAME_STRING( file_name );
  DEFINE_PARAMETER_NAME_STRING( file_names );
  DEFINE_PARAMETER_NAME_STRING( FolderBrowserDialog );
  DEFINE_PARAMETER_NAME_STRING( selected_path );
  // DEFINE_PARAMETER_NAME_STRING(  );
}


void
VirtualMachine::RegisterAdditionalLibrariesFileDialog( void )
{
  StackRecoverer recoverer( this );

  // -- FileDialog 実装 -----
  this->NewSlotOfRootTableByString(
    Tag::FileDialog,
    [&] () {
      TtSquirrel::StackRecoverer recoverer( this, 1 );
      this->Native().NewClass( false );

      // -- Filter 実装 -----
      this->NewSlotOfTopByStringAsStatic(
        Tag::Filter,
        [&] () {
          TtSquirrel::StackRecoverer recoverer( this, 1 );
          this->Native().NewClass( false );

          // -- constructor 定義 -----
          this->NewSlotOfTopByString(
            Tag::constructor,
            [&] () {
              this->NewClosure( [] ( VirtualMachine& vm ) -> int {
                TtSquirrel::Object extensions_object = vm.GetStackTopObject();
                vm.Native().PopTop();
                if ( extensions_object.GetType() == TtSquirrel::ObjectType::String ) {
                  TtSquirrel::StackRecoverer recoverer( &vm );
                  vm.Native().NewArray( 0 );
                  vm.AppendToArrayOfStackTop( [&] () { vm.PushObject( extensions_object ); } );
                  extensions_object = vm.GetStackTopObject();
                }

                TtSquirrel::Object description_object = vm.GetStackTopObject();
                vm.Native().PopTop();

                vm.SetToTopByString( Tag::description, [&] () { vm.PushObject( description_object ); } );
                vm.SetToTopByString( Tag::extensions, [&] () { vm.PushObject( extensions_object ); } );

                return TtSquirrel::Const::NoneReturnValue;
              } );
              Native().SetParamsCheck( 3, "xsa|s" );
            } );

          this->NewNullSlotOfTopByString( Tag::description );
          this->NewNullSlotOfTopByString( Tag::extensions );
        } );

      // -- constructor 定義 -----
      this->NewSlotOfTopByString(
        Tag::constructor,
        [&] () {
          this->NewClosure( [] ( VirtualMachine& vm ) -> int {
            vm.SetBooleanToTopByString( Tag::multiselect, false );
            vm.SetToTopByString( Tag::filters, [&] () { vm.Native().NewArray( 0 );} );

            return TtSquirrel::Const::NoneReturnValue;
          } );
          Native().SetParamsCheck( 1, "x" );
        } );

      // -- プロパティ
      this->NewNullSlotOfTopByString( Tag::multiselect );
      this->NewNullSlotOfTopByString( Tag::filters );

      this->NewNullSlotOfTopByString( Tag::file_name );
      this->NewNullSlotOfTopByString( Tag::file_names );
    } );

  // -- OpenFileDialog 実装 -----
  this->NewSlotOfRootTableByString(
    Tag::OpenFileDialog,
    [&] () {
      TtSquirrel::StackRecoverer recoverer( this, 1 );
      this->GetByStringFromRootTable( Tag::FileDialog );
      this->Native().NewClass( true );

      // -- show_dialog 定義 -----
      this->NewSlotOfTopByString(
        Tag::show_dialog,
        [&] () {
          this->NewClosure( [] ( VirtualMachine& vm ) -> int {
            TtOpenFileDialog dialog;
            dialog.SetMultiselect( vm.GetByStringFromTopAndGetAs<bool>( Tag::multiselect ) );

            vm.ArrayEach(
              [&] () { vm.GetByStringFromTop( Tag::filters ); },
              [&] ( unsigned int ) {
                TtFileDialog::Filter filter;
                filter.SetDescription( vm.GetByStringFromTopAndGetAs<std::string>( Tag::description ) );
                vm.ArrayEach(
                  [&] () { vm.GetByStringFromTop( Tag::extensions ); },
                  [&] ( unsigned int ) {
                    filter.GetExtensions().push_back( vm.GetAsFromTop<std::string>() );
                  } );
                dialog.GetFilters().push_back( filter );
              } );
            if ( dialog.GetFilters().empty() ) {
              dialog.GetFilters().push_back( {"全てのファイル(*.*)", "*.*"} );
            }

            bool ret = vm.GetParentWindow() ? dialog.ShowDialog( *vm.GetParentWindow() ) : dialog.TtFileDialog::ShowDialog();

            if ( ret ) {
              vm.SetStringToTopByString( Tag::file_name, dialog.GetFileName() );
              vm.SetToTopByString(
                Tag::file_names,
                [&] () {
                  vm.NewArrayOf( dialog.GetFileNames(), [&] ( const std::string& one ) {
                    vm.Native().PushString( one );
                  } );
                } );
            }

            vm.Native().PushBoolean( ret );
            return TtSquirrel::Const::ExistReturnValue;
          } );
          Native().SetParamsCheck( 1, "x" );
        } );
    } );

  // -- SaveFileDialog 実装 -----
  this->NewSlotOfRootTableByString(
    Tag::SaveFileDialog,
    [&] () {
      TtSquirrel::StackRecoverer recoverer( this, 1 );
      this->GetByStringFromRootTable( Tag::FileDialog );
      this->Native().NewClass( true );

      // -- show_dialog 定義 -----
      this->NewSlotOfTopByString(
        Tag::show_dialog,
        [&] () {
          this->NewClosure( [] ( VirtualMachine& vm ) -> int {
            TtSaveFileDialog dialog;
            dialog.SetMultiselect( vm.GetByStringFromTopAndGetAs<bool>( Tag::multiselect ) );

            vm.ArrayEach(
              [&] () { vm.GetByStringFromTop( Tag::filters ); },
              [&] ( unsigned int ) {
                TtFileDialog::Filter filter;
                filter.SetDescription( vm.GetByStringFromTopAndGetAs<std::string>( Tag::description ) );
                vm.ArrayEach(
                  [&] () { vm.GetByStringFromTop( Tag::extensions ); },
                  [&] ( unsigned int ) {
                    filter.GetExtensions().push_back( vm.GetAsFromTop<std::string>() );
                  } );
                dialog.GetFilters().push_back( filter );
              } );
            if ( dialog.GetFilters().empty() ) {
              dialog.GetFilters().push_back( {"全てのファイル(*.*)", "*.*"} );
            }

            bool ret = vm.GetParentWindow() ? dialog.ShowDialog( *vm.GetParentWindow() ) : dialog.TtFileDialog::ShowDialog();

            if ( ret ) {
              vm.SetStringToTopByString( Tag::file_name, dialog.GetFileName() );
              vm.SetToTopByString(
                Tag::file_names,
                [&] () {
                  vm.NewArrayOf( dialog.GetFileNames(), [&] ( const std::string& one ) {
                    vm.Native().PushString( one );
                  } );
                } );
            }

            vm.Native().PushBoolean( ret );
            return TtSquirrel::Const::ExistReturnValue;
          } );
          Native().SetParamsCheck( 1, "x" );
        } );
    } );


  // -- FolderBrowserDialog 実装 -----
  this->NewSlotOfRootTableByString(
    Tag::FolderBrowserDialog,
    [&] () {
      TtSquirrel::StackRecoverer recoverer( this, 1 );
      this->Native().NewClass( false );

      // -- constructor 定義 -----
      this->NewSlotOfTopByString(
        Tag::constructor,
        [&] () {
          this->NewClosure( [] ( VirtualMachine& vm ) -> int {
            return TtSquirrel::Const::NoneReturnValue;
          } );
          Native().SetParamsCheck( 1, "x" );
        } );

      // -- show_dialog 定義 -----
      this->NewSlotOfTopByString(
        Tag::show_dialog,
        [&] () {
          this->NewClosure( [] ( VirtualMachine& vm ) -> int {
            TtFolderBrowserDialog dialog;
            {
              StackRecoverer recoverer( &vm );
              vm.GetByStringFromTop( Tag::description );
              if ( vm.GetTopType() == ObjectType::String ) {
                dialog.SetDescription( vm.GetAsFromTop<std::string>() );
              }
            }

            bool ret = vm.GetParentWindow() ? dialog.ShowDialog( *vm.GetParentWindow() ) : dialog.ShowDialog();

            if ( ret ) {
              vm.SetStringToTopByString( Tag::selected_path, dialog.GetSelectedPath() );
            }

            vm.Native().PushBoolean( ret );
            return TtSquirrel::Const::ExistReturnValue;
          } );
          Native().SetParamsCheck( 1, "x" );
        } );

      // -- プロパティ
      this->NewNullSlotOfTopByString( Tag::description );

      this->NewNullSlotOfTopByString( Tag::selected_path );
    } );
}
