// tt_squirrel_object.h

#pragma once

#include "squirrel.h"

#include "tt_squirrel_object_type.h"


namespace TtSquirrel {
  class VirtualMachine;

  // -- Object -----------------------------------------------------------
  class Object {
  public:
    class Null;
    class Integer;
    class Float;
    class Boolean;
    class String;
    class Table;
    class Array;
    class UserData;
    class Closure;
    class NativeClosure;
    class Generator;
    class UserPointer;
    class Thread;
    class FuncProto;
    class Class;
    class Instance;
    class WeakReferrence;
    class Outer;

  public:
    explicit Object( HSQOBJECT object, VirtualMachine& vm, bool need_release = true );
    Object( const Object& other );

    virtual Object& operator =( const Object& other );

    virtual ~Object();

    HSQOBJECT       GetHandle( void );
    VirtualMachine& GetVirtualMachine( void );

    bool operator ==( const Object& other );

    ObjectType GetType( void ) const;

    void AddReference( void );
    void Release( void );

    std::string   ToString( void );
    bool          ToBoolean( void );
    int           ToInteger( void );
    float         ToFloat( void );
    SQUserPointer ToUserPointer( void );

  protected:
    HSQOBJECT       object_;
    VirtualMachine* vm_;
    bool            need_release_;
  };

  // -- Object::Null -----------------------------------------------------
  class Object::Null : public Object {
  };

  // -- Object::Integer --------------------------------------------------
  class Object::Integer : public Object {
  };

  // -- Object::Float ----------------------------------------------------
  class Object::Float : public Object {
  };

  // -- Object::Boolean --------------------------------------------------
  class Object::Boolean : public Object {
  };

  // -- Object::String ---------------------------------------------------
  class Object::String : public Object {
  };

  // -- Object::Table ----------------------------------------------------
  class Object::Table : public Object {
  };

  // -- Object::Array ----------------------------------------------------
  class Object::Array : public Object {
  };

  // -- Object::UserData -------------------------------------------------
  class Object::UserData : public Object {
  };

  // -- Object::Closure --------------------------------------------------
  class Object::Closure : public Object {
  };

  // -- Object::NativeClosure --------------------------------------------
  class Object::NativeClosure : public Object {
  };

  // -- Object::Generator ------------------------------------------------
  class Object::Generator : public Object {
  };

  // -- Object::UserPointer ----------------------------------------------
  class Object::UserPointer : public Object {
  };

  // -- Object::Thread ---------------------------------------------------
  class Object::Thread : public Object {
  };

  // -- Object::FuncProto ------------------------------------------------
  class Object::FuncProto : public Object {
  };

  // -- Object::Class ----------------------------------------------------
  class Object::Class : public Object {
  };

  // -- Object::Instance -------------------------------------------------
  class Object::Instance : public Object {
  };

  // -- Object::WeakReferrence -------------------------------------------
  class Object::WeakReferrence : public Object {
  };

  // -- Object::Outer ----------------------------------------------------
  class Object::Outer : public Object {
  };
}
