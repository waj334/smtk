import sys
# =============================================================================
#
#  Copyright (c) Kitware, Inc.
#  All rights reserved.
#  See LICENSE.txt for details.
#
#  This software is distributed WITHOUT ANY WARRANTY; without even
#  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
#  PURPOSE.  See the above copyright notice for more information.
#
# =============================================================================
import smtk
from smtk import attribute
from smtk import io

import smtk.testing
import os


class TestAttributeItemByPath(smtk.testing.TestCase):

    def setUp(self):
        if smtk.testing.DATA_DIR == '':
            self.skipTest('SMTK test-data directory not provided')

        self.resource = smtk.attribute.Resource.create()
        logger = smtk.io.Logger()
        reader = smtk.io.AttributeReader()
        filenm = os.path.join(smtk.testing.DATA_DIR, 'attribute',
                              'attribute_collection', 'HydraTemplateV1.sbt')
        status = reader.read(self.resource, filenm, logger)
        print(
            '\n'.join([logger.record(i).message for i in range(logger.numberOfRecords())]))
        self.assertFalse(status, 'Could not read {fn}'.format(fn=filenm))

    def testItemInSimplePath(self):
        att = self.resource.createAttribute('hydrostat')
        itemInSimplePath = att.itemAtPath('Hydrostat', '/')
        self.assertIsNotNone(itemInSimplePath, 'Could not find expected item.')
        self.assertEqual(itemInSimplePath.name(), 'Hydrostat',
                         'Got wrong attribute "{nm}".'.format(nm=itemInSimplePath.name()))

    def testLeadingSlash(self):
        att = self.resource.createAttribute('hydrostat')
        itemWithLeadingSlash = att.itemAtPath('/Hydrostat', '/')
        self.assertIsNotNone(itemWithLeadingSlash,
                             'Could not find expected item.')
        self.assertEqual(itemWithLeadingSlash.name(), 'Hydrostat',
                         'Got wrong attribute "{nm}".'.format(nm=itemWithLeadingSlash.name()))

    def testItemInChildPath(self):
        att = self.resource.createAttribute('BasicTurbulenceModel')
        itemInChildren = att.itemAtPath('Method/prandtl', '/')
        self.assertIsNotNone(itemInChildren, 'Could not find expected item.')
        self.assertEqual(itemInChildren.name(
        ), 'prandtl', 'Got wrong attribute "{nm}".'.format(nm=itemInChildren.name()))

    def testItemInGroupPath(self):
        att = self.resource.createAttribute('InitialConditions')
        itemInGroup = att.itemAtPath('InitialConditions/Velocity', '/')
        self.assertIsNotNone(itemInGroup, 'Could not find expected item.')
        self.assertEqual(itemInGroup.name(), 'Velocity',
                         'Got wrong attribute "{nm}".'.format(nm=itemInGroup.name()))
        # Test using the sub-group index as part of the path.
        itemInGroup = att.itemAtPath('InitialConditions/0/Velocity', '/')
        self.assertIsNotNone(itemInGroup, 'Could not find expected item.')
        self.assertEqual(itemInGroup.name(), 'Velocity',
                         'Got wrong attribute "{nm}".'.format(nm=itemInGroup.name()))
        # Test invalid sub-group index.
        itemInGroup = att.itemAtPath('InitialConditions/1/Velocity', '/')
        self.assertIsNone(itemInGroup, 'Found unexpected item.')

    def testItemInDeepPath(self):
        att = self.resource.createAttribute('ppesolver')
        deepItem = att.itemAtPath(
            'PressurePoissonSolver/ppetype/preconditioner', '/')
        self.assertIsNotNone(deepItem, 'Could not find expected item.')
        self.assertEqual(deepItem.name(), 'preconditioner',
                         'Got wrong attribute "{nm}".'.format(nm=deepItem.name()))

        # Do the same thing with a different separator
        deepItem = att.itemAtPath(
            'PressurePoissonSolver-ppetype-preconditioner', '-')
        self.assertIsNotNone(
            deepItem, 'Could not find expected item with alternate path separator.')
        self.assertEqual(deepItem.name(), 'preconditioner',
                         'Got wrong attribute "{nm}" with alt. path separator.'.format(nm=deepItem.name()))


if __name__ == '__main__':
    smtk.testing.process_arguments()
    smtk.testing.main()
