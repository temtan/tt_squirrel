// tt_squirrel_virtual_machine.h

#pragma once

#include <functional>
#include <vector>
#include <unordered_map>
#include <memory>

#include "squirrel.h"

#include "tt_enum.h"
#include "tt_window.h"

#include "tt_squirrel_exception.h"
#include "tt_squirrel_native_api.h"
#include "tt_squirrel_object.h"
#include "tt_squirrel_utility.h"


namespace TtSquirrel {
  class VirtualMachine;

  // -- StackRecoverer ---------------------------------------------------
  class StackRecoverer {
  public:
    explicit StackRecoverer( VirtualMachine* vm, unsigned int leave = 0 );
    ~StackRecoverer();

  private:
    VirtualMachine* vm_;
    int             top_;
  };

  // -- VirtualMachine ---------------------------------------------------
  class VirtualMachine {
  public:
    using CompilerErrorHandler = std::function<void ( const std::string& reason, const std::string& source, int line, int column )>;

  private:
    static std::unordered_map<HSQUIRRELVM, VirtualMachine*> HANDLE_TABLE;
    static void FunctionForCompilerError( HSQUIRRELVM vm, const SQChar* reason, const SQChar* source, SQInteger line, SQInteger column );

  public:
    static VirtualMachine& GetVirtualMachineByHandle( HSQUIRRELVM vm );

  public:
    explicit VirtualMachine( int stack_size = 1024 );
    virtual ~VirtualMachine();

    explicit VirtualMachine( const VirtualMachine& ) = delete;
    VirtualMachine& operator =( const VirtualMachine& ) = delete;

    HSQUIRRELVM GetHandle( void );

    void RegisterExternallyObject( Object* object );
    void UnregisterExternallyObject( Object* object );

    NativeAPI Native( void );

    TtWindow* GetParentWindow( void );
    void SetParentWindow( TtWindow* parent );

    void RegisterStandardLibrariesAndAllAdditionalLibraries( void );
    void RegisterStandardLibraries( void );
    void RegisterAdditionalLibrariesUtility( void );
    void RegisterAdditionalLibrariesMessageBox( void );

    void RegisterDefaultPrintErrorFunction( void );

    // ----------
    using Operation                      = std::function<void ( void )>;
    using Closure                        = std::function<int ( VirtualMachine& vm )>;
    using ParametersOperation            = std::function<unsigned int ( void )>;
    using ParametersOperationPassedTable = std::function<unsigned int ( TtSquirrel::Object table )>;

    void RemainTop( Operation operation );

    template <class TYPE>
    TYPE* GetAsPointerOf( int index ) {
      return reinterpret_cast<TYPE*>( Native().GetAsUserPointer( index ) );
    }
    template <class TYPE>
    void PushAsUserPointer( TYPE* pointer ) {
      this->Native().PushUserPointer( (SQUserPointer)pointer );
    }
    template <class TYPE>
    TYPE* GetInstanceUserPointerAs( int index ) {
      return reinterpret_cast<TYPE*>( Native().GetInstanceUserPointer( index, TypeTag::Create<TYPE>() ) );
    }

    void PushObject( Object& object );

    // only -> int, float, double, const char*, std::string, bool, SQUserPointer
    template <class TYPE>
    void PushAuto( TYPE value );

    void NewSlot( Operation table, Operation key, Operation value, bool is_static = false );
    void NewSlotOfRootTable( Operation key, Operation value );
    void NewSlotOfRootTableByString( const std::string& key, Operation value );
    void NewSlotOfTop( Operation key, Operation value, bool is_static = false );
    void NewSlotOfTopByString( const std::string& key, Operation value, bool is_static = false );
    void NewSlotOfTopByStringAsStatic( const std::string& key, Operation value );
    void NewStringSlotOfTopByString( const std::string& key, const std::string& value, bool is_static = false );
    void NewIntegerSlotOfTopByString( const std::string& key, int value, bool is_static = false );
    void NewFloatSlotOfTopByString( const std::string& key, float value, bool is_static = false );
    void NewBooleanSlotOfTopByString( const std::string& key, bool value, bool is_static = false );
    void NewNullSlotOfTopByString( const std::string& key, bool is_static = false );

    void Set( Operation table, Operation key, Operation value );
    void SetToTop( Operation key, Operation value );
    void SetToTopByString( const std::string& key, Operation value );
    void SetStringToTopByString( const std::string& key, const std::string& value );
    void SetIntegerToTopByString( const std::string& key, int value );
    void SetFloatToTopByString( const std::string& key, float value );
    void SetBooleanToTopByString( const std::string& key, bool value );

    void Get( Operation table, Operation key );
    void GetByString( Operation table, const std::string& key );
    void GetByInteger( Operation table, int key );
    void GetByStringFrom( int index, const std::string& key );
    void GetByIntegerFrom( int index, int key );
    void GetByStringFromTop( const std::string& key );
    void GetByIntegerFromTop( int key );
    void GetFromRootTable( Operation key );
    void GetByStringFromRootTable( const std::string& key );
    void GetByStringFromRootTableJoined( std::vector<std::string> v );

