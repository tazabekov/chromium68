// This file is generated by PAL generator, do not modify.
// To make changes, modify the source file:
// test2.json

#include "remote_pal_ipc/intent_agent/intent_agent_remote_pal_interface_ipc.h"

#include <limits>

#include "base/bind.h"
#include "content/public/renderer/render_frame.h"
#include "pal/ipc/renderer/intent_agent/intent_agent_proxy.h"
#include "third_party/blink/public/web/web_local_frame.h"

namespace pal {

IntentAgentRemotePalInterfaceIPC::IntentAgentRemotePalInterfaceIPC() = default;

IntentAgentRemotePalInterfaceIPC::~IntentAgentRemotePalInterfaceIPC() = default;

IntentAgentRemotePalInterfaceIPC::FrameCallbacks::FrameCallbacks() = default;
IntentAgentRemotePalInterfaceIPC::FrameCallbacks::~FrameCallbacks() = default;

content::RenderFrame* IntentAgentRemotePalInterfaceIPC::getFrame() const {
  content::RenderFrame* frame = content::RenderFrame::FromWebFrame(
      blink::WebLocalFrame::FrameForCurrentContext());
  if (!frame) {
    LOG(ERROR) << "Frame isn't available in Remote 'IntentAgent' Interface";
    return nullptr;
  }
  return frame;
}

// static
template <typename _ID, typename _Map>
_ID IntentAgentRemotePalInterfaceIPC::GetAsyncMsgId(_ID& async_msg_id_,
                                                    _Map& map) {
  _ID max = std::numeric_limits<_ID>::max();
  if (++async_msg_id_ == max)
    async_msg_id_ = 0;
  if (map.size() == (size_t)max) {
    LOG(ERROR) << "Remote PAL Async callback map is full.";
    return async_msg_id_;
  }
  if (map.find(async_msg_id_) != map.end())
    return GetAsyncMsgId<_ID, _Map>(async_msg_id_, map);
  return async_msg_id_;
}

// static
int IntentAgentRemotePalInterfaceIPC::invoke_intent_callback_id_ = 0;

void IntentAgentRemotePalInterfaceIPC::InvokeIntent(
    const std::string& action,
    const std::string& type,
    const std::string& data,
    const std::string& app_id,
    int32_t callback_index,
    const InvokeIntentRespondCallback& on_invoke_intent_done) {
  content::RenderFrame* frame = getFrame();
  if (!frame)
    return;

  size_t pal_async_callback_id =
      GetAsyncMsgId<int, InvokeIntentRespondCallbackMAP>(
          invoke_intent_callback_id_, invoke_intent_respond_callbacks_);
  invoke_intent_respond_callbacks_[pal_async_callback_id] =
      on_invoke_intent_done;

  std::unique_ptr<pal::IntentAgentProxy> proxy(
      new pal::IntentAgentProxy(frame));
  proxy->InvokeIntent(pal_async_callback_id, action, type, data, app_id,
                      callback_index);
}

void IntentAgentRemotePalInterfaceIPC::RespondIntent(bool result,
                                                     uint32_t session_id,
                                                     const std::string& data) {
  content::RenderFrame* frame = getFrame();
  if (!frame)
    return;
  std::unique_ptr<pal::IntentAgentProxy> proxy(
      new pal::IntentAgentProxy(frame));
  proxy->RespondIntent(result, session_id, data);
  return;
}

}  // namespace pal