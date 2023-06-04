// tt_squirrel_native_api.h

#pragma once

#include <string>

#include "squirrel.h"
#include "sqstdio.h"
#include "sqstdstring.h"

#include "tt_squirrel_object_type.h"


namespace TtSquirrel {
  // -- NativeAPI --------------------------------------------------------
  class NativeAPI {
  public:
    explicit NativeAPI( HSQUIRRELVM& vm );
  public:
    // -- Virtual Machine ------------------------------------------------
    static HSQUIRRELVM Open( int stack_size );
    void               Close( void );

    void            SetErrorHandler( void );

    SQPRINTFUNCTION GetPrintFunction( void );
    void            SetPrintFunction( SQPRINTFUNCTION print_function, SQPRINTFUNCTION error_function );
    SQPRINTFUNCTION GetErrorFunction( void );

    SQUserPointer   GetForeignPointer( void );
    void            SetForeignPointer( SQUserPointer pointer );

    HSQUIRRELVM     NewThread( int initial_stack_size );

    void            PushRootTable( void );
    void            PushConstTable( void );
    void            PushRegistryTable( void );
    void            SetRootTable( void );
    void            SetConstTable( void );

    void            SuspendVM( void );
    void            WakeupVM( bool resumedret, bool retval, bool raiseerror, bool throwerror );

    int             GetVMState( void );
    void            Move( HSQUIRRELVM destination, int index );
    void            MoveObjectBetweenVirtualMachine( HSQUIRRELVM destination, int index ) { this->Move( destination, index ); }

    SQUserPointer   GetSharedForeignPointer( void );
    void            SetSharedForeignPointer( SQUserPointer pointer );

    SQRELEASEHOOK   GetSharedReleaseHook( void );
    void            SetSharedReleaseHook( SQRELEASEHOOK hook );

    SQRELEASEHOOK   GetVMReleaseHook( void );
    void            SetVMReleaseHook( SQRELEASEHOOK hook );

    int             CollectGarbage( void );
    void            ResurrectUnreachable( void );

    int             GetVersion( void );

    // -- Compiler -------------------------------------------------------
    void Compile( SQLEXREADFUNC read, SQUserPointer pointer, const std::string& source_name, bool raise_error );
    void CompileBuffer( const std::string& s, int size, const std::string& source_name, bool raise_error );
    void EnableDebugInfo( bool enable );
    void NotifyAllExceptions( bool enable );
    void SetCompilerErrorHandler( SQCOMPILERERROR f );

    // -- Stack Operations -----------------------------------------------
    int  Cmp( void );
    int  Compare( void ) { return this->Cmp(); }
    int  GetTop( void );
    int  GetTopIndex( void ) { return this->GetTop(); }
    void SetTop( int top_index );
    void ResizeStackSize( int top_index ) { this->SetTop( top_index ); }
    void Pop( int count );
    void PopTop( void );
    void Push( int index );
    void Remove( int index );
    void ReserveStack( int size );

    // -- Object creation and handling -----------------------------------
    void   BindEnv( int index );
    void   BindEnvironment( int index ) { this->BindEnv( index ); }
    void   CreateInstance( int index );

    void   GetClosureInfo( int index, unsigned int* nparams, unsigned int* nfreevars );
    void   GetClosureName( int index );
    void   GetClosureRoot( int index );
    void   SetClosureRoot( int index );

    int    GetSize( int index );
    SQHash GetHash( int index );

    int            GetAsInteger( int index );
    float          GetAsFloat( int index );
    std::string    GetAsString( int index );
    bool           GetAsBoolean( int index );
    SQUserPointer  GetAsUserPointer( int index );
    HSQUIRRELVM    GetAsThread( int index );
    std::pair<SQUserPointer, SQUserPointer> GetAsUserData( int index );

    SQUserPointer  GetInstanceUserPointer( int index, SQUserPointer type_tag );
    void           SetInstanceUserPointer( int index, SQUserPointer pointer );

    char*         GetScratchPad( int min_size );
    ObjectType    GetType( int index );
    void          TypeOf( int index );
    SQUserPointer GetTypeTag( int index );

    void          NewClass( bool hasbase );
    void          NewClosure( SQFUNCTION func, unsigned int nfreevars );
    SQUserPointer NewUserData( unsigned int size );
    void          PushInteger( int n );
    void          PushFloat( float f );
    void          PushString( const std::string& str );
    void          PushBoolean( bool flag );
    void          PushUserPointer( SQUserPointer pointer );
    void          PushNull( void );
    void          PushThread( HSQUIRRELVM thread );

    void          SetNativeClosureName( int index, const std::string& name );
    void          SetParamsCheck( int nparamscheck, const std::string& typemask );
    SQRELEASEHOOK GetReleaseHook( int index );
    void          SetReleaseHook( int index, SQRELEASEHOOK hook );
    void          SetTypeTag( int index, SQUserPointer type_tag );

    bool          ToBoolean( int index );
    void          ToString( int index );

    void          SetClassUserDataSize( int index, int user_data_size );

    HSQMEMBERHANDLE GetMemberHandle( int index );
    void            GetByHandle( int index, const HSQMEMBERHANDLE* handle );
    void            SetByHandle( int index, const HSQMEMBERHANDLE* handle );

    // -- Calls ----------------------------------------------------------
    void              Call( int param_count, bool need_return_value, bool raise_exception_in_error = true );
    void              GetCallEE( void );
    void              GetLastError( void );
    const std::string GetLocal( unsigned int level, unsigned int index );
    void              ResetError( void );
    void              Resume( bool retval, bool raiseerror );
    SQRESULT          ThrowError( const std::string& err );
    SQRESULT          ThrowObject( void );

