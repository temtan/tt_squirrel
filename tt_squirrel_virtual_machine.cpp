// tt_squirrel_virtual_machine.cpp

#include "tt_utility.h"

#include "tt_squirrel_native_api.h"
#include "tt_squirrel_utility.h"
#include "tt_squirrel_exception.h"

#include "tt_squirrel_virtual_machine.h"

using namespace TtSquirrel;

// for printf debug
// PCI( Native().GetTopIndex() );
// PCSS( this->GetTopType().ToString() );

namespace Tag {
#define DEFINE_PARAMETER_NAME_STRING( name ) static const std::string name( #name )
  DEFINE_PARAMETER_NAME_STRING( puts );
  DEFINE_PARAMETER_NAME_STRING( print );
  // DEFINE_PARAMETER_NAME_STRING(  );
}


// -- StackRecoverer ---------------------------------------------------
StackRecoverer::StackRecoverer( VirtualMachine* vm, unsigned int leave ) :
vm_( vm ),
top_( vm->Native().GetTop() + leave )
{
}

StackRecoverer::~StackRecoverer()
{
  vm_->Native().SetTop( top_ );
}


// -- VirtualMachine -----------------------------------------------------
std::unordered_map<HSQUIRRELVM, VirtualMachine*>
VirtualMachine::HANDLE_TABLE;

void
VirtualMachine::FunctionForCompilerError( HSQUIRRELVM vm, const SQChar* reason, const SQChar* source, SQInteger line, SQInteger column )
{
  auto it = HANDLE_TABLE.find( vm );
  if ( it != HANDLE_TABLE.end() ) {
    it->second->last_compiler_error_ = std::make_shared<CompilerException>( reason, source, static_cast<int>( line ), static_cast<int>( column ) );
    if ( it->second->compiler_error_handler_ ) {
      it->second->compiler_error_handler_( reason, source, static_cast<int>( line ), static_cast<int>( column ) );
    }
  }
}


VirtualMachine&
VirtualMachine::GetVirtualMachineByHandle( HSQUIRRELVM vm )
{
  auto it = HANDLE_TABLE.find( vm );
  if ( it == HANDLE_TABLE.end() ) {
    throw TT_SQUIRREL_INTERNAL_EXCEPTION;
  }
  return *it->second;
}


VirtualMachine::VirtualMachine( int stack_size ) :
vm_( NativeAPI::Open( stack_size ) ),
parent_window_( nullptr )
{
  HANDLE_TABLE.insert( {vm_, this} );
  Native().SetCompilerErrorHandler( VirtualMachine::FunctionForCompilerError );
  this->RegisterErrorHandler( [] ( VirtualMachine& vm ) -> int {
    return Const::NoneReturnValue;
  } );
}


VirtualMachine::~VirtualMachine()
{
  if ( auto it = HANDLE_TABLE.find( vm_ ); it != HANDLE_TABLE.end() ) {
    HANDLE_TABLE.erase( it );
  }
  for ( auto& object : externally_objects_ ) {
    object->vm_ = nullptr;
  }
  if ( vm_ ) {
    this->Native().Close();
    vm_ = nullptr;
  }
}

HSQUIRRELVM
VirtualMachine::GetHandle( void )
{
  return vm_;
}


void
VirtualMachine::RegisterExternallyObject( Object* object )
{
  externally_objects_.push_back( object );
}

void
VirtualMachine::UnregisterExternallyObject( Object* object )
{
  externally_objects_.erase( std::remove( externally_objects_.begin(), externally_objects_.end(), object ), externally_objects_.end() );
}


NativeAPI
VirtualMachine::Native( void )
{
  return NativeAPI( vm_ );
}

TtWindow*
VirtualMachine::GetParentWindow( void )
{
  return parent_window_ ? parent_window_ : &TtExtraordinarilyWindow::Null;
}

void
VirtualMachine::SetParentWindow( TtWindow* parent )
{
  parent_window_ = parent;
}


