// addition/path.cpp

#include "tt_path.h"
#include "tt_string.h"

#include "tt_squirrel_virtual_machine.h"

using namespace TtSquirrel;


namespace Tag {
#define DEFINE_PARAMETER_NAME_STRING( name ) static const std::string name( #name )
  DEFINE_PARAMETER_NAME_STRING( Path );
  DEFINE_PARAMETER_NAME_STRING( file_exist );
  DEFINE_PARAMETER_NAME_STRING( basename );
  DEFINE_PARAMETER_NAME_STRING( dirname );
  DEFINE_PARAMETER_NAME_STRING( has_extension );
  DEFINE_PARAMETER_NAME_STRING( remove_extension );
  DEFINE_PARAMETER_NAME_STRING( change_extension );
  DEFINE_PARAMETER_NAME_STRING( get_execute_file_directory_path );
  DEFINE_PARAMETER_NAME_STRING( get_file_not_exist_path_from );
  DEFINE_PARAMETER_NAME_STRING( remove_can_not_use_character_as_file_path_from );
  // DEFINE_PARAMETER_NAME_STRING(  );
}


void
VirtualMachine::RegisterAdditionalLibrariesPath( void )
{
  StackRecoverer recoverer( this );

  // -- Path 実装 -----
  this->NewSlotOfRootTableByString(
    Tag::Path,
    [&] () {
      TtSquirrel::StackRecoverer recoverer( this, 1 );
      this->Native().NewTable();

      // -- file_exist 定義
      this->NewSlotOfTopByString(
        Tag::file_exist,
        [&] () {
          this->NewClosure( [] ( VirtualMachine& vm ) -> int {
            vm.Native().PushBoolean( TtPath::FileExists( vm.GetAsFromTop<std::string>() ) );
            return TtSquirrel::Const::ExistReturnValue;
          } );
          Native().SetParamsCheck( 2, ".s" );
        } );

      // -- basename 定義
      this->NewSlotOfTopByString(
        Tag::basename,
        [&] () {
          this->NewClosure( [] ( VirtualMachine& vm ) -> int {
            vm.Native().PushString( TtPath::BaseName( vm.GetAsFromTop<std::string>() ) );
            return TtSquirrel::Const::ExistReturnValue;
          } );
          Native().SetParamsCheck( 2, "ts" );
        } );

      // -- dirname 定義
      this->NewSlotOfTopByString(
        Tag::dirname,
        [&] () {
          this->NewClosure( [] ( VirtualMachine& vm ) -> int {
            vm.Native().PushString( TtPath::DirName( vm.GetAsFromTop<std::string>() ) );
            return TtSquirrel::Const::ExistReturnValue;
          } );
          Native().SetParamsCheck( 2, "ts" );
        } );

      // -- has_extension 定義
      this->NewSlotOfTopByString(
        Tag::has_extension,
        [&] () {
          this->NewClosure( [] ( VirtualMachine& vm ) -> int {
            std::string extension = vm.GetAsFromTop<std::string>();
            vm.Native().PopTop();
            std::string path = vm.GetAsFromTop<std::string>();
            vm.Native().PopTop();
            vm.Native().PushBoolean( TtString::EndWith( TtString::ToLower( path ), "." + TtString::ToLower( extension ) ) );
            return TtSquirrel::Const::ExistReturnValue;
          } );
          Native().SetParamsCheck( 3, "tss" );
        } );

      // -- remove_extension 定義
      this->NewSlotOfTopByString(
        Tag::remove_extension,
        [&] () {
          this->NewClosure( [] ( VirtualMachine& vm ) -> int {
            vm.Native().PushString( TtPath::RemoveExtension( vm.GetAsFromTop<std::string>() ) );
            return TtSquirrel::Const::ExistReturnValue;
          } );
          Native().SetParamsCheck( 2, "ts" );
        } );

      // -- change_extension 定義
      this->NewSlotOfTopByString(
        Tag::change_extension,
        [&] () {
          this->NewClosure( [] ( VirtualMachine& vm ) -> int {
            std::string extension = vm.GetAsFromTop<std::string>();
            vm.Native().PopTop();
            std::string path = vm.GetAsFromTop<std::string>();
            vm.Native().PopTop();
            vm.Native().PushString( TtPath::ChangeExtension( path, extension ) );
            return TtSquirrel::Const::ExistReturnValue;
          } );
          Native().SetParamsCheck( 3, "tss" );
        } );

      // -- get_execute_file_directory_path 定義
      this->NewSlotOfTopByString(
        Tag::get_execute_file_directory_path,
        [&] () {
          this->NewClosure( [] ( VirtualMachine& vm ) -> int {
            vm.Native().PushString( TtPath::GetExecutingDirectoryPath() );
            return TtSquirrel::Const::ExistReturnValue;
          } );
          Native().SetParamsCheck( 1, "t" );
        } );

      // -- get_not_exist_path_from 定義
      this->NewSlotOfTopByString(
        Tag::get_file_not_exist_path_from,
        [&] () {
          this->NewClosure( [] ( VirtualMachine& vm ) -> int {
            vm.Native().PushString( TtPath::GetFileNotExistPathFrom( vm.GetAsFromTop<std::string>() ) );
            return TtSquirrel::Const::ExistReturnValue;
          } );
          Native().SetParamsCheck( 2, "ts" );
        } );

      // -- remove_can_not_use_character_as_file_path_from 定義
      this->NewSlotOfTopByString(
        Tag::remove_can_not_use_character_as_file_path_from,
        [&] () {
          this->NewClosure( [] ( VirtualMachine& vm ) -> int {
            vm.Native().PushString( TtPath::RemoveCanNotUseCharacterAsFilePathFrom( vm.GetAsFromTop<std::string>() ) );
            return TtSquirrel::Const::ExistReturnValue;
          } );
          Native().SetParamsCheck( 2, "ts" );
        } );

    } );
}
