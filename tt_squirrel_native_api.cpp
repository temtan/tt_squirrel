// tt_squirrel_native_api.cpp

#include "ttl_define.h"

#include "tt_squirrel_exception.h"

#include "tt_squirrel_native_api.h"

#define SQBOOL_TO_BOOLEAN( v ) ((v) ? true : false)
#define RESULT_TEST( f, ... )  ResultTest( f(__VA_ARGS__), #f )

using namespace TtSquirrel;


// -- NativeAPI ----------------------------------------------------------
NativeAPI::NativeAPI( HSQUIRRELVM& vm ) :
vm_( vm )
{
}

// -- Virtual Machine ----------------------------------------------------
HSQUIRRELVM
NativeAPI::Open( int stack_size )
{
  return sq_open( stack_size );
}

void
NativeAPI::Close( void )
{
  sq_close( vm_ );
}


void
NativeAPI::SetErrorHandler( void )
{
  sq_seterrorhandler( vm_ );
}


SQPRINTFUNCTION
NativeAPI::GetPrintFunction( void )
{
  return sq_getprintfunc( vm_ );
}

void
NativeAPI::SetPrintFunction( SQPRINTFUNCTION print_function, SQPRINTFUNCTION error_function )
{
  sq_setprintfunc( vm_, print_function, error_function );
}

SQPRINTFUNCTION
NativeAPI::GetErrorFunction( void )
{
  return sq_geterrorfunc( vm_ );
}


SQUserPointer
NativeAPI::GetForeignPointer( void )
{
  return sq_getforeignptr( vm_ );
}

void
NativeAPI::SetForeignPointer( SQUserPointer pointer )
{
  sq_setforeignptr( vm_, pointer );
}


HSQUIRRELVM
NativeAPI::NewThread( int initial_stack_size )
{
  return sq_newthread( vm_, initial_stack_size );
}


void
NativeAPI::PushRootTable( void )
{
  sq_pushroottable( vm_ );
}

void
NativeAPI::PushConstTable( void )
{
  sq_pushconsttable( vm_ );
}

void
NativeAPI::PushRegistryTable( void )
{
  sq_pushregistrytable( vm_ );
}

void
NativeAPI::SetRootTable( void )
{
  RESULT_TEST( sq_setroottable, vm_ );
}

void
NativeAPI::SetConstTable( void )
{
  RESULT_TEST( sq_setconsttable, vm_ );
}


void
NativeAPI::SuspendVM( void )
{
  RESULT_TEST( sq_suspendvm, vm_ );
}


void
NativeAPI::WakeupVM( bool resumedret, bool retval, bool raiseerror, bool throwerror )
{
  RESULT_TEST( sq_wakeupvm, vm_, resumedret, retval, raiseerror, throwerror );
}


int
NativeAPI::GetVMState( void )
{
  return sq_getvmstate( vm_ );
}

void
NativeAPI::Move( HSQUIRRELVM destination, int index )
{
  sq_move( destination, vm_, index );
}


SQUserPointer
NativeAPI::GetSharedForeignPointer( void )
{
  return sq_getsharedforeignptr( vm_ );
}

void
NativeAPI::SetSharedForeignPointer( SQUserPointer pointer )
{
  sq_setsharedforeignptr( vm_, pointer );
}

SQRELEASEHOOK
NativeAPI::GetSharedReleaseHook( void )
{
  return sq_getsharedreleasehook( vm_ );
}

void
NativeAPI::SetSharedReleaseHook( SQRELEASEHOOK hook )
{
  sq_setsharedreleasehook( vm_, hook );
}


SQRELEASEHOOK
NativeAPI::GetVMReleaseHook( void )
{
  return sq_getvmreleasehook( vm_ );
}

void
NativeAPI::SetVMReleaseHook( SQRELEASEHOOK hook )
{
  sq_setvmreleasehook( vm_, hook );
}


int
NativeAPI::CollectGarbage( void )
{
  return sq_collectgarbage( vm_ );
}

void
NativeAPI::ResurrectUnreachable( void )
{
  RESULT_TEST( sq_resurrectunreachable, vm_ );
}


