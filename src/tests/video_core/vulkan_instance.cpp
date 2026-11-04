// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch_test_macros.hpp>

#include "video_core/vulkan_common/vulkan_instance.h"
#include "video_core/vulkan_common/vulkan_library.h"
#include "video_core/vulkan_common/vulkan_wrapper.h"

// Runs against whatever Vulkan driver the machine has. CI points it at Mesa lavapipe, so the
// software driver gives every runner a device; with no loader or driver the test is skipped.
TEST_CASE("A headless Vulkan instance exposes a physical device", "[video_core][vulkan]") {
    const auto library = Vulkan::OpenLibrary();
    if (!library || !library->IsOpen()) {
        SKIP("no Vulkan loader on this machine");
    }
    Vulkan::vk::InstanceDispatch dld;
    Vulkan::vk::Instance instance;
    try {
        instance = Vulkan::CreateInstance(*library, dld, VK_API_VERSION_1_1);
    } catch (const Vulkan::vk::Exception&) {
        SKIP("no Vulkan driver on this machine");
    }
    REQUIRE(!instance.EnumeratePhysicalDevices().empty());
}
