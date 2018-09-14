// This file is generated by PAL generator, do not modify.
// To make changes, modify the source file:
// test5.json

#include "pal/ipc/browser/native_controls/native_controls_host.h"

#include "base/bind.h"
#include "pal/ipc/native_controls_messages.h"
#include "pal/public/pal.h"
#include "pal/public/pal_factory.h"

namespace pal {

NativeControlsHost::NativeControlsHost()
    : content::BrowserMessageFilter(NativeControlsMsgStart),
      route_ids_(),
      weak_ptr_factory_(this) {
  mColorChosenCallback = base::Bind(&NativeControlsHost::SendColorChosen,
                                    weak_ptr_factory_.GetWeakPtr());
}

NativeControlsHost::~NativeControlsHost() {
  DCHECK_CURRENTLY_ON(content::BrowserThread::IO);
}

// clang-format off
bool NativeControlsHost::OnMessageReceived(const IPC::Message& message) {
  int routing_id = message.routing_id();
  bool handled = true;

  IPC_BEGIN_MESSAGE_MAP_WITH_PARAM(
      NativeControlsHost, message, &routing_id)
    case NativeControlsHostMsg_Unsubscribe::ID:
      route_ids_.erase(routing_id);
      break;
    case NativeControlsHostMsg_Subscribe::ID:
      route_ids_.insert(routing_id);
      AddCallbacks();
      break;
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()
  return handled;
}
// clang-format on

void NativeControlsHost::AddCallbacks() {
  NativeControlsInterface* interface =
      pal::GetInstance()->GetNativeControlsInterface();
  if (!interface)
    return;
  if (!mColorChosenSubscription.get()) {
    mColorChosenSubscription = interface->AddCallback(mColorChosenCallback);
  }
}

void NativeControlsHost::SendColorChosen(int color) {
  for (const int route_id : route_ids_) {
    Send(new NativeControlsMsg_ColorChosen(route_id, color));
  }
}

}  // namespace pal