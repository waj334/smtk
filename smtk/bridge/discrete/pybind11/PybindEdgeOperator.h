//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================

#ifndef pybind_smtk_bridge_discrete_operators_EdgeOperator_h
#define pybind_smtk_bridge_discrete_operators_EdgeOperator_h

#include <pybind11/pybind11.h>

#include "smtk/bridge/discrete/operators/EdgeOperator.h"


namespace py = pybind11;

PySharedPtrClass< smtk::bridge::discrete::EdgeOperator, smtk::operation::NewOp > pybind11_init_smtk_bridge_discrete_EdgeOperator(py::module &m)
{
  PySharedPtrClass< smtk::bridge::discrete::EdgeOperator, smtk::operation::NewOp > instance(m, "EdgeOperator");
  instance
    .def("classname", &smtk::bridge::discrete::EdgeOperator::classname)
    .def_static("create", (std::shared_ptr<smtk::bridge::discrete::EdgeOperator> (*)()) &smtk::bridge::discrete::EdgeOperator::create)
    .def_static("create", (std::shared_ptr<smtk::bridge::discrete::EdgeOperator> (*)(::std::shared_ptr<smtk::bridge::discrete::EdgeOperator> &)) &smtk::bridge::discrete::EdgeOperator::create, py::arg("ref"))
    .def("shared_from_this", (std::shared_ptr<smtk::bridge::discrete::EdgeOperator> (smtk::bridge::discrete::EdgeOperator::*)()) &smtk::bridge::discrete::EdgeOperator::shared_from_this)
    .def("shared_from_this", (std::shared_ptr<const smtk::bridge::discrete::EdgeOperator> (smtk::bridge::discrete::EdgeOperator::*)() const) &smtk::bridge::discrete::EdgeOperator::shared_from_this)
    .def("name", &smtk::bridge::discrete::EdgeOperator::name)
    .def("className", &smtk::bridge::discrete::EdgeOperator::className)
    .def("ableToOperate", &smtk::bridge::discrete::EdgeOperator::ableToOperate)
    ;
  return instance;
}

#endif
