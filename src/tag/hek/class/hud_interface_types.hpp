// SPDX-License-Identifier: GPL-3.0-only

#ifndef INVADER__TAG__HEK__CLASS__HUD_INTERFACE_TYPES_HPP
#define INVADER__TAG__HEK__CLASS__HUD_INTERFACE_TYPES_HPP

#include "../../compiled_tag.hpp"
#include "../../../hek/data_type.hpp"
#include "enum.hpp"
#include "../header.hpp"

namespace Invader::HEK {
    enum HUDInterfaceDestinationType : TagEnum {
        HUD_INTERFACE_DESTINATION_TYPE_TINT_0_1,
        HUD_INTERFACE_DESTINATION_TYPE_HORIZONTAL_OFFSET,
        HUD_INTERFACE_DESTINATION_TYPE_VERTICAL_OFFSET,
        HUD_INTERFACE_DESTINATION_TYPE_FADE_0_1
    };

    enum HUDInterfaceDestination : TagEnum {
        HUD_INTERFACE_DESTINATION_GEOMETRY_OFFSET,
        HUD_INTERFACE_DESTINATION_PRIMARY_MAP,
        HUD_INTERFACE_DESTINATION_SECONDARY_MAP,
        HUD_INTERFACE_DESTINATION_TERTIARY_MAP
    };

    enum HUDInterfaceSource : TagEnum {
        HUD_INTERFACE_SOURCE_PLAYER_PITCH,
        HUD_INTERFACE_SOURCE_PLAYER_PITCH_TANGENT,
        HUD_INTERFACE_SOURCE_PLAYER_YAW,
        HUD_INTERFACE_SOURCE_WEAPON_AMMO_TOTAL,
        HUD_INTERFACE_SOURCE_WEAPON_AMMO_LOADED,
        HUD_INTERFACE_SOURCE_WEAPON_HEAT,
        HUD_INTERFACE_SOURCE_EXPLICIT_USES_LOW_BOUND,
        HUD_INTERFACE_SOURCE_WEAPON_ZOOM_LEVEL
    };

    enum HUDInterfaceMultitextureOverlayAnchor : TagEnum {
        HUD_INTERFACE_MULTITEXTURE_OVERLAY_ANCHOR_TEXTURE,
        HUD_INTERFACE_MULTITEXTURE_OVERLAY_ANCHOR_SCREEN
    };

    enum HUDInterfaceZeroToOneBlendFunction : TagEnum {
        HUD_INTERFACE_ZERO_TO_ONE_BLEND_FUNCTION_ADD,
        HUD_INTERFACE_ZERO_TO_ONE_BLEND_FUNCTION_SUBTRACT,
        HUD_INTERFACE_ZERO_TO_ONE_BLEND_FUNCTION_MULTIPLY,
        HUD_INTERFACE_ZERO_TO_ONE_BLEND_FUNCTION_MULTIPLY2X,
        HUD_INTERFACE_ZERO_TO_ONE_BLEND_FUNCTION_DOT
    };

    enum HUDInterfaceWrapMode : TagEnum {
        HUD_INTERFACE_WRAP_MODE_CLAMP,
        HUD_INTERFACE_WRAP_MODE_WRAP
    };

    enum HUDInterfaceAnchor : TagEnum {
        HUD_INTERFACE_ANCHOR_TOP_LEFT,
        HUD_INTERFACE_ANCHOR_TOP_RIGHT,
        HUD_INTERFACE_ANCHOR_BOTTOM_LEFT,
        HUD_INTERFACE_ANCHOR_BOTTOM_RIGHT,
        HUD_INTERFACE_ANCHOR_CENTER,
    };

    struct HUDInterfaceMeterFlags {
        std::uint8_t use_min_max_for_state_changes : 1;
        std::uint8_t interpolate_between_min_max_flash_colors_as_state_changes : 1;
        std::uint8_t interpolate_color_along_hsv_space : 1;
        std::uint8_t _more_colors_for_hsv_interpolation : 1;
        std::uint8_t invert_interpolation : 1;
    };

    struct HUDInterfaceScalingFlags {
        std::uint16_t don_t_scale_offset : 1;
        std::uint16_t don_t_scale_size : 1;
        std::uint16_t use_high_res_scale : 1;
    };

    struct HUDInterfaceFlashFlags {
        std::uint16_t reverse_default_flashing_colors : 1;
    };

    struct HUDInterfaceNumberFlags {
        std::uint8_t show_leading_zeros : 1;
        std::uint8_t only_show_when_zoomed : 1;
        std::uint8_t draw_a_trailing_m : 1;
    };

    struct HUDInterfaceOverlayFlashFlags {
        std::uint32_t flashes_when_active : 1;
    };

    struct HUDInterfaceMessagingFlags {
        std::uint8_t use_text_from_string_list_instead : 1;
        std::uint8_t override_default_color : 1;
        std::uint8_t width_offset_is_absolute_icon_width : 1;
    };

