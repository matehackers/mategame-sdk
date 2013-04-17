/*
  Hackvision Controller library
  Copyright (C) 2010 nootropic design, LLC
  All rights reserved.
 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
*/

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdio.h>

#include "ButtonController.h"

ButtonController::ButtonController() {
}

ButtonController::ButtonController(uint8_t left, uint8_t right, uint8_t up, uint8_t down, uint8_t fire) {
}


void ButtonController::init(uint8_t left, uint8_t right, uint8_t up, uint8_t down, uint8_t fire) {
}


uint8_t ButtonController::leftPressed(void) {
  return buttons[ButtonController::LEFT];
}

uint8_t ButtonController::rightPressed(void) {
  return buttons[ButtonController::RIGHT];
}

uint8_t ButtonController::upPressed(void) {
  return buttons[ButtonController::UP];
}

uint8_t ButtonController::downPressed(void) {
  return buttons[ButtonController::DOWN];
}

uint8_t ButtonController::firePressed(void) {
  return buttons[ButtonController::FIRE];
}

// Preinstantiate default ButtonController object for the integrated controller
ButtonController Controller = ButtonController();
