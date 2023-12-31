//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================
// .NAME qtNewAttributeWidget - a widget for creating new attribute.
// .SECTION Description
// .SECTION Caveats

#ifndef smtk_extension_qtNewAttributeWidget_h
#define smtk_extension_qtNewAttributeWidget_h

#include <QDialog>

#include "smtk/PublicPointerDefs.h"
#include "smtk/extension/qt/Exports.h"

namespace smtk
{
namespace attribute
{
class SMTKQTEXT_EXPORT qtNewAttributeWidget : public QDialog
{
  Q_OBJECT
  typedef QDialog Superclass;

public:
  qtNewAttributeWidget(QWidget* parent = nullptr);
  ~qtNewAttributeWidget() override;

  QString attributeName() const;
  QString attributeType() const;
  virtual void setBaseWidget(QWidget* baseWidget);
  virtual int showWidget(const QString& name, const QList<QString>& attTypes);

private:
  qtNewAttributeWidget(const qtNewAttributeWidget&); // Not implemented.
  void operator=(const qtNewAttributeWidget&);       // Not implemented.

  class PIMPL;
  PIMPL* Private;

}; // class
}; // namespace attribute
}; // namespace smtk

#endif
