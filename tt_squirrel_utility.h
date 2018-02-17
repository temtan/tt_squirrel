// tt_squirrel_utility.h

#pragma once

#include <unordered_map>
#include <vector>

#include "squirrel.h"

#include "ttl_define.h"

#include "tt_squirrel_object_type.h"


namespace TtSquirrel {
  namespace Const {
    const int StackTop  = -1;
    const int StackBase = 1;

    const SQInteger ExistReturnValue = 1;
    const SQInteger NoneReturnValue  = 0;
    const SQInteger ErrorClosure     = SQ_ERROR;
  }

  namespace Utility {
    std::string ToStringFrom( int i );

    int Pushed( int index, int pushed_count );
    int PushedFromTop( int pushed_count );

    void DefaultPrintFunction( HSQUIRRELVM vm, const SQChar* s, ... );
    void DefaultErrorFunction( HSQUIRRELVM vm, const SQChar* s, ... );

    void ObjectTypeCheck( ObjectType require, ObjectType actual );

    template <class TYPE>
    SQInteger DeleterFor( SQUserPointer pointer, SQInteger ) {
      delete reinterpret_cast<TYPE*>( pointer );
      return 0;
    }
  }

  // -- TypeTag ----------------------------------------------------------
  class TypeTag {
  private:
    static std::unordered_map<std::string, void*> TABLE;

  public:
    template <class TYPE>
    static TypeTag Create( void ) {
      return TypeTag( typeid( TYPE ) );
    };

  public:
    TypeTag( const std::type_info& type );

    void* GetTag( void ) const;
    operator void*( void );

  private:
    void* tag_;
  };

  // -- LocalInformation -------------------------------------------------
  class LocalInformation {
  public:
    explicit LocalInformation( const std::string& name, ObjectType type, const std::string& value );

  public:
    std::string name_;
    ObjectType  type_;
    std::string value_;
  };

  // -- StackInformation -------------------------------------------------
  class StackInformation {
  public:
    explicit StackInformation( SQStackInfos& stack_info );

  public:
    std::string                   function_name_;
    std::string                   source_file_path_;
    unsigned int                  source_file_line_;
    std::vector<LocalInformation> local_infos_;
  };
}