void
VirtualMachine::RegisterStandardLibrariesAndAllAdditionalLibraries( void )
{
  this->RegisterStandardLibraries();
  this->RegisterAdditionalLibrariesUtility();
  this->RegisterAdditionalLibrariesMessageBox();
  this->RegisterAdditionalLibrariesPath();
  this->RegisterAdditionalLibrariesFileDialog();
}

void
VirtualMachine::RegisterStandardLibraries( void )
{
  StackRecoverer recoverer( this );
  Native().PushRootTable();
  Native().Std().RegisterBlobLibrary();
  Native().Std().RegisterIOLibrary();
  Native().Std().RegisterSystemLibrary();
  Native().Std().RegisterMathLibrary();
  Native().Std().RegisterStringLibrary();
}


void
VirtualMachine::RegisterDefaultPrintErrorFunction( void )
{
  Native().SetPrintFunction( Utility::DefaultPrintFunction, Utility::DefaultErrorFunction );
}

// ----------
void
VirtualMachine::RemainTop( Operation operation )
{
  int top = Native().GetTopIndex();
  operation();
  while ( Native().GetTopIndex() > top + 1 ) {
    Native().Remove( -2 );
  }
}


void
VirtualMachine::PushObject( Object& object )
{
  Native().PushObject( object.GetHandle() );
}


template <>
void
VirtualMachine::PushAuto<int>( int value )
{
  this->Native().PushInteger( value );
}

template <>
void
VirtualMachine::PushAuto<int&>( int& value )
{
  this->Native().PushInteger( value );
}

template <>
void
VirtualMachine::PushAuto<float>( float value )
{
  this->Native().PushFloat( value );
}

template <>
void
VirtualMachine::PushAuto<float&>( float& value )
{
  this->Native().PushFloat( value );
}

template <>
void
VirtualMachine::PushAuto<double>( double value )
{
  this->Native().PushFloat( static_cast<float>( value ) );
}

template <>
void
VirtualMachine::PushAuto<double&>( double& value )
{
  this->Native().PushFloat( static_cast<float>( value ) );
}

template <>
void
VirtualMachine::PushAuto<const char*>( const char* value )
{
  this->Native().PushString( value );
}

template <>
void
VirtualMachine::PushAuto<std::string>( std::string value )
{
  this->Native().PushString( value );
}

template <>
void
VirtualMachine::PushAuto<std::string&>( std::string& value )
{
  this->Native().PushString( value );
}

template <>
void
VirtualMachine::PushAuto<const std::string&>( const std::string& value )
{
  this->Native().PushString( value );
}

template <>
void
VirtualMachine::PushAuto<bool>( bool value )
{
  this->Native().PushBoolean( value );
}

template <>
void
VirtualMachine::PushAuto<bool&>( bool& value )
{
  this->Native().PushBoolean( value );
}

template <>
void
VirtualMachine::PushAuto<SQUserPointer>( SQUserPointer value )
{
  this->Native().PushUserPointer( value );
}

template <>
void
VirtualMachine::PushAuto<SQUserPointer&>( SQUserPointer& value )
{
  this->Native().PushUserPointer( value );
}


void
VirtualMachine::NewSlot( Operation table, Operation key, Operation value, bool is_static )
{
  StackRecoverer recoverer( this );
  table();
  if ( NOT( Native().GetType( Const::StackTop ).IsMakeableSlot() ) ) {
    throw ObjectHasNotSlotException( Native().GetType( Const::StackTop ) );
  }
  key();
  value();
  Native().NewSlot( Utility::PushedFromTop( 2 ), is_static );
}

void
VirtualMachine::NewSlotOfRootTable( Operation key, Operation value )
{
  this->NewSlot( [&] () { Native().PushRootTable(); }, key, value );
}

void
VirtualMachine::NewSlotOfRootTableByString( const std::string& key, Operation value )
{
  this->NewSlotOfRootTable( [&] () { Native().PushString( key ); }, value );
}

void
VirtualMachine::NewSlotOfTop( Operation key, Operation value, bool is_static )
{
  this->NewSlot( [&] () { Native().Push( Const::StackTop ); }, key, value, is_static );
}

void
VirtualMachine::NewSlotOfTopByString( const std::string& key, Operation value, bool is_static )
{
  this->NewSlotOfTop( [&] () { Native().PushString( key ); }, value, is_static );
}

