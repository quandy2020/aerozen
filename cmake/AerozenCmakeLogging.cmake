# Copyright (C) 2026 duyongquan <quandy2020@126.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#################################################
# aerozen_build_error macro
macro(aerozen_build_error)
  foreach(str ${ARGN})
    set(msg "\t${str}")
    list(APPEND build_errors ${msg})
  endforeach()
endmacro(aerozen_build_error)

#################################################
# aerozen_build_warning macro
macro(aerozen_build_warning)
  foreach(str ${ARGN})
    list(APPEND build_warnings "${str}")
  endforeach(str ${ARGN})
endmacro(aerozen_build_warning)
