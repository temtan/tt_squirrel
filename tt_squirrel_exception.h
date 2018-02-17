// tt_squirrel_exception.h

#pragma once

#include <string>
#include <vector>

#include "tt_exception.h"

#include "tt_squirrel_object_type.h"
#include "tt_squirrel_utility.h"


namespace TtSquirrel {
  class VirtualMachine;

  // ëΩèdåpè≥óp
  class WithString {
  public:
    explicit WithString( const std::string& str ) : str_( str ) {}
    const std::string& GetString( void ) const { return str_; }
  private:
    std::string str_;
  };

  // -- Exception --------------------------------------------------------
  class Exception : public TtException {
  public:
    virtual std::string GetStandardMessage( void ) const;
  };

  // -- InternalException ------------------------------------------------
  class InternalException : public Exception {
  public:
    explicit InternalException( const char* file, unsigned int line );

    const char*  GetFile( void ) const;
    unsigned int GetLine( void ) const;

    virtual std::string GetStandardMessage( void ) const override;
    virtual std::string Dump( void ) const override;

  private:
    const char*  file_;
    unsigned int line_;
  };
#define TT_SQUIRREL_INTERNAL_EXCEPTION TtSquirrel::InternalException( __FILE__, __LINE__ )

  // -- NativeAPIException -----------------------------------------------
  class NativeAPIException : public Exception {
  public:
    explicit NativeAPIException( const std::string& function_name, const std::string& reason );

    const std::string& GetFunctionName( void );
    const std::string& GetReason( void );

    virtual std::string GetStandardMessage( void ) const override;
    virtual std::string Dump( void ) const override;

  private:
    std::string function_name_;
    std::string reason_;
  };

  // -- ObjectTypeException ----------------------------------------------
  class ObjectTypeException : public Exception {
  public:
    explicit ObjectTypeException( ObjectType actual, ObjectType require );

    ObjectType GetActualType( void ) const;
    ObjectType GetRequireType( void ) const;

    virtual std::string GetStandardMessage( void ) const override;
    virtual std::string Dump( void ) const override;

  private:
    ObjectType actual_;
    ObjectType require_;
  };

  // -- ObjectHasNotSlotException ----------------------------------------
  class ObjectHasNotSlotException : public Exception {
  public:
    explicit ObjectHasNotSlotException( ObjectType actual );

    ObjectType GetActualType( void ) const;

    virtual std::string GetStandardMessage( void ) const override;
    virtual std::string Dump( void ) const override;

  private:
    ObjectType actual_;
  };

  // -- GetOperationException --------------------------------------------
  class GetOperationException : public Exception,
                                private WithString {
  public:
    explicit GetOperationException( const std::string& key_info );

    const std::string& GetKeyInfo( void ) const;

    virtual std::string GetStandardMessage( void ) const override;
    virtual std::string Dump( void ) const override;
  };

  // -- SetOperationException --------------------------------------------
  class SetOperationException : public Exception,
                                private WithString {
  public:
    explicit SetOperationException( const std::string& key_info );

    const std::string& GetKeyInfo( void ) const;

    virtual std::string GetStandardMessage( void ) const override;
    virtual std::string Dump( void ) const override;
  };

  // -- FileExecutionException -------------------------------------------
  class FileExecutionException : public Exception,
                                 private WithString {
  public:
    explicit FileExecutionException( const std::string& filename );

    const std::string& GetFileName( void ) const;

    virtual std::string GetStandardMessage( void ) const override;
    virtual std::string Dump( void ) const override;
  };

  // -- CompilerException ------------------------------------------------
  class CompilerException : public Exception {
  public:
    explicit CompilerException( const std::string& reason, const std::string& source, int line, int column );

    const std::string& GetReason( void ) const;
    const std::string& GetSource( void ) const;
    int                GetLine( void ) const;
    int                GetColumn( void ) const;

    std::string        GetInfoString( void ) const;

    virtual std::string GetStandardMessage( void ) const override;
    virtual std::string Dump( void ) const override;

  private:
    std::string reason_;
    std::string source_;
    int         line_;
    int         column_;
  };

  // -- RuntimeException -------------------------------------------------
  class RuntimeException : public Exception {
  public:
    explicit RuntimeException( const std::string& thrown, std::vector<StackInformation>& call_stack );
    explicit RuntimeException( VirtualMachine& vm );

    const std::string& GetThrown( void ) const;
    const std::vector<StackInformation> GetCallStack( void ) const;

    std::string GetInfoString( void ) const;

    virtual std::string GetStandardMessage( void ) const override;
    virtual std::string Dump( void ) const override;

  private:
    std::string thrown_;
    std::vector<StackInformation> call_stack_;
  };
}
