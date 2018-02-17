// tt_squirrel_exception.cpp

#include <sstream>

#include "tt_utility.h"

#include "tt_squirrel_virtual_machine.h"

#include "tt_squirrel_exception.h"

using namespace TtSquirrel;


// -- Exception ----------------------------------------------------------
std::string
Exception::GetStandardMessage( void ) const
{
  return "エラー";
}

// -- InternalException ------------------------------------------------
InternalException::InternalException( const char* file, unsigned int line ) :
file_( file ),
line_( line )
{
}


const char*
InternalException::GetFile( void ) const
{
  return file_;
}

unsigned int
InternalException::GetLine( void ) const
{
  return line_;
}


std::string
InternalException::GetStandardMessage( void ) const
{
  std::string tmp = "内部エラー";
  tmp.append( "\r\n" );
  tmp.append( "file : " + std::string( file_ ) + "(" + TtUtility::ToStringFrom( line_ ) + ")" );
  return tmp;
}

std::string
InternalException::Dump( void ) const
{
  std::ostringstream os;
  os << typeid( *this ).name() << " : " << file_ << "(" << line_ << ")";
  return os.str();
}


// -- NativeAPIException -------------------------------------------------
NativeAPIException::NativeAPIException( const std::string& function_name, const std::string& reason ) :
function_name_( function_name ),
reason_( reason )
{
}

const std::string&
NativeAPIException::GetFunctionName( void )
{
  return function_name_;
}

const std::string&
NativeAPIException::GetReason( void )
{
  return reason_;
}

std::string
NativeAPIException::GetStandardMessage( void ) const
{
  std::string tmp = "ネイティブ関数のエラー";
  tmp.append( "\r\n" );
  tmp.append( "function : " + function_name_ + " : " + reason_ );
  return tmp;
}

std::string
NativeAPIException::Dump( void ) const
{
  std::ostringstream os;
  os << typeid( *this ).name() << " : " << function_name_ << " : " << reason_;
  return os.str();
}


// -- ObjectTypeException ------------------------------------------------
ObjectTypeException::ObjectTypeException( ObjectType actual, ObjectType require ) :
actual_( actual ),
require_( require )
{
}

ObjectType
ObjectTypeException::GetActualType( void ) const
{
  return actual_;
}

ObjectType
ObjectTypeException::GetRequireType( void ) const
{
  return require_;
}


std::string
ObjectTypeException::GetStandardMessage( void ) const
{
  std::string tmp = "オブジェクトタイプエラー";
  tmp.append( "\r\n" );
  tmp.append( "actual  : " + actual_.ToString() );
  tmp.append( "\r\n" );
  tmp.append( "require : " + require_.ToString() );
  return tmp;
}

std::string
ObjectTypeException::Dump( void ) const
{
  std::ostringstream os;
  os << typeid( *this ).name() << " : actual = " << actual_.ToString()  << " : require = " << require_.ToString();
  return os.str();
}


// -- ObjectHasNotSlotException ------------------------------------------
ObjectHasNotSlotException::ObjectHasNotSlotException( ObjectType actual ) :
actual_( actual )
{
}

ObjectType
ObjectHasNotSlotException::GetActualType( void ) const
{
  return actual_;
}

std::string
ObjectHasNotSlotException::GetStandardMessage( void ) const
{
  std::string tmp = "非スロット持オブジェクトエラー";
  tmp.append( "\r\n" );
  tmp.append( "actual  : " + actual_.ToString() );
  return tmp;
}

std::string
ObjectHasNotSlotException::Dump( void ) const
{
  std::ostringstream os;
  os << typeid( *this ).name() << " : actual = " << actual_.ToString();
  return os.str();
}

// -- GetOperationException ----------------------------------------------
GetOperationException::GetOperationException( const std::string& key_info ) :
WithString( key_info )
{
}

const std::string&
GetOperationException::GetKeyInfo( void ) const
{
  return this->GetString();
}

std::string
GetOperationException::GetStandardMessage( void ) const
{
  std::string tmp = "get 操作エラー";
  tmp.append( "\r\n" );
  tmp.append( "key  : " + this->GetKeyInfo() );
  return tmp;
}

std::string
GetOperationException::Dump( void ) const
{
  std::ostringstream os;
  os << typeid( *this ).name() << " : " << this->GetKeyInfo();
  return os.str();
}

