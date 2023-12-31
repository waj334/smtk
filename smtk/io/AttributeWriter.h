//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================
// .NAME AttributeWriter.h -
// .SECTION Description
// .SECTION See Also

#ifndef smtk_io_AttributeWriter_h
#define smtk_io_AttributeWriter_h

#include "smtk/CoreExports.h"
#include "smtk/PublicPointerDefs.h"
#include "smtk/SystemConfig.h"
#include <string>

namespace smtk
{
namespace io
{
class Logger;
class SetWriter;
class XmlStringWriter;
class SMTKCORE_EXPORT AttributeWriter
{
public:
  AttributeWriter();

  // Set an explicit file version - for stable builds
  bool setFileVersion(unsigned int version);
  // Set the latest file version - for development
  void setMaxFileVersion();
  unsigned int fileVersion() const;

  // Returns true if there was a problem with writing the file
  bool write(
    smtk::attribute::ResourcePtr resource,
    const std::string& filename,
    smtk::io::Logger& logger);
  bool writeContents(
    smtk::attribute::ResourcePtr resource,
    std::string& filecontents,
    smtk::io::Logger& logger,
    bool no_declaration = false);
  //Control which sections of the attribute resource should be written out
  // By Default all sections are processed.  These are advance options!!

  // If val is false then Advance Levels will not be saved
  void includeAdvanceLevels(bool val) { m_includeAdvanceLevels = val; }

  // If val is false then Analyses will not be saved
  void includeAnalyses(bool val) { m_includeAnalyses = val; }

  // If val is false then Attribute Associations will not be saved
  void includeAttributeAssociations(bool val) { m_includeAttributeAssociations = val; }

  // If val is false then Evaluators will not be saved.
  void includeEvaluators(bool val) { m_includeEvaluators = val; }

  // If val is false then definitions will not be saved
  void includeDefinitions(bool val) { m_includeDefinitions = val; }

  // If val is false then Attribute Instances will not be saved
  void includeInstances(bool val) { m_includeInstances = val; }

  // If val is false then the Resource's associations will not be saved
  void includeResourceAssociations(bool val) { m_includeResourceAssociations = val; }

  // If val is false then the Resource's ID will not be saved
  void includeResourceID(bool val) { m_includeResourceID = val; }

  // If val is false then UniqueRoles will not be saved
  void includeUniqueRoles(bool val) { m_includeUniqueRoles = val; }

  // If val is false then views will not be saved
  void includeViews(bool val) { m_includeViews = val; }

  // If val is true then when write(...) will use include file sections in the produced XML
  // and will write out included files (based on the resource's Directory Info) as well
  // This is ignored when calling writeContents(...).  The default is false
  void useDirectoryInfo(bool val) { m_useDirectoryInfo = val; }

  // Write/WriteAsContents will produce a library like XML file containing
  // only attribute instances that are based on the provided list of definitions.
  // If the list is empty then all attributes will be saved.
  // This method will, by default, not include Analyses, AdvanceLevels, Definitions, of
  // View sections.  It will also not include The Attribute Resource's ID or Associations
  // nor will it include Associations of the attributes being saved.
  /// - these sections can be included by calling enabling them after calling
  // this method.
  void treatAsLibrary(const std::vector<smtk::attribute::DefinitionPtr>& includedDefs);

  // Restricts the types of attribute instances written out to those derived from a
  // specified list.  If the list is empty then all attributes will be saved.
  // Any redundant definitions (definitions that can be derived from others in the list)
  // are removed.
  void setIncludedDefinitions(const std::vector<smtk::attribute::DefinitionPtr>& includedDefs);

  // Return the list of definitions to be used to filter attribute instances.
  const std::vector<smtk::attribute::DefinitionPtr>& includedDefinitions() const
  {
    return m_includedDefs;
  }

  // Restricts the types of attribute instances written out to those *not* derived from a
  // specified list.  If the list is empty then no attributes will be omitted.
  // Inclusions are applied before exclusions.
  // This allows you to include base definitions (by adding them to the inclusions) while
  // omitting definitions derived from the base (by adding the derived definition(s) to
  // the exclusions).
  void setExcludedDefinitions(const std::set<smtk::attribute::DefinitionPtr>& excludedDefs);

  // Return the list of definitions to be used to filter out unwanted attribute instances.
  const std::set<smtk::attribute::DefinitionPtr>& excludedDefinitions() const
  {
    return m_excludedDefs;
  }

protected:
  // Instantiates internal writer
  // Caller is responsible for deleting the instance
  XmlStringWriter* newXmlStringWriter(
    smtk::attribute::ResourcePtr resource,
    smtk::io::Logger& logger) const;

private:
  // NOLINTNEXTLINE(modernize-use-default-member-init)
  unsigned int m_fileVersion;
  bool m_includeAdvanceLevels{ true };
  bool m_includeAnalyses{ true };
  bool m_includeAttributeAssociations{ true };
  bool m_includeDefinitions{ true };
  bool m_includeEvaluators{ true };
  bool m_includeInstances{ true };
  bool m_includeResourceAssociations{ true };
  bool m_includeResourceID{ true };
  bool m_includeUniqueRoles{ true };
  bool m_includeViews{ true };
  bool m_useDirectoryInfo{ false };
  std::vector<smtk::attribute::DefinitionPtr> m_includedDefs;
  std::set<smtk::attribute::DefinitionPtr> m_excludedDefs;
};
} // namespace io
} // namespace smtk

#endif /* smtk_io_AttributeWriter_h */
