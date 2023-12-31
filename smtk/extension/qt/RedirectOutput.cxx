//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================

#include "smtk/extension/qt/RedirectOutput.h"
#include "smtk/extension/qt/qtEmittingStringBuffer.h"
#include "smtk/io/Logger.h"

#include <QLoggingCategory>
#include <QObject>
#include <QtGlobal>

namespace smtk
{
namespace extension
{
namespace qt
{

void RedirectOutputToQt(QObject* context, smtk::io::Logger& log)
{
  qtEmittingStringBuffer* stringBuf = new qtEmittingStringBuffer();
  std::ostream* stream = new std::ostream(stringBuf);

  auto cleanup = [=] {
    delete stream;
    delete stringBuf;
  };

  // Connect to the emitting string buffer's flush signal. Since the emitting
  // string buffer is local to the logger and is scoped by its lifetime, we do
  // not need to guard against the logger being out of scope.
  QObject::connect(
    stringBuf,
    &qtEmittingStringBuffer::flush,
    context,
    [&]() {
      QLoggingCategory smtkCategory("SMTK", QtInfoMsg);
      for (auto& record : log.records())
      {
        switch (record.severity)
        {
          case smtk::io::Logger::Debug:
            qCDebug(smtkCategory) << smtk::io::Logger::toString(record, true).c_str();
            break;
          case smtk::io::Logger::Info:
            qCInfo(smtkCategory) << smtk::io::Logger::toString(record, false).c_str();
            break;
          case smtk::io::Logger::Warning:
            qCWarning(smtkCategory) << smtk::io::Logger::toString(record, true).c_str();
            break;
          case smtk::io::Logger::Error:
            qCCritical(smtkCategory) << smtk::io::Logger::toString(record, true).c_str();
            break;
          case smtk::io::Logger::Fatal:
          default:
            qFatal("%s", smtk::io::Logger::toString(record, true).c_str());
            break;
        }
      }

      log.reset();
    },
    Qt::QueuedConnection);

  log.setFlushToStream(stream, false, false);
  log.setCallback(cleanup);
}
} // namespace qt
} // namespace extension
} // namespace smtk