int
NativeAPI::GetVersion( void )
{
  return sq_getversion();
}


// -- Compiler -----------------------------------------------------------
void
NativeAPI::Compile( SQLEXREADFUNC read, SQUserPointer pointer, const std::string& source_name, bool raise_error )
{
  RESULT_TEST( sq_compile, vm_, read, pointer, source_name.c_str(), raise_error );
}

void
NativeAPI::CompileBuffer( const std::string& s, int size, const std::string& source_name, bool raise_error )
{
  RESULT_TEST( sq_compilebuffer, vm_, s.c_str(), size, source_name.c_str(), raise_error );
}

void
NativeAPI::EnableDebugInfo( bool enable )
{
  sq_enabledebuginfo( vm_, enable );
}

void
NativeAPI::NotifyAllExceptions( bool enable )
{
  sq_notifyallexceptions( vm_, enable );
}

void
NativeAPI::SetCompilerErrorHandler( SQCOMPILERERROR f )
{
  sq_setcompilererrorhandler( vm_, f );
}

// -- Stack Operations ---------------------------------------------------
int
NativeAPI::Cmp( void )
{
  return sq_cmp( vm_ );
}

int
NativeAPI::GetTop( void )
{
  return sq_gettop( vm_ );
}

void
NativeAPI::SetTop( int top_index )
{
  sq_settop( vm_, top_index );
}

void
NativeAPI::Pop( int count )
{
  sq_pop( vm_, count );
}

void
NativeAPI::PopTop( void )
{
  sq_poptop( vm_ );
}

void
NativeAPI::Push( int index )
{
  sq_push( vm_, index );
}

void
NativeAPI::Remove( int index )
{
  sq_remove( vm_, index );
}

void
NativeAPI::ReserveStack( int size )
{
  sq_reservestack( vm_, size );
}

// -- Object creation and handling ---------------------------------------
void
NativeAPI::BindEnv( int index )
{
  RESULT_TEST( sq_bindenv, vm_, index );
}

void
NativeAPI::CreateInstance( int index )
{
  RESULT_TEST( sq_createinstance, vm_, index );
}


void
NativeAPI::GetClosureInfo( int index, unsigned int* nparams, unsigned int* nfreevars )
{
  RESULT_TEST( sq_getclosureinfo, vm_, index, nparams, nfreevars );
}

void
NativeAPI::GetClosureName( int index )
{
  RESULT_TEST( sq_getclosurename, vm_, index );
}

void
NativeAPI::GetClosureRoot( int index )
{
  RESULT_TEST( sq_getclosureroot, vm_, index );
}

void
NativeAPI::SetClosureRoot( int index )
{
  RESULT_TEST( sq_setclosureroot, vm_, index );
}


int
NativeAPI::GetSize( int index )
{
  return sq_getsize( vm_, index );
}

SQHash
NativeAPI::GetHash( int index )
{
  return sq_gethash( vm_, index );
}


int
NativeAPI::GetAsInteger( int index )
{
  int tmp;
  RESULT_TEST( sq_getinteger, vm_, index, &tmp );
  return tmp;
}

float
NativeAPI::GetAsFloat( int index )
{
  float tmp;
  RESULT_TEST( sq_getfloat, vm_, index, &tmp );
  return tmp;

}

std::string
NativeAPI::GetAsString( int index )
{
  const char* tmp;
  RESULT_TEST( sq_getstring, vm_, index, &tmp );
  return tmp;
}

bool
NativeAPI::GetAsBoolean( int index )
{
  SQBool tmp;
  RESULT_TEST( sq_getbool, vm_, index, &tmp );
  return tmp == SQTrue;
}

SQUserPointer
NativeAPI::GetAsUserPointer( int index )
{
  SQUserPointer tmp;
  RESULT_TEST( sq_getuserpointer, vm_, index, &tmp );
  return tmp;
}

HSQUIRRELVM
NativeAPI::GetAsThread( int index )
{
  HSQUIRRELVM tmp;
  RESULT_TEST( sq_getthread, vm_, index, &tmp );
  return tmp;
}