    // -- Objects manipulation -------------------------------------------
    void              Clone( int index );
    void              GetAttributes( int index );
    void              SetAttributes( int index );
    void              GetBase( int index );
    void              GetClass( int index );
    void              GetDefaultDelegate( ObjectType type );
    void              GetDelegate( int index );
    void              SetDelegate( int index );
    bool              InstanceOf( void );
    void              WeakReference( int index );
    void              GetWeakReferenceValue( int index );
    const std::string GetFreeVariable( unsigned int index, unsigned int nval );
    void              SetFreeVariable( int index, unsigned int nval );
    void              Clear( int index );

    // -- Table ----------------------------------------------------------
    void NewTable( void );
    void NewTableEx( int initialcapacity );
    void DeleteSlot( int index, bool push_value );
    void RawDeleteSlot( int index, bool pushval );
    void NewSlot( int index, bool is_static );
    void Get( int index );
    void Set( int index );
    void RawGet( int index );
    void RawSet( int index );
    void Next( int index );
    bool NextWithResult( int index );
    void NewMember( int index, bool is_static );
    void RawNewMember( int index, bool is_static );

    // -- Array ----------------------------------------------------------
    void NewArray( int size );
    void ArrayAppend( int index );
    void ArrayPop( int index, bool push_value );
    void ArrayResize( int index, int new_size );
    void ArrayReverse( int index );
    void ArrayRemove( int index, int item_index );
    void ArrayInsert( int index, int destination_position );

    // -- Bytecode serialization -----------------------------------------
    void WriteClosure( SQWRITEFUNC write_function, SQUserPointer user_pointer );
    void ReadClosure( SQREADFUNC read_function, SQUserPointer user_pointer );

    // -- Raw object handling --------------------------------------------
    void         PushObject( HSQOBJECT object );
    HSQOBJECT    GetStackObject( int index );
    bool         Release( HSQOBJECT* object );
    void         AddReference( HSQOBJECT* object );
    unsigned int GetReferenceCount( HSQOBJECT* object );
    unsigned int GetVMReferenceCount( HSQOBJECT* object );

    static void          ResetObject( HSQOBJECT* object );
    static std::string   ObjectToString( HSQOBJECT* object );
    static bool          ObjectToBoolean( HSQOBJECT* object );
    static int           ObjectToInteger( HSQOBJECT* object );
    static float         ObjectToFloat( HSQOBJECT* object );
    static SQUserPointer ObjectToUserPointer( HSQOBJECT* object );
    static SQUserPointer GetObjectTypeTag( HSQOBJECT* object );

    // -- memory allocation ----------------------------------------------
    static void* Malloc( unsigned int size );
    static void* Realloc( void* pointer, unsigned int old_size, unsigned int new_size );
    static void  Free( void* pointer, unsigned int size );

    // -- Debug interface ------------------------------------------------
    void SetDebugHook( void );
    void SetNativeDebugHook( SQDEBUGHOOK hook );
    void GetFunctionInfo( int index, SQFunctionInfo* fi );
    void StackInformations( int level, SQStackInfos* stack_info );

    // -- Standard -------------------------------------------------------
    class Standard {
      friend class NativeAPI;

    private:
      explicit Standard( HSQUIRRELVM& vm );

    public:
      // -- aux -----
      void SetErrorHandlers( void );
      void PrintCallStack( void );

      // -- blob -----
      SQUserPointer CreateBlob( int size );
      SQUserPointer GetBlob( int index );
      int           GetBlobSize( int index );
      void          RegisterBlobLibrary( void );

      // -- IO -----
      SQFILE FOpen( const std::string& filename, const std::string& mode );
      int    FRead( SQUserPointer buffer, int size, int count, SQFILE file );
      int    FWrite( SQUserPointer buffer, int size, int count, SQFILE file );
      int    FSeek( SQFILE file, int offset, int origin );
      int    FTell( SQFILE file );
      int    FFlush( SQFILE file );
      int    Fclose( SQFILE file );
      int    FEof( SQFILE file );
      void   CreateFile( SQFILE file, bool own );
      SQFILE GetFile( int index );
      void   LoadFile( const std::string& filename, bool printerror );
      void   DoFile( const std::string& filename, bool retval, bool printerror );
      void   WriteClosureToFile( const std::string& filename );
      void   RegisterIOLibrary( void );

      // -- math -----
      void RegisterMathLibrary( void );

      // -- regular expression -----
      SQRex* RegexCompile( const std::string& pattern, std::string error );
      void   RegexFree( SQRex* exp );
      bool   RegexMatch( SQRex* exp, const std::string& text );
      bool   RegexSearch( SQRex* exp, const char* text, const char** out_begin, const char** out_end );
      bool   RegexSearchRange( SQRex* exp, const char* text_begin, const char* text_end, const char** out_begin, const char** out_end );
      int    RegexGetSubExpressionCount( SQRex* exp );
      bool   RegexGetSubExpression( SQRex* exp, int n, SQRexMatch* subexp );

      // -- string -----
      void Format( int format_string_index, int* out_string_length, char** output );
      void RegisterStringLibrary( void );

      // -- system -----
      int RegisterSystemLibrary( void );

    private:
      HSQUIRRELVM& vm_;
    };

    Standard Std( void );

    void ResultTest( SQRESULT result, const char* function_name );

    HSQUIRRELVM GetHandle( void );

  private:
    HSQUIRRELVM& vm_;
  };
}
