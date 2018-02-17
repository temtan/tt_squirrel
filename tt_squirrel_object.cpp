// tt_squirrel_object.cpp

#include "ttl_define.h"

#include "tt_squirrel_virtual_machine.h"

#include "tt_squirrel_object.h"

using namespace TtSquirrel;


// -- Object -------------------------------------------------------------
Object::Object( HSQOBJECT object, VirtualMachine& vm, bool need_release ) :
object_( object ),
vm_( &vm ),
need_release_( need_release )
{
}


Object::Object( const Object& other ) :
object_( other.object_ ),
vm_( other.vm_ ),
need_release_( other.need_release_ )
{
  if ( need_release_ ) {
    this->AddReference();
  }
}


Object&
Object::operator =( const Object& other )
{
  if ( need_release_ ) {
    this->Release();
  }
  object_ = other.object_;
  vm_ = other.vm_;
  need_release_ = other.need_release_;
  if ( need_release_ ) {
    this->AddReference();
  }
  return *this;
}


Object::~Object()
{
  if ( need_release_ ) {
    this->Release();
  }
}


HSQOBJECT
Object::GetHandle( void )
{
  return object_;
}

VirtualMachine&
Object::GetVirtualMachine( void )
{
  return *vm_;
}


bool
Object::operator ==( const Object& other )
{
  return (this->object_._type == other.object_._type) && (this->object_._unVal.raw == other.object_._unVal.raw);
}


ObjectType
Object::GetType( void ) const
{
  return object_._type;
}


void
Object::AddReference( void )
{
  vm_->Native().AddReference( &object_ );
}

void
Object::Release( void )
{
  vm_->Native().Release( &object_ );
}


std::string
Object::ToString( void )
{
  return NativeAPI::ObjectToString( &object_ );
}

bool
Object::ToBoolean( void )
{
  return NativeAPI::ObjectToBoolean( &object_ );
}

int
Object::ToInteger( void )
{
  return NativeAPI::ObjectToInteger( &object_ );
}

float
Object::ToFloat( void )
{
  return NativeAPI::ObjectToFloat( &object_ );
}

SQUserPointer
Object::ToUserPointer( void )
{
  return NativeAPI::ObjectToUserPointer( &object_ );
}


// -- Object::Null -------------------------------------------------------
// -- Object::Integer ----------------------------------------------------
// -- Object::Float ------------------------------------------------------
// -- Object::Boolean ----------------------------------------------------
// -- Object::String -----------------------------------------------------
// -- Object::Table ------------------------------------------------------
// -- Object::Array ------------------------------------------------------
// -- Object::UserData ---------------------------------------------------
// -- Object::Closure ----------------------------------------------------
// -- Object::NativeClosure ----------------------------------------------
// -- Object::Generator --------------------------------------------------
// -- Object::UserPointer ------------------------------------------------
// -- Object::Thread -----------------------------------------------------
// -- Object::FuncProto --------------------------------------------------
// -- Object::Class ------------------------------------------------------
// -- Object::Instance ---------------------------------------------------
// -- Object::WeakReferrence ---------------------------------------------
