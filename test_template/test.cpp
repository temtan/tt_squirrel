#include "test.h"

#include "tt_squirrel_virtual_machine.h"


// PCD()
// PCA( type, obj )
// PCC( c )
// PCI( i )
// PCP( p )
// PCS( s )
// PCF( f )
// PCSS( s )


int
main( int argc, char *argv[] )
{
  TtSquirrel::VirtualMachine vm;
  vm.Native().SetPrintFunction( TtSquirrel::Utility::DefaultPrintFunction, TtSquirrel::Utility::DefaultErrorFunction );

  return 0;
}
