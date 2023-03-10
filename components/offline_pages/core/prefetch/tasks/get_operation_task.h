// Copyright 2017 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_OFFLINE_PAGES_CORE_PREFETCH_TASKS_GET_OPERATION_TASK_H_
#define COMPONENTS_OFFLINE_PAGES_CORE_PREFETCH_TASKS_GET_OPERATION_TASK_H_

#include <memory>
#include <string>
#include <vector>

#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "components/offline_pages/core/prefetch/prefetch_types.h"
#include "components/offline_pages/task/task.h"

namespace offline_pages {
class PrefetchNetworkRequestFactory;
class PrefetchStore;

// Task that attempts to fetch results for a completed operation.  Searches the
// URLs table for operations with rows in the appropriate state and fires off
// concurrent GetOperation requests.
class GetOperationTask : public Task {
 public:
  using OperationResultList = std::unique_ptr<std::vector<std::string>>;

  // This is a repeating version of PrefetchRequestFinishedCallback as it may be
  // called more than once when multiple requests are placed.
  using GetOperationFinishedCallback =
      base::RepeatingCallback<void(PrefetchRequestStatus status,
                                   const std::string& operation_name,
                                   const std::vector<RenderPageInfo>& pages)>;

  GetOperationTask(PrefetchStore* store,
                   PrefetchNetworkRequestFactory* request_factory,
                   GetOperationFinishedCallback callback);

  GetOperationTask(const GetOperationTask&) = delete;
  GetOperationTask& operator=(const GetOperationTask&) = delete;

  ~GetOperationTask() override;

 private:
  // Task implementation.
  void Run() override;
  void StartGetOperationRequests(OperationResultList list);

  raw_ptr<PrefetchStore> prefetch_store_;
  raw_ptr<PrefetchNetworkRequestFactory> request_factory_;
  GetOperationFinishedCallback callback_;

  base::WeakPtrFactory<GetOperationTask> weak_factory_{this};
};

}  // namespace offline_pages

#endif  // COMPONENTS_OFFLINE_PAGES_CORE_PREFETCH_TASKS_GET_OPERATION_TASK_H_