    ENDIAN_TEMPLATE(EndianType) struct HUDInterfaceMultitextureOverlayEffector {
        PAD(0x40);
        EndianType<HUDInterfaceDestinationType> destination_type;
        EndianType<HUDInterfaceDestination> destination;
        EndianType<HUDInterfaceSource> source;
        PAD(0x2);
        Bounds<EndianType<float>> in_bounds;
        Bounds<EndianType<float>> out_bounds;
        PAD(0x40);
        ColorRGB<EndianType> tint_color_lower_bound;
        ColorRGB<EndianType> tint_color_upper_bound;
        EndianType<WaveFunction> periodic_function;
        PAD(0x2);
        EndianType<float> function_period;
        EndianType<float> function_phase;
        PAD(0x20);

        ENDIAN_TEMPLATE(NewType) operator HUDInterfaceMultitextureOverlayEffector<NewType>() const noexcept {
            HUDInterfaceMultitextureOverlayEffector<NewType> copy = {};
            COPY_THIS(destination_type);
            COPY_THIS(destination);
            COPY_THIS(source);
            COPY_THIS(in_bounds);
            COPY_THIS(out_bounds);
            COPY_THIS(tint_color_lower_bound);
            COPY_THIS(tint_color_upper_bound);
            COPY_THIS(periodic_function);
            COPY_THIS(function_period);
            COPY_THIS(function_phase);
            return copy;
        }
    };
    static_assert(sizeof(HUDInterfaceMultitextureOverlayEffector<BigEndian>) == 0xDC);

    ENDIAN_TEMPLATE(EndianType) struct HUDInterfaceMultitextureOverlay {
        PAD(0x2);
        EndianType<std::int16_t> type;
        EndianType<FramebufferBlendFunction> framebuffer_blend_function;
        PAD(0x2);
        PAD(0x20);
        EndianType<HUDInterfaceAnchor> primary_anchor;
        EndianType<HUDInterfaceMultitextureOverlayAnchor> secondary_anchor;
        EndianType<HUDInterfaceMultitextureOverlayAnchor> tertiary_anchor;
        EndianType<HUDInterfaceZeroToOneBlendFunction> zero_to_one_blend_function;
        EndianType<HUDInterfaceZeroToOneBlendFunction> one_to_two_blend_function;
        PAD(0x2);
        Point2D<EndianType> primary_scale;
        Point2D<EndianType> secondary_scale;
        Point2D<EndianType> tertiary_scale;
        Point2D<EndianType> primary_offset;
        Point2D<EndianType> secondary_offset;
        Point2D<EndianType> tertiary_offset;
        TagDependency<EndianType> primary; // bitmap
        TagDependency<EndianType> secondary; // bitmap
        TagDependency<EndianType> tertiary; // bitmap
        EndianType<HUDInterfaceWrapMode> primary_wrap_mode;
        EndianType<HUDInterfaceWrapMode> secondary_wrap_mode;
        EndianType<HUDInterfaceWrapMode> tertiary_wrap_mode;
        PAD(0x2);
        PAD(0xB8);
        TagReflexive<EndianType, HUDInterfaceMultitextureOverlayEffector> effectors;
        PAD(0x80);

        ENDIAN_TEMPLATE(NewType) operator HUDInterfaceMultitextureOverlay<NewType>() const noexcept {
            HUDInterfaceMultitextureOverlay<NewType> copy = {};
            COPY_THIS(type);
            COPY_THIS(framebuffer_blend_function);
            COPY_THIS(primary_anchor);
            COPY_THIS(secondary_anchor);
            COPY_THIS(tertiary_anchor);
            COPY_THIS(zero_to_one_blend_function);
            COPY_THIS(one_to_two_blend_function);
            COPY_THIS(primary_scale);
            COPY_THIS(secondary_scale);
            COPY_THIS(tertiary_scale);
            COPY_THIS(primary_offset);
            COPY_THIS(secondary_offset);
            COPY_THIS(tertiary_offset);
            COPY_THIS(primary);
            COPY_THIS(secondary);
            COPY_THIS(tertiary);
            COPY_THIS(primary_wrap_mode);
            COPY_THIS(secondary_wrap_mode);
            COPY_THIS(tertiary_wrap_mode);
            COPY_THIS(effectors);
            return copy;
        }
    };
    static_assert(sizeof(HUDInterfaceMultitextureOverlay<BigEndian>) == 0x1E0);

    #define COMPILE_MULTITEXTURE_OVERLAY(reflexive_struct) ADD_REFLEXIVE_START(reflexive_struct) { \
                                                                ADD_DEPENDENCY_ADJUST_SIZES(reflexive.primary); \
                                                                ADD_DEPENDENCY_ADJUST_SIZES(reflexive.secondary); \
                                                                ADD_DEPENDENCY_ADJUST_SIZES(reflexive.tertiary); \
                                                                ADD_REFLEXIVE(reflexive.effectors); \
                                                           } ADD_REFLEXIVE_END
}
#endif
