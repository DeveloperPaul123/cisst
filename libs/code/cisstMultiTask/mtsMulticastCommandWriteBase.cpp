/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id: mtsMulticastCommandWriteBase.cpp,v 1.3 2008/09/04 05:15:38 anton Exp $

  Author(s):  Ankur Kapoor, Peter Kazanzides, Anton Deguet
  Created on: 2004-04-30

  (C) Copyright 2004-2007 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/

#include <cisstMultiTask/mtsMulticastCommandWriteBase.h>


void mtsMulticastCommandWriteBase::AddCommand(BaseType * command) {
    if (command) {
        if (typeid(*(command->GetArgumentPrototype())) != typeid(*(this->GetArgumentPrototype()))) {
            CMN_LOG(1) << "Class mtsMulticastCommandWriteBase: AddCommand: command argument type don't match" << std::endl;
            exit(0);
        } else {
            this->Commands.push_back(command);
        }
    }
}


void mtsMulticastCommandWriteBase::ToStream(std::ostream & outputStream) const {
    outputStream << "mtsMulticastCommandWrite: " << this->Name;
    if (Commands.size() != 0) {
        outputStream << "\n  Registered observers:" << std::endl;
        for (unsigned int i = 0; i < Commands.size(); i++) {
            outputStream << "  . CallBack [" << i << "]: " << *(Commands[i]);
        }
    } else {
        outputStream << " with no registered observers";
    }
}

