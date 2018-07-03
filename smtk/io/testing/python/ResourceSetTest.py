#!/usr/bin/python
import sys
#=============================================================================
#
#  Copyright (c) Kitware, Inc.
#  All rights reserved.
#  See LICENSE.txt for details.
#
#  This software is distributed WITHOUT ANY WARRANTY; without even
#  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
#  PURPOSE.  See the above copyright notice for more information.
#
#=============================================================================
from pprint import pprint
import smtk
import smtk.attribute
import smtk.common


def RSTest():
    """Tests for ResourceSet
    Currently does not test for ResourceInfo
    as it is not implemented in the wrapper yet
    Otherwise this is a direct manual port of the cxx test"""

    status = 0
    result = False
    n = 0
    resourceSet = smtk.resource.Set()

    resource1 = smtk.attribute.Resource.New()
    print(resource1)
    result = resourceSet.add(
        resource1, "resource1", "", smtk.resource.Set.TEMPLATE)

    n = resourceSet.numberOfResources()
    if result == False:
        print("add() call failed")
        status = status + 1
    elif n != 1:
        print("Wrong number of resources: %i, should be 1" % n)
        status = status + 1

    resource2 = smtk.attribute.Resource.New()
    result = resourceSet.add(
        resource2, "resource2", "path2", smtk.resource.Set.INSTANCE)

    n = resourceSet.numberOfResources()
    if result == False:
        print("add() call failed")
        status = status + 1
    elif n != 2:
        print("Wrong number of resources: %i, should be 2" % n)
        status = status + 1

    result = resourceSet.add(
        resource1, "resource1-different-id", "", smtk.resource.Set.SCENARIO)
    n = resourceSet.numberOfResources()
    if result == False:
        print("add() call failed")
        status = status + 1
    elif n != 3:
        print("Wrong number of resources: %i, should be 3" % n)
        status = status + 1

    result = resourceSet.add(resource2, "resource2")
    n = resourceSet.numberOfResources()
    if result == True:
        print("add() call didn't fail failed")
        status = status + 1
    elif n != 3:
        print("Wrong number of resources: %i, should be 3" % n)
        status = status + 1

    ids = resourceSet.resourceIds()
    if len(ids) != 3:
        print("Wrong number of ids: %i, should be 3")
        status = status + 1
    else:
        expectedNames = [
            "resource1", "resource2", "resource1-different-id"]
        for i in range(len(ids)):
            if ids[i] != expectedNames[i]:
                print("Wrong resource name %s, should be %s" %
                      (ids[i], expectedNames[i]))
                status = status + 1

    # TODO: ResourceInfo tests (function not implemented)
    # TODO: ResourcePtr is not implemented

    resource = resourceSet.get("resource2")
    if resource == None:
        print("get() failed")
        status = status + 1

    print("Number of errors: %i" % status)
    return status


def test():
    return RSTest()


if __name__ == '__main__':
    sys.exit(test())
