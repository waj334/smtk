//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================
#ifndef smtk_operation_Observer_h
#define smtk_operation_Observer_h

#include "smtk/common/Observers.h"
#include "smtk/operation/Operation.h"

namespace smtk
{
namespace operation
{

/**\brief Enumerate events that an operation may encounter.
 *
 * No event is provided for operation deletion because
 * (1) operation deletion is not managed and
 * (2) "this" is not complete in class destructors (subclass data is already
 * freed).
 * So, there is no easy way to observe when an operation is about to be
 * deleted but is still valid.
 */
enum class EventType
{
  WILL_OPERATE, //!< The operation will commence if no observers cancel it.
  DID_OPERATE   //!< The operation has completed or been canceled.
};

/**\brief Signature for methods observing operations.
  *
  * Observers must not modify nor retain a pointer to the provided operation.
  * Observers must return an integer. If the event type is WILL_OPERATE and
  * the integer is non-zero, the operation will be canceled.
  */
typedef std::function<int(const Operation&, EventType, Operation::Result)> Observer;

/// Operation observers are a specialization of the common SMTK observer pattern.
typedef smtk::common::Observers<Observer> Observers;
} // namespace operation
} // namespace smtk

#endif // smtk_operation_Observer_h
