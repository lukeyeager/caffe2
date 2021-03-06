#include <cuda.h>
#include <cuda_runtime.h>

#include <sstream>

#include "caffe2/core/common_gpu.h"
#include "glog/logging.h"

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);

  int gpu_count;
  CUDA_CHECK(cudaGetDeviceCount(&gpu_count));
  for (int i = 0; i < gpu_count; ++i) {
    LOG(INFO) << "Querying device ID = " << i;
    caffe2::DeviceQuery(i);
  }

  std::stringstream sstream;
  // Find topology
  int can_access;
  for (int i = 0; i < gpu_count; ++i) {
    for (int j = 0; j < gpu_count; ++j) {
      CUDA_CHECK(cudaDeviceCanAccessPeer(&can_access, i, j));
      sstream << ((i == j || can_access) ? "+" : "-") << " ";
    }
    sstream << std::endl;
  }
  LOG(INFO) << "Access pattern: " << std::endl << sstream.str();
}