void
VirtualMachine::NewSlotOfTopByStringAsStatic( const std::string& key, Operation value )
{
  this->NewSlotOfTopByString( key, value, true );
}

void
VirtualMachine::NewStringSlotOfTopByString( const std::string& key, const std::string& value, bool is_static )
{
  this->NewSlotOfTopByString( key, [&] () { Native().PushString( value ); }, is_static );
}

void
VirtualMachine::NewBooleanSlotOfTopByString( const std::string& key, bool value, bool is_static )
{
  this->NewSlotOfTopByString( key, [&] () { Native().PushBoolean( value ); }, is_static );
}

void
VirtualMachine::NewIntegerSlotOfTopByString( const std::string& key, int value, bool is_static )
{
  this->NewSlotOfTopByString( key, [&] () { Native().PushInteger( value ); }, is_static );
}

void
VirtualMachine::NewFloatSlotOfTopByString( const std::string& key, float value, bool is_static )
{
  this->NewSlotOfTopByString( key, [&] () { Native().PushFloat( value ); }, is_static );
}

void
VirtualMachine::NewNullSlotOfTopByString( const std::string& key, bool is_static )
{
  this->NewSlotOfTopByString( key, [&] () { Native().PushNull(); }, is_static );
}


void
VirtualMachine::Set( Operation table, Operation key, Operation value )
{
  StackRecoverer recoverer( this );
  table();
  key();
  value();
  Native().Set( Utility::PushedFromTop( 2 ) );
}

void
VirtualMachine::SetToTop( Operation key, Operation value )
{
  this->Set( [&] () { Native().Push( Const::StackTop ); }, key, value );
}

void
VirtualMachine::SetToTopByString( const std::string& key, Operation value )
{
  this->SetToTop( [&] () { Native().PushString( key ); }, value );
}

void
VirtualMachine::SetStringToTopByString( const std::string& key, const std::string& value )
{
  this->SetToTopByString( key, [&] () { Native().PushString( value ); } );
}

void
VirtualMachine::SetIntegerToTopByString( const std::string& key, int value )
{
  this->SetToTopByString( key, [&] () { Native().PushInteger( value ); } );
}

void
VirtualMachine::SetFloatToTopByString( const std::string& key, float value )
{
  this->SetToTopByString( key, [&] () { Native().PushFloat( value ); } );
}

void
VirtualMachine::SetBooleanToTopByString( const std::string& key, bool value )
{
  this->SetToTopByString( key, [&] () { Native().PushBoolean( value ); } );
}


void
VirtualMachine::Get( Operation table, Operation key )
{
  StackRecoverer recoverer( this, 1 );
  table();
  key();
  Native().Get( Utility::Pushed( Const::StackTop, 1 ) );
  Native().Remove( Utility::PushedFromTop( 1 ) );
}

void
VirtualMachine::GetByString( Operation table, const std::string& key )
{
  this->Get( table, [&] () { Native().PushString( key ); } );
}

void
VirtualMachine::GetByInteger( Operation table, int key )
{
  this->Get( table, [&] () { Native().PushInteger( key ); } );
}

void
VirtualMachine::GetByStringFrom( int index, const std::string& key )
{
  Native().PushString( key );
  try {
    Native().Get( Utility::Pushed( index, 1 ) );
  }
  catch ( NativeAPIException ) {
    throw GetOperationException( "string : " + key );
  }
}

void
VirtualMachine::GetByIntegerFrom( int index, int key )
{
  Native().PushInteger( key );
  try {
    Native().Get( Utility::Pushed( index, 1 ) );
  }
  catch ( NativeAPIException ) {
    throw GetOperationException( "integer : " + Utility::ToStringFrom( index ) );
  }
}

void
VirtualMachine::GetByStringFromTop( const std::string& key )
{
  this->GetByStringFrom( Const::StackTop, key );
}

void
VirtualMachine::GetByIntegerFromTop( int key )
{
  this->GetByIntegerFrom( Const::StackTop, key );
}

void
VirtualMachine::GetFromRootTable( Operation key )
{
  this->Get( [&] () { Native().PushRootTable(); }, key );
}