std::pair<SQUserPointer, SQUserPointer>
NativeAPI::GetAsUserData( int index )
{
  SQUserPointer pointer;
  SQUserPointer type_tag;
  RESULT_TEST( sq_getuserdata, vm_, index, &pointer, &type_tag );
  return std::pair<SQUserPointer, SQUserPointer>( pointer, type_tag );
}


SQUserPointer
NativeAPI::GetInstanceUserPointer( int index, SQUserPointer type_tag )
{
  SQUserPointer tmp;
  RESULT_TEST( sq_getinstanceup, vm_, index, &tmp, type_tag );
  return tmp;
}

void
NativeAPI::SetInstanceUserPointer( int index, SQUserPointer pointer )
{
  RESULT_TEST( sq_setinstanceup, vm_, index, pointer );
}


char*
NativeAPI::GetScratchPad( int min_size )
{
  return sq_getscratchpad( vm_, min_size );
}

ObjectType
NativeAPI::GetType( int index )
{
  return sq_gettype( vm_, index );
}

void
NativeAPI::TypeOf( int index )
{
  RESULT_TEST( sq_typeof, vm_, index );
}

SQUserPointer
NativeAPI::GetTypeTag( int index )
{
  SQUserPointer type_tag;
  RESULT_TEST( sq_gettypetag, vm_, index, &type_tag );
  return type_tag;
}


void
NativeAPI::NewClass( bool hasbase )
{
  RESULT_TEST( sq_newclass, vm_, hasbase );
}

void
NativeAPI::NewClosure( SQFUNCTION func, unsigned int nfreevars )
{
  sq_newclosure( vm_, func, nfreevars );
}

SQUserPointer
NativeAPI::NewUserData( unsigned int size )
{
  return sq_newuserdata( vm_, size );
}

void
NativeAPI::PushInteger( int n )
{
  sq_pushinteger( vm_, n );
}

void
NativeAPI::PushFloat( float f )
{
  sq_pushfloat( vm_, f );
}

void
NativeAPI::PushString( const std::string& str )
{
  sq_pushstring( vm_, str.c_str(), str.size() );
}

void
NativeAPI::PushBoolean( bool flag )
{
  sq_pushbool( vm_, flag ? SQTrue : SQFalse );
}

void
NativeAPI::PushUserPointer( SQUserPointer pointer )
{
  sq_pushuserpointer( vm_, pointer );
}

void
NativeAPI::PushNull( void )
{
  sq_pushnull( vm_ );
}

void
NativeAPI::PushThread( HSQUIRRELVM thread )
{
  sq_pushthread( vm_, thread );
}


void
NativeAPI::SetNativeClosureName( int index, const std::string& name )
{
  RESULT_TEST( sq_setnativeclosurename, vm_, index, name.c_str() );
}

void
NativeAPI::SetParamsCheck( int nparamscheck, const std::string& typemask )
{
  RESULT_TEST( sq_setparamscheck, vm_, nparamscheck, typemask.c_str() );
}

SQRELEASEHOOK
NativeAPI::GetReleaseHook( int index )
{
  return sq_getreleasehook( vm_, index );
}

void
NativeAPI::SetReleaseHook( int index, SQRELEASEHOOK hook )
{
  sq_setreleasehook( vm_, index, hook );
}

void
NativeAPI::SetTypeTag( int index, SQUserPointer type_tag )
{
  RESULT_TEST( sq_settypetag, vm_, index, type_tag );
}


bool
NativeAPI::ToBoolean( int index )
{
  unsigned int tmp;
  sq_tobool( vm_, index, &tmp );
  return SQBOOL_TO_BOOLEAN( tmp );
}

void
NativeAPI::ToString( int index )
{
  sq_tostring( vm_, index );
}


void
NativeAPI::SetClassUserDataSize( int index, int user_data_size )
{
  RESULT_TEST( sq_setclassudsize, vm_, index, user_data_size );
}


HSQMEMBERHANDLE
NativeAPI::GetMemberHandle( int index )
{
  HSQMEMBERHANDLE handle;
  RESULT_TEST( sq_getmemberhandle, vm_, index, &handle );
  return handle;
}

