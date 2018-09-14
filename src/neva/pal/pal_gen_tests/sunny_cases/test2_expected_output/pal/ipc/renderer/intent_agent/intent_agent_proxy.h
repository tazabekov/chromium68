// This file is generated by PAL generator, do not modify.
// To make changes, modify the source file:
// test2.json

#ifndef PAL_PUBLIC_RENDERER_INTENT_AGENT_PROXY_H_
#define PAL_PUBLIC_RENDERER_INTENT_AGENT_PROXY_H_

#include "content/public/renderer/render_frame.h"
#include "pal/ipc/pal_export.h"

namespace pal {

class PAL_EXPORT IntentAgentProxy {
 public:
  IntentAgentProxy(content::RenderFrame* frame);
  virtual ~IntentAgentProxy() = default;

  void InvokeIntent(int pal_async_callback_id,
                    const std::string& action,
                    const std::string& type,
                    const std::string& data,
                    const std::string& app_id,
                    int32_t callback_index);

  void RespondIntent(bool result, uint32_t session_id, const std::string& data);

 private:
  content::RenderFrame* render_frame_;
};

}  // namespace pal

#endif  // PAL_PUBLIC_RENDERER_INTENT_AGENT_PROXY_H_