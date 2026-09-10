/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <string>

#include "helper.h"
#include "route_guide_service.h"
#include "absl/flags/parse.h"
#if ROUTE_GUIDE_HAS_ABSL_LOG_INITIALIZE
#include "absl/log/initialize.h"
#endif

int main(int argc, char** argv) {
  absl::ParseCommandLine(argc, argv);
#if ROUTE_GUIDE_HAS_ABSL_LOG_INITIALIZE
  absl::InitializeLog();
#endif
  std::string db = routeguide::GetDbFileContent(argc, argv);
  RouteGuideServer server(db);
  if (!server.Start("0.0.0.0:50051")) {
    std::cerr << "Failed to start server" << std::endl;
    return 1;
  }
  std::cout << "Server listening on 0.0.0.0:" << server.port() << std::endl;
  server.Wait();

  return 0;
}
