//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================
#include "smtk/markup/OntologyIdentifier.h"

#include "smtk/markup/Traits.h"

namespace smtk
{
namespace markup
{

OntologyIdentifier::~OntologyIdentifier() = default;

void OntologyIdentifier::initialize(
  const nlohmann::json& data,
  smtk::resource::json::Helper& helper)
{
  (void)data;
  (void)helper;
  m_ontologyId = smtk::string::Token::fromHash(data["ontology_id"].get<smtk::string::Hash>());
}

bool OntologyIdentifier::setOntologyId(const smtk::string::Token& ontologyId)
{
  if (m_ontologyId == ontologyId)
  {
    return false;
  }
  m_ontologyId = ontologyId;
  return true;
}

const smtk::string::Token& OntologyIdentifier::ontologyId() const
{
  return m_ontologyId;
}

smtk::string::Token& OntologyIdentifier::ontologyId()
{
  return m_ontologyId;
}

ArcEndpointInterface<arcs::OntologyIdentifiersToIndividuals, ConstArc, OutgoingArc>
OntologyIdentifier::subjects() const
{
  return this->outgoing<arcs::OntologyIdentifiersToIndividuals>();
}

ArcEndpointInterface<arcs::OntologyIdentifiersToIndividuals, NonConstArc, OutgoingArc>
OntologyIdentifier::subjects()
{
  return this->outgoing<arcs::OntologyIdentifiersToIndividuals>();
}

ArcEndpointInterface<arcs::OntologyToIdentifiers, ConstArc, IncomingArc>
OntologyIdentifier::parent() const
{
  return this->incoming<arcs::OntologyToIdentifiers>();
}

ArcEndpointInterface<arcs::OntologyToIdentifiers, NonConstArc, IncomingArc>
OntologyIdentifier::parent()
{
  return this->incoming<arcs::OntologyToIdentifiers>();
}

} // namespace markup
} // namespace smtk
