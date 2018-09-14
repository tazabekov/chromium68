// This file is generated by PAL generator, do not modify.
// To make changes, modify the source file:
// test3.json

#include "remote_pal_ipc/remote_pal_ipc.h"

#include <mutex>

#include "remote_pal_ipc/sample/sample_remote_pal_interface_ipc.h"

namespace pal {

std::unique_ptr<SampleInterface> sample_interface_instance;
std::once_flag sample_interface_instance_flag;

SampleInterface* RemotePalIPC::GetSampleInterface() {
  std::call_once(sample_interface_instance_flag, []() {
    sample_interface_instance.reset(new SampleRemotePalInterfaceIPC());
  });
  return sample_interface_instance.get();
}

}  // namespace pal