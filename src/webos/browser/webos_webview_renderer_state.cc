// Copyright (c) 2017-2018 LG Electronics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#include "webos/browser/webos_webview_renderer_state.h"

#include "content/public/browser/browser_thread.h"

namespace webos {

// static
WebOSWebViewRendererState* WebOSWebViewRendererState::GetInstance() {
  return base::Singleton<WebOSWebViewRendererState>::get();
}

WebOSWebViewRendererState::WebOSWebViewRendererState() {}

WebOSWebViewRendererState::~WebOSWebViewRendererState() {}

void WebOSWebViewRendererState::RegisterWebViewInfo(
    int render_process_id,
    int routing_id,
    const WebViewInfo& webview_info) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::IO);
  RenderId render_id(render_process_id, routing_id);
  webview_info_map_[render_id] = webview_info;
}

void WebOSWebViewRendererState::UnRegisterWebViewInfo(int render_process_id,
                                                      int routing_id) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::IO);
  RenderId render_id(render_process_id, routing_id);
  webview_info_map_.erase(render_id);
}

bool WebOSWebViewRendererState::GetInfo(int render_process_id,
                                        int routing_id,
                                        WebViewInfo* webview_info) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::IO);
  RenderId render_id(render_process_id, routing_id);
  WebViewInfoMap::iterator iter = webview_info_map_.find(render_id);
  if (iter != webview_info_map_.end()) {
    *webview_info = iter->second;
    return true;
  }
  return false;
}

}  // namespace webos