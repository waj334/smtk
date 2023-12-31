//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================
#ifndef smtk_session_polygon_Session_h
#define smtk_session_polygon_Session_h

#include "smtk/session/polygon/Exports.h"
#include "smtk/session/polygon/PointerDefs.h"
#include "smtk/session/polygon/internal/Entity.h"

#include "smtk/model/Session.h"

namespace smtk
{
namespace model
{
class Model;
}
namespace session
{
namespace polygon
{
namespace internal
{
class pmodel;
class vertex;
} // namespace internal

/**\brief Methods that handle translation between polygon and SMTK instances.
  *
  * While the TDUUID class keeps a map from SMTK UUIDs to polygon ToolDataUser
  * pointers, this is not enough to handle everything SMTK provides:
  * there is no way to track cell-use or shell entities since they do
  * not inherit ToolDataUser instances. Also, some engines (e.g., facet)
  * do not appear to store some entity types (e.g., RefGroup).
  *
  * Also, simply loading a polygon file does not translate the entire model
  * into SMTK; instead, it assigns UUIDs to entities if they do not already
  * exist. This class (Session) provides a method for requesting the
  * entity, arrangement, and/or tessellation information for a UUID be
  * mapped into SMTK from polygon.
  */
class SMTKPOLYGONSESSION_EXPORT Session : public smtk::model::Session
{
public:
  smtkTypeMacro(Session);
  smtkSuperclassMacro(smtk::model::Session);
  smtkSharedFromThisMacro(smtk::model::Session);
  smtkCreateMacro(smtk::model::Session);
  typedef smtk::model::SessionInfoBits SessionInfoBits;
  ~Session() override;

  Session(const Session&) = delete;
  Session& operator=(const Session&) = delete;

  SessionInfoBits allSupportedInformation() const override;

  template<typename T, typename U, typename V>
  void consistentInternalDelete(T& container, U& modified, V& expunged, bool logDebug);

  std::string defaultFileExtension(const smtk::model::Model&) const override;

  internal::EntityIdToPtr::iterator beginStorage();
  internal::EntityIdToPtr::iterator endStorage();
  internal::EntityIdToPtr::const_iterator beginStorage() const;
  internal::EntityIdToPtr::const_iterator endStorage() const;

  void addStorage(
    const smtk::common::UUID& uid,
    smtk::session::polygon::internal::entity::Ptr storage);
  bool removeStorage(const smtk::common::UUID& uid);

protected:
  friend class Neighborhood;
  friend class Operation;
  friend class Resource;
  friend class internal::pmodel;
  friend class SessionIOJSON;

  Session();

  smtk::model::SessionInfoBits transcribeInternal(
    const smtk::model::EntityRef& entity,
    SessionInfoBits requestedInfo,
    int depth = -1) override;

  bool removeFaceReferences(const smtk::model::Face& f);
  bool removeEdgeReferences(const smtk::model::Edge& e);
  bool removeVertReferences(const smtk::model::Vertex& v);

  template<typename T>
  typename T::Ptr findStorage(const smtk::common::UUID& uid)
  {
    internal::EntityIdToPtr::iterator it = m_storage.find(uid);
    if (it != m_storage.end())
      return smtk::dynamic_pointer_cast<T>(it->second);
    static typename T::Ptr blank;
    return blank;
  }

  template<typename T>
  T findOrAddStorage(const smtk::common::UUID& uid)
  {
    internal::EntityIdToPtr::iterator it = m_storage.find(uid);
    if (it != m_storage.end())
      return smtk::dynamic_pointer_cast<T>(it->second);

    T blank = T::create();
    it = m_storage
           .insert(internal::EntityIdToPtr::value_type(
             uid, smtk::dynamic_pointer_cast<internal::entity>(blank)))
           .first;
    return smtk::dynamic_pointer_cast<T>(it->second);
  }

  smtk::model::SessionIOPtr createIODelegate(const std::string& format) override;

  internal::EntityIdToPtr::iterator findStorageIterator(const smtk::common::UUID& uid);

  internal::EntityIdToPtr m_storage;
  int m_nextModelNumber{ 0 };
};

} // namespace polygon
} //namespace session
} // namespace smtk

#endif // smtk_session_polygon_Session_h