void
NativeAPI::GetByHandle( int index, const HSQMEMBERHANDLE* handle )
{
  RESULT_TEST( sq_getbyhandle, vm_, index, handle );
}

void
NativeAPI::SetByHandle( int index, const HSQMEMBERHANDLE* handle )
{
  RESULT_TEST( sq_setbyhandle, vm_, index, handle );
}


// -- Calls --------------------------------------------------------------
void
NativeAPI::Call( int param_count, bool need_return_value , bool raise_exception_in_error )
{
  RESULT_TEST( sq_call, vm_, param_count,
               need_return_value        ? SQTrue : SQFalse,
               raise_exception_in_error ? SQTrue : SQFalse );
}

void
NativeAPI::GetCallEE( void )
{
  RESULT_TEST( sq_getcallee, vm_ );
}

void
NativeAPI::GetLastError( void )
{
  sq_getlasterror( vm_ );
}

const std::string
NativeAPI::GetLocal( unsigned int level, unsigned int index )
{
  const char* tmp = sq_getlocal( vm_, level, index );
  return tmp ? tmp : "";
}

void
NativeAPI::ResetError( void )
{
  sq_reseterror( vm_ );
}

void
NativeAPI::Resume( bool retval, bool raiseerror )
{
  RESULT_TEST( sq_resume, vm_, retval, raiseerror );
}

SQRESULT
NativeAPI::ThrowError( const std::string& err )
{
  return sq_throwerror( vm_, err.c_str() );
}

SQRESULT
NativeAPI::ThrowObject( void )
{
  return sq_throwobject( vm_ );
}

// -- Objects manipulation -----------------------------------------------
void
NativeAPI::Clone( int index )
{
  RESULT_TEST( sq_clone, vm_, index );
}

void
NativeAPI::GetAttributes( int index )
{
  RESULT_TEST( sq_getattributes, vm_, index );
}

void
NativeAPI::SetAttributes( int index )
{
  RESULT_TEST( sq_setattributes, vm_, index );
}

void
NativeAPI::GetBase( int index )
{
  RESULT_TEST( sq_getbase, vm_, index );
}

void
NativeAPI::GetClass( int index )
{
  RESULT_TEST( sq_getclass, vm_, index );
}

void
NativeAPI::GetDefaultDelegate( ObjectType type )
{
  RESULT_TEST( sq_getdefaultdelegate, vm_, type.ToValue() );
}

void
NativeAPI::GetDelegate( int index )
{
  RESULT_TEST( sq_getdelegate, vm_, index );
}

void
NativeAPI::SetDelegate( int index )
{
  RESULT_TEST( sq_setdelegate, vm_, index );
}

bool
NativeAPI::InstanceOf( void )
{
  SQBool ret = sq_instanceof( vm_ );
  this->ResultTest( ret, FUNC_NAME_OF( sq_instanceof ) );
  return SQBOOL_TO_BOOLEAN( ret );
}

void
NativeAPI::WeakReference( int index )
{
  sq_weakref( vm_, index );
}

void
NativeAPI::GetWeakReferenceValue( int index )
{
  RESULT_TEST( sq_getweakrefval, vm_, index );
}

const std::string
NativeAPI::GetFreeVariable( unsigned int index, unsigned int nval )
{
  return sq_getfreevariable( vm_, index, nval );
}

void
NativeAPI::SetFreeVariable( int index, unsigned int nval )
{
  RESULT_TEST( sq_setfreevariable, vm_, index, nval );
}

void
NativeAPI::Clear( int index )
{
  RESULT_TEST( sq_clear, vm_, index );
}

// -- Table --------------------------------------------------------------
void
NativeAPI::NewTable( void )
{
  sq_newtable( vm_ );
}

void
NativeAPI::NewTableEx( int initialcapacity )
{
  sq_newtableex( vm_, initialcapacity );
}

void
NativeAPI::DeleteSlot( int index, bool push_value )
{
  RESULT_TEST( sq_deleteslot, vm_, index, push_value );
}

