// Copyright (C) 2018-2025 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "core/operator_set.hpp"
#include "exceptions.hpp"
#include <string>

namespace ov {
namespace frontend {
namespace onnx {
namespace com_microsoft {
namespace opset_1 {

namespace detail { 

// Hold EPCtx attrs
struct EPContextAttributes {
    int64_t main_context = 1;
    std::string ep_cache_context;
    int64_t embed_mode = 1;
    std::string ep_sdk_version;
    std::string onnx_model_filename;
    std::string hardware_architecture;
    std::string partition_name;
    std::string source;
    std::string notes;
    int64_t max_size = 0;
};

// Extract EPCtx attrs from Node
EPContextAttributes extract_epctx_attrs(const ov::frontend::onnx::Node& node) {
    EPContextAttributes attr;
    attr.main_context = node.get_attribute_value<int64_t>("main_context", 1);
    attr.ep_cache_context = node.get_attribute_value<std::string>("ep_cache_context", "");
    attr.embed_mode = node.get_attribute_value<int64_t>("embed_mode", 1);
    attr.ep_sdk_version = node.get_attribute_value<std::string>("ep_sdk_version", "");
    attr.onnx_model_filename = node.get_attribute_value<std::string>("onnx_model_filename", "");
    attr.hardware_architecture = node.get_attribute_value<std::string>("hardware_architecture", "");
    attr.partition_name = node.get_attribute_value<std::string>("partition_name", "");
    attr.source = node.get_attribute_value<std::string>("source", "");
    attr.notes = node.get_attribute_value<std::string>("notes", "");
    attr.max_size = node.get_attribute_value<int64_t>("max_size", 0);
    return attr;
}

// Validate EPCtx's necessary attrs
void validate_epctx_attrs(const EPContextAttributes& attr) {
    FRONT_END_GENERAL_CHECK(attr.main_context == 0 || attr.main_context == 1,"EPContext: 'main_context' must be 0 (refers to secondary/reference context) or 1 (primary context).");
    FRONT_END_GENERAL_CHECK(!attr.ep_cache_context.empty(),"EPContext: 'ep_cache_context' attribute must not be empty.");
    FRONT_END_GENERAL_CHECK(attr.embed_mode == 0 || attr.embed_mode == 1,"EPContext: 'embed_mode' must be 0 (external file) or 1 (embedded payload).");
    FRONT_END_GENERAL_CHECK(attr.source == "OVEP" || attr.source == "OpenVINOExecutionProvider" || attr.source == "","EPContext: 'source' attribute must be one of ['OVEP', 'OpenVINOExecutionProvider', '']. Found: '" + attr.source + "'");
    FRONT_END_GENERAL_CHECK(attr.max_size >= 0,"EPContext: 'max_size' attribute must be non-negative.");
}

}  // namespace detail


ov::OutputVector ep_context(const ov::frontend::onnx::Node& node) {

    detail::EPContextAttributes attr = detail::extract_epctx_attrs(node);

    detail::validate_epctx_attrs(attr);

    return {};
}

ONNX_OP("EPContext",OPSET_SINCE(1),com_microsoft::opset_1::ep_context,MICROSOFT_DOMAIN);

}  // namespace opset_1
}  // namespace com_microsoft
}  // namespace onnx
}  // namespace frontend
}  // namespace ov

