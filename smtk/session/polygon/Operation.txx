//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================
#ifndef smtk_session_polygon_Operation_txx
#define smtk_session_polygon_Operation_txx

#include "smtk/session/polygon/Operation.h"

#include "smtk/attribute/IntItem.h"

#include "smtk/model/Edge.h"

namespace smtk
{
namespace session
{
namespace polygon
{

template<typename T, typename U, typename V>
void Operation::pointsForLoop(
  T& polypts,
  int numPtsToUse,
  U& curCoord,
  U finish,
  int numCoordsPerPoint,
  V pmodel)
{
  // Map user point coordinates into model points.
  polypts.reserve(polypts.size() + numPtsToUse);
  U specEnd = curCoord + numPtsToUse * numCoordsPerPoint;
  for (; curCoord != finish && curCoord != specEnd; curCoord += numCoordsPerPoint)
  {
    internal::Point pt = pmodel->projectPoint(curCoord, curCoord + numCoordsPerPoint);
    if (!polypts.empty() && polypts.back() == pt)
    {
      continue;
    }
    polypts.push_back(pt);
  }
}

template<typename T, typename U, typename V, typename W>
void Operation::pointsForLoop(
  T& polypts,
  int numEdgesToUse,
  U& curEdge,
  U edgesFinish,
  V& curEdgeDir,
  V edgeDirFinish,
  W& outerLoopEdges)
{
  auto ceent = std::dynamic_pointer_cast<smtk::model::Entity>(*curEdge);
  auto resource = std::static_pointer_cast<smtk::session::polygon::Resource>(ceent->resource());

  auto edgesStop = curEdge + numEdgesToUse;
  smtk::attribute::IntItem::value_type::const_iterator edgeDirStop = curEdgeDir + numEdgesToUse;
  //model::Orientation lastOrient = model::UNKNOWN;
  //model::Edge lastEdge;
  bool firstInLoop = true;
  for (; curEdge != edgesFinish && curEdge != edgesStop && curEdgeDir != edgeDirFinish &&
       curEdgeDir != edgeDirStop;
       ++curEdge, ++curEdgeDir)
  {
    // TODO: More sanity checking... verify prev-tail === head.
    if (firstInLoop)
    {
      firstInLoop = false;
    }
    else
    { // Don't double-include endpoints.
      polypts.erase(polypts.end() - 1);
    }
    internal::EdgePtr edgeRec = resource->template findStorage<internal::edge>((*curEdge)->id());
    if (!edgeRec)
    {
      smtk::model::EntityRef edgeRef(std::dynamic_pointer_cast<smtk::model::Entity>(*curEdge));
      smtkErrorMacro(
        smtk::io::Logger::instance(), "Skipping missing edge record " << edgeRef.name());
      continue;
    }
    outerLoopEdges.push_back(std::make_pair(
      smtk::model::Edge(std::dynamic_pointer_cast<smtk::model::Entity>(*curEdge)),
      *curEdgeDir != 0));
    if (*curEdgeDir < 0)
    {
      polypts.insert(polypts.end(), edgeRec->pointsRBegin(), edgeRec->pointsREnd());
    }
    else
    {
      polypts.insert(polypts.end(), edgeRec->pointsBegin(), edgeRec->pointsEnd());
    }
  }
}

template<typename T, typename U>
void Operation::pointsInLoopOrderFromOrientedEdges(
  T& polypts,
  U begin,
  U end,
  smtk::shared_ptr<internal::pmodel> pmodel)
{
  auto resource = std::static_pointer_cast<smtk::session::polygon::Resource>(
    begin->first.component()->resource());

  (void)pmodel;
  U oit; // Oriented-edges-of-loop iterator
  bool firstInLoop = true;
  for (oit = begin; oit != end; ++oit)
  {
    // Add points of edge to polypts in loop order
    // TODO: More sanity checking... verify prev-tail === head.
    if (firstInLoop)
    {
      firstInLoop = false;
    }
    else
    { // Don't double-include endpoints.
      polypts.erase(polypts.end() - 1);
    }
    internal::EdgePtr edgeRec = resource->template findStorage<internal::edge>(oit->first.entity());
    if (!edgeRec)
    {
      std::cerr << "Skipping missing edge record " << oit->first.name() << "\n";
      continue;
    }
    if (!oit->second)
    {
      polypts.insert(polypts.end(), edgeRec->pointsRBegin(), edgeRec->pointsREnd());
    }
    else
    {
      polypts.insert(polypts.end(), edgeRec->pointsBegin(), edgeRec->pointsEnd());
    }
  }
}

} // namespace polygon
} // namespace session
} // namespace smtk

#endif // smtk_session_polygon_Operation_txx
