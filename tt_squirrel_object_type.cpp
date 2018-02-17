// tt_squirrel_object_type.cpp

#include "tt_squirrel_object_type.h"

using namespace TtSquirrel;


// -- ObjectType ---------------------------------------------------------
ObjectType ObjectType::Null(           OT_NULL );
ObjectType ObjectType::Integer(        OT_INTEGER );
ObjectType ObjectType::Float(          OT_FLOAT );
ObjectType ObjectType::Boolean(        OT_BOOL );
ObjectType ObjectType::String(         OT_STRING );
ObjectType ObjectType::Table(          OT_TABLE );
ObjectType ObjectType::Array(          OT_ARRAY );
ObjectType ObjectType::UserData(       OT_USERDATA );
ObjectType ObjectType::Closure(        OT_CLOSURE );
ObjectType ObjectType::NativeClosure(  OT_NATIVECLOSURE );
ObjectType ObjectType::Generator(      OT_GENERATOR );
ObjectType ObjectType::UserPointer(    OT_USERPOINTER );
ObjectType ObjectType::Thread(         OT_THREAD );
ObjectType ObjectType::FuncProto(      OT_FUNCPROTO );
ObjectType ObjectType::Class(          OT_CLASS );
ObjectType ObjectType::Instance(       OT_INSTANCE );
ObjectType ObjectType::WeakReferrence( OT_WEAKREF );
ObjectType ObjectType::Outer(          OT_OUTER );


ObjectType::ObjectType( SQObjectType object_type ) :
ot_( object_type )
{
}

SQObjectType
ObjectType::ToValue( void ) const
{
  return ot_;
}

ObjectType::operator int( void ) const
{
  return ot_;
}


std::string
ObjectType::ToString( void ) const
{
  switch ( ot_ ) {
  case OT_NULL:          return "Null";
  case OT_INTEGER:       return "Integer";
  case OT_FLOAT:         return "Float";
  case OT_BOOL:          return "Boolean";
  case OT_STRING:        return "String";
  case OT_TABLE:         return "Table";
  case OT_ARRAY:         return "Array";
  case OT_USERDATA:      return "UserData";
  case OT_CLOSURE:       return "Closure";
  case OT_NATIVECLOSURE: return "NativeClosure";
  case OT_GENERATOR:     return "Generator";
  case OT_USERPOINTER:   return "UserPointer";
  case OT_THREAD:        return "Thread";
  case OT_FUNCPROTO:     return "FuncProto";
  case OT_CLASS:         return "Class";
  case OT_INSTANCE:      return "Instance";
  case OT_WEAKREF:       return "WeakReferrence";
  case OT_OUTER:         return "Outer";

  default:
    return "Unknown";
  }
}




bool
ObjectType::IsMakeableSlot( void ) const
{
  switch ( ot_ ) {
  case OT_TABLE:
  case OT_CLASS:
  case OT_INSTANCE:
    return true;
  default:
    return false;
  }
}

bool
ObjectType::IsSettable( void ) const
{
  switch ( ot_ ) {
  case OT_TABLE:
  case OT_USERDATA:
  case OT_INSTANCE:
  case OT_ARRAY:
    return true;
  default:
    return false;
  }
}

bool
ObjectType::IsCallable( void ) const
{
  switch ( ot_ ) {
  case OT_CLOSURE:
  case OT_NATIVECLOSURE:
  case OT_CLASS:
    return true;

  default:
    return false;
  }
}
