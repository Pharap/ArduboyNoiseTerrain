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

// For UINT32_MAX
#include <limits.h>

#include <Arduboy2.h>

Arduboy2 arduboy;

#include "Tiles.h"

enum class TileType : uint8_t
{
	Type0,
	Type1,
	Type2,
	Type3,
	Type4,
	Type5,
	Type6,
	Type7,
	Type8,
	Type9,
};

constexpr TileType firstTileType = TileType::Type0;
constexpr TileType lastTileType = TileType::Type9;
constexpr size_t tileTypeCount = ((static_cast<size_t>(lastTileType) - static_cast<size_t>(firstTileType)) + 1);

uint32_t hash(uint32_t value)
{
	value ^= (value << 13);
	value ^= (value >> 17);
	value ^= (value << 5);
	return value;
}

uint32_t hash(uint16_t x, uint16_t y)
{
	const uint32_t x2 = x;
	const uint32_t y2 = y;
	const uint32_t value = ((x2 << 16) | (y2 << 0));

	return hash(value * (x2 ^ y2));
}

float noise(size_t x, size_t y)
{
	return hash(x, y) / static_cast<float>(UINT32_MAX);
}

float smoothNoise(float x, float y)
{
	const size_t integralX = static_cast<size_t>(x);
	const size_t integralY = static_cast<size_t>(y);
	
	const size_t x0 = integralX;
	const size_t y0 = integralY;
	const size_t x1 = (integralX - 1);
	const size_t y1 = (integralY - 1);
	
	const float fractionalX = (x - integralX);
	const float fractionalY = (y - integralY);	
	const float inverseFractionalX = (1 - fractionalX);
	const float inverseFractionalY = (1 - fractionalY);
	
	const float value0 = (fractionalX * fractionalY) * noise(x0, y0);
	const float value1 = (fractionalX * inverseFractionalY) * noise(x0, y1);
	const float value2 = (inverseFractionalX * fractionalY) * noise(x1, y0);
	const float value3 = (inverseFractionalX * inverseFractionalY) * noise(x1, y1);
	
	return (value0 + value1 + value2 + value3);
}

float turbulence(float x, float y, size_t size)
{	
	float value = 0;
	
	for(float scale = size; scale >= 1; scale /= 2)
	{
		value += smoothNoise(x / scale, y / scale) * scale;
	}
	
	return ((128.0 * value) / static_cast<float>(size));
}

TileType mapHeight(uint8_t value)
{
	uint8_t step = 255 / tileTypeCount;

	for(uint8_t i = 0; i < tileTypeCount; ++i)
		if(value < (i * step))
			return static_cast<TileType>(i);

	return lastTileType;
}

TileType generateTile(uint8_t x, uint8_t y)
{
	return mapHeight(static_cast<uint8_t>(smoothNoise(x, y) * 256));
}

TileType generateTile(uint8_t x, uint8_t y, size_t size)
{
	return mapHeight(static_cast<uint8_t>(turbulence(x, y, size)));
}

constexpr size_t width = 16;
constexpr size_t height = 8;

TileType tileBuffer[height][width];

uint8_t mapX = 128;
uint8_t mapY = 128;
uint8_t mapSize = 64;

void fillBuffer(uint8_t baseX, uint8_t baseY, uint8_t size)
{
	for(size_t y = 0; y < height; ++y)
		for(size_t x = 0; x < width; ++x)
			tileBuffer[y][x] = generateTile(baseX + x, baseY + y, size);
}

void fillColumn(uint8_t baseX, uint8_t baseY, uint8_t x, uint8_t size)
{
	for(size_t y = 0; y < height; ++y)
		tileBuffer[y][x] = generateTile(baseX + x, baseY + y, size);
}

void fillRow(uint8_t baseX, uint8_t baseY, uint8_t y, uint8_t size)
{
	for(size_t x = 0; x < width; ++x)
		tileBuffer[y][x] = generateTile(baseX + x, baseY + y, size);
}

void moveRight()
{
	if(mapX < 255)
	{
		// Increase map coordinate
		++mapX;

		// Move buffer contents left
		for(size_t x = 1; x < width; ++x)
			for(size_t y = 0; y < height; ++y)
				tileBuffer[y][x - 1] = tileBuffer[y][x];
		
		// Generate new column
		fillColumn(mapX, mapY, (width - 1), mapSize);
	}
}

void moveLeft()
{
	if(mapX > 0)
	{
		// Decrease map coordinate
		--mapX;

		// Move buffer contents right
		for(size_t x = (width - 1); x > 0; --x)
			for(size_t y = 0; y < height; ++y)
				tileBuffer[y][x] = tileBuffer[y][x - 1];
		
		// Generate new column
		fillColumn(mapX, mapY, 0, mapSize);
	}
}

void moveDown()
{
	if(mapY < 255)
	{
		// Increase map coordinate
		++mapY;

		// Move buffer contents up
		for(size_t y = 1; y < height; ++y)
			for(size_t x = 0; x < width; ++x)
				tileBuffer[y - 1][x] = tileBuffer[y][x];
		
		// Generate new row
		fillRow(mapX, mapY, height - 1, mapSize);
	}
}

void moveUp()
{
	if(mapY > 0)
	{
		// Decrease map coordinate
		--mapY;

		// Move buffer contents down
		for(size_t y = (height - 1); y > 0; --y)
			for(size_t x = 0; x < width; ++x)
				tileBuffer[y][x] = tileBuffer[y - 1][x];
		
		// Generate new column
		fillRow(mapX, mapY, 0, mapSize);
	}
}

void renderBuffer()
{
	for(size_t y = 0; y < height; ++y)
		for(size_t x = 0; x < width; ++x)
		{
			const int16_t drawX = (x * tileWidth);
			const int16_t drawY = (y * tileHeight);
			
			const TileType tile = tileBuffer[y][x];
			const size_t tileIndex = static_cast<size_t>(tile);
			
			Sprites::drawOverwrite(drawX, drawY, tiles, tileIndex);
		}
}

void setup()
{
	arduboy.begin();
	fillBuffer(mapX, mapY, mapSize);
}

void loop()
{
	if(!arduboy.nextFrame())
		return;
		
	arduboy.pollButtons();
	
	if(arduboy.pressed(LEFT_BUTTON))
		moveLeft();
	
	if(arduboy.pressed(RIGHT_BUTTON))
		moveRight();
	
	if(arduboy.pressed(UP_BUTTON))
		moveUp();
	
	if(arduboy.pressed(DOWN_BUTTON))
		moveDown();
	
	arduboy.clear();
	
	renderBuffer();
	
	arduboy.display();
}