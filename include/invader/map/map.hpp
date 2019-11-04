// SPDX-License-Identifier: GPL-3.0-only

#ifndef INVADER__MAP__MAP_HPP
#define INVADER__MAP__MAP_HPP

#include <string>
#include <vector>
#include <cstddef>
#include <memory>

#include "../resource/resource_map.hpp"
#include "../hek/map.hpp"
#include "tag.hpp"

namespace Invader {
    /**
     * Class for handling compiled maps
     */
    class Map {
    friend class Tag;
    public:
        enum DataMapType {
            DATA_MAP_CACHE,
            DATA_MAP_BITMAP,
            DATA_MAP_SOUND,
            DATA_MAP_LOC
        };

        /**
         * Create a Map by copying the given data, bitmaps, loc, and sound data. Compressed maps can be loaded this
         * way.
         *
         * @param data              pointer to map data
         * @param data_size         length of map data
         * @param bitmaps_data      pointer to bitmaps data
         * @param bitmaps_data_size length of bitmaps data
         * @param loc_data          pointer to loc data
         * @param loc_data_size     length of loc data
         * @param sounds_data       pointer to sounds data
         * @param sounds_data_size  length of sounds data
         * @return                   map
         */
        static Map map_with_copy(const std::byte *data, std::size_t data_size,
                                 const std::byte *bitmaps_data = nullptr, std::size_t bitmaps_data_size = 0,
                                 const std::byte *loc_data = nullptr, std::size_t loc_data_size = 0,
                                 const std::byte *sounds_data = nullptr, std::size_t sounds_data_size = 0);

        /**
         * Create a Map by moving the given data, bitmaps, loc, and sound data. Compressed maps can be loaded this way.
         * @param  data         map data vector
         * @param  bitmaps_data bitmap data vector
         * @param  loc_data     loc data vector
         * @param  sounds_data  sound data vector
         * @return              map
         */
        static Map map_with_move(std::vector<std::byte> &&data,
                                 std::vector<std::byte> &&bitmaps_data = std::vector<std::byte>(),
                                 std::vector<std::byte> &&loc_data = std::vector<std::byte>(),
                                 std::vector<std::byte> &&sounds_data = std::vector<std::byte>());

        /**
         * Create a Map by using the pointers to the given data, bitmaps, loc, and sound data. The caller is
         * responsible for ensuring that these pointers are valid for the lifespan of the Map. Compressed maps cannot
         * be loaded this way.
         *
         * @param data              pointer to map data
         * @param data_size         length of map data
         * @param bitmaps_data      pointer to bitmaps data
         * @param bitmaps_data_size length of bitmaps data
         * @param loc_data          pointer to loc data
         * @param loc_data_size     length of loc data
         * @param sounds_data       pointer to sounds data
         * @param sounds_data_size  length of sounds data
         * @return                  map
         */
        static Map map_with_pointer(std::byte *data, std::size_t data_size,
                                    std::byte *bitmaps_data = nullptr, std::size_t bitmaps_data_size = 0,
                                    std::byte *loc_data = nullptr, std::size_t loc_data_size = 0,
                                    std::byte *sounds_data = nullptr, std::size_t sounds_data_size = 0);

        /**
         * Get the data at the specified offset
         * @param  offset       offset
         * @param  minimum_size minimum number of bytes to guarantee
         * @return              pointer to the data
         * @throws              OutOfBoundsException if data is out of bounds
         */
        std::byte *get_data_at_offset(std::size_t offset, std::size_t minimum_size = 0, DataMapType map_type = DATA_MAP_CACHE);

        /**
         * Get the data at the specified offset
         * @param  offset       offset
         * @param  minimum_size minimum number of bytes to guarantee
         * @return              pointer to the data
         * @throws              OutOfBoundsException if data is out of bounds
         */
        const std::byte *get_data_at_offset(std::size_t offset, std::size_t minimum_size = 0, DataMapType map_type = DATA_MAP_CACHE) const;

        /**
         * Get the tag data at the specified offset
         * @param  offset       offset
         * @param  minimum_size minimum number of bytes to guarantee
         * @return              pointer to the data
         * @throws              OutOfBoundsException if data is out of bounds
         */
        std::byte *get_tag_data_at_offset(std::size_t offset, std::size_t minimum_size = 0);