void
NativeAPI::RawDeleteSlot( int index, bool pushval )
{
  RESULT_TEST( sq_rawdeleteslot, vm_, index, pushval );
}

void
NativeAPI::NewSlot( int index, bool is_static )
{
  RESULT_TEST( sq_newslot, vm_, index, is_static );
}

void
NativeAPI::Get( int index )
{
  RESULT_TEST( sq_get, vm_, index );
}

void
NativeAPI::Set( int index )
{
  RESULT_TEST( sq_set, vm_, index );
}

void
NativeAPI::RawGet( int index )
{
  RESULT_TEST( sq_rawget, vm_, index );
}

void
NativeAPI::RawSet( int index )
{
  RESULT_TEST( sq_rawset, vm_, index );
}

void
NativeAPI::Next( int index )
{
  RESULT_TEST( sq_next, vm_, index );
}

void
NativeAPI::NewMember( int index, bool is_static )
{
  RESULT_TEST( sq_newmember, vm_, index, is_static );
}

void
NativeAPI::RawNewMember( int index, bool is_static )
{
  RESULT_TEST( sq_rawnewmember, vm_, index, is_static );
}

// -- Array --------------------------------------------------------------
void
NativeAPI::NewArray( int size )
{
  sq_newarray( vm_, size );
}

void
NativeAPI::ArrayAppend( int index )
{
  RESULT_TEST( sq_arrayappend, vm_, index );
}

void
NativeAPI::ArrayPop( int index, bool pushval )
{
  RESULT_TEST( sq_arraypop, vm_, index, pushval );
}

void
NativeAPI::ArrayResize( int index, int new_size )
{
  RESULT_TEST( sq_arrayresize, vm_, index, new_size );
}

void
NativeAPI::ArrayReverse( int index )
{
  RESULT_TEST( sq_arrayreverse, vm_, index );
}

void
NativeAPI::ArrayRemove( int index, int item_index )
{
  RESULT_TEST( sq_arrayremove, vm_, index, item_index );
}

void
NativeAPI::ArrayInsert( int index, int destination_position )
{
  RESULT_TEST( sq_arrayinsert, vm_, index, destination_position );
}

// -- Bytecode serialization ---------------------------------------------
void
NativeAPI::WriteClosure( SQWRITEFUNC write_function, SQUserPointer user_pointer )
{
  RESULT_TEST( sq_writeclosure, vm_, write_function, user_pointer );
}

void
NativeAPI::ReadClosure( SQREADFUNC read_function, SQUserPointer user_pointer )
{
  RESULT_TEST( sq_readclosure, vm_, read_function, user_pointer );
}

// -- Raw object handling ------------------------------------------------
void
NativeAPI::PushObject( HSQOBJECT object )
{
  sq_pushobject( vm_, object );
}

HSQOBJECT
NativeAPI::GetStackObject( int index )
{
  HSQOBJECT object;
  RESULT_TEST( sq_getstackobj, vm_, index, &object );
  return object;
}

bool
NativeAPI::Release( HSQOBJECT* object )
{
  return SQBOOL_TO_BOOLEAN( sq_release( vm_, object ) );
}

void
NativeAPI::AddReference( HSQOBJECT* object )
{
  sq_addref( vm_, object );
}

unsigned int
NativeAPI::GetReferenceCount( HSQOBJECT* object )
{
  return sq_getrefcount( vm_, object );
}

unsigned int
NativeAPI::GetVMReferenceCount( HSQOBJECT* object )
{
  return sq_getvmrefcount( vm_, object );
}

void
NativeAPI::ResetObject( HSQOBJECT* object )
{
  sq_resetobject( object );
}


std::string
NativeAPI::ObjectToString( HSQOBJECT* object )
{
  return sq_objtostring( object );
}


bool
NativeAPI::ObjectToBoolean( HSQOBJECT* object )
{
  return SQBOOL_TO_BOOLEAN( sq_objtobool( object ) );
}

int
NativeAPI::ObjectToInteger( HSQOBJECT* object )
{
  return sq_objtointeger( object );
}

float
NativeAPI::ObjectToFloat( HSQOBJECT* object )
{
  return sq_objtofloat( object );
}

