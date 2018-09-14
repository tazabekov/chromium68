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

#ifndef NEVA_APP_RUNTIME_WEBVIEW_PROFILE_H_
#define NEVA_APP_RUNTIME_WEBVIEW_PROFILE_H_

#include <string>

#include "neva/app_runtime/public/app_runtime_export.h"

namespace app_runtime {

class BrowserContextAdapter;

class APP_RUNTIME_EXPORT WebViewProfile {
 public:
  WebViewProfile(const std::string& storage_name);

  static WebViewProfile* GetDefaultProfile();

  void SetProxyServer(const std::string& ip,
                      const std::string& port,
                      const std::string& username,
                      const std::string& password);
  void AppendExtraWebSocketHeader(const std::string& key,
                                  const std::string& value);

  void RemoveBrowsingData(int remove_browsing_data_mask);

  void FlushCookieStore();

 private:
  friend class WebView;

  WebViewProfile(BrowserContextAdapter* adapter);

  BrowserContextAdapter* GetBrowserContextAdapter() const;

  BrowserContextAdapter* browser_context_adapter_;
};

}  // namespace app_runtime

#endif  // NEVA_APP_RUNTIME_WEBVIEW_PROFILE_H_