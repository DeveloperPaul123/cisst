/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id$

  Author(s):  Ankur Kapoor, Peter Kazanzides, Anton Deguet
  Created on: 2005-05-02

  (C) Copyright 2005-2010 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/


#include <cisstMultiTask/mtsCommandQueuedWrite.h>


void mtsCommandQueuedWriteBase::ToStream(std::ostream & outputStream) const {
    outputStream << "mtsCommandQueuedWrite: MailBox \"";
    if (this->MailBox) {
        outputStream << this->MailBox->GetName();
    } else {
        outputStream << "Undefined";
    }
    outputStream << "\" for command " << *(this->ActualCommand)
                 << " currently " << (this->IsEnabled() ? "enabled" : "disabled");
}


mtsBlockingType mtsCommandQueuedWriteBase::BlockingFlagGet(void)
{
    return *(this->BlockingFlagQueue.Get());
}


mtsCommandQueuedWriteGeneric::mtsCommandQueuedWriteGeneric(mtsMailBox * mailBox, mtsCommandWriteBase * actualCommand, size_t size):
    BaseType(mailBox, actualCommand, size),
    ArgumentQueueSize(size),
    ArgumentsQueue()
{
    if (this->ActualCommand) {
        this->SetArgumentPrototype(ActualCommand->GetArgumentPrototype());
    }
    const mtsGenericObject * argumentPrototype = dynamic_cast<const mtsGenericObject *>(this->GetArgumentPrototype());
    if (argumentPrototype) {
        ArgumentsQueue.SetSize(size, *argumentPrototype);
        BlockingFlagQueue.SetSize(size, MTS_NOT_BLOCKING);
    } else {
        CMN_LOG_INIT_DEBUG << "Class mtsCommandQueuedWriteGeneric: constructor: can't find argument prototype from actual command \""
                           << this->GetName() << "\"" << std::endl;
    }
}

void mtsCommandQueuedWriteGeneric::Allocate(size_t size)
{
    if (ArgumentsQueue.GetSize() != size) {
        if (ArgumentsQueue.GetSize() > 0) {
            // Probably should never happen
            CMN_LOG_INIT_WARNING << "Class mtsCommandQueuedWriteGeneric: Allocate: changing ArgumentsQueue size from " << ArgumentsQueue.GetSize()
                                 << " to " << size << std::endl;
        }
        const mtsGenericObject * argumentPrototype = dynamic_cast<const mtsGenericObject *>(this->GetArgumentPrototype());
        if (argumentPrototype) {
            CMN_LOG_INIT_DEBUG << "Class mtsCommandQueuedWriteGeneric: Allocate: resizing argument queue to " << size
                               << " with \"" << argumentPrototype->Services()->GetName() << "\"" << std::endl;
            ArgumentsQueue.SetSize(size, *argumentPrototype);
            BlockingFlagQueue.SetSize(size, MTS_NOT_BLOCKING);
        } else {
            CMN_LOG_INIT_ERROR << "Class mtsCommandQueuedWriteGeneric: Allocate: can't find argument prototype from actual command \""
                               << this->GetName() << "\"" << std::endl;
        }
     }
}


mtsExecutionResult mtsCommandQueuedWriteGeneric::Execute(const mtsGenericObject & argument,
                                                         mtsBlockingType blocking)
{
    // check if this command is enabled
    if (!this->IsEnabled()) {
        return mtsExecutionResult::COMMAND_DISABLED;
    }
    // check if there is a mailbox (i.e. if the command is associated to an interface
    if (!MailBox) {
        CMN_LOG_RUN_ERROR << "Class mtsCommandQueuedWriteGeneric: Execute: no mailbox for \""
                          << this->Name << "\"" << std::endl;
        return mtsExecutionResult::COMMAND_HAS_NO_MAILBOX;
    }
    // copy the argument to the local storage.
    if (!ArgumentsQueue.Put(argument)) {
        CMN_LOG_RUN_ERROR << "Class mtsCommandQueuedWriteGeneric: Execute: ArgumentsQueue full for \""
                          << this->Name << "\"" << std::endl;
        return mtsExecutionResult::COMMAND_ARGUMENT_QUEUE_FULL;
    }
    // copy the blocking flag to the local storage.
    if (!BlockingFlagQueue.Put(blocking)) {
        CMN_LOG_RUN_ERROR << "Class mtsCommandQueuedWriteGeneric: Execute: BlockingFlagQueue full for \""
                          << this->Name << "\"" << std::endl;
        ArgumentsQueue.Get(); // pop argument
        return mtsExecutionResult::COMMAND_ARGUMENT_QUEUE_FULL;
    }
    // finally try to queue to mailbox
    if (!MailBox->Write(this)) {
        CMN_LOG_RUN_ERROR << "Class mtsCommandQueuedWriteGeneric: Execute: mailbox full for \""
                          << this->Name << "\"" << std::endl;
        ArgumentsQueue.Get();  // pop argument and blocking flag from local storage
        BlockingFlagQueue.Get();
        return mtsExecutionResult::INTERFACE_COMMAND_MAILBOX_FULL;
    }
    return mtsExecutionResult::COMMAND_QUEUED;
}


void mtsCommandQueuedWriteGeneric::ToStream(std::ostream & outputStream) const {
    outputStream << "mtsCommandQueuedWrite: MailBox \"";
    if (this->MailBox) {
        outputStream << this->MailBox->GetName();
    } else {
        outputStream << "Undefined";
    }
    outputStream << "\" for command " << *(this->ActualCommand)
                 << " currently " << (this->IsEnabled() ? "enabled" : "disabled");
}

