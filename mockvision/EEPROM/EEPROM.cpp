/*
  EEPROM.cpp - EEPROM library
  Copyright (c) 2006 David A. Mellis.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/******************************************************************************
 * Includes
 ******************************************************************************/

#include "EEPROM.h"

EEPROMClass::EEPROMClass()
{

  if ( !(file = fopen("virtual_EEPROM", "r+")) &&
       !(file = fopen("virtual_EEPROM", "w+"))
  )
  {
    
    fprintf(stderr, "Error opening EEPROM file\n");
  }

}

EEPROMClass::~EEPROMClass()
{
  fclose(file);
}

uint8_t EEPROMClass::read(int address)
{
  uint8_t data;

  fseek(file, address, SEEK_SET);

  fread(&data, sizeof(uint8_t), 1, file);

  return data;
}

void EEPROMClass::write(int address, uint8_t value)
{
  fseek(file, address, SEEK_SET);

  fwrite(&value, sizeof(uint8_t), 1, file);
  fflush(file);
}

EEPROMClass EEPROM;