void
VirtualMachine::GetByStringFromRootTable( const std::string& key )
{
  this->GetFromRootTable( [&] () { Native().PushString( key ); } );
}

void
VirtualMachine::GetByStringFromRootTableJoined( std::vector<std::string> v )
{
  Native().PushRootTable();
  for ( std::string& str : v ) {
    this->GetByStringFromTop( str );
    Native().Remove( Utility::PushedFromTop( 1 ) );
  }
}

// only int, float, std::string, bool, SQUserPointer
template <>
int
VirtualMachine::GetAsFrom<int>( int index )
{
  return Native().GetAsInteger( index );
}

template <>
float
VirtualMachine::GetAsFrom<float>( int index )
{
  return Native().GetAsFloat( index );
}

template <>
std::string
VirtualMachine::GetAsFrom<std::string>( int index )
{
  return Native().GetAsString( index );
}

template <>
bool
VirtualMachine::GetAsFrom<bool>( int index )
{
  return Native().GetAsBoolean( index );
}

template <>
SQUserPointer
VirtualMachine::GetAsFrom<SQUserPointer>( int index )
{
  return Native().GetAsUserPointer( index );
}

#ifdef DEBUG
template int           VirtualMachine::GetByStringFromTopAndGetAs<int>(           const std::string& key );
template float         VirtualMachine::GetByStringFromTopAndGetAs<float>(         const std::string& key );
template std::string   VirtualMachine::GetByStringFromTopAndGetAs<std::string>(   const std::string& key );
template bool          VirtualMachine::GetByStringFromTopAndGetAs<bool>(          const std::string& key );
template SQUserPointer VirtualMachine::GetByStringFromTopAndGetAs<SQUserPointer>( const std::string& key );
#endif

std::string
VirtualMachine::ToStringFromTopAndGetAsString( void )
{
  TtSquirrel::StackRecoverer recoverer( this );

  this->Native().ToString( TtSquirrel::Const::StackTop );
  return this->GetAsFromTop<std::string>();
}


bool
VirtualMachine::InstanceOf( Operation klass, Operation instance )
{
  StackRecoverer recoverer( this );
  klass();
  instance();
  return Native().InstanceOf();
}

bool
VirtualMachine::InstanceAtTopOf( Operation klass )
{
  return this->InstanceOf( klass, [&] () { Native().Push( Utility::PushedFromTop( 1 ) ); } );
}


void
VirtualMachine::Foreach( Operation target, ForeachProcess process )
{
  StackRecoverer recoverer( this );
  target();
  Native().PushNull();
  int top = Native().GetTopIndex();
  while ( Native().NextWithResult( Const::StackTop - 1 ) ) {
    process( *this );
    Native().ResizeStackSize( top );
  }
  Native().Pop( 1 );
}


void
VirtualMachine::Call( Operation clouser, ParametersOperation parameters, bool push_return_value )
{
  StackRecoverer recoverer( this, push_return_value ? 1 : 0 );
  clouser();
  unsigned int params_count = parameters();
  try {
    Native().Call( params_count, push_return_value, true );
  }
  catch ( NativeAPIException ) {
    this->ThrowIfErrorOccurred();
    throw;
  }
  if ( push_return_value ) {
    Native().Remove( Utility::PushedFromTop( 1 ) );
  }
}

void
VirtualMachine::CallAndPushReturnValue( Operation clouser, ParametersOperation parameters )
{
  this->Call( clouser, parameters, true );
}

void
VirtualMachine::CallAndNoReturnValue( Operation clouser, ParametersOperation parameters )
{
  this->Call( clouser, parameters, false );
}

void
VirtualMachine::CallObjectOfGetByStringFromTop( const std::string& key, ParametersOperationPassedTable parameters, bool push_return_value )
{
  auto object = this->GetStackTopObject();
  this->Call(
    [&] () {
      this->PushObject( object );
      this->GetByStringFromTop( key );
      this->Native().Remove( TtSquirrel::Utility::PushedFromTop( 1 ) );
    },
    [&] () { return parameters( object ); },
    push_return_value );
}

