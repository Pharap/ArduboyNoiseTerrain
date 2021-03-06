#pragma once

//
//  Copyright (C) 2020 Pharap (@Pharap)
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

// For uint8_t
#include <stdint.h>

constexpr uint8_t tileWidth = 8;
constexpr uint8_t tileHeight = 8;

constexpr uint8_t tiles[] PROGMEM
{
	// Dimensions
	tileWidth, tileHeight,

	// Frame 0 - Tile0
	0x81, 0x00, 0x3C, 0x42, 0x42, 0x3C, 0x00, 0x81,

	// Frame 1 - Tile1
	0x81, 0x00, 0x00, 0x44, 0x7E, 0x40, 0x00, 0x81,

	// Frame 2 - Tile2
	0x81, 0x00, 0x44, 0x62, 0x52, 0x4C, 0x00, 0x81,

	// Frame 3 - Tile3
	0x81, 0x00, 0x24, 0x42, 0x4A, 0x4A, 0x34, 0x81,

	// Frame 4 - Tile4
	0x81, 0x00, 0x30, 0x28, 0x24, 0x7C, 0x20, 0x81,

	// Frame 5 - Tile5
	0x81, 0x00, 0x4E, 0x4A, 0x4A, 0x32, 0x00, 0x81,

	// Frame 6 - Tile6
	0x81, 0x00, 0x3C, 0x52, 0x52, 0x24, 0x00, 0x81,

	// Frame 7 - Tile7
	0x81, 0x00, 0x02, 0x62, 0x1A, 0x06, 0x00, 0x81,

	// Frame 8 - Tile8
	0x81, 0x00, 0x34, 0x4A, 0x4A, 0x34, 0x00, 0x81,

	// Frame 9 - Tile9
	0x81, 0x00, 0x0C, 0x52, 0x52, 0x3C, 0x00, 0x81,
};