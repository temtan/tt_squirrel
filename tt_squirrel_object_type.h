// tt_squirrel_object_type.h

#pragma once

#include <string>

#include "squirrel.h"


namespace TtSquirrel {
  // -- ObjectType -------------------------------------------------------
  class ObjectType {
  public:
    static ObjectType Null;
    static ObjectType Integer;
    static ObjectType Float;
    static ObjectType Boolean;
    static ObjectType String;
    static ObjectType Table;
    static ObjectType Array;
    static ObjectType UserData;
    static ObjectType Closure;
    static ObjectType NativeClosure;
    static ObjectType Generator;
    static ObjectType UserPointer;
    static ObjectType Thread;
    static ObjectType FuncProto;
    static ObjectType Class;
    static ObjectType Instance;
    static ObjectType WeakReferrence;
    static ObjectType Outer;

  public:
    ObjectType( SQObjectType object_type );

    SQObjectType ToValue( void ) const;
    operator int( void ) const;
    std::string ToString( void ) const;

    bool IsMakeableSlot( void ) const;
    bool IsSettable( void ) const;
    bool IsCallable( void ) const;

    friend bool operator ==( ObjectType& l, ObjectType& r ) { return l.ot_ == r.ot_; }
    friend bool operator !=( ObjectType& l, ObjectType& r ) { return l.ot_ != r.ot_; }

  private:
    SQObjectType ot_;
  };
}