void
VirtualMachine::CallObjectOfGetByStringFromTopAndPushReturnValue( const std::string& key, ParametersOperationPassedTable parameters )
{
  this->CallObjectOfGetByStringFromTop( key, parameters, true );
}

void
VirtualMachine::CallObjectOfGetByStringFromTopAndNoReturnValue( const std::string& key, ParametersOperationPassedTable parameters )
{
  this->CallObjectOfGetByStringFromTop( key, parameters, false );
}


void
VirtualMachine::CallPrint( const std::string& str )
{
  this->CallAndNoReturnValue(
    [&] () { this->GetByStringFromRootTable( Tag::print ); },
    [&] () {
      Native().PushRootTable();
      Native().PushString( str );
      return 2;
    } );
}

void
VirtualMachine::CallPuts( const std::string& str )
{
  this->CallAndNoReturnValue(
    [&] () { this->GetByStringFromRootTable( Tag::puts ); },
    [&] () {
      Native().PushRootTable();
      Native().PushString( str );
      return 2;
    } );
}


void
VirtualMachine::NewClosure( Closure closure, const std::string& name )
{
  StackRecoverer recoverer( this, 1 );
  SQFUNCTION func = [] ( HSQUIRRELVM vm_handle ) -> SQInteger {
    VirtualMachine& vm = VirtualMachine::GetVirtualMachineByHandle( vm_handle );

    try {
      SQUserPointer tmp = vm.Native().GetAsUserData( Const::StackTop ).first;
      vm.Native().PopTop();
      SQInteger ret = (**static_cast<Closure**>( tmp ))( vm );
      return ret;
    }
    catch ( CompilerException& ex ) {
      vm.last_compiler_error_ = std::shared_ptr<CompilerException>( new CompilerException( ex ) );
      return SQ_ERROR;
    }
    catch ( RuntimeException& ex ) {
      vm.last_runtime_error_ = std::shared_ptr<RuntimeException>( new RuntimeException( ex ) );
      return SQ_ERROR;
    }
    catch ( NativeAPIException& ex ) {
      return vm.Native().ThrowError( "Error in callback: " + ex.GetFunctionName() + ":" + ex.GetReason() );
    }
  };
  this->CreateObject( closure );
  Native().NewClosure( func, 1 );
  if ( NOT( name.empty() ) ) {
    Native().SetNativeClosureName( Const::StackTop, name.c_str() );
  }
}


void
VirtualMachine::AppendToArray( Operation array, Operation element )
{
  StackRecoverer recoverer( this );
  array();
  this->AppendToArrayOf( Const::StackTop, element );
}

void
VirtualMachine::AppendToArrayOf( int index, Operation element )
{
  StackRecoverer recoverer( this );
  element();
  Native().ArrayAppend( Utility::Pushed( index, 1 ) );
}

void
VirtualMachine::AppendToArrayOfStackTop( Operation element )
{
  this->AppendToArrayOf( Const::StackTop, element );
}


void
VirtualMachine::ArrayEach( Operation array, EachProcess process )
{
  StackRecoverer recoverer( this );
  array();
  unsigned int size = Native().GetSize( Const::StackTop );
  for ( unsigned int i = 0; i < size; ++i ) {
    StackRecoverer recoverer( this );
    this->GetByIntegerFrom( Const::StackTop, i );
    process( i );
  }
}


void
VirtualMachine::CompileBuffer( const std::string& buffer, const std::string& buffer_name )
{
  try {
    Native().CompileBuffer( buffer, static_cast<int>( buffer.size() ), buffer_name, true );
  }
  catch ( NativeAPIException ) {
    this->ThrowIfErrorOccurred();
    throw;
  }
}

void
VirtualMachine::DoBufferAt( Operation          table,
                            const std::string& buffer,
                            bool               push_return_value,
                            const              std::string& buffer_name )
{
  StackRecoverer recoverer( this, push_return_value ? 1 : 0 );
  try {
    this->Call(
      [&] () { this->CompileBuffer( buffer, buffer_name ); },
      [&] () { table(); return 1; },
      push_return_value );
  }
  catch ( NativeAPIException ) {
    this->ThrowIfErrorOccurred();
    throw;
  }
}

