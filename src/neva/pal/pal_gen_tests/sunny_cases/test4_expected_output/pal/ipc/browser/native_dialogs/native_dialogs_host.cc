// This file is generated by PAL generator, do not modify.
// To make changes, modify the source file:
// test4.json

#include "pal/ipc/browser/native_dialogs/native_dialogs_host.h"

#include "base/bind.h"
#include "pal/ipc/native_dialogs_messages.h"
#include "pal/public/pal.h"
#include "pal/public/pal_factory.h"

namespace pal {

NativeDialogsHost::NativeDialogsHost()
    : content::BrowserMessageFilter(NativeDialogsMsgStart) {}

NativeDialogsHost::~NativeDialogsHost() {
  DCHECK_CURRENTLY_ON(content::BrowserThread::IO);
}

// clang-format off
bool NativeDialogsHost::OnMessageReceived(const IPC::Message& message) {
  int routing_id = message.routing_id();
  bool handled = true;

  IPC_BEGIN_MESSAGE_MAP_WITH_PARAM(
      NativeDialogsHost, message, &routing_id)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()
  return handled;
}
// clang-format on

}  // namespace pal