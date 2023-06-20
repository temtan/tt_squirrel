// addition/utility.cpp

#include "tt_squirrel_virtual_machine.h"

using namespace TtSquirrel;


namespace Tag {
#define DEFINE_PARAMETER_NAME_STRING( name ) static const std::string name( #name )
  DEFINE_PARAMETER_NAME_STRING( puts );
  DEFINE_PARAMETER_NAME_STRING( print );
  DEFINE_PARAMETER_NAME_STRING( file );
  DEFINE_PARAMETER_NAME_STRING( write_string );
  DEFINE_PARAMETER_NAME_STRING( writen );
  DEFINE_PARAMETER_NAME_STRING( read_line );
  DEFINE_PARAMETER_NAME_STRING( readn );
  // DEFINE_PARAMETER_NAME_STRING(  );
}


void
VirtualMachine::RegisterAdditionalLibrariesUtility( void )
{
  StackRecoverer recoverer( this );

  // -- puts -----
  this->NewSlotOfRootTableByString(
    Tag::puts,
    [&] () {
      this->NewClosure( [] ( VirtualMachine& vm ) -> int {
        TtSquirrel::Object obj = vm.GetStackTopObject( false );
        vm.CallAndNoReturnValue(
          [&] () { vm.GetByStringFromRootTable( Tag::print ); },
          [&] () {
            vm.Native().PushRootTable();
            vm.PushObject( obj );
            return 2;
          } );
        vm.CallPrint( "\n" );
        return TtSquirrel::Const::NoneReturnValue;
      } );
      Native().SetParamsCheck( 2, "t." );
    } );

  // -- add to file class -----
  {
    StackRecoverer recoverer( this );
    this->GetByStringFromRootTable( Tag::file );

    // -- write_string -----
    this->NewSlotOfTopByString(
      Tag::write_string,
      [&] () {
        this->NewClosure( [] ( VirtualMachine& vm ) -> int {
          TtSquirrel::Object str = vm.GetStackTopObject( false );
          TtSquirrel::Object file_instance = vm.GetStackObject( Const::StackTop - 1 );
          vm.Foreach(
            [&] () { vm.PushObject( str ); },
            [&file_instance] ( VirtualMachine& vm ) {
              TtSquirrel::Object c = vm.GetStackTopObject( false );
              vm.PushObject( file_instance );
              vm.CallObjectOfGetByStringFromTopAndNoReturnValue(
                Tag::writen,
                [&] ( TtSquirrel::Object object ) {
                  vm.PushObject( object );
                  vm.PushObject( c );
                  vm.Native().PushInteger( 'c' );
                  return 3;
                } );
            } );
          return TtSquirrel::Const::NoneReturnValue;
        } );
        Native().SetParamsCheck( 2, "xs" );
      } );

    // -- read_line -----
    this->NewSlotOfTopByString(
      Tag::read_line,
      [&] () {
        this->NewClosure( [] ( VirtualMachine& vm ) -> int {
          TtSquirrel::Object file_instance = vm.GetStackObject( Const::StackTop );
          int save_top = vm.Native().GetTopIndex();
          std::string line;

          for (;;) {
            vm.CallObjectOfGetByStringFromTopAndPushReturnValue(
              Tag::readn,
              [&] ( TtSquirrel::Object object ) {
                vm.PushObject( object );
                vm.Native().PushInteger( 'c' );
                return 2;
              } );
            char c = vm.GetAsFromTop<int>();
            line.append( 1, c );
            if ( c == '\n' ) {
              break;
            }
            vm.Native().ResizeStackSize( save_top );
          }

          vm.Native().PushString( line );
          return TtSquirrel::Const::ExistReturnValue;
        } );
        Native().SetParamsCheck( 1, "x" );
      } );
  }
}
