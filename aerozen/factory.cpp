/*
 * Copyright (C) 2026 duyongquan <quandy2020@126.com>
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

#include "aerozen/factory.hpp"
#include "aerozen/utils/never_destroyed.hpp"

namespace aerozen {
MessageFactory& Factory::Instance() {
    static aerozen::utils::NeverDestroyed<MessageFactory> instance;
    return instance.Access();
}

void Factory::Register(const std::string& _msgType, FactoryFn _factoryfn) {
    Factory::Instance().Register(_msgType, _factoryfn);
}

void Factory::Types(std::vector<std::string>& _types) {
    Factory::Instance().Types(_types);
}

Factory::MessagePtr Factory::New(const std::string& _msgType) {
    return Factory::Instance().New(_msgType);
}

Factory::MessagePtr Factory::New(const std::string& _msgType,
                                 const std::string& _args) {
    return Factory::Instance().New(_msgType, _args);
}

void Factory::LoadDescriptors(const std::string& _paths) {
    Factory::Instance().LoadDescriptors(_paths);
}

}  // namespace aerozen
