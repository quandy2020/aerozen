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

 #include "aerozen/utils/environment.hpp"

 #include <algorithm>
 #include <cstdlib>
 #include <string>
 #include <utility>
 #include <vector>

 extern char ** environ;
 
 namespace aerozen {
 namespace utils {
 
 /////////////////////////////////////////////////
 bool env(const std::string &_name, std::string &_value, bool _allowEmpty)
 {
   bool valid = false;
   std::string v;
   const char *cvar = std::getenv(_name.c_str());
   if (cvar != nullptr)
   {
     v = cvar;
     valid = true;
 
     if (v[0] == '\0' && !_allowEmpty)
     {
       valid = false;
     }
   }
   if (valid)
   {
     _value = v;
     return true;
   }
   return false;
 }
 
 /////////////////////////////////////////////////
 bool setenv(const std::string &_name, const std::string &_value)
 {
   if (0 != ::setenv(_name.c_str(), _value.c_str(), true))
   {
     return false;
   }
   return true;
 }
 
 /////////////////////////////////////////////////
 bool unsetenv(const std::string &_name)
 {
   if (0 != ::unsetenv(_name.c_str()))
   {
     return false;
   }
   return true;
 }
 
 /////////////////////////////////////////////////
 bool clearenv()
 {
   bool success = true;
   if (0 != ::clearenv())
   {
     success = false;
   }
   return success;
 }
 
 /////////////////////////////////////////////////
 EnvironmentMap env()
 {
   EnvironmentMap ret;
 
   char **currentEnv = nullptr;
   currentEnv = environ;
   // In the case that clearenv() was just called
   // currentEnv will be nullptr
   if (currentEnv == nullptr)
     return {};
 
   std::vector<std::string> envStrings;
   for (; *currentEnv; ++currentEnv)
   {
     envStrings.emplace_back(*currentEnv);
   }
   return envStringsToMap(envStrings);
 }
 
 /////////////////////////////////////////////////
 bool setenv(const EnvironmentMap &_vars)
 {
   bool success = true;
   for (const auto &[key, value] : _vars)
   {
     success &= setenv(key, value);
   }
   return success;
 }
 
 /////////////////////////////////////////////////
 EnvironmentMap envStringsToMap(const EnvironmentStrings &_envStrings)
 {
   EnvironmentMap ret;
   for (const auto &pair : _envStrings)
   {
     auto eqPos = pair.find('=');
     if (eqPos != std::string::npos)
     {
       ret.emplace(pair.substr(0, eqPos), pair.substr(eqPos + 1));
     }
   }
   return ret;
 }
 
 /////////////////////////////////////////////////
 EnvironmentStrings envMapToStrings(const EnvironmentMap &_envMap)
 {
   EnvironmentStrings ret;
   auto sorted = std::vector<std::pair<std::string, std::string>>(
     _envMap.begin(), _envMap.end());
   std::sort(sorted.begin(), sorted.end());
   for (auto [key, value] : sorted)
   {
     key += '=';
     key += value;
     ret.push_back(key);
   }
   return ret;
 }
 
 /////////////////////////////////////////////////
 std::string printenv()
 {
   std::string ret;
   for (const auto &entry : envMapToStrings(env()))
   {
     ret.append(entry);
     ret.append("\n");
   }
   return ret;
 }

 }  // namespace utils
 }  // namespace aerozen
 