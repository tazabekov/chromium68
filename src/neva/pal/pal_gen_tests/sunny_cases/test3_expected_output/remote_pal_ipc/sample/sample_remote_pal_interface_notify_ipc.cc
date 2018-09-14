// This file is generated by PAL generator, do not modify.
// To make changes, modify the source file:
// test3.json

#include "remote_pal_ipc/sample/sample_remote_pal_interface_ipc.h"

namespace pal {

void SampleRemotePalInterfaceIPC::NotifyProcessData(int pal_async_callback_id,
                                                    int error_code,
                                                    const std::string& data) {
  ProcessDataRespondCallbackMAP::iterator it =
      process_data_respond_callbacks_.find(pal_async_callback_id);
  if (it == process_data_respond_callbacks_.end())
    return;
  it->second.Run(error_code, data);
}

void SampleRemotePalInterfaceIPC::NotifySampleUpdate(int frame_id,
                                                     const std::string& value) {
  FrameCallbacksMAP::iterator it = callbacks_.find(frame_id);
  if (it == callbacks_.end())
    return;
  it->second->sample_update_callbacks_.Notify(value);
}

}  // namespace pal