SQUserPointer
NativeAPI::ObjectToUserPointer( HSQOBJECT* object )
{
  return sq_objtouserpointer( object );
}

SQUserPointer
NativeAPI::GetObjectTypeTag( HSQOBJECT* object )
{
  SQUserPointer type_tag;
  if ( SQ_FAILED( sq_getobjtypetag( object, &type_tag ) ) ) {
    throw NativeAPIException(
      FUNC_NAME_OF( sq_getobjtypetag ), "object is not instance or user data or class" );
  }
  return type_tag;
}

// -- memory allocation --------------------------------------------------
void*
NativeAPI::Malloc( unsigned int size )
{
  return sq_malloc( size );
}

void*
NativeAPI::Realloc( void* pointer, unsigned int old_size, unsigned int new_size )
{
  return sq_realloc( pointer, old_size, new_size );
}

void
NativeAPI::Free( void* pointer, unsigned int size )
{
  sq_free( pointer, size );
}

// -- Debug interface ----------------------------------------------------
void
NativeAPI::SetDebugHook( void )
{
  sq_setdebughook( vm_ );
}

void
NativeAPI::SetNativeDebugHook( SQDEBUGHOOK hook )
{
  sq_setnativedebughook( vm_, hook );
}

void
NativeAPI::GetFunctionInfo( int index, SQFunctionInfo* fi )
{
  RESULT_TEST( sq_getfunctioninfo, vm_, index, fi );
}

void
NativeAPI::StackInformations( int level, SQStackInfos* stack_info )
{
  RESULT_TEST( sq_stackinfos, vm_, level, stack_info );
}


// -- Standard -----------------------------------------------------------
NativeAPI::Standard::Standard( HSQUIRRELVM& vm ) :
vm_( vm )
{
}


// -- aux -----
#include "sqstdaux.h"

void
NativeAPI::Standard::SetErrorHandlers( void )
{
  sqstd_seterrorhandlers( vm_ );
}

void
NativeAPI::Standard::PrintCallStack( void )
{
  sqstd_printcallstack( vm_ );
}


// -- blob -----
#include "sqstdblob.h"

SQUserPointer
NativeAPI::Standard::CreateBlob( int size )
{
  return sqstd_createblob( vm_, size );
}

SQUserPointer
NativeAPI::Standard::GetBlob( int index )
{
  SQUserPointer tmp;
  NativeAPI( vm_ ).RESULT_TEST( sqstd_getblob, vm_, index, &tmp );
  return tmp;
}

int
NativeAPI::Standard::GetBlobSize( int index )
{
  return sqstd_getblobsize( vm_, index );
}

void
NativeAPI::Standard::RegisterBlobLibrary( void )
{
  NativeAPI( vm_ ).RESULT_TEST( sqstd_register_bloblib, vm_ );
}


// -- IO -----
#include "sqstdio.h"

SQFILE
NativeAPI::Standard::FOpen( const std::string& filename, const std::string& mode )
{
  return sqstd_fopen( filename.c_str(), mode.c_str() );
}

int
NativeAPI::Standard::FRead( SQUserPointer buffer, int size, int count, SQFILE file )
{
  return sqstd_fread( buffer, size, count, file );
}

int
NativeAPI::Standard::FWrite( SQUserPointer buffer, int size, int count, SQFILE file )
{
  return sqstd_fwrite( buffer, size, count, file );
}

int
NativeAPI::Standard::FSeek( SQFILE file, int offset, int origin )
{
  return sqstd_fseek( file, offset, origin );
}

int
NativeAPI::Standard::FTell( SQFILE file )
{
  return sqstd_ftell( file );
}

int
NativeAPI::Standard::FFlush( SQFILE file )
{
  return sqstd_fflush( file );
}

int
NativeAPI::Standard::Fclose( SQFILE file )
{
  return sqstd_fclose( file );
}

int
NativeAPI::Standard::FEof( SQFILE file )
{
  return sqstd_feof( file );
}


void
NativeAPI::Standard::CreateFile( SQFILE file, bool own )
{
  NativeAPI( vm_ ).RESULT_TEST( sqstd_createfile, vm_, file, own );
}

