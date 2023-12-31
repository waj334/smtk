//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================
#ifndef smtk_model_DefaultSession_h
#define smtk_model_DefaultSession_h

#include "smtk/model/Session.h"

namespace smtk
{
namespace model
{

/**\brief A session that does no transcription.
  *
  * Sessions of this type do no transcription for one of two reasons:
  * 1. They hold "native" SMTK models, or
  * 2. They forward all requests to a remote session (which they
  *    "back" with local storage).
  *
  * Instances of this session report their name() as "native".
  * In the first case, they have an empty remoteName().
  * In the second, their remoteName() will be the type of the
  * session they provide backing storage for.
  *
  * SMTK does not provide a protocol or transport for forwarding requests.
  * However, it does provide mechanisms for serialization and deserialization
  * of storage, operators, and operator results.
  * Since Session instances are the interface between modeling kernels such
  * as OpenCascade, this makes them the place where requests must
  * be forwarded if they are going to be.
  *
  * By default this session will create instances of RemoteOperation
  * when asked for an Operation.
  * The RemoteOperation class calls virtual methods on DefaultSession
  * in order to perform operations remotely (i.e., DefaultSession acts
  * as a delegate for operators.)
  * Subclasses which inherit DefaultSession in order to provide request
  * forwarding must implement transcribeInternal, ableToOperateDelegate,
  * and operateDelegate methods.
  *
  * See the unitDefaultSession test for an example of how
  * forwarding works.
  *
  * See the Remus remote session for an implementation that can
  * forward requests to Remus workers.
  */
class SMTKCORE_EXPORT DefaultSession : public Session
{
public:
  smtkTypeMacro(DefaultSession);
  smtkSuperclassMacro(Session);
  smtkSharedFromThisMacro(Session);
  smtkCreateMacro(DefaultSession);

  void backsRemoteSession(
    const std::string& remoteSessionName,
    const smtk::common::UUID& sessionId);
  virtual std::string remoteName() const;
  // OperationPtr op(const std::string& opName) const override;

  /// Return an empty string. Subclasses should override this.
  std::string defaultFileExtension(const Model&) const override { return ""; }

protected:
  friend class RemoteOperation;

  DefaultSession();

  SessionInfoBits transcribeInternal(const EntityRef& entity, SessionInfoBits flags, int depth = -1)
    override;

  void setImportingOperations(bool isImporting);

  std::string m_remoteSessionName;
};

} // namespace model
} // namespace smtk

#endif // smtk_model_DefaultSession_h