        /**
         * Get the tag data at the specified offset
         * @param  offset       offset
         * @param  minimum_size minimum number of bytes to guarantee
         * @return              pointer to the data
         * @throws              OutOfBoundsException if data is out of bounds
         */
        const std::byte *get_tag_data_at_offset(std::size_t offset, std::size_t minimum_size = 0) const;

        /**
         * Resolve the tag data pointer
         * @param  pointer      pointer
         * @param  minimum_size minimum number of bytes to guarantee
         * @return              pointer to the data
         * @throws              OutOfBoundsException if data is out of bounds
         */
        std::byte *resolve_tag_data_pointer(std::uint32_t offset, std::size_t minimum_size = 0);

        /**
         * Resolve the tag data pointer
         * @param  pointer      pointer
         * @param  minimum_size minimum number of bytes to guarantee
         * @return              pointer to the data
         * @throws              OutOfBoundsException if data is out of bounds
         */
        const std::byte *resolve_tag_data_pointer(std::uint32_t offset, std::size_t minimum_size = 0) const;

        /**
         * Get a pointer to the uncompressed map data
         * @return data
         */
        std::byte *get_data() noexcept;

        /**
         * Get the data length in bytes
         * @return data length in bytes
         */
        std::size_t get_data_length() const noexcept;

        /**
         * Get the tag count
         * @return the tag count
         */
        std::size_t get_tag_count() const noexcept;

        /**
         * Get the tag at the specified index
         * @param index the tag index
         * @return      the tag
         * @throws      OutOfBoundsException if index is invalid
         */
        Tag &get_tag(std::size_t index);

        /**
         * Get the tag at the specified index
         * @param index the tag index
         * @return      the tag
         * @throws      OutOfBoundsException if index is invalid
         */
        const Tag &get_tag(std::size_t index) const;

        /**
         * Get the scenario tag ID
         * @return The scenario tag ID
         */
        std::size_t get_scenario_tag_id() const noexcept;

        /**
         * Get the tag data header
         * @return reference to the tag data header
         */
        HEK::CacheFileTagDataHeader &get_tag_data_header() noexcept;

        /**
         * Get the tag data header
         * @return reference to the tag data header
         */
        const HEK::CacheFileTagDataHeader &get_tag_data_header() const noexcept;

        /**
         * Get the cache file header
         * @return reference to the cache file header
         */
        const HEK::CacheFileHeader &get_cache_file_header() const noexcept;

        /**
         * Get whether the map was originally compressed
         * @return true if the map was compressed
         */
        bool is_compressed() const noexcept;

        /**
         * Get whether the map is obviously protected
         * @return true if the map is obviously protected
         */
        bool is_protected() const noexcept;

        Map(Map &&);
    private:
        /** Map data if managed */
        std::vector<std::byte> data_m;

        /** Map data */
        std::byte *data = nullptr;

        /** Map data length */
        std::size_t data_length = 0;


        /** Bitmaps data if managed */
        std::vector<std::byte> bitmap_data_m;

        /** Bitmaps data */
        std::byte *bitmap_data = nullptr;

        /** Bitmaps data length */
        std::size_t bitmap_data_length = 0;


        /** Loc data if managed */
        std::vector<std::byte> loc_data_m;

        /** Loc data */
        std::byte *loc_data = nullptr;

        /** Loc data length */
        std::size_t loc_data_length = 0;


        /** Sounds data if managed */
        std::vector<std::byte> sound_data_m;

        /** Sounds data */
        std::byte *sound_data = nullptr;

        /** Sounds data length */
        std::size_t sound_data_length = 0;


        /** Tag array */
        std::vector<Tag> tags;

        /** Scenario tag ID */
        std::size_t scenario_tag_id = 0;

        /** Tag data */
        std::byte *tag_data = nullptr;

        /** Tag data length */
        std::size_t tag_data_length = 0;

        /** Base memory address */
        std::uint32_t base_memory_address = HEK::CACHE_FILE_PC_BASE_MEMORY_ADDRESS;

        /** Header */
        HEK::CacheFileHeader header;

        /** Map is compressed */
        bool compressed = false;


        /** Load the map now */
        void load_map();

        /** Populate tag array */
        void populate_tag_array();

        /** Get BSPs */
        void get_bsps();

        /**
         * Decompress if we are compressed
         * @param data      pointer to data
         * @param data_size size of data buffer
         * @return true if it was decompressed
         */
        bool decompress_if_needed(const std::byte *data, std::size_t data_size);

        Map() = default;
    };
}
#endif
