#include <random>

#include "caffe2/proto/caffe2.pb.h"
#include "caffe2/core/context.h"
#include "gtest/gtest.h"

namespace caffe2 {

// This is a test that make sure the random number generator works as expected,
// with a specific seed that generates specific responses. I think it should
// be the same number across platforms since we use mt19937 explicitly.
TEST(CPUContextTest, TestRandomNumberGenerator) {
  DeviceOption option;
  option.set_random_seed(1701);
  CPUContext context(option);
  std::uniform_int_distribution<int> dist(0, 100);
  /*
  // These numbers are manually verified off-line.
  EXPECT_EQ(dist(context.RandGenerator()), 46);
  EXPECT_EQ(dist(context.RandGenerator()), 4);
  EXPECT_EQ(dist(context.RandGenerator()), 94);
  EXPECT_EQ(dist(context.RandGenerator()), 26);
  EXPECT_EQ(dist(context.RandGenerator()), 67);
  */
}

TEST(CPUContextTest, TestAllocDealloc) {
  float* data = static_cast<float*>(CPUContext::New(10 * sizeof(float)));
  EXPECT_NE(data, nullptr);
  float* dst_data = static_cast<float*>(CPUContext::New(10 * sizeof(float)));
  EXPECT_NE(dst_data, nullptr);
  for (int i = 0; i < 10; ++i) {
    data[i] = i;
  }
  DeviceOption option;
  CPUContext context(option);
  context.Copy<float, CPUContext, CPUContext>(dst_data, data, 10);
  for (int i = 0; i < 10; ++i) {
    EXPECT_FLOAT_EQ(dst_data[i], i);
  }
  CPUContext::Delete(data);
  CPUContext::Delete(dst_data);
}

}  // namespace caffe2
