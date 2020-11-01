// tt_squirrel_object.cpp

#include "ttl_define.h"

#include "tt_squirrel_virtual_machine.h"

#include "tt_squirrel_object.h"

using namespace TtSquirrel;


// -- Object -------------------------------------------------------------
Object::Object( HSQOBJECT object, VirtualMachine& vm, bool auto_reference ) :
object_( object ),
vm_( &vm ),
auto_reference_( auto_reference )
{
  if ( vm_ ) {
    vm_->RegisterExternallyObject( this );
  }
  if ( auto_reference_ ) {
    this->AddReference();
  }
}

Object::Object( const Object& other ) :
Object( other.object_, *other.vm_, other.auto_reference_ )
{
}


Object&
Object::operator =( const Object& other )
{
  if ( auto_reference_ ) {
    this->Release();
  }
  this->object_         = other.object_;
  this->vm_             = other.vm_;
  this->auto_reference_ = other.auto_reference_;
  if ( auto_reference_ ) {
    this->AddReference();
  }
  return *this;
}


Object::~Object()
{
  if ( auto_reference_ ) {
    this->Release();
  }
  if ( vm_ ) {
    vm_->UnregisterExternallyObject( this );
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
  if ( vm_ ) {
    vm_->Native().AddReference( &object_ );
  }
}

void
Object::Release( void )
{
  if ( vm_ ) {
    vm_->Native().Release( &object_ );
  }
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