    // only -> int, float, std::string, bool, SQUserPointer
    template <class TYPE>
    TYPE GetAsFrom( int index );

    template <class TYPE>
    TYPE GetAsFromTop( void ) {
      return this->GetAsFrom<TYPE>( Const::StackTop );
    }

    template <class TYPE>
    TYPE GetByStringFromTopAndGetAs( const std::string& key ) {
      StackRecoverer recoverer( this );
      this->GetByStringFromTop( key );
      return this->GetAsFromTop<TYPE>();
    }
    template <class TYPE>
    TYPE GetByIntegerFromTopAndGetAs( int key ) {
      StackRecoverer recoverer( this );
      this->GetByIntegerFromTop( key );
      return this->GetAsFromTop<TYPE>();
    }
    std::string ToStringFromTopAndGetAsString( void );

    bool InstanceOf( Operation klass, Operation instance );
    bool InstanceAtTopOf( Operation klass );

    // StackTop = value ; Stacktop -1 = key(index)
    using ForeachProcess = std::function<void ( VirtualMachine& vm )>;
    void Foreach( Operation target, ForeachProcess process );

    void Call( Operation clouser, ParametersOperation parameters, bool push_return_value );
    void CallAndPushReturnValue( Operation clouser, ParametersOperation parameters );
    void CallAndNoReturnValue( Operation clouser, ParametersOperation parameters );
    void CallObjectOfGetByStringFromTop( const std::string& key, ParametersOperationPassedTable parameters, bool push_return_value );
    void CallObjectOfGetByStringFromTopAndPushReturnValue( const std::string& key, ParametersOperationPassedTable parameters );
    void CallObjectOfGetByStringFromTopAndNoReturnValue( const std::string& key, ParametersOperationPassedTable parameters );

    void CallPrint( const std::string& str );
    void CallPuts( const std::string& str );

    void NewClosure( Closure closure, const std::string& name = "" );

    template <class TYPE>
    void NewTableOfEnum( void ) {
      this->Native().NewTable();
      for ( auto& name : TtEnum<TYPE>::GetNames() ) {
        this->NewIntegerSlotOfTopByString( name, static_cast<int>( TtEnum<TYPE>::Parse( name ) ) );
      }
    }

    template <class TYPE>
    void NewArrayOf( const std::vector<TYPE>& v, std::function<void ( const typename std::vector<TYPE>::value_type& one )> function ) {
      this->Native().NewArray( 0 );
      for ( const TYPE& tmp : v ) {
        this->AppendToArrayOfStackTop( [&] ( void ) { function( tmp ); } );
      }
    }

    void AppendToArray( Operation array, Operation element );
    void AppendToArrayOf( int index, Operation element );
    void AppendToArrayOfStackTop( Operation element );

    using EachProcess = std::function<void ( unsigned int index )>;
    void ArrayEach( Operation array, EachProcess process );

    void CompileBuffer( const std::string& buffer, const std::string& buffer_name = "" );
    void DoBufferAt( Operation          table,
                     const std::string& buffer,
                     bool               push_return_value = false,
                     const              std::string& buffer_name = "" );
    void DoBufferAtTop( const std::string& buffer,
                        bool               push_return_value = false,
                        const              std::string& buffer_name = "" );
    void DoBufferAtRoot( const std::string& buffer,
                         bool               push_return_value = false,
                         const std::string& buffer_name = "" );
    void DoFile( const std::string& filename, bool push_return_value = false );

    ObjectType GetTopType( void );

    Object GetNullObject( bool auto_reference = true );
    Object GetStackObject( int index, bool auto_reference = true );
    Object GetStackTopObject( bool auto_reference = true );

    std::vector<StackInformation> GetCallStack( void );

    void RegisterCompilerErrorHandler( CompilerErrorHandler handler );
    void RegisterErrorHandler( Closure closure );

    template <class TYPE>
    void CreateObject( const TYPE& value ) {
      StackRecoverer recoverer( this, 1 );
      SQUserPointer tmp = Native().NewUserData( sizeof( TYPE* ) );
      *static_cast<TYPE**>( tmp ) = new TYPE( value );
      SQRELEASEHOOK hook = [] ( SQUserPointer p, SQInteger ) -> SQInteger {
        delete *static_cast<TYPE**>( p );
        return 0;
      };
      Native().SetReleaseHook( Const::StackTop, hook );
    }

  private:
    void ThrowIfErrorOccurred( void );

  private:
    HSQUIRRELVM vm_;

    TtWindow* parent_window_;

    std::shared_ptr<CompilerException> last_compiler_error_;
    std::shared_ptr<RuntimeException>  last_runtime_error_;

    CompilerErrorHandler compiler_error_handler_;

    std::vector<TtSquirrel::Object*> externally_objects_;
  };
}
