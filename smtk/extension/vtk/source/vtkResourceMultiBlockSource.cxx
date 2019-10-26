//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================
#include "smtk/extension/vtk/source/vtkResourceMultiBlockSource.h"

#include "vtkInformation.h"
#include "vtkInformationStringKey.h"
#include "vtkInformationVector.h"
#include "vtkMultiBlockDataSet.h"

using UUID = smtk::common::UUID;
using SequenceType = vtkResourceMultiBlockSource::SequenceType;

vtkInformationKeyMacro(vtkResourceMultiBlockSource, COMPONENT_ID, String);

//----------------------------------------------------------------------------
vtkResourceMultiBlockSource::vtkResourceMultiBlockSource() = default;

//----------------------------------------------------------------------------
vtkResourceMultiBlockSource::~vtkResourceMultiBlockSource()
{
  this->ClearCache();
}

//----------------------------------------------------------------------------
void vtkResourceMultiBlockSource::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//----------------------------------------------------------------------------
void vtkResourceMultiBlockSource::SetDataObjectUUID(vtkInformation* info, const UUID& id)
{
  // FIXME: Eventually this should encode the UUID without string conversion
  info->Set(vtkResourceMultiBlockSource::COMPONENT_ID(), id.toString().c_str());
}

//----------------------------------------------------------------------------
UUID vtkResourceMultiBlockSource::GetDataObjectUUID(vtkInformation* datainfo)
{
  // FIXME: Eventually this should decode the UUID without string conversion
  UUID id;
  if (!datainfo)
  {
    return id;
  }

  const char* uuidChar = datainfo->Get(vtkResourceMultiBlockSource::COMPONENT_ID());
  if (uuidChar)
  {
    id = UUID(uuidChar);
  }
  return id;
}

//----------------------------------------------------------------------------
void vtkResourceMultiBlockSource::SetResourceId(vtkMultiBlockDataSet* dataset, const UUID& uid)
{
  if (dataset->GetNumberOfBlocks() <= BlockId::Components)
  {
    dataset->SetNumberOfBlocks(BlockId::NumberOfBlocks);
  }
  vtkResourceMultiBlockSource::SetDataObjectUUID(dataset->GetMetaData(BlockId::Components), uid);
}

//----------------------------------------------------------------------------
UUID vtkResourceMultiBlockSource::GetResourceId(vtkMultiBlockDataSet* dataset)
{
  if (dataset->GetNumberOfBlocks() <= BlockId::Components)
  {
    return UUID::null();
  }
  return vtkResourceMultiBlockSource::GetDataObjectUUID(dataset->GetMetaData(BlockId::Components));
}

//----------------------------------------------------------------------------
smtk::resource::ComponentPtr vtkResourceMultiBlockSource::GetComponent(
  const smtk::resource::ResourcePtr& resource, vtkInformation* info)
{
  if (resource == nullptr)
  {
    return smtk::resource::ComponentPtr();
  }

  return resource->find(vtkResourceMultiBlockSource::GetDataObjectUUID(info));
}

//----------------------------------------------------------------------------
smtk::resource::ComponentPtr vtkResourceMultiBlockSource::GetComponent(vtkInformation* info)
{
  return vtkResourceMultiBlockSource::GetComponent(this->GetResource(), info);
}

//----------------------------------------------------------------------------
smtk::resource::ResourcePtr vtkResourceMultiBlockSource::GetResource()
{
  return this->Resource.lock();
}

//----------------------------------------------------------------------------
void vtkResourceMultiBlockSource::SetResource(const smtk::resource::ResourcePtr& resource)
{
  this->Resource = resource;
  this->Modified();
}

void vtkResourceMultiBlockSource::DumpBlockStructureWithUUIDsInternal(
  vtkMultiBlockDataSet* dataset, int& counter, int indent)
{
  if (!dataset)
  {
    return;
  }
  int nb = dataset->GetNumberOfBlocks();
  for (int ii = 0; ii < nb; ++ii)
  {
    std::cout << std::setfill(' ') << std::setw(indent) << " " << std::setfill(' ') << std::setw(4)
              << ii << " " << std::setfill(' ') << std::setw(4) << (counter++) << " ";
    UUID uid;
    if (dataset->HasMetaData(ii))
    {
      uid = vtkResourceMultiBlockSource::GetDataObjectUUID(dataset->GetMetaData(ii));
    }
    if (uid)
    {
      std::cout << uid;
    }
    else
    {
      std::cout << " no uuid                            ";
    }
    auto block = dataset->GetBlock(ii);
    std::cout << "  " << (block ? block->GetClassName() : "(null)") << "\n";
    auto mbds = vtkMultiBlockDataSet::SafeDownCast(block);
    if (mbds)
    {
      vtkResourceMultiBlockSource::DumpBlockStructureWithUUIDsInternal(mbds, counter, indent + 2);
    }
  }
}

bool vtkResourceMultiBlockSource::SetCachedData(
  const UUID& uid, vtkDataObject* data, int sequenceNumber)
{
  if (!data)
  {
    return false;
  }
  CacheEntry entry{ data, sequenceNumber };
  auto it = this->Cache.find(uid);
  bool exists = (it != this->Cache.end());
  if (!exists || it->second.SequenceNumber < sequenceNumber)
  {
    if (exists)
    { // Release the existing entry's data.
      it->second.Data->UnRegister(this);
    }
    this->Cache[uid] = entry;
    data->Register(this);
    return true;
  }
  return false;
}

SequenceType vtkResourceMultiBlockSource::GetCachedDataSequenceNumber(const UUID& uid) const
{
  auto it = this->Cache.find(uid);
  if (it == this->Cache.end())
  {
    return static_cast<SequenceType>(-1);
  }
  return it->second.SequenceNumber;
}

vtkDataObject* vtkResourceMultiBlockSource::GetCachedDataObject(const UUID& uid)
{
  auto it = this->Cache.find(uid);
  if (it == this->Cache.end())
  {
    return nullptr;
  }
  return it->second.Data;
}

bool vtkResourceMultiBlockSource::RemoveCacheEntry(const UUID& uid)
{
  auto it = this->Cache.find(uid);
  if (it == this->Cache.end())
  {
    return false;
  }
  it->second.Data->UnRegister(this);
  this->Cache.erase(it);
  return true;
}

bool vtkResourceMultiBlockSource::RemoveCacheEntriesExcept(const std::set<UUID>& exceptions)
{
  bool didRemove = false;
  for (auto it = this->Cache.begin(); it != this->Cache.end(); /* do nothing */)
  {
    if (exceptions.find(it->first) == exceptions.end())
    {
      it->second.Data->UnRegister(this);
      didRemove = true;
      it = this->Cache.erase(it);
    }
    else
    {
      ++it;
    }
  }
  return didRemove;
}

void vtkResourceMultiBlockSource::ClearCache()
{
  for (auto& entry : this->Cache)
  {
    entry.second.Data->UnRegister(this);
  }
  this->Cache.clear();
}