// -- SetOperationException ----------------------------------------------
SetOperationException::SetOperationException( const std::string& key_info ) :
WithString( key_info )
{
}

const std::string&
SetOperationException::GetKeyInfo( void ) const
{
  return this->GetString();
}

std::string
SetOperationException::GetStandardMessage( void ) const
{
  std::string tmp = "set 操作エラー";
  tmp.append( "\r\n" );
  tmp.append( "key  : " + this->GetKeyInfo() );
  return tmp;
}

std::string
SetOperationException::Dump( void ) const
{
  std::ostringstream os;
  os << typeid( *this ).name() << " : " << this->GetKeyInfo();
  return os.str();
}

// -- FileExecutionException -------------------------------------------
FileExecutionException::FileExecutionException( const std::string& filename ) :
WithString( filename )
{
}


const std::string&
FileExecutionException::GetFileName( void ) const
{
  return this->GetString();
}

std::string
FileExecutionException::GetStandardMessage( void ) const
{
  std::string tmp = "ファイル実行エラー";
  tmp.append( "\r\n" );
  tmp.append( "file  : " + this->GetFileName() );
  return tmp;
}

std::string
FileExecutionException::Dump( void ) const
{
  std::ostringstream os;
  os << typeid( *this ).name() << " : " << this->GetFileName();
  return os.str();
}

// -- CompilerException --------------------------------------------------
CompilerException::CompilerException( const std::string& reason, const std::string& source, int line, int column ) :
reason_( reason ),
source_( source ),
line_( line ),
column_( column )
{
}


const std::string&
CompilerException::GetReason( void ) const
{
  return reason_;
}

const std::string&
CompilerException::GetSource( void ) const
{
  return source_;
}

int
CompilerException::GetLine( void ) const
{
  return line_;
}

int
CompilerException::GetColumn( void ) const
{
  return column_;
}


std::string
CompilerException::GetInfoString( void ) const
{
  char buf[1024];
  sprintf_s( buf, sizeof( buf ), "%s : line = (%d) : column = (%d) : %s", source_.c_str(), line_, column_, reason_.c_str() );
  return buf;
}


std::string
CompilerException::GetStandardMessage( void ) const
{
  std::string tmp = "コンパイルエラー";
  tmp.append( "\r\n" );
  tmp.append( this->GetInfoString() );
  return tmp;
}

std::string
CompilerException::Dump( void ) const
{
  std::ostringstream os;
  os << typeid( *this ).name() << " : " << this->GetInfoString();
  return os.str();
}


// -- RuntimeException ---------------------------------------------------
RuntimeException::RuntimeException( const std::string& thrown, std::vector<StackInformation>& call_stack ) :
thrown_( thrown ),
call_stack_( call_stack )
{
}

RuntimeException::RuntimeException( VirtualMachine& vm ) :
thrown_( "unknown" ),
call_stack_( vm.GetCallStack() )
{
  if ( vm.Native().GetTopIndex() >= 1 ) {
    try {
      thrown_ = vm.Native().GetAsString( Const::StackTop );
    }
    catch ( Exception& ) {
    }
  }
}


const std::string&
RuntimeException::GetThrown( void ) const
{
  return thrown_;
}

const std::vector<StackInformation>
RuntimeException::GetCallStack( void ) const
{
  return call_stack_;
}


std::string
RuntimeException::GetInfoString( void ) const
{
  std::string tmp;
  tmp.append( "Exception [" + thrown_ + "]" );

  tmp.append( "\r\n\r\nCall Stack" );
  for ( auto& one : call_stack_ ) {
    tmp.append( "\r\n[" + one.function_name_ + "()] " );
    tmp.append( one.source_file_path_ + "(" + TtUtility::ToStringFrom( one.source_file_line_ ) + ")" );
    for ( auto& local : one.local_infos_ ) {
      tmp.append( "\r\n  [" + local.name_ + "] " + local.value_ );
    }
  }
  return tmp;
}


std::string
RuntimeException::GetStandardMessage( void ) const
{
  std::string tmp = "実行時エラー";
  tmp.append( "\r\n" );
  tmp.append( this->GetInfoString() );
  return tmp;
}

std::string
RuntimeException::Dump( void ) const
{
  std::ostringstream os;
  os << typeid( *this ).name() << " : " << thrown_;
  return os.str();
}