void
VirtualMachine::DoBufferAtTop( const std::string& buffer,
                               bool               push_return_value,
                               const              std::string& buffer_name )
{
  this->DoBufferAt( [&] () { Native().Push( Utility::PushedFromTop( 1 ) ); },
                    buffer, push_return_value, buffer_name );
}

void
VirtualMachine::DoBufferAtRoot( const std::string& buffer,
                                bool               push_return_value,
                                const std::string& buffer_name )
{
  this->DoBufferAt( [&] () { Native().PushRootTable(); },
                    buffer, push_return_value, buffer_name );
}


void
VirtualMachine::DoFile( const std::string& filename, bool push_return_value )
{
  StackRecoverer recoverer( this, push_return_value ? 1 : 0 );

  Native().PushRootTable();
  try {
    Native().Std().DoFile( filename.c_str(), push_return_value, true );
  }
  catch ( NativeAPIException ) {
    this->ThrowIfErrorOccurred();
    throw FileExecutionException( filename );
  }
  if ( push_return_value ) {
    Native().Remove( Utility::PushedFromTop( 1 ) );
  }
}


ObjectType
VirtualMachine::GetTopType( void )
{
  return Native().GetType( Const::StackTop );
}


Object
VirtualMachine::GetNullObject( bool auto_reference )
{
  StackRecoverer recoverer( this );
  this->Native().PushNull();
  return this->GetStackTopObject( auto_reference );
}

Object
VirtualMachine::GetStackObject( int index, bool auto_reference )
{
  HSQOBJECT object = this->Native().GetStackObject( index );
  Object tmp( object, *this, auto_reference );
  return tmp;
}

Object
VirtualMachine::GetStackTopObject( bool auto_reference )
{
  return this->GetStackObject( Const::StackTop, auto_reference );
}


std::vector<StackInformation>
VirtualMachine::GetCallStack( void )
{
  std::vector<StackInformation> v;
  int level = 1;
  SQStackInfos stack_info;
  for ( int level = 1; ; ++level ) {
    try {
      Native().StackInformations( level, &stack_info );
    }
    catch ( NativeAPIException ) {
      break;
    }

    StackInformation tmp( stack_info );

    for ( unsigned int seq = 0; ; ++seq ) {
      std::string name = Native().GetLocal( level, seq );
      if ( name.empty() ) {
        break;
      }
      ObjectType type = this->GetTopType();

      std::string value = type.ToString();
      switch ( type.ToValue() ) {
      case OT_INTEGER:
        value = TtUtility::ToStringFrom( Native().GetAsInteger( Const::StackTop ) );
        break;

      case OT_FLOAT:
        value = TtUtility::ToStringFrom( Native().GetAsFloat( Const::StackTop ) );
        break;

      case OT_STRING:
        value = "\""+ Native().GetAsString( Const::StackTop ) + "\"";
        break;

      case OT_BOOL:
        value = Native().GetAsBoolean( Const::StackTop ) ? "true" : "false";
        break;
      }
      tmp.local_infos_.push_back( LocalInformation( name, type, value ) );
      Native().PopTop();
    }
    v.push_back( tmp );
  }
  return v;
}


void
VirtualMachine::RegisterCompilerErrorHandler( CompilerErrorHandler handler )
{
  compiler_error_handler_ = handler;
}

void
VirtualMachine::RegisterErrorHandler( Closure closure )
{
  Closure tmp = [&, closure] ( VirtualMachine& vm ) -> int {
    last_runtime_error_ = std::shared_ptr<RuntimeException>( new RuntimeException( vm ) );
    return closure( vm );
  };
  this->NewClosure( tmp );
  Native().SetErrorHandler();
}


void
VirtualMachine::ThrowIfErrorOccurred( void )
{
  if ( last_compiler_error_ ) {
    CompilerException ex( *last_compiler_error_ );
    last_compiler_error_.reset();
    throw ex;
  }
  if ( last_runtime_error_ ) {
    RuntimeException ex( *last_runtime_error_ );
    last_runtime_error_.reset();
    throw ex;
  }
}
