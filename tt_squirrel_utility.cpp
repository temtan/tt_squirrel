// tt_squirrel_utility.cpp

#include <sstream>

#include <stdio.h>
#include <stdarg.h>

#include "tt_squirrel_exception.h"

#include "tt_squirrel_utility.h"

using namespace TtSquirrel;


std::string
Utility::ToStringFrom( int i )
{
  std::ostringstream os;
  os << i;
  return os.str();
}


int
Utility::Pushed( int index, int pushed_count )
{
  return index > 0 ? index : index - pushed_count;
}

int
Utility::PushedFromTop( int pushed_count )
{
  return Utility::Pushed( Const::StackTop, pushed_count );
}


void
Utility::DefaultPrintFunction( HSQUIRRELVM vm, const SQChar* s, ... )
{
  static_cast<void>( vm );

  va_list arglist;
  va_start( arglist, s );
  vfprintf_s( stdout, s, arglist );
  va_end( arglist );
}

void
Utility::DefaultErrorFunction( HSQUIRRELVM vm, const SQChar* s, ... )
{
  static_cast<void>( vm );

  va_list arglist;
  va_start( arglist, s );
  vfprintf_s( stderr, s, arglist );
  va_end( arglist );
}


void
Utility::ObjectTypeCheck( ObjectType require, ObjectType actual )
{
  if ( require != actual ) {
    throw ObjectTypeException( require, actual );
  }
}

// -- TypeTag ------------------------------------------------------------
std::unordered_map<std::string, void*>
TypeTag::TABLE;

TypeTag::TypeTag( const std::type_info& type ) :
tag_( NULL )
{
  if ( TABLE.find( type.name() ) == TABLE.end() ) {
    TABLE[type.name()] = new int( 0 );
  }
  tag_ = TABLE[type.name()];
}

void*
TypeTag::GetTag( void ) const
{
  return tag_;
}

TypeTag::operator void*( void )
{
  return this->GetTag();
}

// -- LocalInformation ---------------------------------------------------
LocalInformation::LocalInformation( const std::string& name, ObjectType type, const std::string& value ) :
name_( name ),
type_( type ),
value_( value )
{
}

// -- StackInformation ---------------------------------------------------
StackInformation::StackInformation( SQStackInfos& stack_info ) :
function_name_( stack_info.funcname ? stack_info.funcname : "unknown" ),
source_file_path_( stack_info.source ? stack_info.source : "unknown" ),
source_file_line_( stack_info.line ),
local_infos_()
{
}
