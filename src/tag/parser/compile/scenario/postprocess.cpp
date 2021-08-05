// SPDX-License-Identifier: GPL-3.0-only

#include <invader/tag/parser/definition/scenario.hpp>
#include <invader/build/build_workload.hpp>
#include <invader/file/file.hpp>
#include <invader/tag/hek/class/model_collision_geometry.hpp>
#include <invader/script/compiler.hpp>
#include <invader/tag/parser/compile/scenario.hpp>

namespace Invader::Parser {
    void Invader::Parser::Scenario::postprocess_hek_data() {
        if(this->script_syntax_data.size() >= sizeof(ScenarioScriptNodeTable::C<LittleEndian>)) {
            auto *table = reinterpret_cast<ScenarioScriptNodeTable::C<BigEndian> *>(this->script_syntax_data.data());
            table->first_element_ptr = 0;

            // FFFFFFFF-out all the tag IDs
            std::size_t element_count = std::min(static_cast<std::size_t>(table->size.read()), static_cast<std::size_t>(this->script_syntax_data.size() - sizeof(*table)) / sizeof(ScenarioScriptNode::C<LittleEndian>));
            auto *nodes = reinterpret_cast<ScenarioScriptNode::C<BigEndian> *>(table + 1);

            for(std::uint16_t i = 0; i < element_count; i++) {
                // Check if we know the class
                std::optional<TagFourCC> tag_class;
                auto &node = nodes[i];

                // Check the class type
                switch(node.type.read()) {
                    case ScenarioScriptValueType::SCENARIO_SCRIPT_VALUE_TYPE_SOUND:
                        tag_class = TagFourCC::TAG_FOURCC_SOUND;
                        break;

                    case ScenarioScriptValueType::SCENARIO_SCRIPT_VALUE_TYPE_EFFECT:
                        tag_class = TagFourCC::TAG_FOURCC_EFFECT;
                        break;

                    case ScenarioScriptValueType::SCENARIO_SCRIPT_VALUE_TYPE_DAMAGE:
                        tag_class = TagFourCC::TAG_FOURCC_DAMAGE_EFFECT;
                        break;

                    case ScenarioScriptValueType::SCENARIO_SCRIPT_VALUE_TYPE_LOOPING_SOUND:
                        tag_class = TagFourCC::TAG_FOURCC_SOUND_LOOPING;
                        break;

                    case ScenarioScriptValueType::SCENARIO_SCRIPT_VALUE_TYPE_ANIMATION_GRAPH:
                        tag_class = TagFourCC::TAG_FOURCC_MODEL_ANIMATIONS;
                        break;

                    case ScenarioScriptValueType::SCENARIO_SCRIPT_VALUE_TYPE_ACTOR_VARIANT:
                        tag_class = TagFourCC::TAG_FOURCC_ACTOR_VARIANT;
                        break;

                    case ScenarioScriptValueType::SCENARIO_SCRIPT_VALUE_TYPE_DAMAGE_EFFECT:
                        tag_class = TagFourCC::TAG_FOURCC_DAMAGE_EFFECT;
                        break;

                    case ScenarioScriptValueType::SCENARIO_SCRIPT_VALUE_TYPE_OBJECT_DEFINITION:
                        tag_class = TagFourCC::TAG_FOURCC_OBJECT;
                        break;

                    default:
                        continue;
                }

                if(tag_class.has_value()) {
                    // Check if we should leave it alone
                    auto flags = node.flags.read();
                    if((flags & ScenarioScriptNodeFlagsFlag::SCENARIO_SCRIPT_NODE_FLAGS_FLAG_IS_GLOBAL) || (flags & ScenarioScriptNodeFlagsFlag::SCENARIO_SCRIPT_NODE_FLAGS_FLAG_IS_SCRIPT_CALL)) {
                        continue;
                    }
                    node.data = TagID::null_tag_id();
                }
            }
        }
    }
}