SQFILE
NativeAPI::Standard::GetFile( int index )
{
  SQFILE tmp;
  NativeAPI( vm_ ).RESULT_TEST( sqstd_getfile, vm_, index, &tmp );
  return tmp;
}

void
NativeAPI::Standard::LoadFile( const std::string& filename, bool printerror )
{
  NativeAPI( vm_ ).RESULT_TEST( sqstd_loadfile, vm_, filename.c_str(), printerror );
}

void
NativeAPI::Standard::DoFile( const std::string& filename, bool retval, bool printerror )
{
  NativeAPI( vm_ ).RESULT_TEST( sqstd_dofile, vm_, filename.c_str(), retval, printerror );
}

void
NativeAPI::Standard::WriteClosureToFile( const std::string& filename )
{
  NativeAPI( vm_ ).RESULT_TEST( sqstd_writeclosuretofile, vm_, filename.c_str() );
}

void
NativeAPI::Standard::RegisterIOLibrary( void )
{
  NativeAPI( vm_ ).RESULT_TEST( sqstd_register_iolib, vm_ );
}


// -- math -----
#include "sqstdmath.h"

void
NativeAPI::Standard::RegisterMathLibrary( void )
{
  NativeAPI( vm_ ).RESULT_TEST( sqstd_register_mathlib, vm_ );
}

// -- regular expression -----
#include "sqstdstring.h"

SQRex*
NativeAPI::Standard::RegexCompile( const std::string& pattern, std::string error )
{
  const char* tmp;
  SQRex* rex;
  rex = sqstd_rex_compile( pattern.c_str(), &tmp );
  error = tmp;
  return rex;
}

void
NativeAPI::Standard::RegexFree( SQRex* exp )
{
  sqstd_rex_free( exp );
}

bool
NativeAPI::Standard::RegexMatch( SQRex* exp, const std::string& text )
{
  return SQBOOL_TO_BOOLEAN( sqstd_rex_match( exp, text.c_str() ) );
}

bool
NativeAPI::Standard::RegexSearch( SQRex* exp, const char* text, const char** out_begin, const char** out_end )
{
  return SQBOOL_TO_BOOLEAN( sqstd_rex_search( exp, text, out_begin, out_end ) );
}

bool
NativeAPI::Standard::RegexSearchRange( SQRex* exp, const char* text_begin, const char* text_end, const char** out_begin, const char** out_end )
{
  return SQBOOL_TO_BOOLEAN( sqstd_rex_searchrange( exp, text_begin, text_end, out_begin, out_end ) );
}

int
NativeAPI::Standard::RegexGetSubExpressionCount( SQRex* exp )
{
  return sqstd_rex_getsubexpcount( exp );
}

bool
NativeAPI::Standard::RegexGetSubExpression( SQRex* exp, int n, SQRexMatch* subexp )
{
  return SQBOOL_TO_BOOLEAN( sqstd_rex_getsubexp( exp, n, subexp ) );
}


// -- string -----
void
NativeAPI::Standard::Format( int format_string_index, int* out_string_length, char** output )
{
  NativeAPI( vm_ ).RESULT_TEST( sqstd_format, vm_, format_string_index, out_string_length, output );
}

void
NativeAPI::Standard::RegisterStringLibrary( void )
{
  NativeAPI( vm_ ).RESULT_TEST( sqstd_register_stringlib, vm_ );
}

// -- system -----
#include "sqstdsystem.h"

int
NativeAPI::Standard::RegisterSystemLibrary( void )
{
  return sqstd_register_systemlib( vm_ );
}


NativeAPI::Standard
NativeAPI::Std( void )
{
  return Standard( vm_ );
}


void
NativeAPI::ResultTest( SQRESULT result, const char* function_name )
{
  if ( SQ_FAILED( result ) ) {
    this->GetLastError();
    std::string reason = this->GetAsString( -1 );
    this->PopTop();
    throw NativeAPIException( function_name, reason );
  }
}


HSQUIRRELVM
NativeAPI::GetHandle( void )
{
  return vm_;
}
