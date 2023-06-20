// addition/message_box.cpp

#include "tt_message_box.h"

#include "tt_squirrel_virtual_machine.h"

using namespace TtSquirrel;


namespace Tag {
#define DEFINE_PARAMETER_NAME_STRING( name ) static const std::string name( #name )
  DEFINE_PARAMETER_NAME_STRING( MessageBox );
  DEFINE_PARAMETER_NAME_STRING( Icon );
  DEFINE_PARAMETER_NAME_STRING( DefaultButton );
  DEFINE_PARAMETER_NAME_STRING( Result );
  DEFINE_PARAMETER_NAME_STRING( None );
  DEFINE_PARAMETER_NAME_STRING( Exclamation );
  DEFINE_PARAMETER_NAME_STRING( Warning );
  DEFINE_PARAMETER_NAME_STRING( Information );
  DEFINE_PARAMETER_NAME_STRING( Asterisk );
  DEFINE_PARAMETER_NAME_STRING( Question );
  DEFINE_PARAMETER_NAME_STRING( Stop );
  DEFINE_PARAMETER_NAME_STRING( Error );
  DEFINE_PARAMETER_NAME_STRING( Hand );
  DEFINE_PARAMETER_NAME_STRING( First );
  DEFINE_PARAMETER_NAME_STRING( Second );
  DEFINE_PARAMETER_NAME_STRING( Third );
  DEFINE_PARAMETER_NAME_STRING( Fourth );
  DEFINE_PARAMETER_NAME_STRING( Abort );
  DEFINE_PARAMETER_NAME_STRING( Cancel );
  DEFINE_PARAMETER_NAME_STRING( Ignore );
  DEFINE_PARAMETER_NAME_STRING( No );
  DEFINE_PARAMETER_NAME_STRING( Ok );
  DEFINE_PARAMETER_NAME_STRING( Retry );
  DEFINE_PARAMETER_NAME_STRING( Yes );
  DEFINE_PARAMETER_NAME_STRING( OkCancel );
  DEFINE_PARAMETER_NAME_STRING( RetryCancel );
  DEFINE_PARAMETER_NAME_STRING( YesNo );
  DEFINE_PARAMETER_NAME_STRING( YesNoCancel );
  DEFINE_PARAMETER_NAME_STRING( AbortRetryIgnore );
  // DEFINE_PARAMETER_NAME_STRING(  );
}


// ジェネリックラムダ用
template <class T> class TypeArgumentAs {};

void
VirtualMachine::RegisterAdditionalLibrariesMessageBox( void )
{
  StackRecoverer recoverer( this );

  // -- MessageBox 実装 -----
  this->NewSlotOfRootTableByString(
    Tag::MessageBox,
    [&] () {
      TtSquirrel::StackRecoverer recoverer( this, 1 );
      this->Native().NewTable();

      // Icon 定義
      this->NewSlotOfTopByString(
        Tag::Icon,
        [&] () {
          TtSquirrel::StackRecoverer recoverer( this, 1 );
          this->Native().NewTable();
          this->NewIntegerSlotOfTopByString( Tag::None,        TtMessageBox::Icon::NONE        );
          this->NewIntegerSlotOfTopByString( Tag::Exclamation, TtMessageBox::Icon::EXCLAMATION );
          this->NewIntegerSlotOfTopByString( Tag::Warning,     TtMessageBox::Icon::WARNING );
          this->NewIntegerSlotOfTopByString( Tag::Information, TtMessageBox::Icon::INFORMATION );
          this->NewIntegerSlotOfTopByString( Tag::Asterisk,    TtMessageBox::Icon::ASTERISK    );
          this->NewIntegerSlotOfTopByString( Tag::Question,    TtMessageBox::Icon::QUESTION    );
          this->NewIntegerSlotOfTopByString( Tag::Stop,        TtMessageBox::Icon::STOP        );
          this->NewIntegerSlotOfTopByString( Tag::Error,       TtMessageBox::Icon::ERROR       );
          this->NewIntegerSlotOfTopByString( Tag::Hand,        TtMessageBox::Icon::HAND        );
        } );

      // DefaultButton 定義
      this->NewSlotOfTopByString(
        Tag::DefaultButton,
        [&] () {
          TtSquirrel::StackRecoverer recoverer( this, 1 );
          Native().NewTable();
          this->NewIntegerSlotOfTopByString( Tag::First,  TtMessageBox::DefaultButton::FIRST  );
          this->NewIntegerSlotOfTopByString( Tag::Second, TtMessageBox::DefaultButton::SECOND );
          this->NewIntegerSlotOfTopByString( Tag::Third,  TtMessageBox::DefaultButton::THIRD  );
          this->NewIntegerSlotOfTopByString( Tag::Fourth, TtMessageBox::DefaultButton::FOURTH );
        } );

      // Result 定義
      this->NewSlotOfTopByString(
        Tag::Result,
        [&] () {
          TtSquirrel::StackRecoverer recoverer( this, 1 );
          Native().NewTable();
          this->NewIntegerSlotOfTopByString( Tag::Abort,  TtMessageBox::Result::ABORT  );
          this->NewIntegerSlotOfTopByString( Tag::Cancel, TtMessageBox::Result::CANCEL );
          this->NewIntegerSlotOfTopByString( Tag::Ignore, TtMessageBox::Result::IGNORE );
          this->NewIntegerSlotOfTopByString( Tag::No,     TtMessageBox::Result::NO     );
          this->NewIntegerSlotOfTopByString( Tag::Ok,     TtMessageBox::Result::OK     );
          this->NewIntegerSlotOfTopByString( Tag::Retry,  TtMessageBox::Result::RETRY  );
          this->NewIntegerSlotOfTopByString( Tag::Yes,    TtMessageBox::Result::YES    );
        } );

      auto define_closure = [&] <class MessageBoxType> ( TypeArgumentAs<MessageBoxType>, const std::string& name ) {
        this->NewSlotOfTopByString(
          name,
          [&] () {
            this->NewClosure( [] ( VirtualMachine& vm ) -> int {
              int icon = vm.GetAsFromTop<int>();
              vm.Native().PopTop();
              std::string caption = vm.GetAsFromTop<std::string>();
              vm.Native().PopTop();
              std::string message = vm.GetAsFromTop<std::string>();
              vm.Native().PopTop();

              TtWindow* parent = vm.GetParentWindow();
              int result = parent ? MessageBoxType::Show( *parent, message, caption, icon )
                : MessageBoxType::Show( message, caption, icon );

              vm.Native().PushInteger( result );
              return TtSquirrel::Const::ExistReturnValue;
            } );
            Native().SetParamsCheck( 4, "tssi" );
          } );
      };
      define_closure( TypeArgumentAs<TtMessageBoxOk>(),               Tag::Ok );
      define_closure( TypeArgumentAs<TtMessageBoxOkCancel>(),         Tag::OkCancel );
      define_closure( TypeArgumentAs<TtMessageBoxRetryCancel>(),      Tag::RetryCancel );
      define_closure( TypeArgumentAs<TtMessageBoxYesNo>(),            Tag::YesNo );
      define_closure( TypeArgumentAs<TtMessageBoxYesNoCancel>(),      Tag::YesNoCancel );
      define_closure( TypeArgumentAs<TtMessageBoxAbortRetryIgnore>(), Tag::AbortRetryIgnore );
    } );
}
